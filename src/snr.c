/*******************************************************************************
 *
 * Project: e_calc
 * 
 * snr.c
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
#include "lib/decibel.h"

#include "lib/snr.h"

void snr_printcalcs (void) {

    printf ("noise_figure\n");



}

void snr_calchelp (char *calc) {

    if (0 == strcasecmp (calc, "noise_figure")) {
        printf ("e_calc snr noise_figure <snr_in> <snr_out>\n");
    }

}

int snr_parse (int argc, char *argv[]) {

    double snr_in;
    double snr_out;
    double nf;

    char *value;

    if (argc < 3) {
        snr_printcalcs ();
        exit (EXIT_FAILURE);
    }

    /***** noise_figure *****/

    if (0 == strcasecmp (argv[2], "noise_figure")) {
        if (argc != 5) {
            snr_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!decibel_sscanf (argv[3], &snr_in)) {
            snr_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (!decibel_sscanf (argv[4], &snr_out)) {
            snr_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        nf = snr_noise_figure_calc(&snr_in, &snr_out);

        if (!(value = decibel_sprintf (4, nf))) {
            exit (EXIT_FAILURE);
        }

        printf ("noise_figure = %s\n", value);
        free(value);
    }

    else {
        snr_printcalcs();
        exit (EXIT_FAILURE);
    }

    return 1;
}






