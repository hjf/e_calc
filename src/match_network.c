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

    printf ("l\n");
    printf ("pi\n");

}

void match_network_calchelp (char *calc) {

    if (0 == strcasecmp (calc, "l")) {
        printf ("e_calc match_network l <generator Impedance> <load Impedance> <Cuttof Freq>\n");
    }
    if (0 == strcasecmp (calc, "pi")) {
        printf ("e_calc match_network pi <generator Impedance> <load Impedance> <Cuttof Freq> <width freq>\n");
    }
    if (0 == strcasecmp (calc, "pi2")) {
        printf ("e_calc match_network pi2 <generator Impedance> <load Impedance> <Cuttof Freq>\n");
    }
}


int match_network_parse (int argc, char *argv[]) {

    struct match_network result;
    double Rg;
    double Rl;
    double Fcut;
    double Fbw;
    double middle;
    char *value;

    if (argc < 3) {
        match_network_printcalcs();
        exit (EXIT_FAILURE);
    }

    if (0 == strcasecmp (argv[2], "l")) {
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


        result = match_network_l (Rg, Rl, Fcut);


        if ( result.series )
            printf("Series First\n");
        else
            printf("Shunt First\n");

        

        if (!(value = inductor_sprintf (4, result.L[0]))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("L = %s\n", value);
            free (value);
        }

        if (!(value = capacitor_sprintf (4, result.C[0]))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("C = %s\n", value);
            free (value);
        }
        if (!(value = frequency_sprintf (4, result.BW))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("BW = %s\n", value);
            free (value);
        }

    }
    else if (0 == strcasecmp (argv[2], "pi")) {
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

        if (!frequency_sscanf (argv[6], &Fbw)) {
            match_network_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }


        result = match_network_pi (Rg, Rl, Fcut, Fbw);

        
        printf("highpass\n");
        if (!(value = inductor_sprintf (4, result.L[0]))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("L[0] = %s\n", value);
            free (value);
        }

        middle = capacitor_series_calc(2, result.C);
        if (!(value = capacitor_sprintf (4, middle))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("C = %s\n", value);
            free (value);
        }

        if (!(value = inductor_sprintf (4, result.L[1]))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("L[1] = %s\n", value);
            free (value);
        }

        printf("lowpass\n");
        if (!(value = capacitor_sprintf (4, result.C[0]))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("C[0] = %s\n", value);
            free (value);
        }

        middle = inductor_series_calc(2, result.L);
        if (!(value = inductor_sprintf (4, middle))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("L = %s\n", value);
            free (value);
        }

        if (!(value = capacitor_sprintf (4, result.C[1]))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("c[1] = %s\n", value);
            free (value);
        }

        if (!(value = frequency_sprintf (4, result.BW))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("BW = %s\n", value);
            free (value);
        }
    }

    else if (0 == strcasecmp (argv[2], "pi2")) {
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

        
        result = match_network_pi2 (Rg, Rl, Fcut);

        printf("highpass\n");
        if (!(value = inductor_sprintf (4, result.L[0]))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("L[0] = %s\n", value);
            free (value);
        }

        middle = capacitor_series_calc(2, result.C);
        if (!(value = capacitor_sprintf (4, middle))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("C = %s\n", value);
            free (value);
        }

        if (!(value = inductor_sprintf (4, result.L[1]))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("L[1] = %s\n", value);
            free (value);
        }

        printf("lowpass\n");
        if (!(value = capacitor_sprintf (4, result.C[0]))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("C[0] = %s\n", value);
            free (value);
        }

        middle = inductor_series_calc(2, result.L);
        if (!(value = inductor_sprintf (4, middle))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("L = %s\n", value);
            free (value);
        }

        if (!(value = capacitor_sprintf (4, result.C[1]))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("c[1] = %s\n", value);
            free (value);
        }

        if (!(value = frequency_sprintf (4, result.BW))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("BW = %s\n", value);
            free (value);
        }
    }

   else {
        match_network_printcalcs();
        exit (EXIT_FAILURE);
    }

    return 1;
}






