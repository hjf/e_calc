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

#include "frequency.h"

const sprintf_t frequency_sprintf_table[] = {
    {1e3,   1,      "hz"},
    {1e6,   1e-3,   "Khz"},
    {1e9,   1e-6,   "Mhz"},
    {1e12,  1e-9,   "Ghz"},
    {-1,    1e-12,  "Thz"}
};

const sscanf_t frequency_sscanf_table[] = {
    {1,      3, "%lf%n%*1[hH]%*1[zZ]%n"},
    {1e3,    4, "%lf%n%*1[kK]%*1[hH]%*1[zZ]%n"},
    {1e6,    4, "%lf%n%*1[mM]%*1[hH]%*1[zZ]%n"},
    {1e9,    4, "%lf%n%*1[gG]%*1[hH]%*1[zZ]%n"},
    {1e12,   4, "%lf%n%*1[tT]%*1[hH]%*1[zZ]%n"},
    {0,      0, ""}
};

