/*******************************************************************************
 *
 * Project: e_calc
 * 
 * tank.c
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
#include "lib/tank.h"
#include "lib/frequency.h"
#include "lib/resister.h"
#include "lib/capacitor.h"
#include "lib/inductor.h"

void tank_printcalcs (void) {

    printf ("tank tank\n");

}

void tank_calchelp (char *calc) {

    if (0 == strcasecmp (calc, "tank")) {
        printf ("e_calc tank tank <Center freq> <width freq> <Impedance>\n");
    }
}


int tank_parse (int argc, char *argv[]) {

    struct Tank result;
    double Z;
    double Fcenter;
    double Fwidth;
    char *value;
    int i;

    if (argc < 3) {
        tank_printcalcs();
        exit (EXIT_FAILURE);
    }

    if (0 == strcasecmp (argv[2], "tank")) {
        if (argc < 6) {
            tank_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        if (!frequency_sscanf (argv[3], &Fcenter)) {
            tank_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        if (!frequency_sscanf (argv[4], &Fwidth)) {
            tank_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }
        if (!resister_sscanf (argv[5], &Z)) {
            tank_calchelp (argv[2]);
            exit (EXIT_FAILURE);
        }


        result = Tank_calc (Fcenter, Fwidth, Z);


        if (!(value = inductor_sprintf (4, result.L))) {
                exit (EXIT_FAILURE);
            }
        else {
            printf ("L = %s\n", value);
            free (value);
        }

        if (!(value = capacitor_sprintf (4, result.C))) {
            exit (EXIT_FAILURE);
        }
        else {
            printf ("C = %s\n", value);
            free (value);
        }
    }

    else {
        tank_printcalcs();
        exit (EXIT_FAILURE);
    }

    return 1;
}






