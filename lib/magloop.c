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

    // reciprocal of the resistivity
    // conductivity
    double Conductivity;

    // radius of the loop conductor
    double LoopConductorRadius;

    // circumference of the loop conductor
    double LoopConductorCircumference;

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

    Conductivity=1/Resistivity;

    LoopConductorRadius=LoopConductorDiameter/2;
    LoopConductorCircumference=M_PI*LoopConductorDiameter;

    LoopDiamater=LoopCircumference/M_PI;
    LoopRadius=LoopDiamater/2;
    LoopArea=M_PI*SQUARED(LoopRadius);

    /**** alocate output space *****/

    // fixme what if the step dont evenly divide
    // we should just set the last freq to the upper limit    
    
    Frequency=FrequencyLowLimit;
    *nOuts = 0;
    do {

        Frequency=Frequency+FrequencyStep;
        (*nOuts)++;

    } while (Frequency < FrequencyHighLimit || Frequency == FrequencyHighLimit);
    
    
            
    if (NULL == (result = malloc( (*nOuts + 1) * sizeof(magloop_out_t) ) ) ) {

        //fixme pRINT AN ERROR?
        return NULL;
    }

    for ( Frequency=FrequencyLowLimit, *nOuts = 0;
          Frequency < FrequencyHighLimit || Frequency == FrequencyHighLimit;
          Frequency=Frequency+FrequencyStep, (*nOuts)++
    ) {

        result[*nOuts].Frequency = Frequency;

        result[*nOuts].LoopDiamater = LoopDiamater;
        Wavelength=W_F(Frequency);
        
        // calculate the radiation resistance
        result[*nOuts].RadiationResistance = CalcRadiationResistance(
            LoopArea,
            Wavelength,
            isSquare,
            1);

        // add skin effect to RfResistanceLoss
        RfResistanceSkinEffect = CalcSkinEffect(
            Frequency,
            RelativePermeabilityConductor,
            Conductivity,
            LoopCircumference,
            LoopConductorCircumference);

        // the rf resistance loss decreases as
        // the number of loops increase.
        // in this case 1 loop only

        result[*nOuts].RfResistanceLoss2=RfResistanceSkinEffect;

        //RfResistanceLoss2=RfResistanceSkinEffect/NumLoops;


        /*
        // if more than one loop is present, estimate the proximity effect
        if (NumLoops > 1)    
            result[*nOuts].RfResistanceLoss2 = CalcProximity(
                RfResistanceSkinEffect,
                LoopCircumference,
                NumLoops);
        }
        */

        // calculate the efficiency of the loop
        result[*nOuts].Efficiency = CalcEfficiency(
            result[*nOuts].RadiationResistance,
            result[*nOuts].RfResistanceLoss2);

        // calculate the inductance of the loop

        if (!isSquare) {
            result[*nOuts].LoopInductance = CalcLoopInductance(
                LoopRadius,
                RelativePermeabilityConductor,
                LoopConductorRadius,
                1/*,
                K*/);
        }
        else {
            result[*nOuts].LoopInductance = CalcSquareLoopInductance(
                LoopDiamater,
                RelativePermeabilityConductor,
                LoopConductorRadius,
                1/*,
                K*/);
        }
        
        // calculate the reactance of the loop
        // as well as series input impedance

        result[*nOuts].Xl= XL(
            Frequency,
            result[*nOuts].LoopInductance
        );

        result[*nOuts].LoopSeriesImpedance=sqrt(
            result[*nOuts].RadiationResistance
            + result[*nOuts].RfResistanceLoss2
        );

        Xc=result[*nOuts].Xl;

        result[*nOuts].C = C_XcF(Xc, Frequency);

        result[*nOuts].Q=result[*nOuts].Xl
                        / (2*(    result[*nOuts].RadiationResistance
                                + result[*nOuts].RfResistanceLoss2));

        result[*nOuts].SixDbBandwidth = Fbw_FcutQ(Frequency, result[*nOuts].Q);

        result[*nOuts].SwrBandwidth=2*(result[*nOuts].SixDbBandwidth/(3/0.512));
        result[*nOuts].LcDynamicResistance=result[*nOuts].Xl*result[*nOuts].Q;
        
        result[*nOuts].VMAX = E_PR(TxPower, result[*nOuts].LcDynamicResistance);

        // calculate the area  of the small coupling loop
        // fixme this should be an input
        double Zin=50.0;
        result[*nOuts].PickupLoopArea = CalcPickupLoopArea(
            LoopArea,
            result[*nOuts].LoopSeriesImpedance,
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

    // reciprocal of the resistivity
    // conductivity
    double Conductivity;

    // radius of the loop conductor
    double LoopConductorRadius;

    // circumference of the loop conductor
    double LoopConductorCircumference;

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

    Conductivity=1/Resistivity;

    LoopConductorRadius=LoopConductorDiameter/2;
    LoopConductorCircumference=M_PI*LoopConductorDiameter;

    LoopDiamater=LoopCircumference/M_PI;
    LoopRadius=LoopDiamater/2;
    LoopArea=M_PI*SQUARED(LoopRadius);

    /**** alocate output space *****/

    // fixme what if the step dont evenly divide
    // we should just set the last freq to the upper limit    
    
    Frequency=FrequencyLowLimit;
    *nOuts = 0;
    do {

        Frequency=Frequency+FrequencyStep;
        (*nOuts)++;

    } while (Frequency < FrequencyHighLimit || Frequency == FrequencyHighLimit);
    
    
            
    if (NULL == (result = malloc( (*nOuts + 1) * sizeof(magloop_out_t) ) ) ) {

        //fixme pRINT AN ERROR?
        return NULL;
    }

    for ( Frequency=FrequencyLowLimit, *nOuts = 0;
          Frequency < FrequencyHighLimit || Frequency == FrequencyHighLimit;
          Frequency=Frequency+FrequencyStep, (*nOuts)++
    ) {

        result[*nOuts].Frequency = Frequency;

        result[*nOuts].LoopDiamater = LoopDiamater;
        Wavelength=W_F(Frequency);
        
        // calculate the radiation resistance
        result[*nOuts].RadiationResistance = CalcRadiationResistance(
            LoopArea,
            Wavelength,
            isSquare,
            nLoops);

        // add skin effect to RfResistanceLoss
        RfResistanceSkinEffect = CalcSkinEffect(
            Frequency,
            RelativePermeabilityConductor,
            Conductivity,
            LoopCircumference,
            LoopConductorCircumference);

        // the rf resistance loss decreases as
        // the number of loops increase.
        // in glens original code this var is overwrote by proximity()
        //RfResistanceLoss2=RfResistanceSkinEffect/NumLoops;

        // if more than one loop is present, estimate the proximity effect
        if (nLoops > 1) {
            result[*nOuts].RfResistanceLoss2 = CalcProximity(
                RfResistanceSkinEffect,
                LoopCircumference,
                nLoops);
        }
        

        // calculate the efficiency of the loop
        result[*nOuts].Efficiency = CalcEfficiency(
            result[*nOuts].RadiationResistance,
            result[*nOuts].RfResistanceLoss2);

        // calculate the inductance of the loop

        if (!isSquare) {
            result[*nOuts].LoopInductance = CalcLoopInductance(
                LoopRadius,
                RelativePermeabilityConductor,
                LoopConductorRadius,
                nLoops/*,
                K*/);
        }
        else {
            result[*nOuts].LoopInductance = CalcSquareLoopInductance(
                LoopDiamater,
                RelativePermeabilityConductor,
                LoopConductorRadius,
                nLoops/*,
                K*/);
        }
        
        // calculate the reactance of the loop
        // as well as series input impedance

        result[*nOuts].Xl= XL(
            Frequency,
            result[*nOuts].LoopInductance
        );

        result[*nOuts].LoopSeriesImpedance=sqrt(
            result[*nOuts].RadiationResistance
            + result[*nOuts].RfResistanceLoss2
        );

        Xc=result[*nOuts].Xl;

        result[*nOuts].C = C_XcF(Xc, Frequency);

        result[*nOuts].Q=result[*nOuts].Xl
                        / (2*(    result[*nOuts].RadiationResistance
                                + result[*nOuts].RfResistanceLoss2));

        result[*nOuts].SixDbBandwidth = Fbw_FcutQ(Frequency, result[*nOuts].Q);

        result[*nOuts].SwrBandwidth=2*(result[*nOuts].SixDbBandwidth/(3/0.512));
        result[*nOuts].LcDynamicResistance=result[*nOuts].Xl*result[*nOuts].Q;
        
        result[*nOuts].VMAX = E_PR(TxPower, result[*nOuts].LcDynamicResistance);

        // calculate the area  of the small coupling loop
        // fixme this should be an input
        double Zin=50.0;
        result[*nOuts].PickupLoopArea = CalcPickupLoopArea(
            LoopArea,
            result[*nOuts].LoopSeriesImpedance,
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
@brief Fuction to calculate rf loss resistance for skin effect

@param Frequency,
@param RelativePermeabilityConductor,
@param Conductivity,
@param LoopCircumference,
@param LoopConductorCircumference);

@returns rf loss resistance for skin effect

*******************************************************************************/

double CalcSkinEffect(
    double Frequency,
    double RelativePermeabilityConductor,
    double Conductivity,
    double LoopCircumference,
    double LoopConductorCircumference)
{

    double ResistivitySkinEffect;
    double RfResistanceSkinEffect;

    // This is the standard model for skin effect
    //
    ResistivitySkinEffect = sqrt((    M_PI
                                    * Frequency
                                    * PERMEABILITY_FS 
                                    * RelativePermeabilityConductor
                                 ) / Conductivity);

    RfResistanceSkinEffect=(LoopCircumference/LoopConductorCircumference)*ResistivitySkinEffect;

    return RfResistanceSkinEffect;
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




