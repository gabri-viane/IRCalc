#include "math.h"
/*
 * Approximate sine and tangent functions without using math.h
 * Uses Taylor series expansion around 0 (Maclaurin series)
 *
 * sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ...
 * tan(x) = sin(x) / cos(x)
 * where cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! + ...
 */

// helper function: power(x, n)
double power(double x, int n) {
    double result = 1.0;
    for (int i = 0; i < n; i++) result *= x;
    return result;
}

// helper function: factorial(n)
double factorial(int n) {
    double result = 1.0;
    for (int i = 2; i <= n; i++) result *= i;
    return result;
}

// reduce angle to range [-PI, PI] to improve precision
double normalize(double x) {
    while (x > PI)  x -= TWO_PI;
    while (x < -PI) x += TWO_PI;
    return x;
}

// sine approximation
double sin(double x) {
    x = normalize(x);
    double term, sum = 0.0;
    int sign = 1;

    for (int n = 1; n <= 13; n += 2) { // use up to x^13 term
        term = sign * power(x, n) / factorial(n);
        sum += term;
        sign = -sign;
    }

    return sum;
}

// cosine approximation
double cos(double x) {
    x = normalize(x);
    double term, sum = 0.0;
    int sign = 1;

    for (int n = 0; n <= 14; n += 2) { // use up to x^14 term
        term = sign * power(x, n) / factorial(n);
        sum += term;
        sign = -sign;
    }

    return sum;
}

// tangent approximation using sin/cos
double tan(double x) {
    double s = sin(x);
    double c = cos(x);
    if (c == 0.0) {
        // handle near vertical asymptote
        return (s > 0) ? 1e30 : -1e30;
    }
    return s / c;
}