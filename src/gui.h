#pragma once
#ifndef H_GUI_INCULUDED
#define H_GUI_INCLUDED 1

#include <fxcg/display.h>
#include <fxcg/keyboard.h>

#include "commons.h"
#include "messages.h"

/**************************
    FUNZIONI DI SUPPORTO
***************************/

void fillArea(unsigned x, unsigned y, unsigned w, unsigned h, unsigned short col);

/***************************
 * PAGINE IMPOSTAZIONI     *
 ***************************/
#define MAX_LENGTH_SETTINGS 50
#define CREATE_SETTINGS_PAGE() {{}, 0, 0}
#define CREATE_SETTINGS_ROW(text, pointerToValue, type, fieldtype) {text, pointerToValue, type, fieldtype, true}
#define CREATE_SETTINGS_ROW_IN(text, pointerToValue, type) CREATE_SETTINGS_ROW(text, pointerToValue, type, INPUT_FIELD)
#define CREATE_SETTINGS_ROW_IN_CALC(text, pointerToValue, type) CREATE_SETTINGS_ROW(text, pointerToValue, type, INPUT_FIELD | COMPUTABLE_FIELD)
#define CREATE_SETTINGS_ROW_OUT(text, pointerToValue, type) CREATE_SETTINGS_ROW(text, pointerToValue, type, OUTPUT_FIELD)
#define CREATE_SETTINGS_ROW_OUT_CALC(text, pointerToValue, type) CREATE_SETTINGS_ROW(text, pointerToValue, type, OUTPUT_FIELD | COMPUTABLE_FIELD)

// Tipologia di campo di impostazioni
typedef enum {
    INPUT_FIELD = 0b0001,
    OUTPUT_FIELD = 0b0010,
    COMPUTABLE_FIELD = 0b0100
} FieldType;

// Tipo di dato associato all'impostazione
typedef enum {
    BOOL,
    CHAR,
    UNSIGNED_CHAR,
    SHORT,
    UNSIGNED_SHORT,
    INT,
    UNSIGNED_INT,
    LONG,
    UNSIGNED_LONG,
    DOUBLE,
    FLOAT,
    ARRAY,
    STRING,
    PAGE_CHANGE,
    MENU_CHANGE,
} ElementType;

/*
Riga che rappresenta un campo delle impostazioni:
- text : puntatore al testo (dovrebbe essere costante, credo)
- pointerToValue : puntatore al valore associato
- type : tipologia di valore che viene puntato da questa impostazione
- shown : indica se nel menu deve essere mostrata o meno

Se type è PAGE_CHANGE allora il puntatore dovrebbe puntare all'ID della pagina da cambiare
*/
typedef struct {
    const unsigned char* text;
    void* pointerToValue;
    ElementType type;
    unsigned short field;
    bool shown;
} SettingRow;

/**
 * @brief Rappresenta un insieme di impostazioni, precisamente una pagina
 * Contiene anche l'indice dell'opzione selezionata

 - rows : array di puntatori a SettingRow, dovrebbe essere gestito tramite le funzioni addSettingRow e removeSettinRow

 - size : dimensione corrente dell'array, gestita tramite le funzioni di aggiunta e rimozione

 - selected : indice corrente selezionato, gestito dalla funzione definita come "handleSettingsPage"
 */
typedef struct {
    SettingRow* rows[MAX_LENGTH_SETTINGS];
    unsigned char size;
    unsigned char selected;
} SettingsPage;

void addSettingRow(SettingsPage* sp, SettingRow* sr);

void removeSettingRow(SettingsPage* sp, SettingRow* sr);

void visibilitySettingRow(SettingsPage* sp, unsigned char index, bool show);

void drawSettingRow(SettingRow* row, int* x, int* y, bool selected);

void drawSettingsPage(SettingsPage* page);

typedef struct {
    // Puntatore alla base dell'array
    void* basePtr;
    //Prefisso che viene mostrato nell'array per ogni elemento
    unsigned char* elem_prefix;
    // Puntatore alla lunghezza dell'array
    unsigned char* length;
    // Dimensione dei singoli elementi contenuti nell'array
    unsigned int esize;
    // Indice dell'elemento selezionato
    unsigned char selected;
    // Puntatore all'elemento selezionato
    void* selectedPtr;
} ArrayPage;

/**
 * @brief Si occupa solo del disegnare le opzioni laterali e la relativa selezionata
 *
 * @param ap Il punatore alla struttura dati ArrayPage da disegnare
 */
void drawArrayRightMenu(ArrayPage* ap);

