#include "solver.h"
#include "commons.h"

double getTotalTime(CellTimes* times) {
    if (times->total_th_time) {
        return times->total_th_time;
    }
    times->total_th_time = 1.0 * times->gg_mese * times->turni * times->ore_turno;
    return times->total_th_time;
}

unsigned long int getDomandaSingolaMacchina(CellData* cell) {
    // Se sono tutte in parallelo divid la domanda per "line_size"
    bool all_parallel = true;
    // Numero di macchine in parallelo
    unsigned char line_size = 1;
    for (unsigned char i = 0; i < cell->n_stazioni; i++) {
        MachineFlow* currentStation = &cell->stazioni[i];
        if (currentStation->n_macchine == 1) {  // Non ho macchine in parallelo, il sistema diventa in serie
            all_parallel = false;
            break;
        }
        // Controllo se in questa stazione ho meno macchine in parallelo: se sè aggiorno il numero di macchine
        // in modo che siano il minimo fino ad ora (poichè per ora sono tutte in parallelo e il TaktTime dipende
        //  dal numero di macchine minore in parallelo)
        if (currentStation->n_macchine < line_size) {
            line_size = currentStation->n_macchine;
        }
    }

    if (all_parallel) {
        return cell->domanda_pezzi / line_size;
    }
    return cell->domanda_pezzi;
}

double getTaktTime(CellData* cell) {
    if (cell->TT != 0.0) {
        return cell->TT;
    }
    if (cell->n_stazioni == 0) {  // non ho configurazioni delle stazioni: tutte le macchine sono in serie
        cell->TT = (getTotalTime(&cell->times) * 3600.0) / cell->domanda_pezzi;
    } else {
        cell->TT = (getTotalTime(&cell->times) * 3600.0) / getDomandaSingolaMacchina(cell);
    }
    return cell->TT;
}

double getAdjTaktTime(CellData* cell, SOLVER_TYPE solver) {
    OEE* oee_data;
    switch (solver) {
        case CELL_SOLVER:
        case OPERATOR_SOLVER:
            oee_data = &cell->cell_or_operatorsOEE;
            break;
        case ROBOT_SOLVER:
            oee_data = &cell->robotsOEE;
            break;
        default:
            return -1.0f;
    }
    if (oee_data->TT_adj != 0.0) {
        return oee_data->TT_adj;
    }
    computeOEE(cell, solver);
    oee_data->TT_adj = getTaktTime(cell) * oee_data->OEE;
    return oee_data->TT_adj;
}

double getUnavailableTime(CellData* cell) {
    return cell->tempo_guasti + cell->tempo_manutenzione + cell->tempo_setup;
}

void computeDisp(CellData* cell, SOLVER_TYPE solver) {
    OEE* ref;
    // Se sto considerando un SOLVER_TYPE operatore allora rimane 0 e non considero il
    // valore di 99% dell'efficienza dovuta ad ogni robot. Altrimenti questo valore indica
    // quante volte la disponibilitè deve essere moltiplicata per 0,99 (ovvero disp=99%)
    unsigned char numero_robot = 0;
    switch (solver) {
        case CELL_SOLVER:
        case OPERATOR_SOLVER:
            ref = &cell->cell_or_operatorsOEE;
            break;
        case ROBOT_SOLVER:
            ref = &cell->robotsOEE;
            numero_robot = cell->n_robot;
            break;
        default:
            return;
    }
    double tt = getTotalTime(&cell->times);
    // Calcolo il tempo disponibile al netto di tempi di setup, manutenzione e guasti
    cell->times.total_disp_time = tt - getUnavailableTime(cell);
    // Calcolo la disponibilitè
    ref->disponibilita = (cell->times.total_disp_time) / tt;
    // Per ogni robot che ho, se sto considerando la cella con robot, moltiplico per 99%
    for (short i = 0; i < numero_robot; i++) {
        ref->disponibilita *= 0.99;
    }
    // è da ri-calcolare l'OEE complessivo poichè ho modificato la disponibilitè
    ref->computed = false;
}

