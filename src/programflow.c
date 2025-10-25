#include "programflow.h"

#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <string.h>

#include "datatypes.h"
#include "gui.h"
#include "session.h"
#include "solver.h"

CellData cd = CREATE_CELL();
Gripping grp = CREATE_GRIPPING();
GrippingResult grp_res = {0.0, 0.0};
AppStatus as = {};

/**************************** CELL TIMES ****************************/
SettingRow ct_turniGG = CREATE_SETTINGS_ROW_IN(T_CELL_TIMES_TURNI_GG, &cd.times.turni, UNSIGNED_CHAR);
SettingRow ct_oreTurno = CREATE_SETTINGS_ROW_IN(T_CELL_TIMES_ORE_TURNO, &cd.times.ore_turno, UNSIGNED_CHAR);
SettingRow ct_ggMese = CREATE_SETTINGS_ROW_IN(T_CELL_TIMES_GG_MESE, &cd.times.gg_mese, UNSIGNED_CHAR);
SettingRow ct_tTot = CREATE_SETTINGS_ROW_OUT_CALC(T_CELL_TIMES_T_TOT, &cd.times.total_th_time, DOUBLE);
SettingRow ct_tTotDisp = CREATE_SETTINGS_ROW_OUT_CALC(T_CELL_TIMES_T_DISP, &cd.times.total_disp_time, DOUBLE);

SettingsPage celltimes_menu = CREATE_SETTINGS_PAGE();
/************************ END CELL TIMES ****************************/

/**************************** OEE CELLA/OPER ****************************/
SettingRow oee_cp_disp = CREATE_SETTINGS_ROW_IN_CALC(T_OEE_DISP, &cd.cell_or_operatorsOEE.disponibilita, DOUBLE);
SettingRow oee_cp_rend = CREATE_SETTINGS_ROW_IN_CALC(T_OEE_REND, &cd.cell_or_operatorsOEE.rendimento, DOUBLE);
SettingRow oee_cp_qual = CREATE_SETTINGS_ROW_IN_CALC(T_OEE_QUAL, &cd.cell_or_operatorsOEE.qualita, DOUBLE);
SettingRow oee_cp_oee = CREATE_SETTINGS_ROW_IN_CALC(T_OEE_OEE, &cd.cell_or_operatorsOEE.OEE, DOUBLE);
SettingRow oee_cp_computed = CREATE_SETTINGS_ROW_IN(T_OEE_COMPUTED, &cd.cell_or_operatorsOEE.computed, BOOL);
SettingRow oee_cp_tt_adj = CREATE_SETTINGS_ROW_OUT_CALC(T_OEE_TT_ADJ, &cd.cell_or_operatorsOEE.TT_adj, DOUBLE);

SettingsPage oee_cell_oper_menu = CREATE_SETTINGS_PAGE();
/************************ END CELL OEE OPER ****************************/

/**************************** OEE ROBOT ****************************/
SettingRow oee_rbt_disp = CREATE_SETTINGS_ROW_IN_CALC(T_OEE_DISP, &cd.robotsOEE.disponibilita, DOUBLE);
SettingRow oee_rbt_rend = CREATE_SETTINGS_ROW_IN_CALC(T_OEE_REND, &cd.robotsOEE.rendimento, DOUBLE);
SettingRow oee_rbt_qual = CREATE_SETTINGS_ROW_IN_CALC(T_OEE_QUAL, &cd.robotsOEE.qualita, DOUBLE);
SettingRow oee_rbt_oee = CREATE_SETTINGS_ROW_IN_CALC(T_OEE_OEE, &cd.robotsOEE.OEE, DOUBLE);
SettingRow oee_rbt_computed = CREATE_SETTINGS_ROW_IN(T_OEE_COMPUTED, &cd.robotsOEE.computed, BOOL);
SettingRow oee_rbt_tt_adj = CREATE_SETTINGS_ROW_OUT_CALC(T_OEE_TT_ADJ, &cd.robotsOEE.TT_adj, DOUBLE);

SettingsPage oee_robot_menu = CREATE_SETTINGS_PAGE();
/************************ END CELL OEE ROBOT ****************************/

/**************************** PROD OPER ****************************/
SettingRow prod_oper_oraria = CREATE_SETTINGS_ROW_OUT_CALC(T_PROD_ORARIA, &cd.prod_oper.oraria, DOUBLE);
SettingRow prod_oper_turno = CREATE_SETTINGS_ROW_OUT_CALC(T_PROD_TURNO, &cd.prod_oper.turno, UNSIGNED_INT);
SettingRow prod_oper_giornaliera = CREATE_SETTINGS_ROW_OUT_CALC(T_PROD_GIORN, &cd.prod_oper.giornaliera, UNSIGNED_INT);
SettingRow prod_oper_mensile = CREATE_SETTINGS_ROW_OUT_CALC(T_PROD_MESE, &cd.prod_oper.mensile, UNSIGNED_INT);

SettingsPage prod_oper_menu = CREATE_SETTINGS_PAGE();
/************************ END PROD OPER ****************************/

/**************************** PROD ROBOT ****************************/
SettingRow prod_rbt_oraria = CREATE_SETTINGS_ROW_OUT_CALC(T_PROD_ORARIA, &cd.prod_robot.oraria, DOUBLE);
SettingRow prod_rbt_turno = CREATE_SETTINGS_ROW_OUT_CALC(T_PROD_TURNO, &cd.prod_robot.turno, UNSIGNED_INT);
SettingRow prod_rbt_giornaliera = CREATE_SETTINGS_ROW_OUT_CALC(T_PROD_GIORN, &cd.prod_robot.giornaliera, UNSIGNED_INT);
SettingRow prod_rbt_mensile = CREATE_SETTINGS_ROW_OUT_CALC(T_PROD_MESE, &cd.prod_robot.mensile, UNSIGNED_INT);

SettingsPage prod_robot_menu = CREATE_SETTINGS_PAGE();
/************************ END PROD ROBOT ****************************/

