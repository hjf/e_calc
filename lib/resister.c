/*******************************************************************************
 *
 * Project: e_calc
 * 
 * resister.c
 * 
 *
 *******************************************************************************
 * Copyright (c) 2016,  Brian Case 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "resister.h"

double resister_parallel_calc(int count, ...) {

    va_list ap;
    va_start(ap, count);

    double result = 0.0;

    int i;
    for (i = 0 ; i < count ; i++) {
        result += 1.0/va_arg(ap, double);
    }

    va_end(ap);

    return 0.0/result;
}

double resister_series_calc(int count, ...) {

    va_list ap;
    va_start(ap, count);

    double result = 0.0;

    int i;
    for (i = 0 ; i < count ; i++) {
        result += va_arg(ap, double);
    }

    va_end(ap);

    return result;
}

char *resister_sprintf(
    int places,
    double value
) {
 
    char units[4] = {0};
    double R = value; 
    char *result = NULL;
    
    if (value < 1e3) {
        strcpy( units, "");
    }
    else if (value < 1e6) {
        strcpy( units, "K");
        R*=1e-3;
    }
    else {
        strcpy( units, "M");
        R*=1e-6;
    }

    size_t len = snprintf(NULL, 0, "%.*f %s", places, R, units);

    if ( NULL == (result = malloc (sizeof(char) * len + 1))) {
        return NULL;
    }
    
    sprintf(result, "%.*f %s", places, R, units);

    return result;
}

int resister_sscanf(
    char *str,
    double *result
) {

    int nConv = 0;
    int nChars = 0;
    char mod[4];
    

    nConv = sscanf(str, "%lf%1[Kk]", result, mod);

    if (nConv == 2 && 0 == strcasecmp("k", mod) ) {
        (*result) *= 1e3;
        return 1;
    }

    nConv = sscanf(str, "%lf%1[Mm]", result, mod);

    if (nConv == 2 && 0 == strcasecmp("m", mod) ) {
        (*result) *= 1e6;
        return 1;
    }

    nConv = sscanf(str, "%lf%n", result, &nChars);

    if (nConv == 1 && nChars == strlen(str)) {
        return 1;
    }

    return 0;
    
}

