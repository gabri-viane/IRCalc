#pragma once
#ifndef H_SOLVER_INCLUDED
#define H_SOLVER_INCLUDED 1

#include "commons.h"
#include "datatypes.h"

#define SOLVER_TYPE unsigned char
//CELL_SOLVER non tiene conto dell'inefficienza e delle pause operatori
#define CELL_SOLVER 0
#define OPERATOR_SOLVER 1
#define ROBOT_SOLVER 2

double getTotalTime(CellTimes* times);

/*
Devo controllare se ho delle macchine in parallelo come configurazione della cella in quanto
in questo caso il TaktTime viene calcolato dividendo la domanda sulle singole macchine.

Nel caso di un sistema con macchine in serie o mischiate tra serie e parallelo, restituisce
la domanda totale. Se il sistema è composto da sole macchine in serie allora la domanda viene
divisa per il numero di macchine in parallelo.
*/
unsigned long int getDomandaSingolaMacchina(CellData* cell);

/*
Calcola il TaktTime della cella in base al tempo teorico totale disponibile 
e la domanda attesa
*/
double getTaktTime(CellData* cell);

/*
Calcola l'Adjusted Takt Time, usa l'OEE calcolato, e dato che può differire da quello della
cella con operatori e quella con i robot bisogna specificare che solutore usare:
- CELL_SOLVER (0) nel caso di OEE generico della cella
- OPERATOR_SOLVER (1) nel caso di OEE dovuto agli operatori
- ROBOT_SOLVER (2) nel caso di OEE dovuto ai robot
*/
double getAdjTaktTime(CellData* cell,SOLVER_TYPE solver);

/*
Calcola il tempo di non-disponibilità della cella: questo tempo è calcolato
usando il tempo di setup, di manuetenzione e di guasto
*/
double getUnavailableTime(CellData* cell);

/*
Calcola la disponibilità a seconda del solutore da usare:
- CELL_SOLVER (0) nel caso di cella operata da operatori: in questo caso sono ignorati i tempi pausa e inefficienza operatore
- OPERATOR_SOLVER (1) nel caso di cella operata da operatori
- ROBOT_SOLVER (2) nel caso di cella operata da robot
*/
void computeDisp(CellData* cell, SOLVER_TYPE solver);
/*
Calcola il rendimento a seconda del solutore da usare:
- CELL_SOLVER (0) nel caso di cella operata da operatori: in questo caso sono ignorati i tempi pausa e inefficienza operatore
- OPERATOR_SOLVER (1) nel caso di cella operata da operatori
- ROBOT_SOLVER (2) nel caso di cella operata da robot
*/
void computeRend(CellData* cell, SOLVER_TYPE solver);
/*
Calcola la qualità a seconda del solutore da usare:
- CELL_SOLVER (0) nel caso di cella operata da operatori: in questo caso sono ignorati i tempi pausa e inefficienza operatore
- OPERATOR_SOLVER (1) nel caso di cella operata da operatori
- ROBOT_SOLVER (2) nel caso di cella operata da robot
*/
void computeQual(CellData* cell, SOLVER_TYPE solver);

/*
Calcola l'OEE usando i valori presenti nei campi della struttura OEE. Se risulta già calcolato
ovvero il valore "OEE.computed == true" allora viene restituito il valore calcolato precedentemente
senza ricalcolarlo.
Viene calcolato l'OEE secondo il solver specificato:
- CELL_SOLVER (0) nel caso di cella operata da operatori: in questo caso sono ignorati i tempi pausa e inefficienza operatore
- OPERATOR_SOLVER (1) nel caso di cella operata da operatori
- ROBOT_SOLVER (2) nel caso di cella operata da robot
*/
void computeOEE(CellData* cell, SOLVER_TYPE solver);

/*
Calcola la produttività teorica e effettiva (che tiene conto dell'OEE) a secondo del
solutore scelto:
- CELL_SOLVER (0) nel caso di OEE generico della cella
- OPERATOR_SOLVER (1) nel caso di OEE dovuto agli operatori
- ROBOT_SOLVER (2) nel caso di OEE dovuto ai robot
*/
void computeProduttivita(CellData* cell, SOLVER_TYPE solver);


void computeGripper(Gripping* gp,GrippingResult* gr);

#endif // !H_SOLVER_INCLUDED
