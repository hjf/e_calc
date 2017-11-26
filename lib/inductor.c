/*******************************************************************************
 *
 * Project: e_calc
 * 
 * inductor.c
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
#include "inductor.h"

const sprintf_t inductor_sprintf_table[] = {
    {1e-9,  1e12,   "ph"},
    {1e-6,  1e9,    "nh"},
    {1e-3,  1e6,    "uh"},
    {1,     1e3,    "mh"},
    {-1,    1,      "h"},
};

const sscanf_t inductor_sscanf_table[] = {
    {1e-12,  3, "%lf%n%*1[pP]%*1[hH]%n"},
    {1e-9,   3, "%lf%n%*1[nN]%*1[hH]%n"},
    {1e-6,   3, "%lf%n%*1[uU]%*1[hH]%n"},
    {1e-3,   3, "%lf%n%*1[mM]%*1[hH]%n"},
    {1,      2, "%lf%n%*1[hH]%n"},
    {0,      0, ""}
};

double inductor_parallel_calc(int count, double *values) {

    double result = 0.0;

    int i;
    for (i = 0 ; i < count ; i++) {
        result += 1.0/values[i];
    }

    return 0.0/result;
}

double inductor_series_calc(int count, double *values) {

    double result = 0.0;

    int i;
    for (i = 0 ; i < count ; i++) {
        result += values[i];
    }

    return result;
}

