#pragma once

#ifndef H_INCLUDED_DATATYPES 
#define H_INCLUDED_DATATYPES 1

#include "commons.h"

typedef struct {
	unsigned char ore_turno;
	unsigned char turni;
	unsigned char gg_mese;
	//Tempo totale teorico in secondi
	unsigned long int total_th_time;
	//Tempo totale disponibile per produzione, in secondi (usato per la disponibilitò)
	unsigned long int total_disp_time;

	/*Questo non posso averlo qua salvato perchò dipende se ho i robot o gli operatori*/
	//Tempo totale netto disponibile per produzione, in secondi (usato per il rendimento)
	//unsigned long int total_net_disp_time;
}CellTimes;

//Rappresenta i tempi macchina di scarico e carico
typedef struct {
	//Tempo di caricamento in secondi
	unsigned char load_time;
	//Tempo di scaricamento in secondi
	unsigned char unload_time;
}MachineTimes;

//Rappresenta i tempi di una macchina, con distinzione tra robot e operatore
typedef struct {
	//tempi dell'operatoredi carico e scarico
	MachineTimes operatore;
	//tempi del robot di carico e scarico
	MachineTimes robot;
	//Tempo di lavoro della macchina in secondi
	unsigned char work_time;
	//Efficienza della singola macchina in percentuale 0 - 100%
	unsigned char efficienza;
	//PPM della singola macchina
	unsigned long ppms;
}Machine;

//Rappresenta i dati di un robot
typedef struct {
	//Array di massimo 5 macchine che il robot può asservire  (sono gli indici dell'array CellData.macchine)
	unsigned char serve[5];
	//Quante macchine effettivamente serve
	unsigned char n_macchine;
} RobotData;

//Rappresenta i dati di un operatore
typedef struct {
	//Array di massimo 5 macchine che l'operatore può asservire (sono gli indici dell'array CellData.macchine)
	unsigned char serve[5];
	//Quante macchine effettivamente serve
	unsigned char n_macchine;
} OperatorData;

typedef struct {
	double disponibilita;
	double rendimento;
	double qualita;

	double OEE;
	bool computed;
}OEE;

typedef struct {
	double oraria;
	unsigned long int turno;
	unsigned long int giornaliera;
	unsigned long int mensile;
}Produttivita;

typedef struct {
	//Contiene gli ID delle macchine in parallelo: 11 = non c'ò
	unsigned char parallele[5];
	//Indica il numero di macchine in parallelo (sicuramente >=1 poichò la macchina stessa ò in parallelo)
	unsigned char n_macchine;
}MachineFlow;

typedef struct {
	CellTimes times;
	unsigned long int domanda_pezzi;//pz
	unsigned long int tempo_setup;//secondi
	unsigned long int tempo_manutenzione;//secondi
	unsigned long int tempo_guasti;//secondi
	//PPM totali della cella considerando gli operatori
	unsigned long int ppm_cella_oper;
	//PPM totali della cella considerando i robot
	unsigned long int ppm_cella_robot;

	/*********************************
				MACCHINE
	**********************************/
	Machine macchine[10];
	//Serve per salvare la stazioni delle macchine, l'ordine indica le macchine in sequenza
	MachineFlow stazioni[10];
	unsigned char n_macchine; //numero di macchine presenti
	unsigned char n_stazioni; //numero di stazioni presenti (una stazione può avere più macchine in parallelo)

	//Nel caso in cui sia fornita un'unica efficenza della cella
	unsigned char efficienza;


	/*********************************
				OPERATORI
	**********************************/
	//OEE della cella o operatori (nel caso in cui sia specificato degli operatori)
	OEE cell_or_operatorsOEE;
	OperatorData operatori[5];
	unsigned char n_operatori;//numero operatori presenti
	unsigned long int tempo_pausa;//Tempo pausa mensile
	unsigned char ineffiecenza_operatore; //Generalmente il 4%

	/*********************************
				ROBOTS
	**********************************/
	OEE robotsOEE;
	RobotData robots[5];
	unsigned char n_robot;//numero robot presenti
	unsigned char ineffiecenza_robot; //Generalmente lo 0%

	/*********************************
				RISULTATI
	**********************************/
	unsigned int RP_oper; //Ritmo produttivo della cella con operatore (s/pz)
	unsigned int RP_robot; //Ritmo produttivo della cella con robot (s/pz)
	Produttivita prod_robot;//Quella utilizzando solo RP e i tempi
	Produttivita prod_oper;//Quella che utilizza anche l'OEE
}CellData;

#define CREATE_CELL_TIMES() {8, 2, 22, 0l ,0l}
#define CREATE_MACHINE_TIMES() { 5,5 }
#define CREATE_MACHINE() {CREATE_MACHINE_TIMES(),CREATE_MACHINE_TIMES(),10,100}
#define CREATE_ROBOT_DATA() {{11,11,11,11,11}, 0 }
#define CREATE_OPERATOR_DATA() {{ 11,11,11,11,11}, 0 }
#define CREATE_OEE() {1.0, 1.0, 1.0, 0.0, false}
#define CREATE_PRODUTTIVITA() {0.0, 0, 0, 0}
#define CREATE_MACHINE_FLOW() {{11,11,11,11,11}, 0}
#define CREATE_CELL() {CREATE_CELL_TIMES(),\
		0, 0, 0, 0, 0, 0,\
		{CREATE_MACHINE(),CREATE_MACHINE(),CREATE_MACHINE(),CREATE_MACHINE(),CREATE_MACHINE(),CREATE_MACHINE(),CREATE_MACHINE(),CREATE_MACHINE(),CREATE_MACHINE(),CREATE_MACHINE()}, /*Macchine*/\ 
		{CREATE_MACHINE_FLOW(),CREATE_MACHINE_FLOW(),CREATE_MACHINE_FLOW(),CREATE_MACHINE_FLOW(),CREATE_MACHINE_FLOW(),CREATE_MACHINE_FLOW(),CREATE_MACHINE_FLOW(),CREATE_MACHINE_FLOW(),CREATE_MACHINE_FLOW(),CREATE_MACHINE_FLOW()},\
		0, 0,\
		100, /*Efficienza*/\ 
		CREATE_OEE(), /*Dati operatori*/\
		{CREATE_OPERATOR_DATA(),CREATE_OPERATOR_DATA(),CREATE_OPERATOR_DATA(),CREATE_OPERATOR_DATA(),CREATE_OPERATOR_DATA()},\
		0,0,\
		4, /*Inefficienza operatore*/\
		CREATE_OEE(), /*Dati robot*/\
		{CREATE_OPERATOR_DATA(),CREATE_OPERATOR_DATA(),CREATE_OPERATOR_DATA(),CREATE_OPERATOR_DATA(),CREATE_OPERATOR_DATA()},\
		0, 0, /*Risultati*/\
		0, /*RP*/\
		CREATE_PRODUTTIVITA(),CREATE_PRODUTTIVITA()}

#endif