/*******************************************************************************
 *
 * Project: e_calc
 * 
 * smith.c
 * 
 *
 *******************************************************************************
 * Copyright (c) 2017,  Brian Case 
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
#include "lib/distance.h"
#include "lib/smith.h"


int smith_parse_ParallelCollocation(int argc, char *argv[]);
int smith_parse_ParallelLeastSquares(int argc, char *argv[]);
int smith_parse_MultiturnCollocation(int argc, char *argv[]);

char * smith_calcs[] = {
    "ParallelCollocation",
    "ParallelLeastSquares",
    "MultiturnCollocation",
    ""
};

void smith_printcalcs (void) {

    int i;

    for (i = 0 ; *smith_calcs[i] != '\0' ; i++) {
        printf ("%s\n", smith_calcs[i]);
    }

}

void smith_calchelp (char *calc) {

    if (0 == strcasecmp (calc, "ParallelCollocation")) {
        printf ("e_calc smith ParallelCollocation \n");
        printf ("   <Num Wires> \n");
        printf ("   <Num Harmonics> \n");
        printf ("       6 or more for better results\n");
        printf ("   <Wire Diameter> \n");
        printf ("   <Center to Center Wire Spaceing> \n");
    }

    else if (0 == strcasecmp (calc, "ParallelLeastSquares")) {
        printf ("e_calc smith ParallelLeastSquares \n");
        printf ("   <Num Wires> \n");
        printf ("   <Num Harmonics> \n");
        printf ("       6 or more for better results\n");
        printf ("   <Wire Diameter> \n");
        printf ("   <Center to Center Wire Spaceing> \n");
    }

    else if (0 == strcasecmp (calc, "MultiturnCollocation")) {
        printf ("e_calc smith MultiturnCollocation \n");
        printf ("   note this calc is incomplete\n");
        printf ("   <Num Turns> \n");
        printf ("   <Num Harmonics> \n");
        printf ("       6 or more for better results\n");
        printf ("   <Wire Diameter> \n");
        printf ("   <Center to Center Wire Spaceing> \n");
    }

}


int smith_parse (int argc, char *argv[]) {

    if (argc < 3) {
        smith_printcalcs ();
        exit (EXIT_FAILURE);
    }
    
    /***** ParallelCollocation *****/

    if (0 == strcasecmp (argv[2], "ParallelCollocation")) {
        if (argc < 7) { // 4 args plus 3 to get here
            smith_calchelp ("ParallelCollocation");
            
            exit (EXIT_FAILURE);
        }

        return smith_parse_ParallelCollocation(argc - 3, argv + 3);
    }

    /***** ParallelLeastSquares *****/

    else if (0 == strcasecmp (argv[2], "ParallelLeastSquares")) {
        if (argc < 7) { // 4 args plus 3 to get here
            smith_calchelp ("ParallelLeastSquares");
            
            exit (EXIT_FAILURE);
        }

        return smith_parse_ParallelLeastSquares(argc - 3, argv + 3);
    }

    /***** MultiturnCollocation *****/

    else if (0 == strcasecmp (argv[2], "MultiturnCollocation")) {
        if (argc < 7) { // 4 args plus 3 to get here
            smith_calchelp ("MultiturnCollocation");
            
            exit (EXIT_FAILURE);
        }

        return smith_parse_MultiturnCollocation(argc - 3, argv + 3);
    }

    else {
        smith_printcalcs();
        exit (EXIT_FAILURE);
    }

    return 1;
}

#define ARGC_CK(ARGC, N) (ARGC < 4 || ARGC > 4)
/*******************************************************************************
ParallelCollocation
*******************************************************************************/

