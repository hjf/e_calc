/*******************************************************************************
 *
 * Project: e_calc
 * 
 * voltage.c
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

#include "voltage.h"

const sprintf_t voltage_sprintf_table[] = {
    {1e-9,  1e12,   "pv"},
    {1e-6,  1e9,    "nv"},
    {1e-3,  1e6,    "uv"},
    {1,     1e3,    "mv"},
    {1e3,   1,      "v"},
    {1e6,   1e-3,   "Kv"},
    {-1,    1e-6,   "Mv"}
};

const sscanf_t voltage_sscanf_table[] = {
    {1e12,   3, "%lf%n%*1[pP]%*1[vV]%n"},
    {1e9,    3, "%lf%n%*1[nN]%*1[vV]%n"},
    {1e6,    3, "%lf%n%*1[uU]%*1[vV]%n"},
    {1e3,    3, "%lf%n%*1[m]%*1[vV]%n"},
    {1,      2, "%lf%n%*1[vV]%n"},
    {1e3,    3, "%lf%n%*1[kK]%*1[vV]%n"},
    {1e6,    3, "%lf%n%*1[M]%*1[vV]%n"},
    {0,      0, ""}
};

