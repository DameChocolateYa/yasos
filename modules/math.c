#include <stdio.h>

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

double pow(double base, int exponent) {
    double result = 1.0;
    int i;

    if (exponent == 0) {
        return 1.0;
    }

    if (exponent > 0) {
        for (i = 0; i < exponent; ++i) {
            result *= base;
        }
    }
    else {
        for (i = 0; i < -exponent; ++i) {
            result *= base;
        }
        result = 1.0 / result;
    }

    return result;
}

double round(double num, int decimals) {
    double factor = pow(10.0, decimals);
    double x = 0.5;

    long long temp = (long long)(num * factor + (num >= 0 ? x : -x));
    return (double)temp / factor;
}

int fltoint(float n) {
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

double floor(double num) {
    long long int_part = (long long)num;

    if (num < (double)int_part) {
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

int fact(int x) {
    int result = 1;
    for (int i = x; i >= 1; --i) {
        result *= i;
    }
    return result;
}

double log(double x, double base) {
    if (x <= 0 || base <= 0 || base == 1) {
        printf("Error: valores no válidos.\n");
        return -1;
    }

    double low, high;
    double epsilon = 1e-10;

    // Definir rango inicial según base y x
    if (base > 1) {
        low = 0.0;
        high = 1.0;
        while (pow(base, high) < x) {
            high *= 2;
        }
    } else {  // base entre 0 y 1
        high = 0.0;
        low = -1.0;
        while (pow(base, low) > x) {
            low *= 2;
        }
    }

    // Búsqueda binaria
    while ((high - low) > epsilon) {
        double mid = (low + high) / 2;
        double val = pow(base, mid);
        if (val < x) {
            low = mid;
        } else {
            high = mid;
        }
    }

    return (low + high) / 2;
}
