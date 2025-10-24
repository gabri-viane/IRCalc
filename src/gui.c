#include "gui.h"

#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "textinput.h"

void addSettingRow(SettingsPage* sp, SettingRow* sr) {
    if (sp == 0 || sr == 0) {
        return;
    }
    if (sp->size < MAX_LENGTH_SETTINGS - 1) {
        sp->rows[sp->size] = sr;
        sp->size++;
    }
}

void removeSettingRow(SettingsPage* sp, SettingRow* sr) {
    if (sp == 0 || sr == 0) {
        return;
    }
    bool shift = false;
    unsigned char size = sp->size;
    for (unsigned int i = 0; i < size; i++) {
        if (shift) {
            if (i < size - 1) {
                sp->rows[i] = sp->rows[i + 1];  // Copio il successivo in quello corrente
            } else {                            // devo solo rimuovere l'ultimo elemento
                sp->rows[i] = 0;                // NULL
            }
        }
        if (!shift && sp->rows[i] == sr) {
            shift = true;
        }
    }
}

void visibilitySettingRow(SettingsPage* sp, unsigned char index, bool show) {
    if (sp == 0 || index >= MAX_LENGTH_SETTINGS) {
        return;
    }
    SettingRow* sr = sp->rows[index];
    if (sr == 0) {
        return;
    }
    sr->shown = show;
}

void drawSettingRow(SettingRow* row, int* x, int* y, bool selected) {
    const int mode = 0b10000000 | selected;
    PrintMiniMini(x, y, row->text, mode, TEXT_COLOR_BLACK, 0);
    *x += 5;
    unsigned char value[10];
    switch (row->type) {
        case BOOL: {
            bool* bvalue = (bool*)row->pointerToValue;
            strcpy(value, *bvalue ? "Si" : "No");
            break;
        }
        case CHAR: {
            char* cvalue = (char*)row->pointerToValue;
            itoa(*cvalue, value);
            break;
        }
        case INT: {
            int* ivalue = (int*)row->pointerToValue;
            itoa(*ivalue, value);
            break;
        }
        case LONG: {
            long* lvalue = (long*)row->pointerToValue;
            itoa(*lvalue, value);
            break;
        }
        case UNSIGNED_CHAR: {
            unsigned char* ucvalue = (unsigned char*)row->pointerToValue;
            itoa(*ucvalue, value);
            break;
        }
        case UNSIGNED_INT: {
            unsigned int* uivalue = (unsigned int*)row->pointerToValue;
            itoa(*uivalue, value);
            break;
        }
        case UNSIGNED_LONG: {
            unsigned long* ulvalue = (unsigned long*)row->pointerToValue;
            itoa(*ulvalue, value);
            break;
        }
        case DOUBLE: {
            double* dvalue = (double*)row->pointerToValue;
            dtoa(*dvalue, value, 4);
            break;
        }
        case FLOAT: {
            float* fvalue = (float*)row->pointerToValue;
            dtoa(*fvalue, value, 4);
            break;
        }
        case ARRAY: {
            strcpy(value, "[A->]");
            break;
        }
        case PAGE_CHANGE: {
            strcpy(value, "[P->]");
            break;
        }
        default:
            strcpy(value, "");
            break;
    }
    unsigned char color = TEXT_COLOR_BLACK;
    switch (row->field) {
        case INPUT_FIELD:
            color = TEXT_COLOR_BLUE;
            break;
        case INPUT_FIELD | COMPUTABLE_FIELD:
            color = TEXT_COLOR_PURPLE;
            break;
        default:
        case OUTPUT_FIELD:
            color = TEXT_COLOR_BLACK;
            break;
        case OUTPUT_FIELD | COMPUTABLE_FIELD:
            color = TEXT_COLOR_GREEN;
            break;
    }
    PrintMiniMini(x, y, value, mode, color, 0);
}

