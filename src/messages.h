#pragma once

// 1.Definire le funzioni per creare una struttura pulita
// 2.Definire delle funzioni per resettare i valori calcolati (tipo i tempi disponibili calcolati)
// 3.Definire delle funzioni per aggiungere, rimuovere e gestire array
// 4.Definire delle funzioni per impostare i valori

// TODO:
// Gestione salvataggio file della sessione: salvare la struttura della CellData
// Aggiungere una gestione errori con dei messaggi (setto una variabile che contiene il codice errore ad ogni funzione)

#ifndef H_PROBLEM_FLOW_INCLUDED
#define H_PROBLEM_FLOW_INCLUDED 1

#include "commons.h"

#define UNKNOWN_ERROR 0x0000
#define GENERIC_ERROR 0x0001
#define SOLVER_ERROR 0x0002

typedef unsigned int messagecode_t;
typedef const unsigned char* message_t;

#define MAX_MSG_LENGTH 16

#define N_ERRORS_TEXTS 10
#define N_FB_TEXTS 10
#define N_FILE_TEXTS 4
#define N_CELL_TIMES_TEXTS 5
#define N_OEE_TEXTS 6
#define N_PROD_TEXTS 4
#define N_CELL_DATA_TEXTS 26
#define N_CELL_TEXTS() N_CELL_TIMES_TEXTS + N_CELL_DATA_TEXTS 

#define N_GRIPPER_TEXTS 12

#define N_MAIN_MENU_TEXTS 5

#define N_TEXTS 4

extern const unsigned char main_menu_texts[N_MAIN_MENU_TEXTS][MAX_MSG_LENGTH]; 

/**
 * @brief Testi usati per gli errori
 */
extern const unsigned char errors_texts[N_ERRORS_TEXTS][MAX_MSG_LENGTH];
/**
 * @brief Testi usati nei blocchi delle funzioni F1-F6
 */
extern const unsigned char file_texts[N_FILE_TEXTS][MAX_MSG_LENGTH];
/**
 * @brief Testi usati nei blocchi delle funzioni F1-F6
 */
extern const unsigned char fb_texts[N_FB_TEXTS][MAX_MSG_LENGTH];
/**
 * @brief Testi per gestire la struct CellTimes
 */
extern const unsigned char celltimes_texts[N_CELL_TIMES_TEXTS][MAX_MSG_LENGTH];
/**
 * @brief Testi per gestire la struct OEE
 */
extern const unsigned char oee_texts[N_OEE_TEXTS][MAX_MSG_LENGTH];
/**
 * @brief Testi per gestire la struct Produttività
 */
extern const unsigned char prod_texts[N_PROD_TEXTS][MAX_MSG_LENGTH];
/**
 * @brief Testi per gestire la struct CellData
 */
extern const unsigned char celldata_texts[N_CELL_DATA_TEXTS][MAX_MSG_LENGTH];

extern const unsigned char gripper_texts[N_GRIPPER_TEXTS][MAX_MSG_LENGTH];

#define T_FILE_CREA file_texts[0]
#define T_FILE_CARICA file_texts[1]
#define T_FILE_SALVA file_texts[2]
#define T_FILE_MODIFICA file_texts[3]

#define T_FBAR_SET fb_texts[0]
#define T_FBAR_GET fb_texts[1]
#define T_FBAR_ADD fb_texts[2]
#define T_FBAR_REMOVE fb_texts[3]
#define T_FBAR_INIT fb_texts[4]
#define T_FBAR_TOGGLE fb_texts[5]
#define T_FBAR_EMPTY fb_texts[6]
#define T_FBAR_COMPUTE fb_texts[7]
#define T_FBAR_EDIT fb_texts[8]
#define T_FBAR_ARRAY_OPEN fb_texts[9]


#define T_CELL_TIMES_ORE_TURNO celltimes_texts[0]
#define T_CELL_TIMES_TURNI_GG celltimes_texts[1]
#define T_CELL_TIMES_GG_MESE celltimes_texts[2]
#define T_CELL_TIMES_T_TOT celltimes_texts[3]
#define T_CELL_TIMES_T_DISP celltimes_texts[4]


