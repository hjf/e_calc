/*******************************************************************************
 *
 * Project: e_calc
 * 
 * ohmslaw.c
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
#include "lib/voltage.h"
#include "lib/current.h"
#include "lib/power.h"
#include "lib/ohmslaw.h"

void ohmslaw_printcalcs (void) {

    printf ("I_ER <voltage> <resistance>\n");
    printf ("I_PE <power> <voltage>\n");
    printf ("I_PR <power> <resistance>\n");
    printf ("E_IR <current> <resistance>\n");
    printf ("E_PI <power> <current>\n");
    printf ("E_PR <power> <resistance>\n");

    printf ("R_EI <voltage> <current>\n");
    printf ("R_PI <power> <current>\n");
    printf ("R_EP <voltage> <power>\n");
    printf ("P_EI <voltage> <current>\n");
    printf ("P_RI <resistance> <current>\n");
    printf ("P_ER <voltage> <resistance>\n");


}



void ohmslaw_calchelp (char *calc) {

    if (0 == strcasecmp (calc, "I_ER")) {
        printf ("e_calc ohmslaw I_ER <voltage> <resistance>\n");
    }

    if (0 == strcasecmp (calc, "I_PE")) {
        printf ("e_calc ohmslaw I_PE <power> <voltage>\n");
    }

    if (0 == strcasecmp (calc, "I_PR")) {
        printf ("e_calc ohmslaw I_PR <power> <resistance>\n");
    }

    if (0 == strcasecmp (calc, "E_IR")) {
        printf ("e_calc ohmslaw E_IR <current> <resistance>\n");
    }

    if (0 == strcasecmp (calc, "E_PI")) {
        printf ("e_calc ohmslaw E_PI <power> <current>\n");
    }

    if (0 == strcasecmp (calc, "E_PR")) {
        printf ("e_calc ohmslaw E_PR <power> <resistance>\n");
    }

    if (0 == strcasecmp (calc, "R_EI")) {
        printf ("e_calc ohmslaw R_EI <voltage> <current>\n");
    }

    if (0 == strcasecmp (calc, "R_PI")) {
        printf ("e_calc ohmslaw R_PI <power> <current>\n");
    }

    if (0 == strcasecmp (calc, "R_EP")) {
        printf ("e_calc ohmslaw R_EP <voltage> <power>\n");
    }

    if (0 == strcasecmp (calc, "P_EI")) {
        printf ("e_calc ohmslaw P_EI <voltage> <current>\n");
    }

    if (0 == strcasecmp (calc, "P_RI")) {
        printf ("e_calc ohmslaw P_RI <resistance> <current>\n");
    }

    if (0 == strcasecmp (calc, "P_ER")) {
        printf ("e_calc ohmslaw P_ER <voltage> <resistance>\n");
    }

}

int ohmslaw_parse (int argc, char *argv[]) {

    double E;
    double I;
    double R;
    double P;
    char *value;
    if (argc < 3) {
        ohmslaw_printcalcs ();
        exit (EXIT_FAILURE);
    }

    /***** I_ER *****/

    if (0 == strcasecmp (argv[2], "I_ER")) {
        if (argc != 5) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!voltage_sscanf (argv[3], &E)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!resister_sscanf (argv[4], &R)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        I = I_ER (E, R);

        if (!(value = current_sprintf (4, I))) {
            exit (EXIT_FAILURE);
        }

        printf ("I = %s\n", value);
        free(value);
    }

    /***** I_PE *****/

    else if (0 == strcasecmp (argv[2], "I_PE")) {
        if (argc != 5) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!power_sscanf (argv[3], &P)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!voltage_sscanf (argv[4], &E)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        I = I_PE(P, E);

        if (!(value = current_sprintf (4, I))) {
            exit (EXIT_FAILURE);
        }

        printf ("I = %s\n", value);

        free(value);
    }

    /***** I_PR *****/

    else if (0 == strcasecmp (argv[2], "I_PR")) {
        if (argc != 5) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!power_sscanf (argv[3], &P)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!resister_sscanf (argv[4], &R)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        I = I_PR(P, R);

        if (!(value = current_sprintf (4, I))) {
            exit (EXIT_FAILURE);
        }

        printf ("I = %s\n", value);
        free(value);
    }

    /***** E_IR *****/

    else if (0 == strcasecmp (argv[2], "E_IR")) {
        if (argc != 5) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!current_sscanf (argv[3], &I)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!resister_sscanf (argv[4], &R)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        E = E_IR(I, R);

        if (!(value = voltage_sprintf (4, E))) {
            exit (EXIT_FAILURE);
        }

        printf ("E = %s\n", value);
        free(value);
    }

    /***** E_PI *****/

    else if (0 == strcasecmp (argv[2], "E_PI")) {
        if (argc != 5) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!power_sscanf (argv[3], &P)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!current_sscanf (argv[4], &I)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        E = E_PI(P, I);

        if (!(value = voltage_sprintf (4, E))) {
            exit (EXIT_FAILURE);
        }

        printf ("E = %s\n", value);
        free(value);
    }

    /***** E_PR *****/

    else if (0 == strcasecmp (argv[2], "E_PR")) {
        if (argc != 5) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!power_sscanf (argv[3], &P)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!resister_sscanf (argv[4], &R)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        E = E_PR(P, R);

        if (!(value = voltage_sprintf (4, E))) {
            exit (EXIT_FAILURE);
        }

        printf ("E = %s\n", value);
        free(value);
    }

    /***** R_EI *****/

    else if (0 == strcasecmp (argv[2], "R_EI")) {
        if (argc != 5) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!voltage_sscanf (argv[3], &E)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!current_sscanf (argv[4], &I)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        R = R_EI(E, I);

        if (!(value = resister_sprintf (4, R))) {
            exit (EXIT_FAILURE);
        }

        printf ("R = %s\n", value);
        free(value);
    }

    /***** R_PI *****/

    else if (0 == strcasecmp (argv[2], "R_PI")) {
        if (argc != 5) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (! power_sscanf (argv[3], &P)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!current_sscanf (argv[4], &I)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        R = R_PI(P, I);

        if (!(value = resister_sprintf (4, R))) {
            exit (EXIT_FAILURE);
        }

        printf ("R = %s\n", value);
        free(value);
    }

    /***** R_EP *****/

    else if (0 == strcasecmp (argv[2], "R_EP")) {
        if (argc != 5) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (! voltage_sscanf (argv[3], &E)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!power_sscanf (argv[4], &P)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        R = R_EP(E, P);

        if (!(value = resister_sprintf (4, R))) {
            exit (EXIT_FAILURE);
        }

        printf ("R = %s\n", value);
        free(value);
    }

    /***** P_EI *****/

    else if (0 == strcasecmp (argv[2], "P_EI")) {
        if (argc != 5) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!voltage_sscanf (argv[3], &E)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!current_sscanf (argv[4], &I)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        P = P_EI(E, I);

        if (!(value = power_sprintf (4, P))) {
            exit (EXIT_FAILURE);
        }

        printf ("P = %s\n", value);
        free(value);
    }

    /***** P_RI *****/

    else if (0 == strcasecmp (argv[2], "P_RI")) {
        if (argc != 5) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (! resister_sscanf (argv[3], &R)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!current_sscanf (argv[4], &I)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        P = P_RI(R, I);

        if (!(value = resister_sprintf (4, P))) {
            exit (EXIT_FAILURE);
        }

        printf ("P = %s\n", value);
        free(value);
    }

    /***** P_ER *****/

    else if (0 == strcasecmp (argv[2], "P_ER")) {
        if (argc != 5) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!voltage_sscanf (argv[3], &E)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }

        if (!resister_sscanf (argv[4], &R)) {
            ohmslaw_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        
        P = P_ER(E, R);

        if (!(value = power_sprintf (4, P))) {
            exit (EXIT_FAILURE);
        }

        printf ("P = %s\n", value);
        free(value);
    }

    else {
        ohmslaw_printcalcs();
        exit (EXIT_FAILURE);
    }

    return 1;
}