void drawSettingsPage(SettingsPage* page) {
    // Massimo 11 per volta
    int x = 10;
    int y = 0;
    // Offset delle righe da visualizzare
    unsigned int offset = 0;
    // Righe visualizzabili
    unsigned int show_max = page->size;
    if (show_max > 11) {
        show_max = 11;  // Limito ad 11 le righe massime visualizzabili
        if (page->selected > 10) {
            offset = page->selected - 10;  // Calcolo l'offset se ho selezionato dalla riga 12 in poi
        }
    }
    for (unsigned int i = 0; i < show_max; i++, offset++) {
        if (page->rows[offset]->shown) {
            drawSettingRow(page->rows[offset], &x, &y, offset == page->selected);
            x = 10;
            y += 15;
        }
    }
}

void drawArrayRightMenu(ArrayPage* ap) {
    // Massimo 10 per volta
    int x = 192;
    int y = 5;
    PrintMini(&x, &y, "Elementi:", 0x02, 0xFFFFFFFF, 0, 0, TEXT_COLOR_BLACK, TEXT_COLOR_WHITE, 1, 0);
    x = 192;
    y = 25;
    // Offset delle righe da visualizzare
    unsigned int offset = 0;
    // Righe visualizzabili
    unsigned int show_max = *ap->length;
    if (show_max > 10) {
        show_max = 10;  // Limito ad 11 le righe massime visualizzabili
        if (ap->selected > 9) {
            offset = ap->selected - 9;  // Calcolo l'offset se ho selezionato dalla riga 11 in poi
        }
    }
    unsigned char buff[10];
    for (unsigned int i = 0; i < show_max; i++, offset++) {
        const int mode = 0b10000000 | (offset == ap->selected);  // Coloro il background di nero di quella selezionata
        PrintMiniMini(&x, &y, ap->elem_prefix, mode, TEXT_COLOR_BLACK, 0);
        x += 2;
        itoa(offset + 1, buff);
        PrintMiniMini(&x, &y, buff, mode, TEXT_COLOR_BLACK, 0);
        x = 192;  // Mi riporto a sinistra per il prox. elemento
        y += 15;  // Scendo per disegnare il successivo
    }
}

static void emptyDrawHandler() {
}

static void emptyFunctionKeyHandler(int* key) {
}

static void emptyInputHandler(unsigned char value) {
}

static bool emptyArrayHandler(ArrayPage* ap) {
    return false;
}

