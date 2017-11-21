/*******************************************************************************
 *
 * Project: e_calc
 * 
 * io.h
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

#ifndef IO_H
#define IO_H

typedef struct sprintf_struct {
    double limit;
    double factor;
    char unit[512];
} sprintf_t;

typedef struct sscanf_struct {
    double factor;
    int nConv;
    char format[512];
} sscanf_t;

/*******************************************************************************
@brief function to do simple unit conversion and print out a value and unit to a
       string

@param places   numer of decimal places to print the value at
@param value    the value to print
@param table    unit conversion table

@return     an allocated pointer with value and units. this must be free()'ed
*******************************************************************************/

char *io_sprintf(
    int places,
    double value,
    const sprintf_t *table);

/*******************************************************************************

*******************************************************************************/

int io_sscanf(
    char *str,
    double *result,
    const sscanf_t *table);

#endif
