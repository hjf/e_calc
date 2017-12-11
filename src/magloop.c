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

int magloop_parse_circle(int argc, char *argv[]);
int magloop_parse_multicircle(int argc, char *argv[]);
int magloop_parse_square(int argc, char *argv[]);
int magloop_parse_multisquare(int argc, char *argv[]);

char * magloop_calcs[] = {
    "circle",
    "multicircle",
    "square",
    "multisquare",
    ""
};


void magloop_printcalcs (void) {

    int i;

    for (i = 0 ; *magloop_calcs[i] != '\0' ; i++) {
        printf ("%s\n", magloop_calcs[i]);
    }

}

void magloop_calchelp (char *calc) {

    if (0 == strcasecmp (calc, "circle")) {
        printf ("e_calc magloop circle \n");
        printf ("   <LOOP CIRCUMFERENCE> \n");
        printf ("   <CONDUCTOR DIAMETER> \n");
        printf ("   <RESISTIVITY OF LOOP MATERIAL> \n");
        printf ("       Resistivity of aluminum 37nOhms\n");
        printf ("       Resistivity of copper 16.8nOhms\n");
        printf ("   <RELATIVE PERMEABILITY OF LOOP MATERIAL> \n");
        printf ("       Relative permeability of aluminum 1.000022\n");
        printf ("       Relative permeability of copper 0.999994\n");
        printf ("   <TRANSMITTER POWER> \n");
        printf ("   <LOW FREQUENCY LIMIT>\n");
        printf ("   <HIGH FREQUENCY LIMIT>\n");
        printf ("   <FREQUENCY STEP>\n");
    }

    else if (0 == strcasecmp (calc, "multicircle")) {
        printf ("e_calc magloop multicircle \n");
        printf ("   <LOOP CIRCUMFERENCE> \n");
        printf ("   <CONDUCTOR DIAMETER> \n");
        printf ("   <CONDUCTOR SPACING> \n");
        printf ("   <RESISTIVITY OF LOOP MATERIAL> \n");
        printf ("       Resistivity of aluminum 37nOhms\n");
        printf ("       Resistivity of copper 16.8nOhms\n");
        printf ("   <RELATIVE PERMEABILITY OF LOOP MATERIAL> \n");
        printf ("       Relative permeability of aluminum 1.000022\n");
        printf ("       Relative permeability of copper 0.999994\n");
        //printf ("   <LOOP COEFFICIENT OF COUPLING>\n");
        //printf ("       0.6 for a start point\n");
        printf ("   <TRANSMITTER POWER> \n");
        printf ("   <LOW FREQUENCY LIMIT> \n");
        printf ("   <HIGH FREQUENCY LIMIT> \n");
        printf ("   <FREQUENCY STEP> \n");
        printf ("   NUMBER OF LOOP CONDUCTORS>\n");
    }

    else if (0 == strcasecmp (calc, "square")) {
        printf ("e_calc magloop square \n");
        printf ("   <LOOP CIRCUMFERENCE> \n");
        printf ("   <CONDUCTOR DIAMETER> \n");
        printf ("   <RESISTIVITY OF LOOP MATERIAL> \n");
        printf ("       Resistivity of aluminum 37nOhms\n");
        printf ("       Resistivity of copper 16.8nOhms\n");
        printf ("   <RELATIVE PERMEABILITY OF LOOP MATERIAL> \n");
        printf ("       Relative permeability of aluminum 1.000022\n");
        printf ("       Relative permeability of copper 0.999994\n");
        printf ("   <TRANSMITTER POWER> \n");
        printf ("   <LOW FREQUENCY LIMIT>\n");
        printf ("   <HIGH FREQUENCY LIMIT>\n");
        printf ("   <FREQUENCY STEP>\n");
    }

    else if (0 == strcasecmp (calc, "multisquare")) {
        printf ("e_calc magloop multisquare \n");
        printf ("   <LOOP CIRCUMFERENCE> \n");
        printf ("   <CONDUCTOR DIAMETER> \n");
        printf ("   <CONDUCTOR SPACING> \n");
        printf ("   <RESISTIVITY OF LOOP MATERIAL> \n");
        printf ("       Resistivity of aluminum 37nOhms\n");
        printf ("       Resistivity of copper 16.8nOhms\n");
        printf ("   <RELATIVE PERMEABILITY OF LOOP MATERIAL> \n");
        printf ("       Relative permeability of aluminum 1.000022\n");
        printf ("       Relative permeability of copper 0.999994\n");
        //printf ("   <LOOP COEFFICIENT OF COUPLING>\n");
        //printf ("       0.6 for a start point\n");
        printf ("   <TRANSMITTER POWER> \n");
        printf ("   <LOW FREQUENCY LIMIT> \n");
        printf ("   <HIGH FREQUENCY LIMIT> \n");
        printf ("   <FREQUENCY STEP> \n");
        printf ("   NUMBER OF LOOP CONDUCTORS>\n");
    }



}


