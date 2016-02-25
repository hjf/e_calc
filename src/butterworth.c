/*******************************************************************************
 *
 * Project: e_calc
 * 
 * butterworth.c
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
#include "lib/butterworth.h"
#include "lib/frequency.h"
#include "lib/resister.h"
#include "lib/capacitor.h"
#include "lib/inductor.h"
#define ODD(n) ( (n)%2 )
#define EVEN(n) ( (n)+1%2 )

void butterworth_printcalcs (void) {

    printf ("lowpass\n");
    printf ("highpass\n");
    printf ("bandpass\n");
    printf ("bandstop\n");
}

void butterworth_calchelp (char *calc) {

    if (0 == strcasecmp (calc, "lowpass")) {
        printf ("e_calc butterworth lowpass <Cutoff freq> <Impedance> <filter order> <PI or T>\n");
    }
    else if (0 == strcasecmp (calc, "highpass")) {
        printf ("e_calc butterworth highpass <Cutoff freq> <Impedance> <filter order> <PI or T>\n");

    }
    else if (0 == strcasecmp (calc, "bandpass")) {
        printf ("e_calc butterworth bandpass <Center freq> <width freq> <Impedance> <filter order> <PI or T>\n");

    }
    else if (0 == strcasecmp (calc, "bandstop")) {
        printf ("e_calc butterworth bandstop <Center freq> <width freq> <Impedance> <filter order> <PI or T>\n");

    }
}


int butterworth_parse (int argc, char *argv[]) {

    struct Butterworth *result;
    double Fcut;
    double Fcenter;
    double Fwidth;
    double Z;
    int nOrder;
    int i;
    int series = 0;
    char *value;

    if (argc < 3) {
        butterworth_printcalcs ();
        exit (EXIT_FAILURE);
    }

    if (0 == strcasecmp (argv[2], "lowpass")) {
        if (argc < 7) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!frequency_sscanf (argv[3], &Fcut)) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (!resister_sscanf (argv[4], &Z)) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        else if (1 != sscanf (argv[5], "%i", &nOrder)) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (0 != strcasecmp (argv[6], "PI")
              && 0 != strcasecmp (argv[6], "T")) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!(result = Butterworth_Lowpass (Fcut, Z, nOrder)))
            exit (EXIT_FAILURE);

        if (0 == strcasecmp (argv[6], "T"))
            series = 1;
        
        for (i = 0; i < result->nOrder; i++) {
            if (series) {
                if (ODD (i)) {
                    if (!(value = capacitor_sprintf (10, result->C[i]))) {
                        Butterworth_free(result);
                        exit (EXIT_FAILURE);
                    }
                    else {
                        printf ("C[%i] = %s\n", i, value);
                        free (value);
                    }
                }
                else {
                    if (!(value = inductor_sprintf (4, result->L[i]))) {
                        Butterworth_free(result);
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
                        Butterworth_free(result);
                        exit (EXIT_FAILURE);
                    }
                    else {
                        printf ("L[%i] = %s\n", i, value);
                        free (value);
                    }
                }
                else {
                    if (!(value = capacitor_sprintf (4, result->C[i]))) {
                        Butterworth_free(result);
                        exit (EXIT_FAILURE);
                    }
                    else {
                       printf ("C[%i] = %s\n", i, value);
                       free (value);
                    }
                }

            }
        }
        Butterworth_free(result);
    }

    else if (0 == strcasecmp (argv[2], "highpass")) {
        if (argc < 6) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        if (!frequency_sscanf (argv[3], &Fcut)) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        else if (!resister_sscanf (argv[4], &Z)) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        else if (1 != sscanf (argv[5], "%i", &nOrder)) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
    
        else if (0 != strcasecmp (argv[6], "PI") && 0 != strcasecmp (argv[6], "T")) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
    
        if (!(result = Butterworth_Highpass (Fcut, Z, nOrder)))
            exit (EXIT_FAILURE);
    
        if (0 == strcasecmp (argv[6], "T"))
            series = 1;
        for (i = 0; i < result->nOrder; i++) {
            if (series) {
                if (ODD (i)) {
                    if (!(value = inductor_sprintf (4, result->L[i]))) {
                        Butterworth_free(result);
                        exit (EXIT_FAILURE);
                    }
                    else {
                        printf ("L[%i] = %s\n", i, value);
                        free (value);
                    }
                }
                else {
                    if (!(value = capacitor_sprintf (4, result->C[i]))) {
                        Butterworth_free(result);
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
                        Butterworth_free(result);
                        exit (EXIT_FAILURE);
                    }
                    else {
                        printf ("C[%i] = %s\n", i, value);
                        free (value);
                    }
                }
                else {
                    if (!(value = inductor_sprintf (4, result->L[i]))) {
                        Butterworth_free(result);
                        exit (EXIT_FAILURE);
                    }
                    else {
                        printf ("L[%i] = %s\n", i, value);
                        free (value);
                    }
                }
            }
        }
        Butterworth_free(result);
    }

    else if (0 == strcasecmp (argv[2], "bandpass")) {
        if (argc < 7) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        if (!frequency_sscanf (argv[3], &Fcenter)) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        if (!frequency_sscanf (argv[4], &Fwidth)) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        else if (!resister_sscanf (argv[5], &Z)) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        else if (1 != sscanf (argv[6], "%i", &nOrder)) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (0 != strcasecmp (argv[7], "PI") && 0 != strcasecmp (argv[7], "T")) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (0 == strcasecmp (argv[7], "T"))
            series = 1;

        if (!(result = Butterworth_Bandpass (Fcenter, Fwidth, Z, nOrder, series)))
            exit (EXIT_FAILURE);


        for (i = 0; i < result->nOrder; i++) {
            if (!(value = inductor_sprintf (4, result->L[i]))) {
                Butterworth_free(result);
                exit (EXIT_FAILURE);
            }
            else {
                printf ("L[%i] = %s\n", i, value);
                free (value);
            }

            if (!(value = capacitor_sprintf (4, result->C[i]))) {
                Butterworth_free(result);
                exit (EXIT_FAILURE);
            }
            else {
                printf ("C[%i] = %s\n", i, value);
                free (value);
            }
        }
        Butterworth_free(result);
    }

    else if (0 == strcasecmp (argv[2], "bandstop")) {
        if (argc < 7) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        if (!frequency_sscanf (argv[3], &Fcenter)) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        if (!frequency_sscanf (argv[4], &Fwidth)) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        else if (!resister_sscanf (argv[5], &Z)) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        else if (1 != sscanf (argv[6], "%i", &nOrder)) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (0 != strcasecmp (argv[7], "PI") && 0 != strcasecmp (argv[7], "T")) {
            butterworth_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (0 == strcasecmp (argv[7], "T"))
            series = 1;

        if (!(result = Butterworth_Bandstop (Fcenter, Fwidth, Z, nOrder, series)))
            exit (EXIT_FAILURE);


        for (i = 0; i < result->nOrder; i++) {
            if (!(value = inductor_sprintf (4, result->L[i]))) {
                Butterworth_free(result);
                exit (EXIT_FAILURE);
            }
            else {
                printf ("L[%i] = %s\n", i, value);
                free (value);
            }

            if (!(value = capacitor_sprintf (4, result->C[i]))) {
                Butterworth_free(result);
                exit (EXIT_FAILURE);
            }
            else {
                printf ("C[%i] = %s\n", i, value);
                free (value);
            }
        }
        Butterworth_free(result);
    }


    else {
        butterworth_printcalcs();
        exit (EXIT_FAILURE);
    }
    return 1;
}
