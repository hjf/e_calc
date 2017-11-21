/*******************************************************************************
 *
 * Project: e_calc
 * 
 * power.c
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

#include "power.h"

const sprintf_t power_sprintf_table[] = {
    {1e-9,  1e12,   "pW"},
    {1e-6,  1e9,    "nW"},
    {1e-3,  1e6,    "uW"},
    {1,     1e3,    "mW"},
    {1e6,    1,     "W"},
    {1e6,   1e-3,   "KW"},
    {1e9,   1e-6,   "MW"},
    {1e12,  1e-9,   "GW"},
    {-1,    1e-12,  "TW"}
};

const sscanf_t power_sscanf_table[] = {
    {1e-12,  3, "%lf%n%*1[pP]%*1[wW]%n"},
    {1e-9,   3, "%lf%n%*1[nN]%*1[wW]%n"},
    {1e-6,   2, "%lf%n%*1[uU]%*1[wW]%n"},
    {1e-3,   3, "%lf%n%*1[mM]%*1[wW]%n"},
    {1,      2, "%lf%n%*1[wW]%n"},
    {1e3,    3, "%lf%n%*1[kK]%*1[wW]%n"},
    {1e6,    3, "%lf%n%*1[M]%*1[wW]%n"},
    {1e9,    3, "%lf%n%*1[G]%*1[wW]%n"},
    {1e9,    6, "%lf%n%*1[jJ]%*1[iI]%*1[gG]%*1[aA]%*1[wW]%n"},
    {1e9,   10, "%lf%n%*1[jJ]%*1[iI]%*1[gG]%*1[aA]%*1[wW]*1[aA]*1[tT]*1[tT]*1[sS]%n"},
    {1e12,   3, "%lf%n%*1[T]%*1[wW]%n"},
    {0,      0, ""}
};