bool handleArrayElementsMenu(ArrayPage* ap, int* key,
                             void (*functionKeyRequested)(int* key),
                             void (*drawFunctionBar)(void),
                             bool (*showElementCallback)(ArrayPage* ap),
                             bool (*removeElementCallback)(ArrayPage* ap),
                             bool (*addElementCallback)(ArrayPage* ap)) {
    if (functionKeyRequested == NULL) {
        functionKeyRequested = emptyFunctionKeyHandler;
    }
    if (drawFunctionBar == NULL) {
        drawFunctionBar = emptyDrawHandler;
    }
    if (showElementCallback == NULL) {
        showElementCallback = emptyArrayHandler;
    }
    if (removeElementCallback == NULL) {
        removeElementCallback = emptyArrayHandler;
    }
    if (addElementCallback == NULL) {
        addElementCallback = emptyArrayHandler;
    }
    // Salvo la VRAM Corrente prima di mostrare il menu laterale
    SaveVRAM_1();
    // Pulisco la zona a destra del menu mostrato principale
    fillArea(191, 24, 192, 191, COLOR_WHITE);
    // Disegno il menu
    drawArrayRightMenu(ap);
    // Chiamo la funzione che dovrebbe aggiornare la barra dei tasti F1-F6
    drawFunctionBar();
    // Carico la VRAM sul display
    Bdisp_PutDisp_DD();

    while (1) {
        GetKey(key);                                                      // Aspetto il tasto ed aggiorna anche la grafica
        if (*key == KEY_CTRL_DOWN && (ap->selected + 1) < *ap->length) {  // Tasto per scendere nel menu
            ap->selected++;
            ap->selectedPtr = ap->basePtr + ap->esize * ap->selected;
            fillArea(191, 24, 192, 191, COLOR_WHITE);
            drawArrayRightMenu(ap);
            drawFunctionBar();
        } else if (*key == KEY_CTRL_UP && ap->selected > 0) {  // Tasto per salire nel menu
            ap->selected--;
            ap->selectedPtr = ap->basePtr + ap->esize * ap->selected;
            fillArea(191, 24, 192, 191, COLOR_WHITE);
            drawArrayRightMenu(ap);
            drawFunctionBar();
        } else if (*key == KEY_CTRL_EXE) {  // Tasto esegui: può essere usato sia per uscire che per richiamare una funzione
            if (showElementCallback(ap)) {
                fillArea(191, 24, 192, 191, COLOR_WHITE);
                return true;  // Ritorno alla funzione che mi chiama solamente se ho mostrato l'elemento.
            }
        } else if (*key == KEY_CTRL_EXIT) {            // Tasto uscita dal menu
            LoadVRAM_1();
            return false;
        } else if (*key == KEY_CHAR_MINUS) {  // Voglio rimuovere l'elemento corrente
            // Se l'ho rimosso ridisegno lo schermo
            if (removeElementCallback(ap)) {
                fillArea(191, 24, 192, 191, COLOR_WHITE);
                drawArrayRightMenu(ap);
                drawFunctionBar();
            }
        } else if (*key == KEY_CHAR_PLUS) {  // Voglio aggingere un nuovo elemento
            // Se l'ho aggiunro  ridisegno lo schermo
            if (addElementCallback(ap)) {
                fillArea(191, 24, 192, 191, COLOR_WHITE);
                drawArrayRightMenu(ap);
                drawFunctionBar();
            }
        } else if (*key >= KEY_CTRL_F1 && *key <= KEY_CTRL_F6) {  // Richiamato un tasto funzione
            // Ho premuto dei tasti che dovrebbero essere gestiti da chi ha scelto la function bar
            // restituisco 1 ma non pulisco lo schermo ed esco perché vuol dire che devo ancora gestire io
            functionKeyRequested(key);
            fillArea(191, 24, 192, 191, COLOR_WHITE);
            drawArrayRightMenu(ap);
        }
    }
}

bool handleSettingsPage(SettingsPage* page, int* key,
                        void (*functionKeyRequested)(int* key),
                        void (*drawFunctionBar)(void),
                        void (*inputRequest)(unsigned char value)) {
    if (functionKeyRequested == NULL) {
        functionKeyRequested = emptyFunctionKeyHandler;
    }
    if (drawFunctionBar == NULL) {
        drawFunctionBar = emptyDrawHandler;
    }
    if (inputRequest == NULL) {
        inputRequest = emptyInputHandler;
    }

    // Pulisco la pagina tra la status bar e la barra per le funzioni F1-F6 (mode = 2)
    Bdisp_Fill_VRAM(COLOR_WHITE, 2);
    // Disegno il menu
    drawSettingsPage(page);
    // Chiamo la funzione che dovrebbe aggiornare la barra dei tasti F1-F6
    drawFunctionBar();
    // Carico la VRAM sul display
    Bdisp_PutDisp_DD();

    while (1) {
        GetKey(key);  // Aspetto il tasto ed aggiorna anche la grafica
        if (*key >= KEY_CHAR_0 && *key <= KEY_CHAR_9 && (page->rows[page->selected]->field & INPUT_FIELD)) {
            // Premuto un numero su un campo input:  permettere la modifica
            inputRequest(*key);
            Bdisp_Fill_VRAM(COLOR_WHITE, 2);
            drawSettingsPage(page);
        } else if (*key == KEY_CTRL_DOWN && (page->selected + 1) < page->size) {  // Tasto per scendere nel menu
            page->selected++;
            Bdisp_Fill_VRAM(COLOR_WHITE, 2);
            drawSettingsPage(page);
            drawFunctionBar();
        } else if (*key == KEY_CTRL_UP && page->selected > 0) {  // Tasto per salire nel menu
            page->selected--;
            Bdisp_Fill_VRAM(COLOR_WHITE, 2);
            drawSettingsPage(page);
            drawFunctionBar();
        } else if (*key == KEY_CTRL_EXE) {  // Tasto esegui: può essere usato sia per uscire che per richiamare una funzione
            // Bdisp_Fill_VRAM(COLOR_WHITE, 2);
            return true;                     // Ritorno alla funzione che mi chiama
        } else if (*key == KEY_CTRL_EXIT) {  // Tasto uscita dal menu
            Bdisp_Fill_VRAM(COLOR_WHITE, 2);
            return false;
        } else if (*key >= KEY_CTRL_F1 && *key <= KEY_CTRL_F6) {  // Richiamato un tasto funzione
            // Ho premuto dei tasti che dovrebbero essere gestiti da chi ha scelto la function bar
            // restituisco 1 ma non pulisco lo schermo
            functionKeyRequested(key);
            Bdisp_Fill_VRAM(COLOR_WHITE, 2);
            drawSettingsPage(page);
        }
    }
}

