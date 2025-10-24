#include "commons.h"

#include <fxcg/misc.h>

static unsigned char numLength(int value) {
    int res = 0;
    if (value == 0) {
        return 1;
    }
    while (value > 0) {
        value = value / 10;
        res++;
    }
    return res;
}

void dtoa(double value, unsigned char* result, unsigned char decimals) {
    bool negative = value < 0;
    if (negative) {
        value = -value;
    }
    // Conversione
    int magg = (int)value;

    // Procedo già ad inserirlo nella stringa
    unsigned char pos = 0;
    if (negative) {
        result[0] = '-';
        pos++;
    }
    itoa(magg, result + pos);
    if (decimals == 0) {  // se non ho decimali ho già finito
        return;
    }

    double low_d = (value - magg);
    while (decimals > 0) {
        low_d *= 10;
        decimals--;
    }
    int low = (int)low_d;
    // Misura caratteri necessari
    pos += numLength(magg);
    result[pos] = '.';
    pos++;
    // Se sono qua ho decimali
    itoa(low, result + pos);
    pos += numLength(low);
    result[pos] = '\0';
}

static bool isDigit(unsigned char* c) {
    return (*c >= '0' && *c <= '9');
}

double strtod_s(unsigned char* result) {
    if (result == 0) {
        return 0.0;
    }

    int parte_intera = 0;
    int parte_decimale = 0;     //
    unsigned int decimali = 1;  // Numero di decimali: indica per quanto dividere "parte_decimale"
    int cur_pos = 0;            // Posizione del cursore nella stringa

    while (isDigit(&result[cur_pos])) {
        parte_intera = parte_intera * 10 + (result[cur_pos] - '0');
        cur_pos++;
    }

    if (result[cur_pos] != '.') {
        return 1.0 * parte_intera;
    }
    cur_pos++;

    while (isDigit(&result[cur_pos])) {
        parte_decimale = parte_decimale * 10 + (result[cur_pos] - '0');
        cur_pos++;
        decimali *= 10;
    }

    return (1.0 * parte_intera) + ((1.0 * parte_decimale) / decimali);
}