static void addStationEfficency(CellData* cell, unsigned char current_pos, double* current_eff);

static void addStationEfficency(CellData* cell, unsigned char current_pos, double* current_eff) {
    MachineFlow* mf = &cell->stazioni[current_pos];
    double eff_parall = 0.0;
    unsigned char current_machine_id = 11;
    if (mf->n_macchine > 1) {
        for (unsigned char i = 0; i < mf->n_macchine; i++) {
            current_machine_id = mf->parallele[i];
            // Se l'ID salvato è 11 allora vuol dire che non ho una macchina ed esco dal ciclo
            if (current_machine_id == 11) {
                break;
            }
            // Altrimenti ho la macchina e devo prendere la sua efficenza
            eff_parall += 1.0 / (cell->macchine[mf->parallele[current_machine_id]].efficienza / 100.0);
        }
        // Devo invertire il valore
        eff_parall = 1.0 / eff_parall;
        *current_eff = *current_eff * eff_parall;
    } else {
        // Ho solo una macchina, questa: l'id è salvato in "mf->parallele[0]" lo prendo e lo metto in serie al'efficienza corrente
        *current_eff = *current_eff * (cell->macchine[mf->parallele[0]].efficienza / 100.0);
    }
}

void computeRend(CellData* cell, SOLVER_TYPE solver) {
    // Per gli operatori è il 96% per i robot è il 100%
    unsigned char efficienza = 0;
    // Tempo dovuto alle pause operatori
    double extra_time = 0.0;
    double eff_macchine = 1.0;        // Parto dal 100%
    if (cell->efficienza == 0) {      // se non ho impostato un'efficienza generale calcolo quella dovuta alle singole macchine
        if (cell->n_stazioni == 0) {  // Se non ho nulla nelle stazioni vuol dire che ho tutte le macchine in serie
            for (unsigned char i = 0; i < cell->n_macchine; i++) {
                eff_macchine = eff_macchine * (cell->macchine[i].efficienza / 100.0);
            }
        } else {
            // Calcolo il rendimento delle stazioni: in serie e in parallelo
            for (unsigned char i = 0; i < cell->n_stazioni; i++) {
                addStationEfficency(cell, i, &eff_macchine);
            }
        }
    } else {
        // Se la cella ha un efficienza complessiva uso direttamente quella
        eff_macchine = cell->efficienza / 100.0;
    }

    OEE* ref;
    switch (solver) {
        case OPERATOR_SOLVER:
            efficienza = 100 - cell->ineffiecenza_operatore;
            extra_time = cell->tempo_pausa * cell->times.turni * cell->times.gg_mese;
        case CELL_SOLVER:
            ref = &cell->cell_or_operatorsOEE;
            break;
        case ROBOT_SOLVER:
            efficienza = 100 - cell->ineffiecenza_robot;
            ref = &cell->robotsOEE;
            break;
        default:
            return;
    }
    double tt = getTotalTime(&cell->times);
    // Calcolo il tempo disponibile se non l'ho ancora calcolato
    if (!cell->times.total_disp_time) {
        // Calcolo il tempo disponibile al netto di tempi di setup, manutenzione e guasti
        cell->times.total_disp_time = tt - getUnavailableTime(cell);
    }
    // Calcolo il rendimento
    ref->rendimento = ((cell->times.total_disp_time - extra_time) / cell->times.total_disp_time) * (efficienza / 100.0) * eff_macchine;
    // è da ri-calcolare l'OEE complessivo poichè ho modificato il rendimento
    ref->computed = false;
}