static fn_result askNumber(message_t msg, unsigned char* buffer, unsigned char input_mode) {
    unsigned char msg_2[MAX_MSG_LENGTH + 2];
    memcpy((void*)(msg_2 + 2), msg, MAX_MSG_LENGTH);
    msg_2[0] = 'a';
    msg_2[1] = 'a';
    // Salvo come era prima la VRAM
    SaveVRAM_1();
    // Mostro il popup
    MsgBoxPush(2);
    // Non posso fare "msg-2" poiché questa funzione di print salta i primi due caratteri ma apparentemente
    // fa crashare l'add-in in alcuni casi per errore di accesso di lettura, allora devo copiare il messaggio
    // in uno con due caratteri inuili iniziali
    PrintXY(3, 3, msg_2, TEXT_MODE_NORMAL, TEXT_COLOR_BLACK);
    // displayAndGetInput(buffer, 3, 4);
    int res = getTextLine(buffer, 63, 3, 4, 12, input_mode);
    // Chiudo il popup a fine inserimento
    MsgBoxPop();
    // Ricarico la schermata precedente
    LoadVRAM_1();
    // Controllo che non venga ritornata una stringa nulle
    if (buffer[0] == '\0') {
        return 2;  // Se è ritornata non provo nemmeno a convertirla
    }
    // Se ho ricevuto il tasto "EXE" allora converto
    if (res == KEY_PRGM_RETURN) {
        return 0;
    }
    // Qualsiasi altra opzione che ha fatto uscire darà opzione annullata come risultato
    return 1;
}

fn_result askUnsignedChar(message_t msg, unsigned char* result) {
    if (result == 0) {
        return 1;
    }
    unsigned char buffer[64] = {};
    // Copio il numero almeno da renderlo visibile nella finestra di modifica
    itoa(*result, buffer);
    fn_result res = askNumber(msg, buffer, INPUT_MODE_POSINT);
    // Se ho ricevuto il tasto "EXE" allora converto
    if (res == 0) {
        *result = (unsigned char)atoi(buffer);
    }
    // ritorno il risultato
    return res;
}

fn_result askSignedChar(message_t msg, char* result) {
    if (result == 0) {
        return 1;
    }
    unsigned char buffer[64] = {};
    // Copio il numero almeno da renderlo visibile nella finestra di modifica
    itoa(*result, buffer);
    fn_result res = askNumber(msg, buffer, INPUT_MODE_INT);
    // Se ho ricevuto il tasto "EXE" allora converto
    if (res == 0) {
        *result = (char)atoi(buffer);
    }
    // ritorno il risultato
    return res;
}

fn_result askUnsignedInt(message_t msg, unsigned int* result) {
    if (result == 0) {
        return 1;
    }
    unsigned char buffer[64] = {};
    // Copio il numero almeno da renderlo visibile nella finestra di modifica
    itoa(*result, buffer);
    fn_result res = askNumber(msg, buffer, INPUT_MODE_POSINT);
    // Se ho ricevuto il tasto "EXE" allora converto
    if (res == 0) {
        char* end;
        *result = (unsigned int)strtol(buffer, &end, 10);
    }
    // ritorno il risultato
    return res;
}

