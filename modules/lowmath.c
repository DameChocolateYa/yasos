#include <stdio.h>
#include "math.h"

double sqrt(double x) {
    if (x < 0) return 1;
    if (x == 0 || x == 1) return x;

    double guess = x / 2.0;
    double epsilon = 0.000001;
    
    while ((guess * guess - x) > epsilon || (x - guess * guess) > epsilon) {
        guess = (guess + x / guess) / 2.0;
    }
    
    return guess;
}

double round(double num, int decimals) {
    double factor = pow(10.0, decimals);
    double x = 0.5;

    long long temp = (long long)(num * factor + (num >= 0 ? x : -x));
    return (double)temp / factor;
}

int db_to_int(float n) {
    return (int)n;
}

double ceil(double num) {
    long long int_part = (long long)num;

    if (num > (double)int_part) {
        return (double)(int_part + 1);
    }
    else {
        return (double)int_part;
    }
}

double floor(double n) {
    long long int_part = (long long)n;

    if (n < (double)int_part) {
        return (double)(int_part - 1);
    }
    else {
        return (double)int_part;
    }
}

int abs(int x) {
    if (x >= 0) return x;
    else return -x;
}

double fabs(double x) {
    if (x >= 0.0) return x;
    else return -x;
}
