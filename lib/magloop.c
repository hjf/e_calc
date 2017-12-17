/*******************************************************************************
 *
 * Project: e_calc
 * 
 * magloop.c
 * 
 *
 *******************************************************************************
 * Copyright (c) 2014,  Glen E. Gardner Jr 
 *
 * LOOP6.C by Glen E. Gardner Jr.  08/2014
 * de AA8C   
 * glen.gardner@verizon.net
 * glen.gardner@gridtoys.com
 *
 * Reformated for this application by Brian Case
 * KU7F
 * rush at winkey dot org
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

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<strings.h>

#include"frequency.h"
#include"reactance.h"
#include"skineffect.h"
#include"ohmslaw.h"
#include"permeability.h"
#include"magloop.h"
#include"smith.h"


/*******************************************************************************
 function to calc a loop iterating over freq

*******************************************************************************/
// coefficient of coupling

/* this cant be solved for */
#define K 0.5

/* num harmonics for Smiths prox calc */
#define NHARMONICS 12


#define INCR_EQUAL_IF_GREATER(V, I, E) (((V) + (I) > (E)) ? (E) : (V) + (I))

/*******************************************************************************
    function to count how big of a result array we need
*******************************************************************************/

size_t magloop_count_output(
    double dfStart,
    double dfEnd,
    double dfIncr
);

