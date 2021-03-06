/*******************************************************************************
 *
 * Project: e_calc
 * 
 * io.c
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
#include "io.h"


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
    const sprintf_t *table)
{
 
   char *result = NULL;

    do {

        //printf ("places=%i value=%lf table-> limit = %lf factor = %lg unit = %s \n",
        //         places, value, table->limit, table->factor, table->unit);
        if (table->limit == -1) {
            break;
        }
        else if (value < table->limit) {
            break;
        }

        table++;

    } while (table->limit != -1);

    value *= table->factor;
    
    size_t len = snprintf(NULL, 0, "%.*f %s", places, value, table->unit);

    if ( NULL == (result = malloc (sizeof(char) * len + 1))) {
        return NULL;
    }
    
    sprintf(result, "%.*f %s", places, value, table->unit);

    return result;
}

/*******************************************************************************

*******************************************************************************/

int io_sscanf(
    char *str,
    double *result,
    const sscanf_t *table)
{

    int nConv = 0;
    int start = 0;
    int end = 0;
    
    while (table->factor != 0) {
        nConv = 0;
        start = 0;
        end = 0;
        //printf ("str=%s nConv=%i end = %i start = %i table-> factor = %lf nConv = %i format = %s \n",
        //         str, nConv, end, start, table->factor, table->nConv, table->format);

        nConv = sscanf(str, table->format, result, &start, &end);

        nConv += end-start;

        //printf ("str=%s nConv=%i end = %i start = %i table-> factor = %lf nConv = %i format = %s \n",
        //         str, nConv, end, start, table->factor, table->nConv, table->format);
        if (nConv == table->nConv) {
            (*result) *= table->factor;
            //printf("result %lf \n", *result);
            return 1;
        }

        table++;

    }

    nConv = sscanf(str, "%lf", result);
    if (nConv == 1) {
        return 1;
    }

    return 0;
    
}