int magloop_parse (int argc, char *argv[]) {

    if (argc < 3) {
        magloop_printcalcs ();
        exit (EXIT_FAILURE);
    }
    
    /***** circle *****/

    if (0 == strcasecmp (argv[2], "circle")) {
        if (argc < 11) { // 8 args plus 3 to get here
            magloop_calchelp ("circle");
            
            exit (EXIT_FAILURE);
        }

        return magloop_parse_circle(argc - 3, argv + 3);
    }

    /***** multicircle *****/

    if (0 == strcasecmp (argv[2], "multicircle")) {
        if (argc < 12) { // 9 args plus 3 to get here
            magloop_calchelp ("multicircle");
            
            exit (EXIT_FAILURE);
        }

        return magloop_parse_multicircle(argc - 3, argv + 3);
    }

    /***** square *****/

    else if (0 == strcasecmp (argv[2], "square")) {
        if (argc < 11) { // 8 args plus 3 to get here
            magloop_calchelp ("square");
            
            exit (EXIT_FAILURE);
        }

        return magloop_parse_square(argc - 3, argv + 3);
    }

    else {
        magloop_printcalcs();
        exit (EXIT_FAILURE);
    }

    return 1;
}

/*******************************************************************************
circle
*******************************************************************************/

int magloop_parse_circle(int argc, char *argv[]) {

    double LoopCircumference;
    double LoopConductorDiameter;
    double Resistivity;
    double RelativePermeabilityConductor;
    double TxPower;
    double FrequencyLowLimit;
    double FrequencyHighLimit;
    double FrequencyStep;
    size_t nOuts;

    size_t i;
    char *value;

    magloop_out_t *result = NULL;

    if (argc < 8) {
        magloop_calchelp (argv[2]);
        exit (EXIT_FAILURE);
    }

    if (!distance_sscanf (argv[0], &LoopCircumference)) {
        magloop_calchelp ("circle");
        exit (EXIT_FAILURE);
    }
        
    if (!distance_sscanf (argv[1], &LoopConductorDiameter)) {
        magloop_calchelp ("circle");
        exit (EXIT_FAILURE);
    }
        
    if (!resister_sscanf (argv[2], &Resistivity)) {
        magloop_calchelp ("circle");
        exit (EXIT_FAILURE);
    }
        
    if (!inductor_sscanf (argv[3], &RelativePermeabilityConductor)) {
        magloop_calchelp ("circle");
        exit (EXIT_FAILURE);
    }
        
    if (!power_sscanf (argv[4], &TxPower)) {
        magloop_calchelp ("circle");
        exit (EXIT_FAILURE);
    }
        
    if (!frequency_sscanf (argv[5], &FrequencyLowLimit)) {
        magloop_calchelp ("circle");
        exit (EXIT_FAILURE);
    }
        
    if (!frequency_sscanf (argv[6], &FrequencyHighLimit)) {
        magloop_calchelp ("circle");

        exit (EXIT_FAILURE);
    }
        
    if (!frequency_sscanf (argv[7], &FrequencyStep)) {
        magloop_calchelp ("circle");
        exit (EXIT_FAILURE);
    }
    
    if (!(result = magloop_calc (
                LoopCircumference, 
                LoopConductorDiameter,
                0,
                Resistivity,
                RelativePermeabilityConductor,
                TxPower,
                FrequencyLowLimit,
                FrequencyHighLimit,
                FrequencyStep,
                0,
                1,
                &nOuts))
    ) {
        magloop_calchelp ("circle");
        exit (EXIT_FAILURE);
    } 
    
    else {

        for (i = 0; i < nOuts ; i++) {

            printf("\n");

            if (!(value = frequency_sprintf (4, result[i].Frequency))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("Frequency = %s\n", value);
                free (value);
            }
            printf("_______________________________________\n");

            if (!(value = resister_sprintf (4, result[i].RadiationResistance))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("RadiationResistance = %s\n", value);
                free (value);
            }

            if (!(value = distance_sprintf (4, result[i].SkinDepth))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("SkinDepth = %s\n", value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result[i].RfResistanceLoss2))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("RfResistanceLoss2 = %s\n", value);
                free (value);
            }

            printf ("Efficiency = %lf\n", result[i].Efficiency);


            if (!(value = inductor_sprintf (4, result[i].LoopInductance))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("LoopInductance = %s\n", value);
                free (value);
            }

            if (!(value = capacitor_sprintf (4, result[i].C))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("C = %s\n", value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result[i].Xl))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("Xl = %s\n", value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result[i].LoopSeriesImpedance))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("LoopSeriesImpedance = %s\n", value);
                free (value);
            }

            printf ("Q = %lf\n", result[i].Q);
            

            if (!(value = frequency_sprintf (4, result[i].SixDbBandwidth))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("SixDbBandwidth = %s\n", value);
                free (value);
            }

            if (!(value = frequency_sprintf (4, result[i].SwrBandwidth))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("SwrBandwidth = %s\n", value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result[i].LcDynamicResistance))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("LcDynamicResistance = %s\n", value);
                free (value);
            }

            if (!(value = voltage_sprintf (4, result[i].VMAX))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("VMAX = %s\n", value);
                free (value);
            }



            if (!(value = distance_sprintf (4, result[i].LoopDiamater))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("LoopDiamater = %s\n", value);
                free (value);
            }

            if (!(value = distance_sprintf (4, result[i].PickupLoopArea))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("PickupLoopArea = %s\n", value);
                free (value);
            }

            if (result[i].CircCheck == 1) {
                printf ("Circumference check good\n");
            }
            else {
                printf ("Circumference too large\n");
            }

            if (result[i].RadiusCheck == 1) {
                printf ("Radius check good\n");
            }
            else {
                printf ("Radius too large\n");
            }

            printf("\n");
        }

        free(result);

    }

    return 1;
}