magloop_out_t *magloop_calc (
    magloop_in_t *sIn,
    size_t *nOuts)
{

    magloop_out_t *sResult = NULL;


    double dfValue;


    size_t iSize = 0;

    /***** itterate LoopCircumference? *****/

    if (sIn->LoopCircumference < 0) {

        /**** alocate output space *****/

        iSize = magloop_count_output(
            sIn->LoopCircumferenceStart,
            sIn->LoopCircumferenceEnd,
            sIn->LoopCircumferenceIncr);
    
        if (NULL == (sResult = malloc( (iSize + 1) * sizeof(magloop_out_t) ) ) ) {
            fprintf(stderr, "magloop_calc() malloc() failed\n");
            return NULL;
        }

        *nOuts = iSize;

        /***** iterate over the wanted calcs *****/        

        for ( iSize = 0, dfValue = sIn->LoopCircumferenceStart;
              iSize < *nOuts;
              iSize++, dfValue = INCR_EQUAL_IF_GREATER(dfValue,
                                                  sIn->LoopCircumferenceIncr,
                                                  sIn->LoopCircumferenceEnd)
        ) {
             magloop_calc_mainloop(
                dfValue,
                sIn->LoopConductorDiameter,
                sIn->LoopConductorSpacing,
                sIn->Resistivity,
                sIn->RelativePermeabilityConductor,
                /*double K,*/
                sIn->TxPower,
                sIn->Frequency,
                sIn->isSquare,
                sIn->nLoops,
                iSize,
                sResult);
        }
    }

    /***** itterate LoopConductorDiameter? *****/

    else if (sIn->LoopConductorDiameter < 0) {

        /**** alocate output space *****/

        iSize = magloop_count_output(
            sIn->LoopConductorDiameterStart,
            sIn->LoopConductorDiameterEnd,
            sIn->LoopConductorDiameterIncr);
    
        if (NULL == (sResult = malloc( (iSize + 1) * sizeof(magloop_out_t) ) ) ) {
            fprintf(stderr, "magloop_calc() malloc() failed\n");
            return NULL;
        }

        *nOuts = iSize;

        /***** iterate over the wanted calcs *****/        

        for ( iSize = 0, dfValue = sIn->LoopConductorDiameterStart;
              iSize < *nOuts;
              iSize++, dfValue = INCR_EQUAL_IF_GREATER(dfValue,
                                                  sIn->LoopConductorDiameterIncr,
                                                  sIn->LoopConductorDiameterEnd)
        ) {
             magloop_calc_mainloop(
                sIn->LoopCircumference,
                dfValue,
                sIn->LoopConductorSpacing,
                sIn->Resistivity,
                sIn->RelativePermeabilityConductor,
                /*double K,*/
                sIn->TxPower,
                sIn->Frequency,
                sIn->isSquare,
                sIn->nLoops,
                iSize,
                sResult);
        }
    }

    /***** itterate LoopConductorSpacing? *****/

    else if (sIn->LoopConductorSpacing < 0) {

        /**** alocate output space *****/

        iSize = magloop_count_output(
            sIn->LoopConductorSpacingStart,
            sIn->LoopConductorSpacingEnd,
            sIn->LoopConductorSpacingIncr);
    
        if (NULL == (sResult = malloc( (iSize + 1) * sizeof(magloop_out_t) ) ) ) {
            fprintf(stderr, "magloop_calc() malloc() failed\n");
            return NULL;
        }

        *nOuts = iSize;

        /***** iterate over the wanted calcs *****/        

        for ( iSize = 0, dfValue = sIn->LoopConductorSpacingStart;
              iSize < *nOuts;
              iSize++, dfValue = INCR_EQUAL_IF_GREATER(dfValue,
                                                  sIn->LoopConductorSpacingIncr,
                                                  sIn->LoopConductorSpacingEnd)
        ) {
             magloop_calc_mainloop(
                sIn->LoopCircumference,
                sIn->LoopConductorDiameter,            
                dfValue,
                sIn->Resistivity,
                sIn->RelativePermeabilityConductor,
                /*double K,*/
                sIn->TxPower,
                sIn->Frequency,
                sIn->isSquare,
                sIn->nLoops,
                iSize,
                sResult);
        }
    }

    /***** itterate Frequency? *****/

    else if (sIn->Frequency < 0) {

        /**** alocate output space *****/

        iSize = magloop_count_output(
            sIn->FrequencyStart,
            sIn->FrequencyEnd,
            sIn->FrequencyIncr);
    
        if (NULL == (sResult = malloc( (iSize + 1) * sizeof(magloop_out_t) ) ) ) {
            fprintf(stderr, "magloop_calc() malloc() failed\n");
            return NULL;
        }

        *nOuts = iSize;

        /***** iterate over the wanted calcs *****/        

        for ( iSize = 0, dfValue = sIn->FrequencyStart;
              iSize < *nOuts;
              iSize++, dfValue = INCR_EQUAL_IF_GREATER(dfValue,
                                                  sIn->FrequencyIncr,
                                                  sIn->FrequencyEnd)
        ) {
             magloop_calc_mainloop(
                sIn->LoopCircumference,
                sIn->LoopConductorDiameter,            
                sIn->LoopConductorSpacing,
                sIn->Resistivity,
                sIn->RelativePermeabilityConductor,
                /*double K,*/
                sIn->TxPower,
                dfValue,
                sIn->isSquare,
                sIn->nLoops,
                iSize,
                sResult);
        }
    }

    /***** itterate nLoops? *****/

    else if (sIn->nLoops < 0) {

        /**** alocate output space *****/

        iSize = magloop_count_output(
            (double)sIn->nLoopsStart,
            (double)sIn->nLoopsEnd,
            (double)sIn->nLoopsIncr);
    
        if (NULL == (sResult = malloc( (iSize + 1) * sizeof(magloop_out_t) ) ) ) {
            fprintf(stderr, "magloop_calc() malloc() failed\n");
            return NULL;
        }

        *nOuts = iSize;

        /***** iterate over the wanted calcs *****/        

        for ( iSize = 0, dfValue = (double)sIn->nLoopsStart;
              iSize < *nOuts;
              iSize++, dfValue = INCR_EQUAL_IF_GREATER(dfValue,
                                                  (double)sIn->nLoopsIncr,
                                                  (double)sIn->nLoopsEnd)
        ) {
             magloop_calc_mainloop(
                sIn->LoopCircumference,
                sIn->LoopConductorDiameter,            
                sIn->LoopConductorSpacing,
                sIn->Resistivity,
                sIn->RelativePermeabilityConductor,
                /*double K,*/
                sIn->TxPower,
                sIn->Frequency,
                sIn->isSquare,
                (int)dfValue,
                iSize,
                sResult);
        }
    }

    /***** no iteration *****/

    else {

        if (NULL == (sResult = malloc( (1 + 1) * sizeof(magloop_out_t) ) ) ) {
            fprintf(stderr, "magloop_calc() malloc() failed\n");
            return NULL;
        }

        *nOuts = 1;


        magloop_calc_mainloop(
            sIn->LoopCircumference,
            sIn->LoopConductorDiameter,            
            sIn->LoopConductorSpacing,
            sIn->Resistivity,
            sIn->RelativePermeabilityConductor,
            /*double K,*/
            sIn->TxPower,
            sIn->Frequency,
            sIn->isSquare,
            sIn->nLoops,
            iSize,
            sResult);
    
    }

    return sResult;
}