/**************************** CELL DATA ****************************/
SettingRow cd_celltimes = CREATE_SETTINGS_ROW_OUT(T_CELL_DATA_TEMPI, handleMenu_CellTimes, PAGE_CHANGE);
SettingRow cd_domanda = CREATE_SETTINGS_ROW_IN(T_CELL_DATA_DOMANDA, &cd.domanda_pezzi, UNSIGNED_LONG);
SettingRow cd_tSetup = CREATE_SETTINGS_ROW_IN(T_CELL_DATA_T_SETUP, &cd.tempo_setup, DOUBLE);
SettingRow cd_tManut = CREATE_SETTINGS_ROW_IN(T_CELL_DATA_T_MANUT, &cd.tempo_manutenzione, DOUBLE);
SettingRow cd_tGuasti = CREATE_SETTINGS_ROW_IN(T_CELL_DATA_T_GUASTI, &cd.tempo_guasti, DOUBLE);
SettingRow cd_ppmOper = CREATE_SETTINGS_ROW_IN(T_CELL_DATA_PPM_OPER, &cd.ppm_cella_oper, UNSIGNED_LONG);
SettingRow cd_ppmRobot = CREATE_SETTINGS_ROW_IN(T_CELL_DATA_PPM_ROBOT, &cd.ppm_cella_robot, UNSIGNED_LONG);
SettingRow cd_takt_time = CREATE_SETTINGS_ROW_IN_CALC(T_CELL_DATA_TAKT_TIME, &cd.TT, DOUBLE);
SettingRow cd_macchine = CREATE_SETTINGS_ROW_OUT(T_CELL_DATA_MACCHINE_ARR, handleArray_Macchine, ARRAY);
SettingRow cd_n_macchine = CREATE_SETTINGS_ROW_OUT(T_CELL_DATA_N_MACCHINE, &cd.n_macchine, UNSIGNED_CHAR);
SettingRow cd_stazioni = CREATE_SETTINGS_ROW_OUT(T_CELL_DATA_STAZIONI_ARR, 0, ARRAY);
SettingRow cd_n_stazioni = CREATE_SETTINGS_ROW_OUT(T_CELL_DATA_N_STAZIONI, &cd.n_stazioni, UNSIGNED_CHAR);
// Oper
SettingRow cd_oee_cell_oper = CREATE_SETTINGS_ROW_OUT(T_CELL_DATA_OEE_CELLA_OPER, handleMenu_OEE_Cell, PAGE_CHANGE);
SettingRow cd_operatori = CREATE_SETTINGS_ROW_OUT(T_CELL_DATA_OPERATORI_ARR, 0, ARRAY);
SettingRow cd_n_operatori = CREATE_SETTINGS_ROW_OUT(T_CELL_DATA_N_OPERATORI, &cd.n_operatori, UNSIGNED_CHAR);
SettingRow cd_tPausa = CREATE_SETTINGS_ROW_IN(T_CELL_DATA_T_PAUSA, &cd.tempo_pausa, DOUBLE);
SettingRow cd_ineff_oper = CREATE_SETTINGS_ROW_IN(T_CELL_DATA_INEFF_OPER, &cd.ineffiecenza_operatore, UNSIGNED_CHAR);
// Robot
SettingRow cd_oee_robot = CREATE_SETTINGS_ROW_OUT(T_CELL_DATA_OEE_ROBOT, handleMenu_OEE_Robot, PAGE_CHANGE);
SettingRow cd_robot = CREATE_SETTINGS_ROW_OUT(T_CELL_DATA_ROBOT_ARR, handleArray_Robot, ARRAY);
SettingRow cd_n_robot = CREATE_SETTINGS_ROW_OUT(T_CELL_DATA_N_ROBOT, &cd.n_robot, UNSIGNED_CHAR);
SettingRow cd_ineff_robot = CREATE_SETTINGS_ROW_IN(T_CELL_DATA_INEFF_ROBOT, &cd.ineffiecenza_robot, UNSIGNED_CHAR);
// Risultati
SettingRow cd_rp_oper = CREATE_SETTINGS_ROW_IN(T_CELL_DATA_RP_OPER, &cd.RP_oper, UNSIGNED_INT);
SettingRow cd_rp_robot = CREATE_SETTINGS_ROW_IN(T_CELL_DATA_RP_ROBOT, &cd.RP_robot, UNSIGNED_INT);
SettingRow cd_prod_oper = CREATE_SETTINGS_ROW_OUT(T_CELL_DATA_PROD_OPER, handleMenu_Prod_Oper, PAGE_CHANGE);
SettingRow cd_prod_robot = CREATE_SETTINGS_ROW_OUT(T_CELL_DATA_PROD_ROBOT, handleMenu_Prod_Robot, PAGE_CHANGE);

SettingsPage celldata_menu = CREATE_SETTINGS_PAGE();
/**************************** END CELL DATA ****************************/

/**************************** GRIPPER ****************************/
SettingRow grp_mass_obj = CREATE_SETTINGS_ROW_IN(T_GRIPPER_MASS_OBJ, &grp.mass_obj, DOUBLE);
SettingRow grp_mass_grp = CREATE_SETTINGS_ROW_IN_CALC(T_GRIPPER_MASS_GRIPPER, &grp.mass_grip, DOUBLE);
SettingRow grp_coeff_attr = CREATE_SETTINGS_ROW_IN(T_GRIPPER_COEFF_ATTR, &grp.coeff_attr, DOUBLE);
SettingRow grp_coeff_sic = CREATE_SETTINGS_ROW_IN(T_GRIPPER_COEFF_SICUR, &grp.coeff_sic, DOUBLE);
SettingRow grp_acceler = CREATE_SETTINGS_ROW_IN(T_GRIPPER_ACCELER, &grp.acc, DOUBLE);
SettingRow grp_alfa = CREATE_SETTINGS_ROW_IN(T_GRIPPER_ALFA_GRADI, &grp.alpha, DOUBLE);
SettingRow grp_force_fit = CREATE_SETTINGS_ROW_IN(T_GRIPPER_FORCE_FIT, &grp.force_fit, BOOL);
SettingRow grp_vertical = CREATE_SETTINGS_ROW_IN(T_GRIPPER_VERTICAL, &grp.vertical, BOOL);
SettingRow grp_n_griffe = CREATE_SETTINGS_ROW_IN(T_GRIPPER_N_GRIFFE, &grp.n_griffe, UNSIGNED_CHAR);
SettingRow grp_presa_vert = CREATE_SETTINGS_ROW_IN(T_GRIPPER_PRESA_VERTICALE, &grp.presa_verticale, BOOL);
SettingRow grp_force_tot = CREATE_SETTINGS_ROW_OUT_CALC(T_GRIPPER_F_TOT, &grp_res.ForceGripper, DOUBLE);
SettingRow grp_force_grif = CREATE_SETTINGS_ROW_OUT_CALC(T_GRIPPER_F_GRIFFA, &grp_res.ForceGriffa, DOUBLE);

