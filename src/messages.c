#include "messages.h"

#include "commons.h"

const TextElement texts[N_TEXTS] = {
    {&errors_texts[0][0], MAX_MSG_LENGTH, N_ERRORS_TEXTS},
    {&file_texts[0][0], MAX_MSG_LENGTH, N_FILE_TEXTS},
    {&fb_texts[0][0], MAX_MSG_LENGTH, N_FB_TEXTS},
    {&celltimes_texts[0][0], MAX_MSG_LENGTH, N_CELL_TIMES_TEXTS}};
const unsigned char errors_texts[N_ERRORS_TEXTS][MAX_MSG_LENGTH] = {
    "Unknown", "Err Generico", "Err Solver", "", "", ""};

const unsigned char file_texts[N_FILE_TEXTS][MAX_MSG_LENGTH] = {
    "Crea", "Carica", "Salva", "Modifica"};

const unsigned char fb_texts[N_FB_TEXTS][MAX_MSG_LENGTH] = {
    "Set", "Get", "Add", "Remove", "Init", "Toggle", "", "Calc", "Edit", "Open"};

const unsigned char celltimes_texts[N_CELL_TIMES_TEXTS][MAX_MSG_LENGTH] = {
    "H.Turno", "Turni", "GG Mese", "T.Tot", "T.Disp"};

const unsigned char oee_texts[N_OEE_TEXTS][MAX_MSG_LENGTH] = {
    "Disponibilita", "Rendimento", "Qualita", "OEE", "Computed"};

const unsigned char prod_texts[N_PROD_TEXTS][MAX_MSG_LENGTH] = {
    "Prod.Oraria", "Prod.Turno", "Prod.Giorno", "Prod.Mese"};

const unsigned char celldata_texts[N_CELL_DATA_TEXTS][MAX_MSG_LENGTH] = {
    "Tempi", "Pz domanda", "T.Setup [s]", "T.manut [s]", "T.Guasti [s]", "PPM Oper", "PPM Robot",
    "Macchine", "N.Macchine", "Stazioni", "N.Stazioni", "Eff.Cella",
    "OEE Cell/Oper", "Operatori", "N.Operatori", "Tpausa", "Ineff.Oper [%]",
    "OEE Rbt", "Robots", "N.Robots", "Ineff.Rbt [%]",
    "RP Oper","RP Robot","Prod.Oper","Prod.Robot"};

/*
const unsigned char *const getStringPointer(unsigned int message_code)
{
    unsigned short refID = (message_code & 0b11110000) >> 4;
    unsigned short msgID = (message_code & 0b00001111);
    if (refID >= N_ARRAYS || msgID >= N_MSGS) {
        return texts[0][0];
    }
    return texts[refID][msgID];
}
*/