/*******************************************************************************
    function to count how big of a result array we need
*******************************************************************************/

size_t magloop_count_output(
    double dfStart,
    double dfEnd,
    double dfIncr
) {
    size_t iSize = 0;

    double dfValue = dfStart;
    double dfLast;
    
    do {
        dfLast = dfValue; 
        dfValue = dfValue + dfIncr;
        iSize++;

    } while (dfValue <= dfEnd && dfValue > dfLast);
    
    return iSize;
}

/*******************************************************************************
 function to calc a loop with no iteration
*******************************************************************************/

void magloop_calc_mainloop(
    double LoopCircumference,
    double LoopConductorDiameter,
    double LoopConductorSpacing,
    double Resistivity,
    double RelativePermeabilityConductor,
    /*double K,*/
    double TxPower,
    double Frequency,
    int isSquare,
    int nLoops,
    size_t i,
    magloop_out_t *result
) {

    // rf loss resistance for skin effect
    double RfResistanceSkinEffect;

    // radius of the loop conductor
    double LoopConductorRadius;

    // diameter of the loop
    double LoopDiamater;

    // radius of the loop
    double LoopRadius;

    // area of the loop
    double LoopArea;

    // wavelength
    double Wavelength;

    // capacative reactance
    double Xc;

    if(isSquare) {
        LoopDiamater = LoopCircumference / 4;
        LoopRadius = LoopDiamater / 2;
        LoopArea = LoopDiamater * LoopDiamater;
    }

    else {
        LoopDiamater = LoopCircumference / M_PI;
        LoopRadius = LoopDiamater / 2;
        LoopArea = M_PI * SQUARED(LoopRadius);
    
    }

    LoopConductorRadius=LoopConductorDiameter/2;

    result[i].LoopCircumference  = LoopCircumference;
    result[i].LoopConductorDiameter = LoopConductorDiameter;
    result[i].LoopConductorSpacing = LoopConductorSpacing;
    result[i].Frequency = Frequency;
    result[i].nLoops = nLoops;

    result[i].LoopDiamater = LoopDiamater;

    Wavelength=W_F(Frequency);
    
    /***** check some known limits on loop size *****/

    result[i].CircCheck = 0;
    if (LoopCircumference < Wavelength / 3) {
        result[i].CircCheck = 1;
    }

    result[i].RadiusCheck = 0;
    if (LoopRadius < Wavelength / (6 * M_PI)) {
        result[i].RadiusCheck = 1;
    }

          
    // calculate the radiation resistance
    result[i].RadiationResistance = CalcRadiationResistance(
        LoopArea,
        Wavelength,
        isSquare,
        nLoops,
        1);
    
    // skin effect

    result[i].SkinDepth = SKIN_DEPTH_RFP(
        Resistivity,
        Frequency,
        RelativePermeabilityConductor);

    RfResistanceSkinEffect = SKIN_RESISTANCE_THICK (
        LoopCircumference,
        Resistivity,
        LoopConductorDiameter,
        result[i].SkinDepth);

    // only 1 loop?

    if (nLoops == 1) {

        result[i].RfResistanceLoss2=RfResistanceSkinEffect;
    }

    // the rf resistance loss decreases as
    // the number of loops increase.
    
    else {
        
        // more than one loop is present, estimate the proximity effect

        result[i].RfResistanceLoss2 = CalcProximity(
            RfResistanceSkinEffect,
            LoopCircumference,
            LoopConductorDiameter,
            LoopConductorSpacing,
            nLoops);
    }

    // calculate the efficiency of the loop
    result[i].Efficiency = CalcEfficiency(
        result[i].RadiationResistance,
        result[i].RfResistanceLoss2);

    // calculate the inductance of the loop

    //FIXME do we add in the conductors self inductance? is this formula correct?

    //double self = ((PERMEABILITY_FS * RelativePermeabilityConductor) / 4) *LoopCircumference;
    //printf("self inductance %.12lf\n", self);

    if (!isSquare) {
        result[i].LoopInductance = CalcLoopInductance(
            LoopRadius,
            RelativePermeabilityConductor,
            LoopConductorRadius,
            1/*,
            K*/);
    }
    else {
        result[i].LoopInductance = CalcSquareLoopInductance(
            LoopDiamater,
            RelativePermeabilityConductor,
            LoopConductorRadius,
            1/*,
            K*/);
    }
    
    // calculate the reactance of the loop
    // as well as series input impedance

    result[i].Xl= XL(
        Frequency,
        result[i].LoopInductance
    );

    result[i].LoopSeriesImpedance=sqrt(
        result[i].RadiationResistance
        + result[i].RfResistanceLoss2
    );

    Xc=result[i].Xl;

    result[i].C = C_XcF(Xc, Frequency);

    //FIXME why is the series resistance multiplied by 2 before the Q calc

    result[i].Q=result[i].Xl
                    / (2*(    result[i].RadiationResistance
                            + result[i].RfResistanceLoss2));

    result[i].SixDbBandwidth = Fbw_FcutQ(Frequency, result[i].Q);

    result[i].SwrBandwidth=2*(result[i].SixDbBandwidth/(3/0.512));
    result[i].LcDynamicResistance=result[i].Xl*result[i].Q;
    
    result[i].VMAX = E_PR(TxPower, result[i].LcDynamicResistance);

    // calculate the area  of the small coupling loop

    //FIXME this should be an input
    double Zin=50.0;

    result[i].PickupLoopArea = CalcPickupLoopArea(
        LoopArea,
        result[i].LoopSeriesImpedance,
        Zin);

    return;
}

