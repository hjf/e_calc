/*******************************************************************************
 *
 * Project: e_calc
 * 
 * decibel.c
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
#include "decibel.h"


char *decibel_sprintf(
    int places,
    double value
) {
 
    char units[4] = "dB";
    double DB = value; 
    char *result = NULL;
    
    size_t len = snprintf(NULL, 0, "%.*f %s", places, DB, units);

    if ( NULL == (result = malloc (sizeof(char) * len + 1))) {
        return NULL;
    }
    
    sprintf(result, "%.*f %s", places, DB, units);

    return result;
}

int decibel_sscanf(
    char *str,
    double *result
) {

    int nConv = 0;
    int nChars = 0;
    char mod[4];
    

    nConv = sscanf(str, "%lf%1[Dd]%1[Bb]", result, mod, mod+1);

    if (nConv == 3 && 0 == strcasecmp("db", mod) ) {
        return 1;
    }

    nConv = sscanf(str, "%lf%n", result, &nChars);

    if (nConv == 1 && nChars == strlen(str)) {
        return 1;
    }

    return 0;
    
}

