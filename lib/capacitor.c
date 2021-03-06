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

const sprintf_t capacitor_sprintf_table[] = {
    {1e-9,  1e12,   "pF"},
    {1e-6,  1e9,    "nF"},
    {1e-3,  1e6,    "uF"},
    {1,     1e3,    "mF"},
    {-1,    1,      "F"},
};

const sscanf_t capacitor_sscanf_table[] = {
    {1e-12,  3, "%lf%n%*1[pP]%*1[fF]%n"},
    {1e-9,   3, "%lf%n%*1[nN]%*1[fF]%n"},
    {1e-6,   2, "%lf%n%*1[uU]%*1[fF]%n"},
    {1e-3,   3, "%lf%n%*1[mM]%*1[fF]%n"},
    {1,      2, "%lf%n%1[fF]%n"},
    {0,      0, ""}
};

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


