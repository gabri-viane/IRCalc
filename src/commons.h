#pragma once
#ifndef H_COMMONS_INCLUEDED
#define H_COMMONS_INCLUEDED 1

#define bool unsigned char
#define true 1
#define false 0

#define PI 3.14159265358979323846
#define GRAVITY 9.81

typedef unsigned int res_error_t;

/*
0: successo
1: annullata
2: errore conversione
*/
typedef unsigned char fn_result;

void dtoa(double value, unsigned char* result, unsigned char decimals);

double strtod_s(unsigned char* result);


#endif  // !H_COMMONS_INCLUEDED
