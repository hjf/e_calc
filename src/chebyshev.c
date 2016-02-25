/*******************************************************************************
 *
 * Project: e_calc
 * 
 * chebyshev.c
 * 
 *
 *******************************************************************************
 * Copyright (c) 2016,    Brian Case 
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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/chebyshev.h"
#include "lib/frequency.h"
#include "lib/resister.h"
#include "lib/decibel.h"
#include "lib/capacitor.h"
#include "lib/inductor.h"
#define ODD(n) ( (n)%2 )
#define EVEN(n) ( (n)+1%2 )

void chebyshev_printcalcs (void) {

    printf ("lowpass\n");
    printf ("highpass\n");
    printf ("bandpass\n");
    printf ("bandstop\n");
}

void chebyshev_calchelp (char *calc) {

    if (0 == strcasecmp (calc, "lowpass")) {
        printf ("e_calc chebyshev lowpass <Cutoff freq> <Impedance> <filter order> <ripple> <PI or T>\n");
    }
    else if (0 == strcasecmp (calc, "highpass")) {
        printf ("e_calc chebyshev highpass <Cutoff freq> <Impedance> <filter order> <ripple> <PI or T>\n");

    }
    else if (0 == strcasecmp (calc, "bandpass")) {
        printf ("e_calc chebyshev bandpass <Center freq> <width freq> <Impedance> <filter order> <ripple> <PI or T>\n");

    }
    else if (0 == strcasecmp (calc, "bandstop")) {
        printf ("e_calc chebyshev bandstop <Center freq> <width freq> <Impedance> <filter order> <ripple> <PI or T>\n");

    }
}


int chebyshev_parse (int argc, char *argv[]) {

    struct Chebyshev *result;
    double Fcut;
    double Fcenter;
    double Fwidth;
    double Z;
    int nOrder;
    double Ripple;
    int i;
    int series = 0;
    char *value;

    if (argc < 3) {
        chebyshev_printcalcs ();
        exit (EXIT_FAILURE);
    }

    if (0 == strcasecmp (argv[2], "lowpass")) {
        if (argc < 8) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!frequency_sscanf (argv[3], &Fcut)) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (!resister_sscanf (argv[4], &Z)) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        else if (1 != sscanf (argv[5], "%i", &nOrder)) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (!decibel_sscanf (argv[6], &Ripple)) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (0 != strcasecmp (argv[7], "PI")
              && 0 != strcasecmp (argv[7], "T")) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!(result = Chebyshev_Lowpass (Fcut, Z, nOrder, Ripple)))
            exit (EXIT_FAILURE);

        if (0 == strcasecmp (argv[7], "T"))
            series = 1;
        
        for (i = 0; i < result->nOrder; i++) {
            if (series) {
                if (ODD (i)) {
                    if (!(value = capacitor_sprintf (10, result->C[i]))) {
                        Chebyshev_free(result);
                        exit (EXIT_FAILURE);
                    }
                    else {
                        printf ("C[%i] = %s\n", i, value);
                        free (value);
                    }
                }
                else {
                    if (!(value = inductor_sprintf (4, result->L[i]))) {
                        Chebyshev_free(result);
                        exit (EXIT_FAILURE);
                    }
                    else {
                        printf ("L[%i] = %s\n", i, value);
                        free (value);
                    }
                }
            }
            else {
                if (ODD (i)) {
                    if (!(value = inductor_sprintf (4, result->L[i]))) {
                        Chebyshev_free(result);
                        exit (EXIT_FAILURE);
                    }
                    else {
                        printf ("L[%i] = %s\n", i, value);
                        free (value);
                    }
                }
                else {
                    if (!(value = capacitor_sprintf (4, result->C[i]))) {
                        Chebyshev_free(result);
                        exit (EXIT_FAILURE);
                    }
                    else {
                       printf ("C[%i] = %s\n", i, value);
                       free (value);
                    }
                }

            }
        }
        Chebyshev_free(result);
    }

    else if (0 == strcasecmp (argv[2], "highpass")) {
        if (argc < 8) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        if (!frequency_sscanf (argv[3], &Fcut)) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        else if (!resister_sscanf (argv[4], &Z)) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        else if (1 != sscanf (argv[5], "%i", &nOrder)) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
    
        else if (!decibel_sscanf (argv[6], &Ripple)) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (0 != strcasecmp (argv[7], "PI") && 0 != strcasecmp (argv[7], "T")) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
    
        if (!(result = Chebyshev_Highpass (Fcut, Z, nOrder, Ripple)))
            exit (EXIT_FAILURE);
    
        if (0 == strcasecmp (argv[7], "T"))
            series = 1;
        for (i = 0; i < result->nOrder; i++) {
            if (series) {
                if (ODD (i)) {
                    if (!(value = inductor_sprintf (4, result->L[i]))) {
                        Chebyshev_free(result);
                        exit (EXIT_FAILURE);
                    }
                    else {
                        printf ("L[%i] = %s\n", i, value);
                        free (value);
                    }
                }
                else {
                    if (!(value = capacitor_sprintf (4, result->C[i]))) {
                        Chebyshev_free(result);
                        exit (EXIT_FAILURE);
                    }
                    else {
                        printf ("C[%i] = %s\n", i, value);
                        free (value);
                    }
                }
            }
            else {
                if (ODD (i)) {
                    if (!(value = capacitor_sprintf (4, result->C[i]))) {
                        Chebyshev_free(result);
                        exit (EXIT_FAILURE);
                    }
                    else {
                        printf ("C[%i] = %s\n", i, value);
                        free (value);
                    }
                }
                else {
                    if (!(value = inductor_sprintf (4, result->L[i]))) {
                        Chebyshev_free(result);
                        exit (EXIT_FAILURE);
                    }
                    else {
                        printf ("L[%i] = %s\n", i, value);
                        free (value);
                    }
                }
            }
        }
        Chebyshev_free(result);
    }

    else if (0 == strcasecmp (argv[2], "bandpass")) {
        if (argc < 9) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        if (!frequency_sscanf (argv[3], &Fcenter)) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        if (!frequency_sscanf (argv[4], &Fwidth)) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        else if (!resister_sscanf (argv[5], &Z)) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        else if (1 != sscanf (argv[6], "%i", &nOrder)) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (!decibel_sscanf (argv[7], &Ripple)) {
            chebyshev_calchelp (argv[2]);
            printf("gotcha2\n");
            exit (EXIT_FAILURE);
        }

        else if (0 != strcasecmp (argv[8], "PI") && 0 != strcasecmp (argv[8], "T")) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (0 == strcasecmp (argv[8], "T"))
            series = 1;

        if (!(result = Chebyshev_Bandpass (Fcenter, Fwidth, Z, nOrder, Ripple, series)))
            exit (EXIT_FAILURE);


        for (i = 0; i < result->nOrder; i++) {
            if (!(value = inductor_sprintf (4, result->L[i]))) {
                Chebyshev_free(result);
                exit (EXIT_FAILURE);
            }
            else {
                printf ("L[%i] = %s\n", i, value);
                free (value);
            }

            if (!(value = capacitor_sprintf (4, result->C[i]))) {
                Chebyshev_free(result);
                exit (EXIT_FAILURE);
            }
            else {
                printf ("C[%i] = %s\n", i, value);
                free (value);
            }
        }
        Chebyshev_free(result);
    }

    else if (0 == strcasecmp (argv[2], "bandstop")) {
        if (argc < 9) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        if (!frequency_sscanf (argv[3], &Fcenter)) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        if (!frequency_sscanf (argv[4], &Fwidth)) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        else if (!resister_sscanf (argv[5], &Z)) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        else if (1 != sscanf (argv[6], "%i", &nOrder)) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (!decibel_sscanf (argv[7], &Ripple)) {
            chebyshev_calchelp (argv[2]);
            printf("gotcha2\n");
            exit (EXIT_FAILURE);
        }

        else if (0 != strcasecmp (argv[8], "PI") && 0 != strcasecmp (argv[8], "T")) {
            chebyshev_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (0 == strcasecmp (argv[8], "T"))
            series = 1;

        if (!(result = Chebyshev_Bandstop (Fcenter, Fwidth, Z, nOrder, Ripple, series)))
            exit (EXIT_FAILURE);


        for (i = 0; i < result->nOrder; i++) {
            if (!(value = inductor_sprintf (4, result->L[i]))) {
                Chebyshev_free(result);
                exit (EXIT_FAILURE);
            }
            else {
                printf ("L[%i] = %s\n", i, value);
                free (value);
            }

            if (!(value = capacitor_sprintf (4, result->C[i]))) {
                Chebyshev_free(result);
                exit (EXIT_FAILURE);
            }
            else {
                printf ("C[%i] = %s\n", i, value);
                free (value);
            }
        }
        Chebyshev_free(result);
    }


    else {
        chebyshev_printcalcs();
        exit (EXIT_FAILURE);
    }
    return 1;
}
