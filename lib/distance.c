/*******************************************************************************
 *
 * Project: e_calc
 * 
 * distance.c
 * 
 *
 *******************************************************************************
 * Copyright (c) 2017,  Brian Case 
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

#include "io.h"
#include "distance.h"

const sprintf_t distance_sprintf_table[] = {
    {1e-9,  1e12,   "pm"},
    {1e-6,  1e9,    "nm"},
    {1e-3,  1e6,    "um"},
    {1e-2,  1e3,    "mm"},
    {1,     1e2,    "cm"},
    {1e3,   1,      "m"},
    {1e6,   1e-3,   "Km"},
    {-1,    1e-6,   "Mm"}
};


const sscanf_t distance_sscanf_table[] = {
    {.3048,     2, "%lf%n%*1[']%n"},
    {.0254,     2, "%lf%n%*1[\"]%n"},
    {1e-4,      3, "%lf%n%*1[m]%*1[m]%n"},
    {1e-3,      3, "%lf%n%*1[cC]%*1[m]%n"},
    {1,         2, "%lf%n%*1[m]%n"},
    {1e3,       3, "%lf%n%*1[kK]%*1[m]%n"},
    {1e6,       3, "%lf%n%*1[M]%*1[m]%n"},
    {0,         0, ""}
};