SettingsPage gripper_menu = CREATE_SETTINGS_PAGE();
/************************ END GRIPPER ****************************/

/**************************** MENU PRINCIPALE ****************************/
SettingRow mm_save = CREATE_SETTINGS_ROW_OUT(T_MAIN_MENU_SAVE, 0, PAGE_CHANGE);
SettingRow mm_delete = CREATE_SETTINGS_ROW_OUT(T_MAIN_MENU_DELETE, 0, PAGE_CHANGE);
SettingRow mm_cella = CREATE_SETTINGS_ROW_OUT(T_MAIN_MENU_CELL, handleMenu_CellData, PAGE_CHANGE);
SettingRow mm_gripper = CREATE_SETTINGS_ROW_OUT(T_MAIN_MENU_GRIPPER, handleMenu_Gripper, PAGE_CHANGE);

SettingsPage main_menu = CREATE_SETTINGS_PAGE();
/************************ END MENU PRINCIPALE ****************************/

/*****************************FUNCTION BARS DEFINITIONS*************************** */
// Output
FunctionBar FB_handle_OUTPUT = {{{&T_FBAR_GET, handleGetVariable}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}};
// Number elements
FunctionBar FB_handle_number_INPUT = {{{&T_FBAR_SET, handleSetVariable}, {&T_FBAR_INIT, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}};
FunctionBar FB_handle_number_COMPUTE_INPUT = {{{&T_FBAR_COMPUTE, handleComputeVariable}, {&T_FBAR_SET, handleSetVariable}, {&T_FBAR_INIT, 0}, {0, 0}, {0, 0}, {0, 0}}};
// Array elements
FunctionBar FB_handle_array_INPUT = {{{&T_FBAR_ARRAY_OPEN, handleShowArrayMenu}, {&T_FBAR_INIT, handleResetArray}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}};
FunctionBar FB_handle_array_element_INPUT = {{{&T_FBAR_EDIT, handleEditArray}, {&T_FBAR_ADD, handleAddArray}, {&T_FBAR_REMOVE, handleRemoveArray}, {0, 0}, {0, 0}, {0, 0}}};
// Bool elements
FunctionBar FB_handle_bool_INPUT = {{{&T_FBAR_TOGGLE, handleToggleVariable}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}};
FunctionBar FB_handle_bool_COMPUTE_INPUT = {{{&T_FBAR_COMPUTE, handleComputeVariable}, {&T_FBAR_TOGGLE, handleToggleVariable}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}};
// Generic
FunctionBar FB_handle_COMPUTE = {{{&T_FBAR_COMPUTE, handleComputeVariable}, {&T_FBAR_GET, handleGetVariable}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}};

/*****************************END FUNCTION BARS DEFINITIONS*************************** */

/**************CHIAMATE GLOBALI******************/
AppStatus* getAppStatus() {
    return &as;
}

void initMenus() {
    as.cd = &cd;
    as.grip = &grp;
    as.grip_res = &grp_res;
    // CELL_TIMES
    addSettingRow(&celltimes_menu, &ct_oreTurno);
    addSettingRow(&celltimes_menu, &ct_turniGG);
    addSettingRow(&celltimes_menu, &ct_ggMese);
    addSettingRow(&celltimes_menu, &ct_tTot);
    addSettingRow(&celltimes_menu, &ct_tTotDisp);
    // OEE OPER/CELL
    addSettingRow(&oee_cell_oper_menu, &oee_cp_disp);
    addSettingRow(&oee_cell_oper_menu, &oee_cp_rend);
    addSettingRow(&oee_cell_oper_menu, &oee_cp_qual);
    addSettingRow(&oee_cell_oper_menu, &oee_cp_oee);
    addSettingRow(&oee_cell_oper_menu, &oee_cp_computed);
    addSettingRow(&oee_cell_oper_menu, &oee_cp_tt_adj);
    // OEE ROBOT
    addSettingRow(&oee_robot_menu, &oee_rbt_disp);
    addSettingRow(&oee_robot_menu, &oee_rbt_rend);
    addSettingRow(&oee_robot_menu, &oee_rbt_qual);
    addSettingRow(&oee_robot_menu, &oee_rbt_oee);
    addSettingRow(&oee_robot_menu, &oee_rbt_computed);
    addSettingRow(&oee_robot_menu, &oee_rbt_tt_adj);
    // PROD OPER
    addSettingRow(&prod_oper_menu, &prod_oper_oraria);
    addSettingRow(&prod_oper_menu, &prod_oper_turno);
    addSettingRow(&prod_oper_menu, &prod_oper_giornaliera);
    addSettingRow(&prod_oper_menu, &prod_oper_mensile);
    // PROD ROBOT
    addSettingRow(&prod_robot_menu, &prod_rbt_oraria);
    addSettingRow(&prod_robot_menu, &prod_rbt_turno);
    addSettingRow(&prod_robot_menu, &prod_rbt_giornaliera);
    addSettingRow(&prod_robot_menu, &prod_rbt_mensile);
    // CELL DATA
    addSettingRow(&celldata_menu, &cd_celltimes);
    addSettingRow(&celldata_menu, &cd_domanda);
    addSettingRow(&celldata_menu, &cd_tSetup);
    addSettingRow(&celldata_menu, &cd_tManut);
    addSettingRow(&celldata_menu, &cd_tGuasti);
    addSettingRow(&celldata_menu, &cd_ppmOper);
    addSettingRow(&celldata_menu, &cd_ppmRobot);
    addSettingRow(&celldata_menu, &cd_takt_time);
    addSettingRow(&celldata_menu, &cd_macchine);
    addSettingRow(&celldata_menu, &cd_n_macchine);
    addSettingRow(&celldata_menu, &cd_stazioni);
    addSettingRow(&celldata_menu, &cd_n_stazioni);
    addSettingRow(&celldata_menu, &cd_oee_cell_oper);
    addSettingRow(&celldata_menu, &cd_operatori);
    addSettingRow(&celldata_menu, &cd_n_operatori);
    addSettingRow(&celldata_menu, &cd_tPausa);
    addSettingRow(&celldata_menu, &cd_ineff_oper);
    addSettingRow(&celldata_menu, &cd_oee_robot);
    addSettingRow(&celldata_menu, &cd_robot);
    addSettingRow(&celldata_menu, &cd_n_robot);
    addSettingRow(&celldata_menu, &cd_ineff_robot);
    addSettingRow(&celldata_menu, &cd_rp_oper);
    addSettingRow(&celldata_menu, &cd_rp_robot);
    addSettingRow(&celldata_menu, &cd_prod_oper);
    addSettingRow(&celldata_menu, &cd_prod_robot);
    // GRIPPER MENU
    addSettingRow(&gripper_menu, &grp_mass_obj);
    addSettingRow(&gripper_menu, &grp_mass_grp);
    addSettingRow(&gripper_menu, &grp_coeff_attr);
    addSettingRow(&gripper_menu, &grp_coeff_sic);
    addSettingRow(&gripper_menu, &grp_acceler);
    addSettingRow(&gripper_menu, &grp_alfa);
    addSettingRow(&gripper_menu, &grp_force_fit);
    addSettingRow(&gripper_menu, &grp_vertical);
    addSettingRow(&gripper_menu, &grp_n_griffe);
    addSettingRow(&gripper_menu, &grp_presa_vert);
    addSettingRow(&gripper_menu, &grp_force_tot);
    addSettingRow(&gripper_menu, &grp_force_grif);
    // MAIN MENU
    addSettingRow(&main_menu, &mm_save);
    addSettingRow(&main_menu, &mm_delete);
    addSettingRow(&main_menu, &mm_cella);
    addSettingRow(&main_menu, &mm_gripper);
}

/**************END CHIAMATE GLOBALI************ */

/********************GESTIONE CALLBACKS DEFAULT FUNCTION BAR*********************** */

void handleGetVariable(SettingRow* sr) {
    // TODO
    if (sr == 0) {
        return;
    }
}

void handleSetVariable(SettingRow* sr) {
    if (sr == 0) {
        return;
    }
    switch (sr->type) {
        case CHAR:
            askSignedChar(sr->text, (char*)sr->pointerToValue);
            break;
        case UNSIGNED_CHAR:
            askUnsignedChar(sr->text, (unsigned char*)sr->pointerToValue);
            break;
        case INT:
            askSignedInt(sr->text, (int*)sr->pointerToValue);
            break;
        case UNSIGNED_INT:
            askUnsignedInt(sr->text, (unsigned int*)sr->pointerToValue);
            break;
        case SHORT:
        case UNSIGNED_SHORT:
            return;
        case LONG:
            askSignedLong(sr->text, (long*)sr->pointerToValue);
            break;
        case UNSIGNED_LONG:
            askUnsignedLong(sr->text, (unsigned long*)sr->pointerToValue);
            break;
        case DOUBLE:
            askDouble(sr->text, (double*)sr->pointerToValue);
            break;
        case FLOAT:
            askFloat(sr->text, (float*)sr->pointerToValue);
            break;
        case BOOL: {
            *(bool*)sr->pointerToValue = !(*(bool*)sr->pointerToValue);
            break;
        }
        default:
            return;
    }
}

void handleToggleVariable(SettingRow* sr) {
    if (sr == 0) {
        return;
    }
    if (sr->type == BOOL) {
        *(bool*)sr->pointerToValue = !(*(bool*)sr->pointerToValue);
    }
}

void handleComputeVariable(SettingRow* sr) {
    if (sr == 0 || !(sr->field & COMPUTABLE_FIELD)) {
        return;
    }
    if (as.currentComputeHandler == 0) {
        return;
    }
    void (*computeHandler)(SettingRow*) = as.currentComputeHandler;
    computeHandler(sr);
}

void handleResetArray(SettingRow* sr) {
    if (sr == 0 || sr->type != ARRAY) {  // Se non è un array non lo gestisco
        return;
    }
    // Brutale: devo resettare gli array
    if (sr == &cd_macchine) {
        cd.n_macchine = 0;
    } else if (sr == &cd_robot) {
        cd.n_robot = 0;
    } else if (sr == &cd_operatori) {
        cd.n_operatori = 0;
    } else if (sr == &cd_stazioni) {
        cd.n_stazioni = 0;
    }
}

void handleShowArrayMenu(SettingRow* sr) {
    if (sr == 0 || sr->type != ARRAY) {  // Se non è un array non lo gestisco
        return;
    }
    // Esistono due modi per aprire un Array: un tasto F1-F6 oppure il tasto "EXE".
    // Questa funzione può essere chiamata se viene premuto il tasto F1-F6 corrispondente a cui è assegnata
    // questa funzione di callback: in questo caso qua mi occupo di mostrare il menu Array.
    // Altrimenti se è premuto "EXE" sulla SettingRow corrente: in quel caso il menu principale
    //  deve gestire l'array (la funzione ad esempio: handleMenu_CellData).
    // Qua lo gestisce questa funzione

    // Il puntatore presente nella SettingRow deve essere quello per mostrare il menu.
    if (sr->pointerToValue == 0) {
        return;
    }
    void (*handleArrayMenu)(AppStatus*);
    handleArrayMenu = sr->pointerToValue;
    handleArrayMenu(&as);
    // Mi devo occupare qua di ripulire gli handler connessi
    as.currentArrayMenu = 0;
    as.currentArrayMenuHandler = 0;
    as.currentAddArrayElementHandler = 0;
    as.currentRemoveArrayElementHandler = 0;
    as.currentShowArrayElementHandler = 0;
}

bool handleEditArray(SettingRow* sr) {
    if (as.currentArrayMenu == 0 || as.currentShowArrayElementHandler == 0) {
        return false;
    }
    bool (*editCallback)(ArrayPage* ap);
    editCallback = as.currentShowArrayElementHandler;
    return editCallback(as.currentArrayMenu);
}

bool handleAddArray(SettingRow* sr) {
    if (as.currentArrayMenu == 0 || as.currentAddArrayElementHandler == 0) {
        return false;
    }
    bool (*addCallback)(ArrayPage* ap);
    addCallback = as.currentAddArrayElementHandler;
    return addCallback(as.currentArrayMenu);
}

bool handleRemoveArray(SettingRow* sr) {
    if (as.currentArrayMenu == 0 || as.currentRemoveArrayElementHandler == 0) {
        return false;
    }
    bool (*removeCallback)(ArrayPage* ap);
    removeCallback = as.currentRemoveArrayElementHandler;
    return removeCallback(as.currentArrayMenu);
}

/********************END GESTIONE CALLBACKS DEFAULT FUNCTION BAR*********************** */

/*****************************GESTIONE COMUNE MENU**************************** */

void handleFunctionKey(int* key) {
    if (key == 0 || as.currentMenu == 0 || as.currentFBar == 0) {
        return;
    }
    unsigned char selection = as.currentMenu->selected;
    if (selection >= as.currentMenu->size) {
        return;
    }
    SettingRow* sr = as.currentMenu->rows[selection];
    FunctionBar* fb = as.currentFBar;
    // Prendo l'indice del tasto F1-F6 e li mappo tra 0 e 5, inoltre se ho premuto il tasto EXE posso usarlo
    // come shortcut per il tasto F1
    int index = *key - KEY_CTRL_F1;
    if (*key == KEY_CTRL_EXE) {
        index = 0;
    }
    if (index < 0 || index > 5) {
        return;
    }
    FunctionBarElement* fbe = &fb->options[index];
    if (fbe->callback == 0) {  // se non ho una funzione di callback non faccio nulla
        return;
    }
    void (*functionHandler)(SettingRow*);
    functionHandler = fbe->callback;
    functionHandler(sr);
}

void handleDrawFunctionBar() {
    if (as.currentMenu == 0) {
        drawBar(0, true);
        return;
    }
    unsigned char selection = as.currentMenu->selected;
    if (selection >= as.currentMenu->size) {
        return;
    }
    SettingRow* sr = as.currentMenu->rows[selection];
    if (sr->type == PAGE_CHANGE) {  // Non disegno nulla per questa tipo di opzione
        drawBar(0, true);
        return;
    } else if (sr->type == ARRAY) {
        as.currentFBar = &FB_handle_array_INPUT;
        drawBar(as.currentFBar, false);
        return;
    }
    switch (sr->field) {
        case INPUT_FIELD: {
            if (sr->type >= CHAR && sr->type <= FLOAT) {
                as.currentFBar = &FB_handle_number_INPUT;
            } else if (sr->type == BOOL) {
                as.currentFBar = &FB_handle_bool_INPUT;
            }
        } break;
        case INPUT_FIELD | COMPUTABLE_FIELD: {
            if (sr->type >= CHAR && sr->type <= FLOAT) {
                as.currentFBar = &FB_handle_number_COMPUTE_INPUT;
            } else if (sr->type == BOOL) {
                as.currentFBar = &FB_handle_bool_COMPUTE_INPUT;
            }
        } break;
        case OUTPUT_FIELD: {
            as.currentFBar = &FB_handle_OUTPUT;
        } break;
        case OUTPUT_FIELD | COMPUTABLE_FIELD: {
            as.currentFBar = &FB_handle_COMPUTE;
        } break;
        default:
            as.currentFBar = 0;
            break;
    }
    drawBar(as.currentFBar, as.currentFBar == 0);
    Bdisp_PutDisp_DD();
}

void handleEditVariable(unsigned char value) {
    if (as.currentMenu == 0) {
        drawBar(0, true);
        return;
    }
    unsigned char selection = as.currentMenu->selected;
    if (selection >= as.currentMenu->size) {
        return;
    }
    SettingRow* sr = as.currentMenu->rows[selection];
    if (sr == 0) {
        return;
    }
    if (sr->type >= CHAR && sr->type <= FLOAT) {
        // Keyboard_PutKeycode(-1, -1, keycode); //purtroppo non va
        //*(char*)sr->pointerToValue = value;
        handleSetVariable(sr);
    }
}

// Per gli array gli elementi non sono SettingRow: devo disegnare le opzioni: add, remove e edit (ovvero display/mostra)
static void handleDrawFunctionBarArray() {
    if (as.currentArrayMenuHandler == 0) {
        drawBar(0, true);
        return;
    }
    as.currentFBar = &FB_handle_array_element_INPUT;
    drawBar(&FB_handle_array_element_INPUT, false);
    Bdisp_PutDisp_DD();
}

static void handleFunctionKeyArray(int* key) {
    if (key == 0 || as.currentArrayMenu == 0 || as.currentFBar == 0) {
        return;
    }
    unsigned char selection = as.currentArrayMenu->selected;
    if (selection > *as.currentArrayMenu->length) {  // Se selection è 0 e length = 0 allora devo permettere "aggiungi"
        return;
    }

    FunctionBar* fb = as.currentFBar;
    // Prendo l'indice del tasto F1-F6 e li mappo tra 0 e 5, inoltre se ho premuto il tasto EXE posso usarlo
    // come shortcut per il tasto F1
    int index = *key - KEY_CTRL_F1;
    if (*key == KEY_CTRL_EXE) {
        index = 0;
    }
    if (index < 0 || index > 5) {
        return;
    }
    FunctionBarElement* fbe = &fb->options[index];
    if (fbe->callback == 0) {  // se non ho una funzione di callback non faccio nulla
        return;
    }
    void (*functionHandler)(SettingRow*);
    functionHandler = fbe->callback;
    functionHandler(0);  // Alle funzioni degli array non serve la SettingRow, è tenuta solo per compatibilità con i menu
}

/*****************************END GESTIONE COMUNE MENU**************************** */

// *********************** MENU CELL TIMES********************************

static void handleCompute_CellTimes(SettingRow* sr) {
    if (sr == 0) {
        return;
    }
    if (sr == &ct_tTot) {
        // Devo resettarlo così la funzione lo calcola nuovamente
        cd.times.total_th_time = 0;
        getTotalTime(&cd.times);
    } else if (sr == &ct_tTotDisp) {
        // Ricalcolo il tempo disponibile
        cd.times.total_disp_time = cd.times.total_th_time - getUnavailableTime(&cd);
    }
}

void handleMenu_CellTimes(AppStatus* as) {
    as->currentMenu = &celltimes_menu;
    as->currentMenuHandler = handleMenu_CellTimes;
    as->currentComputeHandler = handleCompute_CellTimes;

    // Non ho array da gestire: rimuovo i riferimenti
    as->currentArrayMenuHandler = 0;
    as->currentAddArrayElementHandler = 0;
    as->currentRemoveArrayElementHandler = 0;
    as->currentShowArrayElementHandler = 0;

    int key;
    bool ret = 0;
    handleDrawFunctionBar();
    while (true) {
        ret = handleSettingsPage(&celltimes_menu, &key, handleFunctionKey, handleDrawFunctionBar, handleEditVariable);
        if (!ret) {  // utente ha premuto "EXIT"
            return;
        }
        // Controllo se ha premuto "EXE" su un campo "COMPUTED" allora devo ricalcolare o "ARRAY" ed allora apro la pagina associata
        handleFunctionKey(&key);
    }
}

// *********************** MENU OEE CELL/OPER *****************************

static void handleCompute_OEE_Cell(SettingRow* sr) {
    if (sr == 0) {
        return;
    }
    if (sr == &oee_cp_oee) {
        // Devo resettarlo così la funzione lo calcola nuovamente
        cd.cell_or_operatorsOEE.computed = false;
        computeOEE(&cd, OPERATOR_SOLVER);
    } else if (sr == &oee_cp_disp) {
        computeDisp(&cd, OPERATOR_SOLVER);
    } else if (sr == &oee_cp_rend) {
        computeRend(&cd, OPERATOR_SOLVER);
    } else if (sr == &oee_cp_qual) {
        computeQual(&cd, OPERATOR_SOLVER);
    } else if (sr == &oee_cp_tt_adj) {
        cd.cell_or_operatorsOEE.TT_adj = 0.0;
        getAdjTaktTime(&cd, OPERATOR_SOLVER);
    }
}

void handleMenu_OEE_Cell(AppStatus* as) {
    as->currentMenu = &oee_cell_oper_menu;
    as->currentMenuHandler = handleMenu_OEE_Cell;
    as->currentComputeHandler = handleCompute_OEE_Cell;

    // Non ho array da gestire: rimuovo i riferimenti
    as->currentArrayMenuHandler = 0;
    as->currentAddArrayElementHandler = 0;
    as->currentRemoveArrayElementHandler = 0;
    as->currentShowArrayElementHandler = 0;

    int key;
    bool ret = 0;
    handleDrawFunctionBar();
    while (true) {
        ret = handleSettingsPage(&oee_cell_oper_menu, &key, handleFunctionKey, handleDrawFunctionBar, handleEditVariable);
        if (!ret) {  // utente ha premuto "EXIT"
            return;
        }
        // Controllo se ha premuto "EXE" su un campo "COMPUTED" allora devo ricalcolare o "ARRAY" ed allora apro la pagina associata
        handleFunctionKey(&key);
    }
}

// *********************** MENU OEE ROBOT *****************************

static void handleCompute_OEE_Robot(SettingRow* sr) {
    if (sr == &oee_rbt_oee) {
        // Devo resettarlo così la funzione lo calcola nuovamente
        cd.robotsOEE.computed = false;
        computeOEE(&cd, ROBOT_SOLVER);
    } else if (sr == &oee_rbt_disp) {
        computeDisp(&cd, ROBOT_SOLVER);
    } else if (sr == &oee_rbt_rend) {
        computeRend(&cd, ROBOT_SOLVER);
    } else if (sr == &oee_rbt_qual) {
        computeQual(&cd, ROBOT_SOLVER);
    } else if (sr == &oee_rbt_tt_adj) {
        cd.robotsOEE.TT_adj = 0.0;
        getAdjTaktTime(&cd, ROBOT_SOLVER);
    }
}

void handleMenu_OEE_Robot(AppStatus* as) {
    as->currentMenu = &oee_robot_menu;
    as->currentMenuHandler = handleMenu_OEE_Robot;
    as->currentComputeHandler = handleCompute_OEE_Robot;

    // Non ho array da gestire: rimuovo i riferimenti
    as->currentArrayMenuHandler = 0;
    as->currentAddArrayElementHandler = 0;
    as->currentRemoveArrayElementHandler = 0;
    as->currentShowArrayElementHandler = 0;

    int key;
    bool ret = 0;
    handleDrawFunctionBar();
    while (true) {
        ret = handleSettingsPage(&oee_robot_menu, &key, handleFunctionKey, handleDrawFunctionBar, handleEditVariable);
        if (!ret) {  // utente ha premuto "EXIT"
            return;
        }
        // Controllo se ha premuto "EXE" su un campo "COMPUTED" allora devo ricalcolare
        handleFunctionKey(&key);
    }
}

// *********************** MENU PROD OPER *****************************

static void handleCompute_Prod_Oper(SettingRow* sr) {
    if (sr == &prod_oper_giornaliera ||
        sr == &prod_oper_oraria ||
        sr == &prod_oper_turno ||
        sr == &prod_oper_mensile) {
        computeProduttivita(&cd, OPERATOR_SOLVER);
    }
}

void handleMenu_Prod_Oper(AppStatus* as) {
    as->currentMenu = &prod_oper_menu;
    as->currentMenuHandler = handleMenu_Prod_Oper;
    as->currentComputeHandler = handleCompute_Prod_Oper;

    // Non ho array da gestire: rimuovo i riferimenti
    as->currentArrayMenuHandler = 0;
    as->currentAddArrayElementHandler = 0;
    as->currentRemoveArrayElementHandler = 0;
    as->currentShowArrayElementHandler = 0;

    int key;
    bool ret = 0;
    handleDrawFunctionBar();
    while (true) {
        ret = handleSettingsPage(&prod_oper_menu, &key, handleFunctionKey, handleDrawFunctionBar, handleEditVariable);
        if (!ret) {  // utente ha premuto "EXIT"
            return;
        }
        // Controllo se ha premuto "EXE" su un campo "COMPUTED" allora devo ricalcolare
        handleFunctionKey(&key);
    }
}

// *********************** MENU PROD ROBOT *****************************

static void handleCompute_Prod_Robot(SettingRow* sr) {
    if (sr == &prod_rbt_giornaliera ||
        sr == &prod_rbt_oraria ||
        sr == &prod_rbt_turno ||
        sr == &prod_rbt_mensile) {
        computeProduttivita(&cd, ROBOT_SOLVER);
    }
}

void handleMenu_Prod_Robot(AppStatus* as) {
    as->currentMenu = &prod_robot_menu;
    as->currentMenuHandler = handleMenu_Prod_Robot;
    as->currentComputeHandler = handleCompute_Prod_Robot;

    // Non ho array da gestire: rimuovo i riferimenti
    as->currentArrayMenuHandler = 0;
    as->currentAddArrayElementHandler = 0;
    as->currentRemoveArrayElementHandler = 0;
    as->currentShowArrayElementHandler = 0;

    int key;
    bool ret = 0;
    handleDrawFunctionBar();
    while (true) {
        ret = handleSettingsPage(&prod_robot_menu, &key, handleFunctionKey, handleDrawFunctionBar, handleEditVariable);
        if (!ret) {  // utente ha premuto "EXIT"
            return;
        }
        // Controllo se ha premuto "EXE" su un campo "COMPUTED" allora devo ricalcolare
        handleFunctionKey(&key);
    }
}

// ************************** GESTIONE ARRAY MACCHINE***************************

bool handleArray_Macchine_Add(ArrayPage* ap) {
    if (ap == 0 || ap->basePtr == 0) {
        return false;
    }
    //*ap->length equivale a : as.cd->n_macchine
    if (*ap->length >= 10) {  // Ho raggiunto la max dimensione dell'array Macchine
        return false;
    }
    /* //Non c'è bisogno perché già alla creazione riempo con valori default
    Machine* macc = &(as.cd->macchine[as.cd->n_macchine]);
    // Imposto i valori
    macc->operatore.load_time = 1;
    macc->operatore.unload_time = 1;
    macc->robot.load_time = 1;
    macc->robot.unload_time = 1;
    macc->work_time = 1;
    macc->efficienza = 100;  // Corrisponde al 100%
    macc->ppms = 0;          // Corrisponde a 0 ppm prodotti
    */
    // Aumento il numero di macchine presenti
    as.cd->n_macchine++;
    // Seleziono l'ultimo creato
    ap->selected = *ap->length - 1;
    return true;
}

bool handleArray_Macchine_Remove(ArrayPage* ap) {
    if (ap == 0 || ap->basePtr == 0) {
        return false;
    }
    //*ap->length equivale a : as.cd->n_macchine
    if (*ap->length == 0) {  // Ho raggiunto il minimo numero: non ho macchine da rimuovere
        return false;
    }
    unsigned char to_rem = ap->selected;
    for (unsigned int i = to_rem; i < as.cd->n_macchine - 1; i++) {  // Da quella da rimuovere fino alla fine-1, copio la successiva nella precedente
        Machine* current = &(as.cd->macchine[i]);
        Machine* next = &(as.cd->macchine[i + 1]);
        // Copio i valori della successiva nella preceddente
        current->operatore.load_time = next->operatore.load_time;
        current->operatore.unload_time = next->operatore.unload_time;
        current->robot.load_time = next->robot.load_time;
        current->robot.unload_time = next->robot.unload_time;
        current->work_time = next->work_time;
        current->efficienza = next->efficienza;
        current->ppms = next->ppms;
    }
    if (ap->selected > 0) {
        // Seleziono il precedente
        ap->selected--;
    }
    // Diminuisco il numero di macchine presenti
    as.cd->n_macchine--;
    return true;
}

bool handleArray_Macchine_Display(ArrayPage* ap) {
    if (ap == 0 || ap->basePtr == 0) {
        return false;
    }
    if (*ap->length == 0) {  // Non ho elementi, l'utente ha premuto sul vuoto
        return false;
    }
    Machine* m = &cd.macchine[ap->selected];

    message_t msg = "PPM Macchina";
    askUnsignedLong(msg, &m->ppms);

    message_t msg2 = "Eff. Macchina";
    askUnsignedChar(msg2, &m->efficienza);
    return true;
}

void handleArray_Macchine(AppStatus* as) {
    // Imposto gli handler, chi mi chiama li ripulirà all'uscita
    as->currentArrayMenuHandler = handleArray_Macchine;
    as->currentShowArrayElementHandler = handleArray_Macchine_Display;
    as->currentAddArrayElementHandler = handleArray_Macchine_Add;
    as->currentRemoveArrayElementHandler = handleArray_Macchine_Remove;

    // Creo la struttura che gestisce l'array
    ArrayPage ap = {};
    ap.basePtr = &as->cd->macchine[0];
    ap.esize = sizeof(Machine);
    ap.length = &as->cd->n_macchine;
    ap.selected = 0;
    ap.selectedPtr = ap.basePtr;
    ap.elem_prefix = (unsigned char*)"- Macch.";
    as->currentArrayMenu = &ap;

    int key = 0;
    bool ret = 0;
    while (true) {
        ret = handleArrayElementsMenu(&ap, &key, handleFunctionKeyArray, handleDrawFunctionBarArray,
                                      handleArray_Macchine_Display, handleArray_Macchine_Remove, handleArray_Macchine_Add);
        if (!ret) {  // utente ha premuto "EXIT"
            return;
        }
        // Ha premuto "EXE" sull'elemento dell'array, non lo gestisco io ma "handleArray_Macchine_Display"
        handleFunctionKey(&key);
    }
}

// ************************** GESTIONE ARRAY MACCHINE***************************

bool handleArray_Robot_Add(ArrayPage* ap) {
    if (ap == 0 || ap->basePtr == 0) {
        return false;
    }
    //*ap->length equivale a : as.cd->n_macchine
    if (*ap->length >= 10) {  // Ho raggiunto la max dimensione dell'array Macchine
        return false;
    }
    // Aumento il numero di macchine presenti
    as.cd->n_robot++;
    // Seleziono l'ultimo creato
    ap->selected = *ap->length - 1;
    return true;
}

bool handleArray_Robot_Remove(ArrayPage* ap) {
    if (ap == 0 || ap->basePtr == 0) {
        return false;
    }
    //*ap->length equivale a : as.cd->n_macchine
    if (*ap->length == 0) {  // Ho raggiunto il minimo numero: non ho macchine da rimuovere
        return false;
    }
    unsigned char to_rem = ap->selected;
    for (unsigned int i = to_rem; i < as.cd->n_robot - 1; i++) {  // Da quella da rimuovere fino alla fine-1, copio la successiva nella precedente
        RobotData* current = &(as.cd->robots[i]);
        RobotData* next = &(as.cd->robots[i + 1]);
        // Copio i valori della successiva nella preceddente
        current->n_macchine = next->n_macchine;
        current->serve[0] = next->serve[0];
        current->serve[1] = next->serve[1];
        current->serve[2] = next->serve[2];
        current->serve[3] = next->serve[3];
        current->serve[4] = next->serve[4];
    }
    if (ap->selected > 0) {
        // Seleziono il precedente
        ap->selected--;
    }
    // Diminuisco il numero di macchine presenti
    as.cd->n_robot--;
    return true;
}

bool handleArray_Robot_Display(ArrayPage* ap) {
    if (ap == 0 || ap->basePtr == 0) {
        return false;
    }
    if (*ap->length == 0) {  // Non ho elementi, l'utente ha premuto sul vuoto
        return false;
    }
    return true;
}

void handleArray_Robot(AppStatus* as) {
    // Imposto gli handler, chi mi chiama li ripulirà all'uscita
    as->currentArrayMenuHandler = handleArray_Robot;
    as->currentShowArrayElementHandler = handleArray_Robot_Display;
    as->currentAddArrayElementHandler = handleArray_Robot_Add;
    as->currentRemoveArrayElementHandler = handleArray_Macchine_Remove;

    // Creo la struttura che gestisce l'array
    ArrayPage ap = {};
    ap.basePtr = &as->cd->robots[0];
    ap.esize = sizeof(RobotData);
    ap.length = &as->cd->n_robot;
    ap.selected = 0;
    ap.selectedPtr = ap.basePtr;
    ap.elem_prefix = (unsigned char*)"- Robot ";
    as->currentArrayMenu = &ap;

    int key = 0;
    bool ret = 0;
    while (true) {
        ret = handleArrayElementsMenu(&ap, &key, handleFunctionKeyArray, handleDrawFunctionBarArray,
                                      handleArray_Robot_Display, handleArray_Robot_Remove, handleArray_Robot_Add);
        if (!ret) {  // utente ha premuto "EXIT"
            return;
        }
        // Ha premuto "EXE" sull'elemento dell'array, non lo gestisco io ma "handleArray_Robot_Display"
        handleFunctionKey(&key);
    }
}

// ****************************************************

static void handleCompute_CellData(SettingRow* sr) {
    if (sr == &cd_takt_time) {
        cd.TT = 0.0;
        getTaktTime(&cd);
    }
}

static void setupCellDataMenu(AppStatus* as) {
    as->currentMenu = &celldata_menu;
    as->currentMenuHandler = handleMenu_CellData;
    as->currentComputeHandler = handleCompute_CellData;

    // Non ho array da gestire, sto disegnano quello principale: rimuovo i riferimenti
    as->currentArrayMenuHandler = 0;
    as->currentAddArrayElementHandler = 0;
    as->currentRemoveArrayElementHandler = 0;
    as->currentShowArrayElementHandler = 0;

    handleDrawFunctionBar();
}

void handleMenu_CellData(AppStatus* as) {
    setupCellDataMenu(as);
    int key;
    bool ret = 0;
    while (true) {
        ret = handleSettingsPage(&celldata_menu, &key, handleFunctionKey, handleDrawFunctionBar, handleEditVariable);
        if (!ret) {  // utente ha premuto "EXIT"
            return;
        }
        SettingRow* sr = celldata_menu.rows[celldata_menu.selected];
        if (sr->type == PAGE_CHANGE && sr->pointerToValue != 0) {
            void (*menuHandler)(AppStatus* s);
            menuHandler = sr->pointerToValue;
            menuHandler(as);
            // Quando esco dal menu CellTimes/OEE imposto di nuovo i valori di questo menu
            setupCellDataMenu(as);
        } else if (sr->type == ARRAY && sr->pointerToValue != 0) {  // Ho premuto EXE su un array: devo gestire la pagina dell'array
            // Associo la funzione corretta per gestirlo
            void (*arrayHandler)(AppStatus* s);
            arrayHandler = sr->pointerToValue;
            arrayHandler(as);
            // Resetto l'handler del menu una volta uscito
            as->currentArrayMenu = 0;
            as->currentArrayMenuHandler = 0;
            as->currentShowArrayElementHandler = 0;
            as->currentAddArrayElementHandler = 0;
            as->currentRemoveArrayElementHandler = 0;
        } else {
            // Controllo se ha premuto "EXE" su un campo "COMPUTED" allora devo ricalcolare
            handleFunctionKey(&key);
        }
    }
}

// ****************************************************************************

static void handleCompute_Gripper(SettingRow* sr) {
    if (sr == &grp_mass_grp || sr == &grp_force_grif || sr == &grp_force_tot) {
        grp.mass_grip = 0.0;
        computeGripper(&grp, &grp_res);
    }
}

void handleMenu_Gripper(AppStatus* as) {
    as->currentMenu = &gripper_menu;
    as->currentMenuHandler = handleMenu_Gripper;
    as->currentComputeHandler = handleCompute_Gripper;

    // Non ho array da gestire, sto disegnano quello principale: rimuovo i riferimenti
    as->currentArrayMenuHandler = 0;
    as->currentAddArrayElementHandler = 0;
    as->currentRemoveArrayElementHandler = 0;
    as->currentShowArrayElementHandler = 0;

    handleDrawFunctionBar();

    int key;
    bool ret = 0;
    while (true) {
        ret = handleSettingsPage(&gripper_menu, &key, handleFunctionKey, handleDrawFunctionBar, handleEditVariable);
        if (!ret) {  // utente ha premuto "EXIT"
            return;
        }
        // Controllo se ha premuto "EXE" su un campo "COMPUTED" allora devo ricalcolare
        handleFunctionKey(&key);
    }
}

// ******************************************************

static void setupMainMenu(AppStatus* as) {
    as->currentMenu = &main_menu;
    as->currentMenuHandler = handleMainMenu;
    as->currentComputeHandler = 0;

    // Non ho array da gestire, sto disegnano quello principale: rimuovo i riferimenti
    as->currentArrayMenuHandler = 0;
    as->currentAddArrayElementHandler = 0;
    as->currentRemoveArrayElementHandler = 0;
    as->currentShowArrayElementHandler = 0;
}

void handleMainMenu(AppStatus* as) {
    setupMainMenu(as);
    handleDrawFunctionBar();
    int key;
    bool ret = 0;
    while (true) {
        ret = handleSettingsPage(&main_menu, &key, 0, 0, 0);
        if (!ret) {  // utente ha premuto "EXIT"
            return;
        }
        SettingRow* sr = main_menu.rows[main_menu.selected];
        if (sr->type == PAGE_CHANGE && sr->pointerToValue != 0) {
            void (*menuHandler)(AppStatus* s);
            menuHandler = sr->pointerToValue;
            menuHandler(as);
            // Quando esco dal menu CellTimes/OEE imposto di nuovo i valori di questo menu
            setupMainMenu(as);
        } else if (sr->type == PAGE_CHANGE) {
            if (sr == &mm_save) {
                saveToFile(as);
            } else if (sr == &mm_delete) {
                deleteFile(as);
            }
        }
    }
}