/*******************************************************************************
@brief Fuction to calculate radiation resistance

@param LoopArea     area of the loop
@param Wavelength
@param isSquare     non zero if the loop is square
@param nLoops     number of parallel loops

@returns radiation resistance

*******************************************************************************/

double CalcRadiationResistance(
    double LoopArea,
    double Wavelength,
    int isSquare,
    int nLoops,
    int nTurns)
{
    double RadiationResistance;

    // radiation resistance for a single loop
    // This is the favored model for radiation resistance in a loop
    // It is problematic in that it is an approximation, and it is not 
    // correct in the case of a square loop.
    
    /*
    i have found evidence that this is a more acurate version of the formula

    double n = 120 * M_PI; // this is in ohms but what is it, and is it correct?
    RadiationResistance =     n * (8/3) * CUBED(M_PI)
                            * SQUARED(nTurns * (LoopArea/SQUARED(Wavelength)));
    */

    RadiationResistance = 31171 * ( SQUARED(LoopArea) / FOURTH(Wavelength) );

    // It turns out that the radiation resistance for a round loop
    // is very close to 1.621 times greater than that of a square loop.
    //
    // correction for a square loop

    if (isSquare) {
        RadiationResistance = RadiationResistance / 1.621;
    }

    // radiation resistance increases directly with the 
    // the number of parallel loops
    // so we need to adjust it.
    RadiationResistance = RadiationResistance * nLoops;

    return RadiationResistance;
}



/*******************************************************************************
@brief Fuction to calculate combined rf loss resistance for skin effect and
       proximity effect

@param RfResistanceSkinEffect   rf loss resistance for skin effect
@param LoopCircumference        circumference of the loop
@param nLoops                 number of paralell loops

@returns rf loss resistance for skin effect and proximity effect

*******************************************************************************/

double CalcProximity(
    double RfResistanceSkinEffect,
    double LoopCircumference,
    double LoopConductorDiameter,
    double LoopConductorSpacing,
    double nLoops)
{
    double Ro;
    double Rp;
    double Rpo;

    double RfResistanceLoss2;
    Ro=RfResistanceSkinEffect/LoopCircumference;

    // This parameter is estimated based on Glen Smith ,1971
    // This ends up being a little contrived, but will work
    // provided that the antenna is built with the spacing
    // between conductors equal to or greater than that 
    // recommended by Smith.
    //

    // Yes. Yet another approximation.
    // Boldly assuming that the increase in proximity
    // effect is close to nLoops cubed!
    //Rpo=0.003*nLoops*nLoops*nLoops;

    // try Smiths code

    Rpo = SmithParallelLeastSquares(
        nLoops,
        NHARMONICS,
        LoopConductorDiameter,
        LoopConductorSpacing
    );

    Rp=Ro*Rpo;
    

    RfResistanceLoss2=(Rp+Ro)*LoopCircumference;

    return RfResistanceLoss2;
}

