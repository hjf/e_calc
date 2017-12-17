/*******************************************************************************
 *
 * Project: e_calc
 * 
 * magloop.c
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
#include "lib/inductor.h"
#include "lib/frequency.h"
#include "lib/power.h"
#include "lib/distance.h"
#include "lib/capacitor.h"
#include "lib/voltage.h"
#include "lib/magloop.h"

//#define DEBUG

int magloop_parse_calc(int argc, char *argv[]);

char * magloop_calcs[] = {
    "calc",
    ""
};


void magloop_printcalcs (void) {

    int i;

    for (i = 0 ; *magloop_calcs[i] != '\0' ; i++) {
        printf ("%s\n", magloop_calcs[i]);
    }

}

void magloop_calchelp (char *calc) {

    if (0 == strcasecmp (calc, "calc")) {
        printf ("e_calc magloop byfreq \n");
        printf ("   <single or parallel\n");
        printf ("   <circle or square>\n");
        printf ("   < <Loop Circumference> |\n");
        printf ("        <incr>\n");
        printf ("        <Loop Circumference Start>\n");
        printf ("        <Loop Circumference End>\n");
        printf ("        <Loop Circumference Incr> >\n");
        printf ("   < <Conductor Diameter> |\n");
        printf ("        <incr>\n");
        printf ("        <Conductor Diameter Start>\n");
        printf ("        <Conductor Diameter End>\n");
        printf ("        <Conductor Diameter Incr> >\n");
        printf ("   [ if parallel <Conductor Spacing> |\n");
        printf ("        <incr>\n");
        printf ("        <Conductor Spacing Start>\n");
        printf ("        <Conductor Spacing End>\n");
        printf ("        <Conductor Spacing Incr> ]\n");
        printf ("   <Resistivity Of Loop Material> \n");
        printf ("       Resistivity of aluminum 37nOhms\n");
        printf ("       Resistivity of copper 16.8nOhms\n");
        printf ("   <Relative Permeability Of Loop MateriaL> \n");
        printf ("       Relative permeability of aluminum 1.000022\n");
        printf ("       Relative permeability of copper 0.999994\n");
        printf ("   <Transmitter Power> \n");
        printf ("   < <Frequency> |\n");
        printf ("        <incr>\n");
        printf ("        <Frequency Start>\n");
        printf ("        <Frequency End>\n");
        printf ("        <Frequency Incr> >\n");
        printf ("   [ if parallel <Number Of Loop Conductors> |\n");
        printf ("        <incr>\n");
        printf ("        <Number Of Loop Conductors Start>\n");
        printf ("        <Number Of Loop Conductors End>\n");
        printf ("        <Number Of Loop Conductors Incr> ]\n");
        printf ("\n");
        printf ("NOTE you can only itterate over one argument\n");

    }

    else {
        magloop_printcalcs ();
    }
}


int magloop_parse (int argc, char *argv[]) {

    if (argc < 3) {
        magloop_printcalcs ();
        exit (EXIT_FAILURE);
    }
    
    /***** single *****/

    if (0 == strcasecmp (argv[2], "calc")) {
         // ((8 OR 11) or (10 OR 13)) plus 3 to get here
        if (argc != 11 && argc != 14 && argc != 13 && argc != 16) {
            magloop_calchelp ("calc");
            
            exit (EXIT_FAILURE);
        }

        return magloop_parse_calc(argc - 3, argv + 3);
    }

    else {
        magloop_printcalcs();
        exit (EXIT_FAILURE);
    }

    return 1;
}

/*******************************************************************************
magloop print results
*******************************************************************************/


