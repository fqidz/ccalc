// #include <math.h>
// #include <stdio.h>
// #include <stdlib.h>
#include "sys/param.h"
int main(void)
{
    volatile int i = 1;
    volatile int j = 2;
    MIN(i, j);
    // double lhs = 2091281;
    // double rhs = 102902113810;
    // double result = lhs * rhs;
    //
    // int exponent = (int)(floor(log10(fabs(result))));
    // exponent = exponent > 0 ? exponent : (int)(log10(fabs(result)) - 1);
    // char sign = exponent > 0 ? '+' : '-';
    // double mantissa = result / pow(10, exponent);
    //
    // printf("result: %.15f\n", result);
    // printf("scientific notation: %.15fe%c%d\n", mantissa, sign, abs(exponent));
}
