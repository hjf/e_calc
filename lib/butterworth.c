/*******************************************************************************
 *
 * Project: e_calc
 * 
 * butterworth.c
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
#include "lowpass.h"
#include "highpass.h"
#include "butterworth.h"


#define ODD(n) ( (n)%2 )
#define EVEN(n) ( (n)+1%2 )

/**** Normalised Prototype calculation *****/

#define NPC( i, ORDER) ( 2.0 * sin(((2.0* (double)(i) -1.0)/(2.0*(double)(ORDER))* M_PI) ) )


/*******************************************************************************
function to free the Butterworth struct

@param b    Butterworth struct to free

@return nothing

*******************************************************************************/
void Butterworth_free(struct Butterworth * b) {
    free(b->C);
    free(b->L);
    free(b);
}

struct Butterworth *Butterworth_malloc(unsigned int nOrder) {

    struct Butterworth *result;

    if (NULL == (result = malloc(sizeof(struct Butterworth)) ))
        return NULL;

    if (NULL == (result->C = malloc(sizeof(double) * nOrder) )) {
        free(result);
        return NULL;
    }

    if (NULL == (result->L = malloc(sizeof(double) * nOrder) )) {
        free(result->C);
        free(result);
        return NULL;
    }

    return result;
}

/*******************************************************************************
function to calculate Butterworth Lowpass filter component values

@param Fcut     Cutoff freq
@param Z        Impedance
@param nOrder   order of the filter

@return Butterworth structure of results

@details the stucture returned is l[0] c[1] l[2]... for T (series first)
or c[0] l[1] c[2]... for PI (shunt first)

*******************************************************************************/

struct Butterworth *Butterworth_Lowpass(
    double Fcut,
    double Z,
    unsigned int nOrder
) {

    double Proto;
    
    struct Butterworth *result;

    if (nOrder < 2)
        return NULL;

    if (NULL == (result = Butterworth_malloc(nOrder) ))
        return NULL;

    result->nOrder = nOrder;
    int i;
    for (i = 0 ; i < nOrder ; i++) {
        Proto=NPC(i + 1, nOrder);
        result->C[i]=LOWPASS_C(Proto, Z, Fcut);
        result->L[i]=LOWPASS_L(Proto, Z, Fcut);
    }

    return result;
}

/*******************************************************************************
function to calculate Butterworth Highpass filter component values

@param Fcut     Cutoff freq
@param Z        Impedance
@param nOrder   order of the filter

@return Butterworth structure of results

@details the stucture returned is c[0] l[1] c[2]... for T (series first)
or l[0] c[1] l[2]... for PI (shunt first)

*******************************************************************************/

struct Butterworth *Butterworth_Highpass(
    double Fcut,
    double Z,
    unsigned int nOrder
) {

    double Proto;
  
    struct Butterworth *result;

    if (nOrder < 2)
        return NULL;

    if (NULL == (result = Butterworth_malloc(nOrder) ))
        return NULL;
 
    result->nOrder = nOrder;
    int i;
    for (i = 0 ; i < nOrder ; i++) {

        Proto=NPC(i+1, nOrder);
        result->C[i]=HIGHPASS_C(Proto, Z, Fcut);
        result->L[i]=HIGHPASS_L(Proto, Z, Fcut);
    }

    return result;
}

/*******************************************************************************
function to calculate Butterworth bandpass filter component values

@param Fcenter  Center freq
@param Fwidth   width freq
@param Z        Impedance
@param nOrder   order of the filter
@param series   non zero for for T (series first) or 0 for PI (shunt first)

@return Butterworth structure of results

@details the stucture returned is c[0] l[0] c[1] l[1]...

*******************************************************************************/

struct Butterworth *Butterworth_Bandpass(
    double Fcenter,
    double Fwidth,
    double Z,
    unsigned int nOrder,
    int series
) {

    struct Butterworth *result;

    if (nOrder < 2)
        return NULL;

    if (NULL == (result = Butterworth_malloc(nOrder) ))
        return NULL;

    double Proto;

    result->nOrder = nOrder;
    int i;
    for (i = 0 ; i < nOrder ; i++) {

        Proto=NPC(i + 1, nOrder);
        
        /***** series *****/
  
        if (series) {
            if (ODD(i)) {
                result->C[i] = HIGHPASS_C(Proto, Z, (Fcenter*Fcenter/Fwidth));
                result->L[i] = LOWPASS_L(Proto, Z, Fwidth);
            }
            else {
                result->C[i] = LOWPASS_C(Proto, Z, Fwidth);
                result->L[i] = HIGHPASS_L(Proto, Z, (Fcenter*Fcenter/Fwidth));
            }
        }
        
        /***** shunt *****/

        else {
            if (ODD(i)) {
                result->C[i] = LOWPASS_C(Proto, Z, Fwidth);
                result->L[i] = HIGHPASS_L(Proto, Z, (Fcenter*Fcenter/Fwidth));
            }
            else {
                result->C[i] = HIGHPASS_C(Proto, Z, (Fcenter*Fcenter/Fwidth));
                result->L[i] = LOWPASS_L(Proto, Z, Fwidth);
            }
        }
    }
    
    return result;
}




/*******************************************************************************
function to calculate Butterworth bandstop filter component values

@param Fcenter  Center freq
@param Fwidth   width freq
@param Z        Impedance
@param nOrder   order of the filter
@param series   non zero for for T (series first) or 0 for PI (shunt first)

@return Butterworth structure of results

@details the stucture returned is c[0] l[0] c[1] l[1]...

*******************************************************************************/

struct Butterworth *Butterworth_Bandstop(
    double Fcenter,
    double Fwidth,
    double Z,
    unsigned int nOrder,
    int series
) {

    struct Butterworth *result;

    if (nOrder < 2)
        return NULL;

    if (NULL == (result = Butterworth_malloc(nOrder) ))
        return NULL;

    double Proto;

    result->nOrder = nOrder;
    int i;
    for (i = 0 ; i < nOrder ; i++) {

        Proto=NPC(i + 1, nOrder);
        
        /***** series *****/
  
        if (series) {
            if (ODD(i)) {
                result->C[i] = HIGHPASS_C(Proto, Z, (Fcenter*Fcenter/Fwidth));
                result->L[i] = LOWPASS_L(Proto, Z, Fwidth);
            }
            else {
                result->C[i] = LOWPASS_C(Proto, Z, Fwidth);
                result->L[i] = HIGHPASS_L(Proto, Z, (Fcenter*Fcenter/Fwidth));
            }
        }
        
        /***** shunt *****/

        else {
            if (ODD(i)) {
                result->C[i] = LOWPASS_C(Proto, Z, Fwidth);
                result->L[i] = HIGHPASS_L(Proto, Z, (Fcenter*Fcenter/Fwidth));
            }
            else {
                result->C[i] = HIGHPASS_C(Proto, Z, (Fcenter*Fcenter/Fwidth));
                result->L[i] = LOWPASS_L(Proto, Z, Fwidth);
            }
        }
    }
    
    return result;
}