/*******************************************************************************
multicircle
*******************************************************************************/

int magloop_parse_multicircle(int argc, char *argv[]) {

    double LoopCircumference;
    double LoopConductorDiameter;
    double LoopConductorSpacing;
    double Resistivity;
    double RelativePermeabilityConductor;
    double TxPower;
    double FrequencyLowLimit;
    double FrequencyHighLimit;
    double FrequencyStep;
    int nLoops;
    size_t nOuts;

    size_t i;
    char *value;

    magloop_out_t *result = NULL;

    if (argc < 10) {
        magloop_calchelp ("multicircle");
        exit (EXIT_FAILURE);
    }

    if (!distance_sscanf (argv[0], &LoopCircumference)) {
        magloop_calchelp ("multicircle");
        exit (EXIT_FAILURE);
    }
        
    if (!distance_sscanf (argv[1], &LoopConductorDiameter)) {
        magloop_calchelp ("multicircle");
        exit (EXIT_FAILURE);
    }
        
    if (!distance_sscanf (argv[2], &LoopConductorSpacing)) {
        magloop_calchelp ("multicircle");
        exit (EXIT_FAILURE);
    }
        
    if (!resister_sscanf (argv[3], &Resistivity)) {
        magloop_calchelp ("multicircle");
        exit (EXIT_FAILURE);
    }
        
    if (!inductor_sscanf (argv[4], &RelativePermeabilityConductor)) {
        magloop_calchelp ("multicircle");
        exit (EXIT_FAILURE);
    }
        
    if (!power_sscanf (argv[5], &TxPower)) {
        magloop_calchelp ("multicircle");
        exit (EXIT_FAILURE);
    }
        
    if (!frequency_sscanf (argv[6], &FrequencyLowLimit)) {
        magloop_calchelp ("multicircle");
        exit (EXIT_FAILURE);
    }
        
    if (!frequency_sscanf (argv[7], &FrequencyHighLimit)) {
        magloop_calchelp ("multicircle");

        exit (EXIT_FAILURE);
    }
        
    if (!frequency_sscanf (argv[8], &FrequencyStep)) {
        magloop_calchelp ("multicircle");
        exit (EXIT_FAILURE);
    }
    
    if (1 != sscanf (argv[9], "%i", &nLoops)) {
        magloop_calchelp ("multicircle");
        exit (EXIT_FAILURE);
    }

    if (!(result = magloop_calc (
                LoopCircumference, 
                LoopConductorDiameter,
                LoopConductorSpacing,
                Resistivity,
                RelativePermeabilityConductor,
                TxPower,
                FrequencyLowLimit,
                FrequencyHighLimit,
                FrequencyStep,
                0,
                nLoops,
                &nOuts))
    ) {
        magloop_calchelp ("multicircle");
        exit (EXIT_FAILURE);
    } 
    
    else {

        for (i = 0; i < nOuts ; i++) {

            printf("\n");

            if (!(value = frequency_sprintf (4, result[i].Frequency))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("Frequency = %s\n", value);
                free (value);
            }
            printf("_______________________________________\n");

            if (!(value = resister_sprintf (4, result[i].RadiationResistance))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("RadiationResistance = %s\n", value);
                free (value);
            }

            if (!(value = distance_sprintf (4, result[i].SkinDepth))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("SkinDepth = %s\n", value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result[i].RfResistanceLoss2))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("RfResistanceLoss2 = %s\n", value);
                free (value);
            }

            printf ("Efficiency = %lf\n", result[i].Efficiency);


            if (!(value = inductor_sprintf (4, result[i].LoopInductance))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("LoopInductance = %s\n", value);
                free (value);
            }

            if (!(value = capacitor_sprintf (4, result[i].C))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("C = %s\n", value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result[i].Xl))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("Xl = %s\n", value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result[i].LoopSeriesImpedance))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("LoopSeriesImpedance = %s\n", value);
                free (value);
            }

            printf ("Q = %lf\n", result[i].Q);
            

            if (!(value = frequency_sprintf (4, result[i].SixDbBandwidth))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("SixDbBandwidth = %s\n", value);
                free (value);
            }

            if (!(value = frequency_sprintf (4, result[i].SwrBandwidth))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("SwrBandwidth = %s\n", value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result[i].LcDynamicResistance))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("LcDynamicResistance = %s\n", value);
                free (value);
            }

            if (!(value = voltage_sprintf (4, result[i].VMAX))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("VMAX = %s\n", value);
                free (value);
            }



            if (!(value = distance_sprintf (4, result[i].LoopDiamater))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("LoopDiamater = %s\n", value);
                free (value);
            }

            if (!(value = distance_sprintf (4, result[i].PickupLoopArea))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("PickupLoopArea = %s\n", value);
                free (value);
            }

            if (result[i].CircCheck == 1) {
                printf ("Circumference check good\n");
            }
            else {
                printf ("Circumference too large\n");
            }

            if (result[i].RadiusCheck == 1) {
                printf ("Radius check good\n");
            }
            else {
                printf ("Radius too large\n");
            }

            printf("\n");
        }

        free(result);

    }

    return 1;
}

