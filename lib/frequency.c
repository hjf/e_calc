/*******************************************************************************
 *
 * Project: e_calc
 * 
 * frequency.c
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
#include <stdlib.h>
#include "frequency.h"

char *frequency_sprintf(
    int places,
    double value
) {
 
    char units[4] = {0};
    double F = value; 
    char *result = NULL;
    
    if (value < 1e3) {
        strcpy( units, "HZ");
    }
    else if (value < 1e6) {
        strcpy( units, "KHZ");
        F*=1e-3;
    }
    else {
        strcpy( units, "MHZ");
        F*=1e-6;
    }

    size_t len = snprintf(NULL, 0, "%.*f %s", places, F, units);

    if ( NULL == (result = malloc (sizeof(char) * len + 1))) {
        return NULL;
    }
    
    sprintf(result, "%.*f %s", places, F, units);

    return result;
}

int frequency_sscanf(
    char *str,
    double *result
) {

    int nConv = 0;
    int nChars = 0;
    char mod[4];
    

    nConv = sscanf(str, "%lf%1[Hh]%1[Zz]", result, mod, mod+1);

    if (nConv == 3 && 0 == strcasecmp("hz", mod) )
        return 1;

    nConv = sscanf(str, "%lf%1[Kk]%1[Hh]%1[Zz]", result, mod, mod+1, mod+2);

    if (nConv == 4 && 0 == strcasecmp("khz", mod) ) {
        (*result) *= 1e3;
        return 1;
    }

    nConv = sscanf(str, "%lf%1[Mm]%1[Hh]%1[Zz]", result, mod, mod+1, mod+2);

    if (nConv == 4 && 0 == strcasecmp("mhz", mod) ) {
        (*result) *= 1e6;
        return 1;
    }

    nConv = sscanf(str, "%lf%1[Gg]%1[Hh]%1[Zz]", result, mod, mod+1, mod+2);

    if (nConv == 4 && 0 == strcasecmp("ghz", mod) ) {
        (*result) *= 1e9;
        return 1;
    }

    nConv = sscanf(str, "%lf%1[Tt]%1[Hh]%1[Zz]", result, mod, mod+1, mod+2);

    if (nConv == 4 && 0 == strcasecmp("thz", mod) ) {
        (*result) *= 1e12;
        return 1;
    }
    
    nConv = sscanf(str, "%lf%n", result, &nChars);

    if (nConv == 1 && nChars == strlen(str)) {
        return 1;
    }

    return 0;
    
}