int magloop_print(magloop_out_t sResult) {

    char *value;

    if (!(value = resister_sprintf (4, sResult.RadiationResistance))) {
        exit (EXIT_FAILURE);
    }
    else {
        printf ("RadiationResistance = %s\n", value);
        free (value);
    }

    if (!(value = distance_sprintf (4, sResult.SkinDepth))) {
        exit (EXIT_FAILURE);
    }
    else {
        printf ("SkinDepth = %s\n", value);
        free (value);
    }

    if (!(value = resister_sprintf (4, sResult.RfResistanceLoss2))) {
        exit (EXIT_FAILURE);
    }
    else {
        printf ("RfResistanceLoss2 = %s\n", value);
        free (value);
    }

    printf ("Efficiency = %lf\n", sResult.Efficiency);


    if (!(value = inductor_sprintf (4, sResult.LoopInductance))) {
        exit (EXIT_FAILURE);
    }
    else {
        printf ("LoopInductance = %s\n", value);
        free (value);
    }

    if (!(value = capacitor_sprintf (4, sResult.C))) {
        exit (EXIT_FAILURE);
    }
    else {
        printf ("C = %s\n", value);
        free (value);
    }

    if (!(value = resister_sprintf (4, sResult.Xl))) {
        exit (EXIT_FAILURE);
    }
    else {
        printf ("Xl = %s\n", value);
        free (value);
    }

    if (!(value = resister_sprintf (4, sResult.LoopSeriesImpedance))) {
        exit (EXIT_FAILURE);
    }
    else {
        printf ("LoopSeriesImpedance = %s\n", value);
        free (value);
    }

    printf ("Q = %lf\n", sResult.Q);
    

    if (!(value = frequency_sprintf (4, sResult.SixDbBandwidth))) {
        exit (EXIT_FAILURE);
    }
    else {
        printf ("SixDbBandwidth = %s\n", value);
        free (value);
    }

    if (!(value = frequency_sprintf (4, sResult.SwrBandwidth))) {
        exit (EXIT_FAILURE);
    }
    else {
        printf ("SwrBandwidth = %s\n", value);
        free (value);
    }

    if (!(value = resister_sprintf (4, sResult.LcDynamicResistance))) {
        exit (EXIT_FAILURE);
    }
    else {
        printf ("LcDynamicResistance = %s\n", value);
        free (value);
    }

    if (!(value = voltage_sprintf (4, sResult.VMAX))) {
        exit (EXIT_FAILURE);
    }
    else {
        printf ("VMAX = %s\n", value);
        free (value);
    }



    if (!(value = distance_sprintf (4, sResult.LoopDiamater))) {
        exit (EXIT_FAILURE);
    }
    else {
        printf ("LoopDiamater = %s\n", value);
        free (value);
    }

    if (!(value = distance_sprintf (4, sResult.PickupLoopArea))) {
        exit (EXIT_FAILURE);
    }
    else {
        printf ("PickupLoopArea = %s\n", value);
        free (value);
    }

    if (sResult.CircCheck == 1) {
        printf ("Circumference check good\n");
    }
    else {
        printf ("Circumference too large\n");
    }

    if (sResult.RadiusCheck == 1) {
        printf ("Radius check good\n");
    }
    else {
        printf ("Radius too large\n");
    }

    return 1;
}

/*******************************************************************************
 calc
*******************************************************************************/

