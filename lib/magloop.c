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


/*******************************************************************************
 function to calc a single loop

*******************************************************************************/
// coefficient of coupling

#define K 0.427139

magloop_out_t *magloop_single_calc (
    double LoopCircumference,
    double LoopConductorDiameter,
    double Resistivity,
    double RelativePermeabilityConductor,
    /*double K,*/
    double TxPower,
    double FrequencyLowLimit,
    double FrequencyHighLimit,
    double FrequencyStep,
    int isSquare,
    size_t *nOuts)
{

    magloop_out_t *result = NULL;

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

    // operating frequency
    double Frequency;

    // wavelength
    double Wavelength;

    // capacative reactance
    double Xc;

    LoopConductorRadius=LoopConductorDiameter/2;
    
    LoopDiamater=LoopCircumference/M_PI;
    LoopRadius=LoopDiamater/2;
    LoopArea=M_PI*SQUARED(LoopRadius);

    /**** alocate output space *****/

    double lastFreq = 0;
    size_t i = 0;

    Frequency=FrequencyLowLimit;
    
    do {
        lastFreq = Frequency; 
        Frequency=Frequency+FrequencyStep;
        i++;

    } while (Frequency <= FrequencyHighLimit && Frequency > lastFreq);
    
    /***** step might not divide evenly into the range *****/

    if (Frequency > FrequencyHighLimit) {
        i++;
    }

    *nOuts = i;
            
    if (NULL == (result = malloc( (i + 1) * sizeof(magloop_out_t) ) ) ) {

        //fixme pRINT AN ERROR?
        return NULL;
    }

    for ( i = 0, Frequency = FrequencyLowLimit;
          i < *nOuts;
          i++, Frequency = Frequency + FrequencyStep
    ) {

        /***** step might not divide evenly into the range *****/

        if (Frequency > FrequencyHighLimit) {
            Frequency = FrequencyHighLimit;
        }

        result[i].Frequency = Frequency;

        result[i].LoopDiamater = LoopDiamater;
        Wavelength=W_F(Frequency);
        
        // calculate the radiation resistance
        result[i].RadiationResistance = CalcRadiationResistance(
            LoopArea,
            Wavelength,
            isSquare,
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

        // the rf resistance loss decreases as
        // the number of loops increase.
        // in this case 1 loop only

        result[i].RfResistanceLoss2=RfResistanceSkinEffect;

        //RfResistanceLoss2=RfResistanceSkinEffect/NumLoops;


        /*
        // if more than one loop is present, estimate the proximity effect
        if (NumLoops > 1)    
            result[i].RfResistanceLoss2 = CalcProximity(
                RfResistanceSkinEffect,
                LoopCircumference,
                NumLoops);
        }
        */

        // calculate the efficiency of the loop
        result[i].Efficiency = CalcEfficiency(
            result[i].RadiationResistance,
            result[i].RfResistanceLoss2);

        // calculate the inductance of the loop

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

        result[i].Q=result[i].Xl
                        / (2*(    result[i].RadiationResistance
                                + result[i].RfResistanceLoss2));

        result[i].SixDbBandwidth = Fbw_FcutQ(Frequency, result[i].Q);

        result[i].SwrBandwidth=2*(result[i].SixDbBandwidth/(3/0.512));
        result[i].LcDynamicResistance=result[i].Xl*result[i].Q;
        
        result[i].VMAX = E_PR(TxPower, result[i].LcDynamicResistance);

        // calculate the area  of the small coupling loop
        // fixme this should be an input
        double Zin=50.0;
        result[i].PickupLoopArea = CalcPickupLoopArea(
            LoopArea,
            result[i].LoopSeriesImpedance,
            Zin);



        
    }
    return result;
}

/*******************************************************************************
multi loop calc
*******************************************************************************/

magloop_out_t *magloop_multi_calc (
    double LoopCircumference,
    double LoopConductorDiameter,
    double Resistivity,
    double RelativePermeabilityConductor,
    /*double K,*/
    double TxPower,
    double FrequencyLowLimit,
    double FrequencyHighLimit,
    double FrequencyStep,
    int isSquare,
    int nLoops,
    size_t *nOuts)
{

    magloop_out_t *result = NULL;

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

    // operating frequency
    double Frequency;

    // wavelength
    double Wavelength;

    // capacative reactance
    double Xc;

    LoopConductorRadius=LoopConductorDiameter/2;
    
    LoopDiamater=LoopCircumference/M_PI;
    LoopRadius=LoopDiamater/2;
    LoopArea=M_PI*SQUARED(LoopRadius);

    /**** alocate output space *****/

    double lastFreq = 0;
    size_t i = 0;

    Frequency=FrequencyLowLimit;
    
    do {
        lastFreq = Frequency; 
        Frequency=Frequency+FrequencyStep;
        i++;

    } while (Frequency <= FrequencyHighLimit && Frequency > lastFreq);
    
    /***** step might not divide evenly into the range *****/

    if (Frequency > FrequencyHighLimit) {
        i++;
    }

    *nOuts = i;
            
    if (NULL == (result = malloc( (i + 1) * sizeof(magloop_out_t) ) ) ) {

        //fixme pRINT AN ERROR?
        return NULL;
    }

    for ( i = 0, Frequency = FrequencyLowLimit;
          i < *nOuts;
          i++, Frequency = Frequency + FrequencyStep
    ) {

        /***** step might not divide evenly into the range *****/

        if (Frequency > FrequencyHighLimit) {
            Frequency = FrequencyHighLimit;
        }

        result[i].Frequency = Frequency;

        result[i].LoopDiamater = LoopDiamater;
        Wavelength = W_F(Frequency);
        
        // calculate the radiation resistance
        result[i].RadiationResistance = CalcRadiationResistance(
            LoopArea,
            Wavelength,
            isSquare,
            nLoops);

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

        // the rf resistance loss decreases as
        // the number of loops increase.
        // in glens original code this var is overwrote by proximity()
        //RfResistanceLoss2=RfResistanceSkinEffect/NumLoops;

        // if more than one loop is present, estimate the proximity effect
        if (nLoops > 1) {
            result[i].RfResistanceLoss2 = CalcProximity(
                RfResistanceSkinEffect,
                LoopCircumference,
                nLoops);
        }
        

        // calculate the efficiency of the loop
        result[i].Efficiency = CalcEfficiency(
            result[i].RadiationResistance,
            result[i].RfResistanceLoss2);

        // calculate the inductance of the loop

        if (!isSquare) {
            result[i].LoopInductance = CalcLoopInductance(
                LoopRadius,
                RelativePermeabilityConductor,
                LoopConductorRadius,
                nLoops/*,
                K*/);
        }
        else {
            result[i].LoopInductance = CalcSquareLoopInductance(
                LoopDiamater,
                RelativePermeabilityConductor,
                LoopConductorRadius,
                nLoops/*,
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

        result[i].Q=result[i].Xl
                        / (2*(    result[i].RadiationResistance
                                + result[i].RfResistanceLoss2));

        result[i].SixDbBandwidth = Fbw_FcutQ(Frequency, result[i].Q);

        result[i].SwrBandwidth=2*(result[i].SixDbBandwidth/(3/0.512));
        result[i].LcDynamicResistance=result[i].Xl*result[i].Q;
        
        result[i].VMAX = E_PR(TxPower, result[i].LcDynamicResistance);

        // calculate the area  of the small coupling loop
        // fixme this should be an input

        double Zin=50.0;
        result[i].PickupLoopArea = CalcPickupLoopArea(
            LoopArea,
            result[i].LoopSeriesImpedance,
            Zin);



        
    }
    return result;
}

/*******************************************************************************
@brief Fuction to calculate radiation resistance

@param LoopArea     area of the loop
@param Wavelength
@param isSquare     non zero if the loop is square
@param NumLoops     number of parallel loops

@returns radiation resistance

*******************************************************************************/

double CalcRadiationResistance(
    double LoopArea,
    double Wavelength,
    int isSquare,
    int NumLoops)
{

    double RadiationResistance;

    // radiation resistance for a single loop
    // This is the favored model for radiation resistance in a loop
    // It is problematic in that it is an approximation, and it is not 
    // correct in the case of a square loop.
     
    RadiationResistance =     31171
                            * (SQUARED(LoopArea)/FOURTH(Wavelength));

    // It turns out that the radiation resistance for a round loop
    // is very close to 1.621 times greater than that of a square loop.
    //
    // correction for a square loop
    if (isSquare) {
        RadiationResistance=RadiationResistance/1.621;
    }
    // radiation resistance increases directly with the 
    // the number of parallel loops
    // so we need to adjust it.
    RadiationResistance = RadiationResistance*NumLoops;

    return RadiationResistance;
}



/*******************************************************************************
@brief Fuction to calculate combined rf loss resistance for skin effect and
       proximity effect

@param RfResistanceSkinEffect   rf loss resistance for skin effect
@param LoopCircumference        circumference of the loop
@param NumLoops                 number of paralell loops

@returns rf loss resistance for skin effect and proximity effect

*******************************************************************************/

double CalcProximity(
    double RfResistanceSkinEffect,
    double LoopCircumference,
    double NumLoops)
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
    // effect is close to NumLoops cubed!
    Rpo=0.003*NumLoops*NumLoops*NumLoops;
    //Rpo=1;

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

/*******************************************************************************
@brief Fuction to calculate loop Inductance

@param LoopRadius                       radius of the loop
@param RelativePermeabilityConductor    relative permeability of the conductor
@param LoopConductorRadius              radius of the loop conductor
@param NumLoops                         number of paralell loops
@param K                                coefficient of coupling

@returns loop inductance

*******************************************************************************/

double CalcLoopInductance(
    double LoopRadius,
    double RelativePermeabilityConductor,
    double LoopConductorRadius,
    double NumLoops/*,
    double K*/)
{

    double LoopInductance;

    // This is for a circular loop
    LoopInductance =      LoopRadius 
                        * PERMEABILITY_FS 
                        * RelativePermeabilityConductor
                        *( log(( 8 * LoopRadius ) / LoopConductorRadius) -2 );

    // adjust for the coefficient of coupling in parallel conductor loops

    if(NumLoops>1)LoopInductance=LoopInductance/(NumLoops*K);

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
@param NumLoops                         number of paralell loops
@param K                                coefficient of coupling


@returns loop inductance

*******************************************************************************/

double CalcSquareLoopInductance(
    double LoopDiamater,
    double RelativePermeabilityConductor,
    double LoopConductorRadius,
    double NumLoops/*,
    double K*/)

{

    double LoopInductance;

    LoopInductance=((   2 
                      * PERMEABILITY_FS
                      * RelativePermeabilityConductor
                      * LoopDiamater)/M_PI
                    ) * (log( LoopDiamater / LoopConductorRadius) - 0.77401);

    // adjust for the coefficient of coupling in parallel conductor loops

    if(NumLoops>1)LoopInductance=LoopInductance/(NumLoops*K);

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




