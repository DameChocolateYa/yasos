#include <stdio.h>
#include "math.h"

__attribute__((visibility("default")))
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

__attribute__((visibility("default")))
double round(double num, int decimals) {
    double factor = pow(10.0, decimals);
    double x = 0.5;

    long long temp = (long long)(num * factor + (num >= 0 ? x : -x));
    return (double)temp / factor;
}

__attribute__((visibility("default")))
int db_to_int(float n) {
    return (int)n;
}

__attribute__((visibility("default")))
double ceil(double num) {
    long long int_part = (long long)num;

    if (num > (double)int_part) {
        return (double)(int_part + 1);
    }
    else {
        return (double)int_part;
    }
}

__attribute__((visibility("default")))
double floor(double n) {
    long long int_part = (long long)n;

    if (n < (double)int_part) {
        return (double)(int_part - 1);
    }
    else {
        return (double)int_part;
    }
}

__attribute__((visibility("default")))
int abs(int x) {
    if (x >= 0) return x;
    else return -x;
}

__attribute__((visibility("default")))
double fabs(double x) {
    if (x >= 0.0) return x;
    else return -x;
}