int magloop_parse_calc(int argc, char *argv[]) {

    magloop_in_t sIn;
    size_t nOuts;

    size_t i;
    char *value;

    magloop_out_t *sResult = NULL;

    size_t nArg = 0;

    int count;
    int isIncr = 0;

#ifdef DEBUG
    printf("parsing single/parallel\n");
#endif

    int isParallel = -1;
    sscanf (argv[nArg], "%*1[sS]%*1[iI]%*1[nN]%*1[gG]%*1[lL]%*1[eE]%n", &count);
    if (count == 6) {
        isParallel = 0;
        if (argc != 8 && argc != 11 ) {
            magloop_calchelp ("calc");
            exit (EXIT_FAILURE);
        }
    }

    count = 0;
    sscanf (argv[nArg], "%*1[pP]%*1[aA]%*1[rR]%*1[aA]%*1[lL]%*1[lL]%*1[eE]%*1[lL]%n", &count);
    if (count == 8) {
        isParallel = 1;
        if (argc != 10 && argc != 13) {
            magloop_calchelp ("calc");
            exit (EXIT_FAILURE);
        }
    }
    
    if (isParallel < 0) {
        magloop_calchelp ("calc");
        exit (EXIT_FAILURE);
    }

    nArg++;
    
#ifdef DEBUG
    printf("parsing circle/square\n");
#endif

    count = 0;
    sIn.isSquare = -1;
    sscanf (argv[nArg], "%*1[cC]%*1[iI]%*1[rR]%*1[cC]%*1[lL]%*1[eE]%n", &count);
    if (count == 6) {
        sIn.isSquare = 0;
    }  

    count = 0;
    sscanf (argv[nArg], "%*1[sS]%*1[qQ]%*1[uU]%*1[aA]%*1[rR]%*1[eE]%n", &count);
    if (count == 6) {
        sIn.isSquare = 1;
    }

    if (sIn.isSquare < 0) {
        magloop_calchelp ("calc");
        exit (EXIT_FAILURE);
    }

    nArg++;

    /***** LoopCircumference *****/

#ifdef DEBUG
    printf("parsing LoopCircumference\n");
#endif

    count = 0;
    sscanf (argv[nArg], "%*1[iI]%*1[nN]%*1[cC]%*1[rR]%n", &count);
    if (count == 4) {
        sIn.LoopCircumference = -1;
        isIncr = 1;
        nArg++;
        if (!distance_sscanf (argv[nArg++], &sIn.LoopCircumferenceStart)) {
            magloop_calchelp ("calc");
            exit (EXIT_FAILURE);
        }

        if (!distance_sscanf (argv[nArg++], &sIn.LoopCircumferenceEnd)) {
            magloop_calchelp ("calc");
            exit (EXIT_FAILURE);
        }

        if (!distance_sscanf (argv[nArg++], &sIn.LoopCircumferenceIncr)) {
            magloop_calchelp ("calc");
            exit (EXIT_FAILURE);
        }

    }

    else if (!distance_sscanf (argv[nArg++], &sIn.LoopCircumference)) {
        magloop_calchelp ("calc");
        exit (EXIT_FAILURE);
    }
    
    /***** LoopConductorDiameter *****/

#ifdef DEBUG
    printf("parsing LoopConductorDiameter\n");
#endif

    count = 0;
    sscanf (argv[nArg], "%*1[iI]%*1[nN]%*1[cC]%*1[rR]%n", &count);
    if (count == 4) {
        if (isIncr) {
            printf("Already incr over a argument\n"); 
            magloop_calchelp ("calc");
            exit (EXIT_FAILURE);
        }
        sIn.LoopConductorDiameter = -1;
        isIncr = 1;
        nArg++;
        if (!distance_sscanf (argv[nArg++], &sIn.LoopConductorDiameterStart)) {
            magloop_calchelp ("calc");
            exit (EXIT_FAILURE);
        }

        if (!distance_sscanf (argv[nArg++], &sIn.LoopConductorDiameterEnd)) {
            magloop_calchelp ("calc");
            exit (EXIT_FAILURE);
        }

        if (!distance_sscanf (argv[nArg++], &sIn.LoopConductorDiameterIncr)) {
            magloop_calchelp ("calc");
            exit (EXIT_FAILURE);
        }

    }

    else if (!distance_sscanf (argv[nArg++], &sIn.LoopConductorDiameter)) {
        magloop_calchelp ("calc");
        exit (EXIT_FAILURE);
    }
        
    /***** LoopConductorSpacing *****/

    if (isParallel) {
#ifdef DEBUG
    printf("parsing LoopConductorSpacing\n");
#endif

        count = 0;
        sscanf (argv[nArg], "%*1[iI]%*1[nN]%*1[cC]%*1[rR]%n", &count);
        if (count == 4) {
            if (isIncr) {
                printf("Already incr over a argument\n"); 
                magloop_calchelp ("calc");
                exit (EXIT_FAILURE);
            }
            sIn.LoopConductorSpacing = -1;
            isIncr = 1;
            nArg++;
            if (!distance_sscanf (argv[nArg++], &sIn.LoopConductorSpacingStart)) {
                magloop_calchelp ("calc");
                exit (EXIT_FAILURE);
            }

            if (!distance_sscanf (argv[nArg++], &sIn.LoopConductorSpacingEnd)) {
                magloop_calchelp ("calc");
                exit (EXIT_FAILURE);
            }

            if (!distance_sscanf (argv[nArg++], &sIn.LoopConductorSpacingIncr)) {
                magloop_calchelp ("calc");
                exit (EXIT_FAILURE);
            }

        }

        else if (!distance_sscanf (argv[nArg++], &sIn.LoopConductorSpacing)) {
            magloop_calchelp ("calc");
            exit (EXIT_FAILURE);
        }
    }        

#ifdef DEBUG
    printf("parsing Resistivity\n");
#endif

    if (!resister_sscanf (argv[nArg++], &sIn.Resistivity)) {
        magloop_calchelp ("calc");
        exit (EXIT_FAILURE);
    }
        
#ifdef DEBUG
    printf("parsing RelativePermeabilityConductor\n");
#endif

    if (!inductor_sscanf (argv[nArg++], &sIn.RelativePermeabilityConductor)) {
        magloop_calchelp ("calc");
        exit (EXIT_FAILURE);
    }
        
#ifdef DEBUG
    printf("parsing TxPower\n");
#endif

    if (!power_sscanf (argv[nArg++], &sIn.TxPower)) {
        magloop_calchelp ("calc");
        exit (EXIT_FAILURE);
    }

    /***** Frequency *****/

#ifdef DEBUG
    printf("parsing Frequency\n");
#endif

    count = 0;
    sscanf (argv[nArg], "%*1[iI]%*1[nN]%*1[cC]%*1[rR]%n", &count);
    if (count == 4) {
        if (isIncr) {
            printf("Already incr over a argument\n"); 
            magloop_calchelp ("calc");
            exit (EXIT_FAILURE);
        }
        sIn.Frequency = -1;
        isIncr = 1;
        nArg++;
        if (!frequency_sscanf (argv[nArg++], &sIn.FrequencyStart)) {
            magloop_calchelp ("calc");
            exit (EXIT_FAILURE);
        }

        if (!frequency_sscanf (argv[nArg++], &sIn.FrequencyEnd)) {
            magloop_calchelp ("calc");
            exit (EXIT_FAILURE);
        }

        if (!frequency_sscanf (argv[nArg++], &sIn.FrequencyIncr)) {
            magloop_calchelp ("calc");
            exit (EXIT_FAILURE);
        }

    }

    else if (!frequency_sscanf (argv[nArg++], &sIn.Frequency)) {
        magloop_calchelp ("calc");
        exit (EXIT_FAILURE);
    }
       

    /***** nLoops *****/

    if (isParallel) {
#ifdef DEBUG
    printf("parsing nLoops\n");
#endif

        count = 0;
        sscanf (argv[nArg], "%*1[iI]%*1[nN]%*1[cC]%*1[rR]%n", &count);
        if (count == 4) {
            if (isIncr) {
                printf("Already incr over a argument\n"); 
                magloop_calchelp ("calc");
                exit (EXIT_FAILURE);
            }
            sIn.nLoops = -1;
            isIncr = 1;
            nArg++;
            if (1 != sscanf (argv[nArg++], "%i", &sIn.nLoopsStart)) {
                magloop_calchelp ("calc");
                exit (EXIT_FAILURE);
            }

            if (1 != sscanf (argv[nArg++], "%i", &sIn.nLoopsEnd)) {
                magloop_calchelp ("calc");
                exit (EXIT_FAILURE);
            }

            if (1 != sscanf (argv[nArg++], "%i", &sIn.nLoopsIncr)) {
                magloop_calchelp ("calc");
                exit (EXIT_FAILURE);
            }

        }

        else if (1 != sscanf (argv[nArg++], "%i", &sIn.nLoops)) {
            magloop_calchelp ("calc");
            exit (EXIT_FAILURE);
        }
    }
    else {
        sIn.nLoops = 1;
    }
    
    if (!(sResult = magloop_calc(
                &sIn,
                &nOuts))
    ) {
        magloop_calchelp ("calc");
        exit (EXIT_FAILURE);
    }
    
    else {

        for (i = 0; i < nOuts ; i++) {

            printf("\n");

            if (!(value = distance_sprintf (4, sResult[i].LoopCircumference))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("Loop Circumference = %s\n", value);
                free (value);
            }
            if (!(value = distance_sprintf (4, sResult[i].LoopConductorDiameter))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("Loop Conductor Diameter = %s\n", value);
                free (value);
            }

            if (isParallel) {
                if (!(value = distance_sprintf (4, sResult[i].LoopConductorSpacing))) {
                    exit (EXIT_FAILURE);
                }
                else {
                    printf ("Loop Conductor Spacing = %s\n", value);
                    free (value);
                }
            }

            if (isParallel) {
                printf ("Number of Loops = %i\n", sResult[i].nLoops);
            }

            if (!(value = frequency_sprintf (4, sResult[i].Frequency))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("Frequency = %s\n", value);
                free (value);
            }

            printf("_______________________________________\n");
            printf("\n");

            magloop_print(sResult[i]);

            printf("\n");
        }

        free(sResult);

    }

    return 1;
}


