/*******************************************************************************
 *
 * Project: e_calc
 * 
 * capacitor.c
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
#include "capacitor.h"

double capacitor_series_calc(int count, double *values) {

    double result = 0.0;

    int i;
    for (i = 0 ; i < count ; i++) {
        result += 1.0/values[i];
    }

    return 1.0/result;
}

double capacitor_parallel_calc(int count, double *values) {

    double result = 0.0;

    int i;
    for (i = 0 ; i < count ; i++) {
        result += values[i];
    }

    return result;
}

char *capacitor_sprintf(int places, double value ) {
 
    char units[3] = {0};
    double C = value; 
    if (value < 1e-9) {
        strcpy( units, "pF");
        C*=1e12;
    }
    else if (value < 1e-6) {
        strcpy( units, "nF");
        C*=1e9;
    }
    else if (value < 1e-3) {
        strcpy( units, "uF");
        C*=1e6;
    }
    else if (value < 1) {
        strcpy( units, "mF");
        C*=1e3;
    }
    else {
        strcpy( units, "F");
    }

    size_t len = snprintf(NULL, 0, " %.*f %s", places, C, units);

    char *result;

    if ( NULL == (result = malloc (sizeof(char) * len + 1))) {
        return NULL;
    }
    
    sprintf(result, "%.*f %s", places, C, units);

    return result;
}

int capacitor_sscanf(
    char *str,
    double *result
) {

    int nConv = 0;
    int nChars = 0;
    char mod[4];
    

    nConv = sscanf(str, "%lf%1[Ff]", result, mod);

    if (nConv == 2 && 0 == strcasecmp("f", mod) ) {
        return 1;
    }

    nConv = sscanf(str, "%lf%1[Mm]%1[fF]", result, mod, mod+1);

    if (nConv == 3 && 0 == strcasecmp("mf", mod) ) {
        (*result) *= 1e-3;
        return 1;
    }

    nConv = sscanf(str, "%lf%1[Uu]%1[fF]", result, mod, mod+1);

    if (nConv == 3 && 0 == strcasecmp("uf", mod) ) {
        (*result) *= 1e-3;
        return 1;
    }

    nConv = sscanf(str, "%lf%1[Nn]%1[fF]", result, mod, mod+1);

    if (nConv == 3 && 0 == strcasecmp("nf", mod) ) {
        (*result) *= 1e-3;
        return 1;
    }

    nConv = sscanf(str, "%lf%1[Pp]%1[fF]", result, mod, mod+1);

    if (nConv == 3 && 0 == strcasecmp("pf", mod) ) {
        (*result) *= 1e-3;
        return 1;
    }

    nConv = sscanf(str, "%lf%n", result, &nChars);

    if (nConv == 1 && nChars == strlen(str)) {
        return 1;
    }

    return 0;
    
}

