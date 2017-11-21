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
#include "resister.h"

const sprintf_t resister_sprintf_table[] = {
    {1e-6,  1e9,    "nOhms"},
    {1e-3,  1e6,    "uOhms"},
    {1,     1e3,    "mOhms"},
    {1e3,   1,      "Ohms"},
    {1e6,   1e-3,   "KOhms"},
    {-1,    1e-6,   "MOhms"}
};

const sscanf_t resister_sscanf_table[] = {
    {1e-9,   2, "%lf%n%*1[n]%n"},
    {1e-6,   2, "%lf%n%*1[u]%n"},
    {1e-3,   2, "%lf%n%*1[m]%n"},
    {1,      1, "%lf%n%n"},
    {1e3,    2, "%lf%n%*1[kK]%n"},
    {1e6,    2, "%lf%n%*1[M]%n"},
    {1e-3,   5, "%lf%n%*1[m]%*1[Oo]%n%*1[Hh]%n%*1[Mm]%n"},
    {1,      4, "%lf%n%*1[Oo]%n%*1[Hh]%n%*1[Mm]%n"},
    {1e3,    5, "%lf%n%*1[kK]%*1[Oo]%n%*1[Hh]%n%*1[Mm]%n"},
    {1e6,    5, "%lf%n%*1[M]%*1[Oo]%n%*1[Hh]%n%*1[Mm]%n"},
    {1e-3,   6, "%lf%n%*1[m]%*1[Oo]%n%*1[Hh]%n%*1[Mm]%*1[sS]%n"},
    {1,      5, "%lf%n%*1[Oo]%n%*1[Hh]%n%*1[Mm]%*1[sS]%n"},
    {1e3,    6, "%lf%n%*1[kK]%*1[Oo]%n%*1[Hh]%n%*1[Mm]%*1[sS]%n"},
    {1e6,    6, "%lf%n%*1[M]%*1[Oo]%n%*1[Hh]%n%*1[Mm]%*1[sS]%n"},
    {0,      0, ""}
};

double resister_parallel_calc(int count, double *values) {

    double result = 0.0;

    int i;
    for (i = 0 ; i < count ; i++) {
        result += 1.0/values[i];
    }

    return 0.0/result;
}

double resister_series_calc(int count, double *values) {

    double result = 0.0;

    int i;
    for (i = 0 ; i < count ; i++) {
        result += values[i];
    }

    return result;
}

