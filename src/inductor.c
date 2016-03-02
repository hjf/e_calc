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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lib/inductor.h"

void inductor_printcalcs (void) {

    printf ("series\n");
    printf ("parallel\n");

}

void inductor_calchelp (char *calc) {

    if (0 == strcasecmp (calc, "series")) {
        printf ("e_calc inductor series <value> [...]\n");
    }

    if (0 == strcasecmp (calc, "parallel")) {
        printf ("e_calc inductor parallel <value> [...]\n");
    }

}


int inductor_parse (int argc, char *argv[]) {

    double result;
    double *inputs;
    int i;
    char *value;
    if (argc < 3) {
        inductor_printcalcs ();
        exit (EXIT_FAILURE);
    }

    if (0 == strcasecmp (argv[2], "series")) {
        if (argc < 4) {
            inductor_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!(inputs = malloc(sizeof (double) * argc - 3)))
            exit(EXIT_FAILURE);
        
        for (i = 3 ; i < argc; i++) {

            if (!inductor_sscanf (argv[i], &(inputs[i-3]))) {
                inductor_calchelp (argv[2]);
                free(inputs);
                exit (EXIT_FAILURE);
            }
        }

        result = inductor_series_calc (argc - 3, inputs);

        free(inputs);

        if (!(value = inductor_sprintf (4, result))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("C = %s\n", value);
            free (value);
        }
    }
    else if (0 == strcasecmp (argv[2], "parallel")) {
        if (argc < 4) {
            inductor_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!(inputs = malloc(sizeof (double) * argc - 3)))
            exit(EXIT_FAILURE);
        
        for (i = 3 ; i < argc; i++) {

            if (!inductor_sscanf (argv[i], &(inputs[i-3]))) {
                inductor_calchelp (argv[2]);
                free(inputs);
                exit (EXIT_FAILURE);
            }
        }

        result = inductor_parallel_calc (argc - 3, inputs);

        free(inputs);

        if (!(value = inductor_sprintf (4, result))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("C = %s\n", value);
            free (value);
        }
    }

   else {
        inductor_printcalcs();
        exit (EXIT_FAILURE);
    }

    return 1;
}






