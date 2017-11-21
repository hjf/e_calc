/*******************************************************************************
 *
 * Project: e_calc
 * 
 * resister.h
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

#ifndef RESISTER_H
#define RESISTER_H


#define RSeries_QX(Q, X) ((X)/(Q))

#define RShunt_QX(Q, X) ((X)*(Q))



double resister_parallel_calc(int count, double *values);

double resister_series_calc(int count, double *values);

#include "io.h"

extern const sprintf_t resister_sprintf_table[];

#define resister_sprintf(places, value) io_sprintf((places), (value), resister_sprintf_table)

extern const sscanf_t resister_sscanf_table[];

#define resister_sscanf(str, result) io_sscanf((str), (result), resister_sscanf_table)

#endif


