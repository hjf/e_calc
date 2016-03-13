/*******************************************************************************
 *
 * Project: e_calc
 * 
 * match_network.c
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
#include "lib/match_network.h"
#include "lib/frequency.h"
#include "lib/resister.h"
#include "lib/capacitor.h"
#include "lib/inductor.h"

void match_network_printcalcs (void) {

    printf ("l_lowpass\n");
    printf ("l_highpass\n");

}

void match_network_calchelp (char *calc) {

    if (0 == strcasecmp (calc, "l_lowpass")) {
        printf ("e_calc match_network l_lowpass <generator Impedance> <load Impedance> <Cuttof Freq> <filter order>\n");
    }
    if (0 == strcasecmp (calc, "l_highpass")) {
        printf ("e_calc match_network l_highpass <generator Impedance> <load Impedance> <Cuttof Freq> <filter order>\n");
    }

    if (0 == strcasecmp (calc, "pi_lowpass")) {
        printf ("e_calc match_network l_lowpass <generator Impedance> <load Impedance> <Cuttof Freq>\n");
    }
    if (0 == strcasecmp (calc, "pi_highpass")) {
        printf ("e_calc match_network l_highpass <generator Impedance> <load Impedance> <Cuttof Freq>\n");
    }
}


int match_network_parse (int argc, char *argv[]) {

    struct match_network *result;
    double Rg;
    double Rl;
    double Fcut;
    double Fbw;
    double middle;
    int nOrder;
    char *value;
    int i;

    if (argc < 3) {
        match_network_printcalcs();
        exit (EXIT_FAILURE);
    }

    /***** l_lowpass *****/

    if (0 == strcasecmp (argv[2], "l_lowpass")) {
        if (argc < 7) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!resister_sscanf (argv[3], &Rg)) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!resister_sscanf (argv[4], &Rl)) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!frequency_sscanf (argv[5], &Fcut)) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (1 != sscanf (argv[6], "%i", &nOrder)) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!(result = match_network_l_lowpass (Rg, Rl, Fcut, nOrder)))
            exit (EXIT_FAILURE);

        if (result->series)
            printf("series first\n");
        else
            printf("shunt first\n");

        if (!(value = resister_sprintf (4, Rg))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("Z = %s\n", value);
            free (value);
        }

        for (i = 0; i < result->nOrder; i++) {

            if (!(value = capacitor_sprintf (10, result->C[i]))) {
                match_network_free(result);
                exit (EXIT_FAILURE);
            }
            else {
                printf ("C[%i] = %s\n", i, value);
                free (value);
            }

            if (!(value = inductor_sprintf (4, result->L[i]))) {
                match_network_free(result);
                exit (EXIT_FAILURE);
            }
            else {
                printf ("L[%i] = %s\n", i, value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result->Zi[i]))) {
                match_network_free(result);
                exit (EXIT_FAILURE);
            }
            else {
                printf ("Z = %s\n", value);
                free (value);
            }

        }

        if (!(value = frequency_sprintf (4, result->BW))) {
            match_network_free(result);
            exit (EXIT_FAILURE);
        }
        else {
            printf ("Fbw = %s\n", value);
            free (value);
        }

        match_network_free(result);


    }

    /***** l_highpass *****/

    else if (0 == strcasecmp (argv[2], "l_highpass")) {
        if (argc < 7) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!resister_sscanf (argv[3], &Rg)) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!resister_sscanf (argv[4], &Rl)) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!frequency_sscanf (argv[5], &Fcut)) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (1 != sscanf (argv[6], "%i", &nOrder)) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!(result = match_network_l_highpass (Rg, Rl, Fcut, nOrder)))
            exit (EXIT_FAILURE);

        if (result->series)
            printf("series first\n");
        else
            printf("shunt first\n");

        if (!(value = resister_sprintf (4, Rg))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("Z = %s\n", value);
            free (value);
        }

        for (i = 0; i < result->nOrder; i++) {

            if (!(value = capacitor_sprintf (10, result->C[i]))) {
                match_network_free(result);
                exit (EXIT_FAILURE);
            }
            else {
                printf ("C[%i] = %s\n", i, value);
                free (value);
            }

            if (!(value = inductor_sprintf (4, result->L[i]))) {
                match_network_free(result);
                exit (EXIT_FAILURE);
            }
            else {
                printf ("L[%i] = %s\n", i, value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result->Zi[i]))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("Z = %s\n", value);
                free (value);
            }

        }

        if (!(value = frequency_sprintf (4, result->BW))) {
            match_network_free(result);
            exit (EXIT_FAILURE);
        }
        else {
            printf ("Fbw = %s\n", value);
            free (value);
        }

        match_network_free(result);


    }

    /***** pi_lowpass *****/

    else if (0 == strcasecmp (argv[2], "pi_lowpass")) {
        if (argc < 6) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!resister_sscanf (argv[3], &Rg)) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!resister_sscanf (argv[4], &Rl)) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!frequency_sscanf (argv[5], &Fcut)) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!(result = match_network_l_lowpass (Rg, Rl, Fcut, 2)))
            exit (EXIT_FAILURE);

        if (!(value = resister_sprintf (4, Rg))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("Z = %s\n", value);
            free (value);
        }

        if (!(value = capacitor_sprintf (10, result->C[0]))) {
            match_network_free(result);
            exit (EXIT_FAILURE);
        }
        else {
            printf ("C[0] = %s\n", value);
            free (value);
        }

        middle = inductor_series_calc(2, result->L);

        if (!(value = inductor_sprintf (4, middle ))) {
            match_network_free(result);
            exit (EXIT_FAILURE);
        }
        else {
            printf ("L = %s\n", value);
            free (value);
        }

        if (!(value = resister_sprintf (4, result->Zi[0]))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("Z = %s\n", value);
            free (value);
        }

        if (!(value = capacitor_sprintf (10, result->C[1]))) {
            match_network_free(result);
            exit (EXIT_FAILURE);
        }
        else {
            printf ("C[1] = %s\n", value);
            free (value);
        }

        if (!(value = resister_sprintf (4, result->Zi[1]))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("Z = %s\n", value);
            free (value);
        }

        if (!(value = frequency_sprintf (4, result->BW))) {
            match_network_free(result);
            exit (EXIT_FAILURE);
        }
        else {
            printf ("Fbw = %s\n", value);
            free (value);
        }

        match_network_free(result);


    }

    /***** pi_highpass *****/

    else if (0 == strcasecmp (argv[2], "pi_highpass")) {
        if (argc < 6) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!resister_sscanf (argv[3], &Rg)) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!resister_sscanf (argv[4], &Rl)) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!frequency_sscanf (argv[5], &Fcut)) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!(result = match_network_l_highpass (Rg, Rl, Fcut, 2)))
            exit (EXIT_FAILURE);

        if (!(value = resister_sprintf (4, Rg))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("Z = %s\n", value);
            free (value);
        }

        if (!(value = inductor_sprintf (10, result->L[0]))) {
            match_network_free(result);
            exit (EXIT_FAILURE);
        }
        else {
            printf ("L[0] = %s\n", value);
            free (value);
        }

        middle = capacitor_series_calc(2, result->C);

        if (!(value = capacitor_sprintf (4, middle ))) {
            match_network_free(result);
            exit (EXIT_FAILURE);
        }
        else {
            printf ("C = %s\n", value);
            free (value);
        }

        if (!(value = resister_sprintf (4, result->Zi[0]))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("Z = %s\n", value);
            free (value);
        }

        if (!(value = inductor_sprintf (10, result->L[1]))) {
            match_network_free(result);
            exit (EXIT_FAILURE);
        }
        else {
            printf ("L[1] = %s\n", value);
            free (value);
        }

        if (!(value = resister_sprintf (4, result->Zi[1]))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("Z = %s\n", value);
            free (value);
        }

        if (!(value = frequency_sprintf (4, result->BW))) {
            match_network_free(result);
            exit (EXIT_FAILURE);
        }
        else {
            printf ("Fbw = %s\n", value);
            free (value);
        }

        match_network_free(result);


    }

    else {
        match_network_printcalcs();
        exit (EXIT_FAILURE);
    }

    return 1;
}