/*******************************************************************************
@brief Fuction to calculate loop Efficiency

@param RadiationResistance  radiation resistance
@param RfResistanceLoss2    combined rf loss resistance for skin effect and
                            proximity effect

@returns loop Efficiency

*******************************************************************************/

double CalcEfficiency(
    double RadiationResistance,
    double RfResistanceLoss2)
{

    double Efficiency;

    // ground effects are ignored in this case
    Efficiency=RadiationResistance/(RadiationResistance+RfResistanceLoss2);

    return Efficiency;
}
/*
double CalcEfficiency_Smith(
    double LoopRadius,
    double LoopConductorRadius,
    double Frequency,
    double Conductivity,
    double nLoops,
    double Rp,
    double Ro
{
    double Efficiency;

    Efficiency =
        1/ (1 + (   (8.48 * pow(10, -10) * sqrt((Frequency /1e6) * Conductivity))
                  / (n* CUBED(LoopRadius) * LoopConductorRadius)
                ) 
              * ( 1 + (Rp/Ro))
           );
    
    return Efficiency;
}
*/
/*******************************************************************************
@brief Fuction to calculate loop Inductance

@param LoopRadius                       radius of the loop
@param RelativePermeabilityConductor    relative permeability of the conductor
@param LoopConductorRadius              radius of the loop conductor
@param nLoops                         number of paralell loops
@param K                                coefficient of coupling

@returns loop inductance

*******************************************************************************/

double CalcLoopInductance(
    double LoopRadius,
    double RelativePermeabilityConductor,
    double LoopConductorRadius,
    double nLoops/*,
    double K*/)
{

    double LoopInductance;

    // This is for a circular loop
    LoopInductance =      LoopRadius 
                        * PERMEABILITY_FS 
                        * RelativePermeabilityConductor
                        *( log(( 8 * LoopRadius ) / LoopConductorRadius) -2 );

    // adjust for the coefficient of coupling in parallel conductor loops

    if(nLoops>1)LoopInductance=LoopInductance/(nLoops*K);

    return LoopInductance;
}



/*

mutual inductance

M is the mutual inductance
N1 is the number of turns on the first inductor
N2 = number of turns on the second inductor
P is the permeance of the space occupied by the flux.

M = N1 * N2 * P 

so that means in our case....
M = P

coupling coefficient

K coupling coefficient
m mutual inductance
L1 inductance of first inductor
L2 inductance of second inductor

M= k * sqrt ( L1 * l2)

or in our case calc the impedance of a single loop?

M = K * Lsingle

 i assume that...
M = K * cuberoot(L1 * L2 * L3)


*/


/*******************************************************************************
@brief Fuction to calculate square loop Inductance

@param LoopDiamater                     diameter of the loop
@param RelativePermeabilityConductor    relative permeability of the conductor
@param LoopConductorRadius              radius of the loop conductor
@param nLoops                         number of paralell loops
@param K                                coefficient of coupling


@returns loop inductance

*******************************************************************************/

double CalcSquareLoopInductance(
    double LoopDiamater,
    double RelativePermeabilityConductor,
    double LoopConductorRadius,
    double nLoops/*,
    double K*/)

{

    double LoopInductance;

    LoopInductance=((   2 
                      * PERMEABILITY_FS
                      * RelativePermeabilityConductor
                      * LoopDiamater)/M_PI
                    ) * (log( LoopDiamater / LoopConductorRadius) - 0.77401);

    // adjust for the coefficient of coupling in parallel conductor loops

    if(nLoops>1)LoopInductance=LoopInductance/(nLoops*K);

    return LoopInductance;
}

/*******************************************************************************
@brief Fuction to calculate pickup loop area

@param LoopArea             area of the loop
@param LoopSeriesImpedance  loop series impedance
@param Zin                  series input impedance

@returns pickup loop area

*******************************************************************************/

double CalcPickupLoopArea(
    double LoopArea,
    double LoopSeriesImpedance,
    double Zin)
{

    double PickupLoopArea;

    PickupLoopArea= LoopArea * (LoopSeriesImpedance/sqrt(Zin) );

    return PickupLoopArea;
}