int smith_parse_ParallelCollocation(int argc, char *argv[]) {

    int nWires;
    int nHarmonics;
    double dfWireDia;
    double dfWireSpacing;

    char *value;

    if (ARGC_CK(argc, 4) ) {
        smith_calchelp (argv[2]);
        exit (EXIT_FAILURE);
    }

    if (1 != sscanf (argv[0], "%i", &nWires)) {
        smith_calchelp ("ParallelCollocation");
        exit (EXIT_FAILURE);
    }

    if (1 != sscanf (argv[1], "%i", &nHarmonics)) {
        smith_calchelp ("ParallelCollocation");
        exit (EXIT_FAILURE);
    }

    if (!distance_sscanf (argv[2], &dfWireDia)) {
        smith_calchelp ("ParallelCollocation");
        exit (EXIT_FAILURE);
    }
        
    if (!distance_sscanf (argv[3], &dfWireSpacing)) {
        smith_calchelp ("ParallelCollocation");
        exit (EXIT_FAILURE);
    }

    double dfResN = SmithParallelCollocation(
        nWires,
        nHarmonics,
        dfWireDia,
        dfWireSpacing);

        
    printf("\n");

    if (!(value = resister_sprintf (4, dfResN))) {
        exit (EXIT_FAILURE);
    }
    else {
        printf ("Normalized Resistance = %s\n", value);
        free (value);
    }

    return 1;
}

/*******************************************************************************
ParallelLeastSquares
*******************************************************************************/

int smith_parse_ParallelLeastSquares(int argc, char *argv[]) {

    int nWires;
    int nHarmonics;
    double dfWireDia;
    double dfWireSpacing;

    char *value;

    if (ARGC_CK(argc, 4)) {
        smith_calchelp (argv[2]);
        exit (EXIT_FAILURE);
    }

    if (1 != sscanf (argv[0], "%i", &nWires)) {
        smith_calchelp ("ParallelLeastSquares");
        exit (EXIT_FAILURE);
    }

    if (1 != sscanf (argv[1], "%i", &nHarmonics)) {
        smith_calchelp ("ParallelLeastSquares");
        exit (EXIT_FAILURE);
    }

    if (!distance_sscanf (argv[2], &dfWireDia)) {
        smith_calchelp ("ParallelLeastSquares");
        exit (EXIT_FAILURE);
    }
        
    if (!distance_sscanf (argv[3], &dfWireSpacing)) {
        smith_calchelp ("ParallelLeastSquares");
        exit (EXIT_FAILURE);
    }

    double dfResN = SmithParallelLeastSquares(
        nWires,
        nHarmonics,
        dfWireDia,
        dfWireSpacing);

        
    printf("\n");

    if (!(value = resister_sprintf (4, dfResN))) {
        exit (EXIT_FAILURE);
    }
    else {
        printf ("Normalized Resistance = %s\n", value);
        free (value);
    }

    return 1;
}

/*******************************************************************************
MultiturnCollocation
*******************************************************************************/

int smith_parse_MultiturnCollocation(int argc, char *argv[]) {

    int nTurns;
    int nHarmonics;
    double dfWireDia;
    double dfWireSpacing;

    char *value;

    if (ARGC_CK(argc, 4)) {
        smith_calchelp (argv[2]);
        exit (EXIT_FAILURE);
    }

    if (1 != sscanf (argv[0], "%i", &nTurns)) {
        smith_calchelp ("MultiturnCollocation");
        exit (EXIT_FAILURE);
    }

    if (1 != sscanf (argv[1], "%i", &nHarmonics)) {
        smith_calchelp ("MultiturnCollocation");
        exit (EXIT_FAILURE);
    }

    if (!distance_sscanf (argv[2], &dfWireDia)) {
        smith_calchelp ("MultiturnCollocation");
        exit (EXIT_FAILURE);
    }
        
    if (!distance_sscanf (argv[3], &dfWireSpacing)) {
        smith_calchelp ("MultiturnCollocation");
        exit (EXIT_FAILURE);
    }

    double dfResN = SmithMultiturnCollocation(
        nTurns,
        nHarmonics,
        dfWireDia,
        dfWireSpacing);

        
    printf("\n");

    if (!(value = resister_sprintf (4, dfResN))) {
        exit (EXIT_FAILURE);
    }
    else {
        printf ("Normalized Resistance = %s\n", value);
        free (value);
    }

    return 1;
}