#define T_OEE_DISP oee_texts[0]
#define T_OEE_REND oee_texts[1]
#define T_OEE_QUAL oee_texts[2]
#define T_OEE_OEE oee_texts[3]
#define T_OEE_COMPUTED oee_texts[4]
#define T_OEE_TT_ADJ oee_texts[5]


#define T_PROD_ORARIA prod_texts[0]
#define T_PROD_TURNO prod_texts[1]
#define T_PROD_GIORN prod_texts[2]
#define T_PROD_MESE prod_texts[3]


#define T_CELL_DATA_TEMPI celldata_texts[0]
#define T_CELL_DATA_DOMANDA celldata_texts[1]
#define T_CELL_DATA_T_SETUP celldata_texts[2]
#define T_CELL_DATA_T_MANUT celldata_texts[3]
#define T_CELL_DATA_T_GUASTI celldata_texts[4]
#define T_CELL_DATA_PPM_OPER celldata_texts[5]
#define T_CELL_DATA_PPM_ROBOT celldata_texts[6]
#define T_CELL_DATA_MACCHINE_ARR celldata_texts[7]
#define T_CELL_DATA_N_MACCHINE celldata_texts[8]
#define T_CELL_DATA_STAZIONI_ARR celldata_texts[9]
#define T_CELL_DATA_N_STAZIONI celldata_texts[10]
#define T_CELL_DATA_EFF_CELLA celldata_texts[11]
#define T_CELL_DATA_OEE_CELLA_OPER celldata_texts[12]
#define T_CELL_DATA_OPERATORI_ARR celldata_texts[13]
#define T_CELL_DATA_N_OPERATORI celldata_texts[14]
#define T_CELL_DATA_T_PAUSA celldata_texts[15]
#define T_CELL_DATA_INEFF_OPER celldata_texts[16]
#define T_CELL_DATA_OEE_ROBOT celldata_texts[17]
#define T_CELL_DATA_ROBOT_ARR celldata_texts[18]
#define T_CELL_DATA_N_ROBOT celldata_texts[19]
#define T_CELL_DATA_INEFF_ROBOT celldata_texts[20]
#define T_CELL_DATA_RP_OPER celldata_texts[21]
#define T_CELL_DATA_RP_ROBOT celldata_texts[22]
#define T_CELL_DATA_PROD_OPER celldata_texts[23]
#define T_CELL_DATA_PROD_ROBOT celldata_texts[24]
#define T_CELL_DATA_TAKT_TIME celldata_texts[25]

#define T_GRIPPER_MASS_OBJ gripper_texts[0]
#define T_GRIPPER_MASS_GRIPPER gripper_texts[1]
#define T_GRIPPER_COEFF_ATTR gripper_texts[2]
#define T_GRIPPER_COEFF_SICUR gripper_texts[3]
#define T_GRIPPER_ACCELER gripper_texts[4]
#define T_GRIPPER_ALFA_GRADI gripper_texts[5]
#define T_GRIPPER_FORCE_FIT gripper_texts[6]
#define T_GRIPPER_VERTICAL gripper_texts[7]
#define T_GRIPPER_N_GRIFFE gripper_texts[8]
#define T_GRIPPER_F_TOT gripper_texts[9]
#define T_GRIPPER_F_GRIFFA gripper_texts[10]
#define T_GRIPPER_PRESA_VERTICALE gripper_texts[11]

#define T_MAIN_MENU_SAVE main_menu_texts[0]
#define T_MAIN_MENU_DELETE main_menu_texts[1]
#define T_MAIN_MENU_CELL main_menu_texts[2]
#define T_MAIN_MENU_GRIPPER main_menu_texts[3]

//base_pointer, max_text_size, size
typedef struct {
    //Puntatore di base all'array
    const unsigned char* base_pointer;
    //Dimensione massima delle stringhe puntate
    unsigned char max_text_size;
    //Indica il numero di elementi presenti
    unsigned char size;
} TextElement;

// Array che contiene tutti i testi dei messaggi/menu da mostrare
extern const TextElement texts[N_TEXTS];

/*
const unsigned char *const getStringPointer(messagecode_t message_code);
*/

#endif  // !H_PROBLEM_FLOW_INCLUDED