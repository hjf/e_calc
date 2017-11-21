/*******************************************************************************
 *
 * Project: e_calc
 * 
 * reactance.c
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
#include "lib/resister.h"
#include "lib/capacitor.h"
#include "lib/inductor.h"
#include "lib/frequency.h"
#include "lib/reactance.h"

void reactance_printcalcs (void) {

    printf ("XC\n");
    printf ("XL\n");
    printf ("C_XcF\n");
    printf ("L_XlF\n");
    printf ("XSeries_QR\n");
    printf ("XShunt_QR\n");


}

void reactance_calchelp (char *calc) {

    if (0 == strcasecmp (calc, "XC")) {
        printf ("e_calc reactance XC <frequency> <capacitance>\n");
    }

    if (0 == strcasecmp (calc, "XL")) {
        printf ("e_calc reactance XL <frequency> <inductance>\n");
    }

    if (0 == strcasecmp (calc, "C_XcF")) {
        printf ("e_calc reactance C_XcF <capacitive reactance> <frequency>\n");
    }

    if (0 == strcasecmp (calc, "L_XlF")) {
        printf ("e_calc reactance L_XlF <inductive reactance> <frequency>\n");
    }

    if (0 == strcasecmp (calc, "XSeries")) {
        printf ("e_calc reactance XSeries <q> <impedance>\n");
    }

    if (0 == strcasecmp (calc, "XShunt")) {
        printf ("e_calc reactance XShunt <q> <impedance>\n");
    }

}

int reactance_parse (int argc, char *argv[]) {

    double Z;
    double F;
    double C;
    double L;
    double X;
    double Q;
    char *value;
    if (argc < 3) {
        reactance_printcalcs ();
        exit (EXIT_FAILURE);
    }

    /***** XC *****/

    if (0 == strcasecmp (argv[2], "XC")) {
        if (argc != 5) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!frequency_sscanf (argv[3], &F)) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (!capacitor_sscanf (argv[4], &C)) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        X = XC ((F), (C));

        if (!(value = resister_sprintf (4, X))) {
            exit (EXIT_FAILURE);
        }

        printf ("Xc = %s\n", value);
        free(value);
    }

    /***** XL *****/

    else if (0 == strcasecmp (argv[2], "XL")) {
        if (argc != 5) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!frequency_sscanf (argv[3], &F)) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (!inductor_sscanf (argv[4], &L)) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        X = XL(F, L);

        if (!(value = resister_sprintf (4, X))) {
            exit (EXIT_FAILURE);
        }

        printf ("Xl = %s\n", value);

        free(value);
    }

    /***** L_XlF *****/

    else if (0 == strcasecmp (argv[2], "L_XlF")) {
        if (argc != 5) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (!resister_sscanf (argv[3], &X)) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!frequency_sscanf (argv[4], &F)) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        L = L_XlF(X, F);

        if (!(value = inductor_sprintf (4, L))) {
            exit (EXIT_FAILURE);
        }

        printf ("L = %s\n", value);
        free(value);
    }

    /***** C_XcF *****/

    else if (0 == strcasecmp (argv[2], "C_XcF")) {
        if (argc != 5) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (!resister_sscanf (argv[3], &X)) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!frequency_sscanf (argv[4], &F)) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        C = C_XcF(X, F);

        if (!(value = capacitor_sprintf (4, C))) {
            exit (EXIT_FAILURE);
        }

        printf ("C = %s\n", value);
        free(value);
    }

    /***** XSeries_QR *****/

    else if (0 == strcasecmp (argv[2], "XSeries")) {
        if (argc != 5) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (1 != sscanf (argv[3], "%lf", &Q)) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (!resister_sscanf (argv[4], &Z)) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        X = XSeries_QR(Q, Z);

        if (!(value = resister_sprintf (4, X))) {
            exit (EXIT_FAILURE);
        }

        printf ("X = %s\n", value);
        free(value);
    }

    /***** XShunt_QR *****/

    else if (0 == strcasecmp (argv[2], "XShunt")) {
        if (argc != 5) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (1 != sscanf (argv[3], "%lf", &Q)) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        else if (!resister_sscanf (argv[4], &Z)) {
            reactance_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        X = XShunt_QR(Q, Z);

        if (!(value = resister_sprintf (4, X))) {
            exit (EXIT_FAILURE);
        }

        printf ("X = %s\n", value);
        free(value);
    }

    else {
        reactance_printcalcs();
        exit (EXIT_FAILURE);
    }

    return 1;
}