void computeQual(CellData* cell, SOLVER_TYPE solver) {
    OEE* ref;
    double quality = -1.0;
    switch (solver) {
        case CELL_SOLVER:
        case OPERATOR_SOLVER:
            if (cell->ppm_cella_oper > 0) {
                quality = 1.0 - (cell->ppm_cella_oper / 1e6);
            }
            ref = &cell->cell_or_operatorsOEE;
            break;
        case ROBOT_SOLVER:
            if (cell->ppm_cella_robot > 0) {
                quality = 1.0 - (cell->ppm_cella_robot / 1e6);
            }
            ref = &cell->robotsOEE;
            break;
        default:
            return;
    }
    // Se i ppm cella globali per robot o operatore sono nulli vuol dire che devo calcolare i ppm rispetto alle macchine
    if (quality < 0.0) {
        quality = 1.0;
        unsigned long ppms = 0;
        for (unsigned char i = 0; i < cell->n_macchine; i++) {
            ppms = cell->macchine[i].ppms;
            if (ppms != 0) {
                quality *= (1 - ppms / 1e6);
            }
        }
    }
    // Parti per milione: devo dividere per 1milione per avere la percentuale
    ref->qualita = quality;
    // è da ri-calcolare l'OEE complessivo poichè ho modificato la disponibilitè
    ref->computed = false;
}

void computeOEE(CellData* cell, SOLVER_TYPE solver) {
    OEE* ref;
    switch (solver) {
        case CELL_SOLVER:
        case OPERATOR_SOLVER:
            ref = &cell->cell_or_operatorsOEE;
            break;
        case ROBOT_SOLVER:
            ref = &cell->robotsOEE;
            break;
        default:
            return;
    }
    if (ref->computed) {
        return;
    }
    ref->OEE = ref->disponibilita * ref->rendimento * ref->qualita;
    ref->computed = true;
}

void computeProduttivita(CellData* cell, SOLVER_TYPE solver) {
    unsigned int RP = 0;
    Produttivita* pr = 0;
    OEE* oee = 0;
    switch (solver) {
        case CELL_SOLVER:
            return;
        case OPERATOR_SOLVER:
            RP = cell->RP_oper;
            pr = &cell->prod_oper;
            oee = &cell->cell_or_operatorsOEE;
            break;
        case ROBOT_SOLVER:
            RP = cell->RP_robot;
            pr = &cell->prod_robot;
            oee = &cell->robotsOEE;
            break;
    }
    // Non ho RP, è inutile fare i calcoli
    if (RP == 0 || pr == 0 || oee == 0) {
        return;
    }
    // Ricalcolo l'OEE se necessario
    computeOEE(cell, solver);
    pr->oraria = (3600.0 / RP) * oee->OEE;
    pr->turno = (unsigned long int)(pr->oraria * cell->times.ore_turno);
    pr->giornaliera = pr->turno * cell->times.turni;
    pr->mensile = pr->giornaliera * cell->times.gg_mese;
}

void computeGripper(Gripping* gp, GrippingResult* gr) {
    if (gp == 0 || gr == 0) {
        return ;
    }
    //Calcolo la massa del gripper
    if (gp->mass_grip == 0.0) {
        gp->mass_grip = gp->mass_obj * 0.3;
        if (gp->mass_grip < 1.5) {
            gp->mass_grip = 1.5;
        }
    }
    //Calcolo le forze di gripping
    double accMax = gp->acc;
    if (gp->vertical) {
        accMax += GRAVITY;
    }
    if (gp->force_fit) {
        gr->ForceGripper = gp->mass_obj * accMax * gp->coeff_sic / (gp->coeff_attr);
    } else {
        gr->ForceGripper = gp->mass_obj * accMax * gp->coeff_sic;
    }
    if(gp->alpha != 0.0){
        double mezzo_alpha_rad = (gp->alpha * PI/180.0)/2;
        if(gp->force_fit){
            gr->ForceGripper = gr->ForceGripper * sin(mezzo_alpha_rad);
        }else if(gp->presa_verticale){
            gr->ForceGripper = gr->ForceGripper * tan(mezzo_alpha_rad);
        }
    }
    if (gp->n_griffe > 0) {
        gr->ForceGriffa = gr->ForceGripper / gp->n_griffe;
    }
}
