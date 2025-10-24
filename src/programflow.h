#pragma once

#ifndef H_PROGRAMFLOW_INCLUDED
#define H_PROGRAMFLOW_INCLUDED 1

#include "commons.h"
#include "datatypes.h"
#include "gui.h"

extern CellData cd;

typedef struct {
    unsigned int currentSubMenu;
} MenuStatus;

typedef struct {
    CellData* cd;
    res_error_t last_error;
    bool toBeSaved;

    // Gestione timer
    bool timerRunning;
    int timerID;

    // Pagina corrente mostrata, se 0 allora non c'è mostrata nessuna SettingsPage
    SettingsPage* currentMenu;
    // FunctionBar corrente mostrata, se 0 allora non c'è nessuna opzione selezionabile tramite F1-F6
    FunctionBar* currentFBar;
    // Funzione corrente che gestisce il ciclo del menu (la pagina, SettingsPage o altro)
    void* currentMenuHandler;
    // Funzione corrente che gestisce il tasto esegui dell'elemento selezionato nella pagina
    void* currentComputeHandler;

    // Pagina corrente mostrata, se 0 allora non c'è mostrata nessuna ArrayPage
    ArrayPage* currentArrayMenu;
    /* Funzione corrente che gestisce il menu degli array (aggiunta e rimozione).
    Prende come parametro la struct AppStatus stessa
    */
    void* currentArrayMenuHandler;
    // Funzione corrente che si deve occupare l'elemento selezionato dell'array: bool (*fun)(ArrayPage* ap)
    void* currentShowArrayElementHandler;
    // Funzione corrente che si deve occupare di aggiungere un nuovo elemento, restituisce un bool se l'elemento è stato aggiunto o meno e prende in input un puntatore ArrayPage
    void* currentAddArrayElementHandler;
    // Funzione corrente che si deve occupare di rimuovere un elemento, restituisce un bool se l'elemento è stato aggiunto o meno e prende in input un puntatore ArrayPage
    void* currentRemoveArrayElementHandler;
    // Puntatore al dato corrente da modificare
    void* dataPointer;
} AppStatus;

AppStatus* getAppStatus();

/**
 * @brief Serve per aggiungere le impostazioni, i campi, completare i valori
 * per i vari menu ed elementi dei menu
 */
void initMenus();
/**
 * @brief Gestisce l'opzione "Get" dei tasti F1-F6, chiamando la funzione adatta per richiedere il valore
 * Questa funzione è generica e relativa all'opzione presente nella struct.
 * Copia negli appunti il valore selezionato
 * @param sr La settings Row di cui fare il get
 */
void handleGetVariable(SettingRow* sr);

/**
 * @brief Gestisce l'opzione "Set" dei tasti F1-F6, chiamando la funzione adatta per richiedere il valore (funzioni "ask...")
 * Questa funzione è generica e relativa all'opzione presente nella struct
 * @param sr La settings Row di cui fare il set
 */
void handleSetVariable(SettingRow* sr);
/**
 * @brief
 *
 * @param sr
 */
void handleToggleVariable(SettingRow* sr);
void handleComputeVariable(SettingRow* sr);
/**
 * @brief Usa il puntatore presente nell'elemento SettingRow sr.
 * Questa funzione viene chiamata solo da callback delle function bar.
 * Questa funzione imposta a 0 il numero degli elementi presenti, in questo modo evita problemi che possono crearsi
 * @param sr La setting Row, di tipo ARRAY, da cui partire per mostrare i menu.
 */
void handleResetArray(SettingRow* sr);
/**
 * @brief Usa il puntatore presente nell'elemento SettingRow sr.
 * Questa funzione viene chiamata solo da callback delle function bar.
 * La funzione chiamata dovrebbe impostare i parametri all'interno della struct AppStatus i valori
 * di:
 *
 * - currentArrayMenuHandler : deve sottoscriversi da sola
 *
 * - currentShowArrayElementHandler : deve sottoscrivere la funzione che mostra l'elemento
 *
 * - currentAddArrayElementHandler : deve sottoscrivere la funzione che permette di aggiungere un nuovo elemento
 *
 * - currentRemoveArrayElementHandler : deve sottoscrivere la funzione che permette di rimuovere un elemento
 *
 * @param sr La setting Row, di tipo ARRAY, da cui partire per mostrare i menu.
 */
void handleShowArrayMenu(SettingRow* sr);

/**
 * @brief Questa funzione è chiamata da un pulsante F1-F6. La funzione "handleArrayElementsMenu" (gui.h) chiama
 * il puntatore contenuto in AppStatus, anche questa funzione chiama il puntatore presente in AppStatus.
 * Chiama la funzione, tramite puntatore currentShowArrayElementHandler, che apre il menu
 * che mostra gli elementi dell'elemento selezionato dell'array. La funzione chiamata gestirà un proprio
 * ciclo menu: bisogna gestire il disegno del menu all'uscita della modifica dell'elemento.
 *
 * @param sr Non mi serve, però sono obblicato per la definizione del callback della FunctionBar
 * @return true Quando l'elemento è stato aperto per la modifica.
 * @return false Se l'elemento non è stato aperto
 */
bool handleEditArray(SettingRow* sr);
bool handleAddArray(SettingRow* sr);
bool handleRemoveArray(SettingRow* sr);

void handleFunctionKey(int* key);

void handleDrawFunctionBar();

void handleEditVariable(unsigned char value);

// ***************************************************************

void handleMenu_CellTimes(AppStatus* as);

// ***************************************************************

void handleMenu_OEE_Cell(AppStatus* as);

// ***************************************************************

void handleMenu_OEE_Robot(AppStatus* as);

// ***************************************************************

void handleMenu_Prod_Oper(AppStatus* as);

// ***************************************************************

void handleMenu_Prod_Robot(AppStatus* as);

//********************************************************

bool handleArray_Macchine_Add(ArrayPage* ap);
bool handleArray_Macchine_Remove(ArrayPage* ap);
bool handleArray_Macchine_Display(ArrayPage* ap);
void handleArray_Macchine(AppStatus* as);

// ***************************************************************

void handleMenu_CellData(AppStatus* as);

/*
ID MENU = 0
Gestisce la scelta delle opzioni iniziali:
-crea cella [Funzione]
-carica cella [Funzione]
-salva cella [Funzione]
-modifica dati [Menu]
Contiene e gestisce tutte le variabili e chiamate del programma
*/
void mainHandler();

#endif