/*******************************************************************************
square
*******************************************************************************/

int magloop_parse_square(int argc, char *argv[]) {

    double LoopCircumference;
    double LoopConductorDiameter;
    double Resistivity;
    double RelativePermeabilityConductor;
    double TxPower;
    double FrequencyLowLimit;
    double FrequencyHighLimit;
    double FrequencyStep;
    size_t nOuts;

    size_t i;
    char *value;

    magloop_out_t *result = NULL;

    if (argc < 8) {
        magloop_calchelp ("square");
        exit (EXIT_FAILURE);
    }

    if (!distance_sscanf (argv[0], &LoopCircumference)) {
        magloop_calchelp ("square");
        exit (EXIT_FAILURE);
    }
        
    if (!distance_sscanf (argv[1], &LoopConductorDiameter)) {
        magloop_calchelp ("square");
        exit (EXIT_FAILURE);
    }
        
    if (!resister_sscanf (argv[2], &Resistivity)) {
        magloop_calchelp ("square");
        exit (EXIT_FAILURE);
    }
        
    if (!inductor_sscanf (argv[3], &RelativePermeabilityConductor)) {
        magloop_calchelp ("square");
        exit (EXIT_FAILURE);
    }
        
    if (!power_sscanf (argv[4], &TxPower)) {
        magloop_calchelp ("square");
        exit (EXIT_FAILURE);
    }
        
    if (!frequency_sscanf (argv[5], &FrequencyLowLimit)) {
        magloop_calchelp ("square");
        exit (EXIT_FAILURE);
    }
        
    if (!frequency_sscanf (argv[6], &FrequencyHighLimit)) {
        magloop_calchelp ("square");

        exit (EXIT_FAILURE);
    }
        
    if (!frequency_sscanf (argv[7], &FrequencyStep)) {
        magloop_calchelp ("square");
        exit (EXIT_FAILURE);
    }
    

    if (!(result = magloop_calc (
                LoopCircumference, 
                LoopConductorDiameter,
                0,
                Resistivity,
                RelativePermeabilityConductor,
                TxPower,
                FrequencyLowLimit,
                FrequencyHighLimit,
                FrequencyStep,
                1,
                1,
                &nOuts))
    ) {
        magloop_calchelp ("square");
        exit (EXIT_FAILURE);
    } 
    
    else {

        for (i = 0; i < nOuts ; i++) {

            printf("\n");

            if (!(value = frequency_sprintf (4, result[i].Frequency))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("Frequency = %s\n", value);
                free (value);
            }
            printf("_______________________________________\n");

            if (!(value = resister_sprintf (4, result[i].RadiationResistance))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("RadiationResistance = %s\n", value);
                free (value);
            }

            if (!(value = distance_sprintf (4, result[i].SkinDepth))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("SkinDepth = %s\n", value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result[i].RfResistanceLoss2))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("RfResistanceLoss2 = %s\n", value);
                free (value);
            }

            printf ("Efficiency = %lf\n", result[i].Efficiency);


            if (!(value = inductor_sprintf (4, result[i].LoopInductance))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("LoopInductance = %s\n", value);
                free (value);
            }

            if (!(value = capacitor_sprintf (4, result[i].C))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("C = %s\n", value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result[i].Xl))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("Xl = %s\n", value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result[i].LoopSeriesImpedance))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("LoopSeriesImpedance = %s\n", value);
                free (value);
            }

            printf ("Q = %lf\n", result[i].Q);
            

            if (!(value = frequency_sprintf (4, result[i].SixDbBandwidth))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("SixDbBandwidth = %s\n", value);
                free (value);
            }

            if (!(value = frequency_sprintf (4, result[i].SwrBandwidth))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("SwrBandwidth = %s\n", value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result[i].LcDynamicResistance))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("LcDynamicResistance = %s\n", value);
                free (value);
            }

            if (!(value = voltage_sprintf (4, result[i].VMAX))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("VMAX = %s\n", value);
                free (value);
            }



            if (!(value = distance_sprintf (4, result[i].LoopDiamater))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("LoopDiamater = %s\n", value);
                free (value);
            }

            if (!(value = distance_sprintf (4, result[i].PickupLoopArea))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("PickupLoopArea = %s\n", value);
                free (value);
            }
            printf("\n");
        }

        free(result);

    }

    return 1;
}