/**
 * @brief Disegna un menu laterale (a sinistra di quello principale) con solo i numeri di riferimento all'array.
 * Quando viene premuto il tasto "EXE" si suppone che chi chiama questa funzione
 * mostri l'elemento, quindi la pagina intera è pulita e la saved RAM invalidata.
 * I tasti funzione dovrebbero permettere l'aggiunta e la rimozione degli elementi, gestiti sempre dal lato chiamante.
 * I tasti + e - sono degli shortcut per chiamare le funzioni add e remove element dell'array.
 *
 * @param ap Puntatore alla struttura dati ArrayPage
 * @param key Puntatore alla variabile intera che memorizza il tasto premuto
 * @param functionKeyRequest puntatore a funzione che gestisce il callback quando viene premuto un tasto F1-F6
 * @param functionKeyRequest puntatore a funzione che gestisce il disegno della function bar inferiore
 * @param showElementCallback puntatore a funzione che gestisce la visualizzazione dell'elemento selezionato. La funzione restituisce "true" se l'elemento è stato mostrato. In questo modo la funzione sa se deve ridisegnare il menu dell'array.
 * @param removeElementCallback puntatore a funzione che gestisce la rimozione dell'elemento (chiamata con tasto -). La funzione restituisce "true" se l'elemento è stato rimosso.
 * @param addElementCallback puntatore a funzione che gestisce l'aggiunta di un nuovo elemento nell'array (chiamata con tasto +). La funzione restituisce "true" se l'elemento è stato aggiunto.
 *
 * @return 1 se è stato premuto il tasto "EXE" altrimenti 0 se è premuto il tasto "EXIT"
 */
bool handleArrayElementsMenu(ArrayPage* ap, int* key,
                             void (*functionKeyRequested)(int* key),
                             void (*drawFunctionBar)(void),
                             bool (*showElementCallback)(ArrayPage* ap),
                             bool (*removeElementCallback)(ArrayPage* ap),
                             bool (*addElementCallback)(ArrayPage* ap));

/*
Restituisce 0 nel caso di nessuna opzione scelta, altrimenti restituisce 1 e
l'indice dell'opzione scelta è direttamente quello presente nella variabile "page".

Richiede il puntatore a dove viene salvato il carattere premuto in modo da poter gestiere dalla funzione
che chiama questa anche la pressione dei caratteri.
in questo modo se viene premuto un tasto F1-F6 viene restituito 1 sia dalla funzione sia impostato nella
variabile "functionKeyRequest". Inoltre si ha anche il valore del tasto premuto poiché è usato il puntatore "key".
Il puntatore a funzione "inputRequest" viene chiamato solo quando viene premuto un tasto da 0 a 9 sopra un campo di input.
*/
bool handleSettingsPage(SettingsPage* page, int* key,
                        void (*functionKeyRequested)(int* key), void (*drawFunctionBar)(void), void (*inputRequest)(unsigned char value));

fn_result askUnsignedChar(message_t msg, unsigned char* result);
fn_result askSignedChar(message_t msg, char* result);
fn_result askUnsignedInt(message_t msg, unsigned int* result);
fn_result askSignedInt(message_t msg, signed int* result);
fn_result askUnsignedLong(message_t msg, unsigned long* result);
fn_result askSignedLong(message_t msg, signed long* result);
fn_result askDouble(message_t msg, double* result);
fn_result askFloat(message_t msg, float* result);
fn_result askBoolean(message_t msg, bool* result);

/***************************
 *     BARRA INFERIORE     *
 ***************************/

/**
 * @brief Rappresenta un elemento della barra funzioni (F1-F6):
 * ha il testo da disegnare e il puntatore alla funzione di callback.
 * La funzione di callback deve essere:

        void (*callback)(SettingRow*)
 */
typedef struct {
    // Testo da mostrare a grafica
    unsigned char* text;
    void (*callback)(SettingRow*);
} FunctionBarElement;

/**
 * @brief Rappresenta una barra con l'insieme di massimo 6 FunctionBarElement. Sono rappresentati
 * in ordine sopra i pulsanti: nel caso in cui non si voglia un'opzione inserire {0,0}.
 */
typedef struct {
    FunctionBarElement options[6];
} FunctionBar;

/**
 * @brief Questa funzione si occupa solo di disegnare la barra inferiore, non gestisce nessun input.
 * L'input deve essere gestito da chi ha chiesto di disegnare la barra
 *
 * @param fb FunctionBar: per non disegnare un tasto funzione mettere il puntatore a 0.
 * @param clear Se "true" pulisce l'area e basta
 */
void drawBar(FunctionBar* fb, bool clear);

#endif