fn_result askSignedInt(message_t msg, signed int* result) {
    if (result == 0) {
        return 1;
    }
    unsigned char buffer[64] = {};
    // Copio il numero almeno da renderlo visibile nella finestra di modifica
    itoa(*result, buffer);
    fn_result res = askNumber(msg, buffer, INPUT_MODE_INT);
    // Se ho ricevuto il tasto "EXE" allora converto
    if (res == 0) {
        *result = atoi(buffer);
    }
    // ritorno il risultato
    return res;
}

fn_result askUnsignedLong(message_t msg, unsigned long* result) {
    if (result == 0) {
        return 1;
    }
    unsigned char buffer[64] = {};
    // Copio il numero almeno da renderlo visibile nella finestra di modifica
    itoa(*result, buffer);
    fn_result res = askNumber(msg, buffer, INPUT_MODE_POSINT);
    // Se ho ricevuto il tasto "EXE" allora converto
    if (res == 0) {
        char* end;
        *result = (unsigned long)strtol(buffer, &end, 10);
    }
    // ritorno il risultato
    return res;
}

fn_result askSignedLong(message_t msg, signed long* result) {
    if (result == 0) {
        return 1;
    }
    unsigned char buffer[64] = {};
    // Copio il numero almeno da renderlo visibile nella finestra di modifica
    itoa(*result, buffer);
    fn_result res = askNumber(msg, buffer, INPUT_MODE_INT);
    // Se ho ricevuto il tasto "EXE" allora converto
    if (res == 0) {
        char* end;
        *result = strtol(buffer, &end, 10);
    }
    // ritorno il risultato
    return res;
}

fn_result askDouble(message_t msg, double* result) {
    if (result == 0) {
        return 1;
    }
    unsigned char buffer[64] = {};
    // Copio il numero almeno da renderlo visibile nella finestra di modifica
    dtoa(*result, buffer, 4);
    fn_result res = askNumber(msg, buffer, INPUT_MODE_FLOAT);
    // Se ho ricevuto il tasto "EXE" allora converto
    if (res == 0) {
        *result = strtod_s(buffer);
    }
    // ritorno il risultato
    return res;
}

fn_result askFloat(message_t msg, float* result) {
    if (result == 0) {
        return 1;
    }
    unsigned char buffer[64] = {};
    // Copio il numero almeno da renderlo visibile nella finestra di modifica
    dtoa(*result, buffer, 4);
    fn_result res = askNumber(msg, buffer, INPUT_MODE_FLOAT);
    // Se ho ricevuto il tasto "EXE" allora converto
    if (res == 0) {
        *result = (float)strtod_s(buffer);
    }
    // ritorno il risultato
    return res;
}

void fillArea(unsigned x, unsigned y, unsigned w, unsigned h, unsigned short col) {
    unsigned short* s = (unsigned short*)GetVRAMAddress();
    s += (y * 384) + x;
    while (h--) {
        unsigned w2 = w;
        while (w2--)
            *s++ = col;
        s += 384 - w;
    }
}

void drawBar(FunctionBar* fb, bool clear) {
    // x: [2 380]
    // y: [179 191]
    int starting_x = 2;
    const int starting_y = 179;
    int y_text = 181;
    const int ending_y = 191;
    const int width = 63;
    fillArea(0, 179 + 24, 383, 13, COLOR_WHITE);
    if (clear) {
        Bdisp_PutDisp_DD();
        return;
    }
    for (unsigned char i = 0; i < 6; i++) {
        if (fb->options[i].callback == 0 || fb->options[i].text == 0 || fb->options[i].text[0] == '\0') {
            continue;
        }
        starting_x = 2 + i * width;
        Bdisp_Rectangle(starting_x, starting_y, starting_x + width, ending_y, TEXT_COLOR_BLUE);
        starting_x++;
        PrintMiniMini(&starting_x, &y_text, fb->options[i].text, 0b10000000, TEXT_COLOR_BLACK, 0);
    }
    Bdisp_PutDisp_DD();
}