/*******************************************************************************
multisquare
*******************************************************************************/

int magloop_parse_multisquare(int argc, char *argv[]) {

    double LoopCircumference;
    double LoopConductorDiameter;
    double LoopConductorSpacing;
    double Resistivity;
    double RelativePermeabilityConductor;
    double TxPower;
    double FrequencyLowLimit;
    double FrequencyHighLimit;
    double FrequencyStep;
    int nLoops;
    size_t nOuts;

    size_t i;
    char *value;

    magloop_out_t *result = NULL;

    if (argc < 9) {
        magloop_calchelp ("multisquare");
        exit (EXIT_FAILURE);
    }

    if (!distance_sscanf (argv[0], &LoopCircumference)) {
        magloop_calchelp ("multisquare");
        exit (EXIT_FAILURE);
    }
        
    if (!distance_sscanf (argv[1], &LoopConductorDiameter)) {
        magloop_calchelp ("multisquare");
        exit (EXIT_FAILURE);
    }
        
    if (!distance_sscanf (argv[2], &LoopConductorSpacing)) {
        magloop_calchelp ("multicircle");
        exit (EXIT_FAILURE);
    }
        
    if (!resister_sscanf (argv[3], &Resistivity)) {
        magloop_calchelp ("multisquare");
        exit (EXIT_FAILURE);
    }
        
    if (!inductor_sscanf (argv[4], &RelativePermeabilityConductor)) {
        magloop_calchelp ("multisquare");
        exit (EXIT_FAILURE);
    }
        
    if (!power_sscanf (argv[5], &TxPower)) {
        magloop_calchelp ("multisquare");
        exit (EXIT_FAILURE);
    }
        
    if (!frequency_sscanf (argv[6], &FrequencyLowLimit)) {
        magloop_calchelp ("multisquare");
        exit (EXIT_FAILURE);
    }
        
    if (!frequency_sscanf (argv[7], &FrequencyHighLimit)) {
        magloop_calchelp ("multisquare");

        exit (EXIT_FAILURE);
    }
        
    if (!frequency_sscanf (argv[8], &FrequencyStep)) {
        magloop_calchelp ("multisquare");
        exit (EXIT_FAILURE);
    }
    
    if (1 != sscanf (argv[8], "%i", &nLoops)) {
        magloop_calchelp ("multisquare");
        exit (EXIT_FAILURE);
    }

    if (!(result = magloop_calc (
                LoopCircumference, 
                LoopConductorDiameter,
                LoopConductorSpacing,
                Resistivity,
                RelativePermeabilityConductor,
                TxPower,
                FrequencyLowLimit,
                FrequencyHighLimit,
                FrequencyStep,
                1,
                nLoops,
                &nOuts))
    ) {
        magloop_calchelp ("multisquare");
        exit (EXIT_FAILURE);
    } 
    
    else {

        for (i = 0; i < nOuts ; i++) {

            printf("\n");

            if (!(value = frequency_sprintf (4, result[i].Frequency))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("Frequency = %s\n", value);
                free (value);
            }
            printf("_______________________________________\n");

            if (!(value = resister_sprintf (4, result[i].RadiationResistance))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("RadiationResistance = %s\n", value);
                free (value);
            }

            if (!(value = distance_sprintf (4, result[i].SkinDepth))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("SkinDepth = %s\n", value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result[i].RfResistanceLoss2))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("RfResistanceLoss2 = %s\n", value);
                free (value);
            }

            printf ("Efficiency = %lf\n", result[i].Efficiency);


            if (!(value = inductor_sprintf (4, result[i].LoopInductance))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("LoopInductance = %s\n", value);
                free (value);
            }

            if (!(value = capacitor_sprintf (4, result[i].C))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("C = %s\n", value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result[i].Xl))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("Xl = %s\n", value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result[i].LoopSeriesImpedance))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("LoopSeriesImpedance = %s\n", value);
                free (value);
            }

            printf ("Q = %lf\n", result[i].Q);
            

            if (!(value = frequency_sprintf (4, result[i].SixDbBandwidth))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("SixDbBandwidth = %s\n", value);
                free (value);
            }

            if (!(value = frequency_sprintf (4, result[i].SwrBandwidth))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("SwrBandwidth = %s\n", value);
                free (value);
            }

            if (!(value = resister_sprintf (4, result[i].LcDynamicResistance))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("LcDynamicResistance = %s\n", value);
                free (value);
            }

            if (!(value = voltage_sprintf (4, result[i].VMAX))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("VMAX = %s\n", value);
                free (value);
            }



            if (!(value = distance_sprintf (4, result[i].LoopDiamater))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("LoopDiamater = %s\n", value);
                free (value);
            }

            if (!(value = distance_sprintf (4, result[i].PickupLoopArea))) {
                exit (EXIT_FAILURE);
            }
            else {
                printf ("PickupLoopArea = %s\n", value);
                free (value);
            }
            printf("\n");
        }

        free(result);

    }

    return 1;
}
