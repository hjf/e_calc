/*******************************************************************************
 *
 * Project: e_calc
 * 
 * chebyshev.c
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
#include "chebyshev.h"

/*******************************************************************************
function to free the Chebyshev struct

@param c    Chebyshev struct to free

@return nothing

*******************************************************************************/


void Chebyshev_free(struct Chebyshev * c) {
    free(c->C);
    free(c->L);
    free(c);
}


double Chebyshev_Y (int nOrder, double Ripple) { 

    //double e = pow(1/(pow(10.0,(-1*Ripple)/20.0)),2.0)-1;
    double e2xrr = exp(2.0*(Ripple/17.37));
    double bt = log(   (e2xrr+1.0)/(e2xrr-1.0)  );
    double Y = (exp(bt/(2.0*(double)nOrder))-exp((-1.0*bt)/(2.0*(double)nOrder)))/2.0;

    return Y;
}

#define ODD(n) ( (n)%2 )
#define EVEN(n) ( (n)+1%2 )

#define Chebyshev_A(i, nOrder) ( sin(((2.0*(double)(i)-1.0)/(2.0*(double)(nOrder)))*M_PI) )
#define Chebyshev_B(Y, i, nOrder) ( ((Y)*(Y))+pow(sin(((double)(i)*M_PI)/(nOrder)),2.0) )


struct Chebyshev *Chebyshev_malloc(unsigned int nOrder) {

    struct Chebyshev *result;

    if (NULL == (result = malloc(sizeof(struct Chebyshev)) ))
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
function to calculate Chebyshev Lowpass filter component values

@param Fcut     Cutoff freq
@param Z        Impedance
@param nOrder   order of the filter
@param Ripple   passband ripple

@return Chebyshev structure of results

@details the stucture returned is l[0] c[1] l[2]... for T (series first)
or c[0] l[1] c[2]... for PI (shunt first)

*******************************************************************************/

struct Chebyshev *Chebyshev_Lowpass(
    double Fcut,
    double Z,
    unsigned int nOrder,
    double Ripple
) {
  
    struct Chebyshev *result;

    if (nOrder < 2)
        return NULL;

    if (NULL == (result = Chebyshev_malloc(nOrder) ))
        return NULL;

    double *A;
    double *B;
    double *Proto;

    if (NULL == (A = malloc(sizeof(double) * nOrder) )) {
        Chebyshev_free(result);
    }

    if (NULL == (B = malloc(sizeof(double) * nOrder) )) {
        free(A);
        Chebyshev_free(result);
    }
    if (NULL == (Proto = malloc(sizeof(double) * nOrder) )) {
        free(A);
        free(B);
        Chebyshev_free(result);
    }

    result->nOrder = nOrder;

    double Y = Chebyshev_Y (nOrder, Ripple);

    int i;
    for (i = 0 ; i < nOrder ; i++) {

        A[i]=Chebyshev_A((i + 1), nOrder);
        B[i]=Chebyshev_B(Y, (i + 1), nOrder);
        if (i == 0)
            Proto[i] = (2.0*A[i])/Y;
        else
            Proto[i] = (4.0*A[i-1]*A[i])/(B[i-1]*Proto[i-1]);

        result->C[i]=LOWPASS_C(Proto[i], Z, Fcut);
        result->L[i]=LOWPASS_L(Proto[i], Z, Fcut);

    }
    
    free(A);
    free(B);
    free(Proto);

    return result;
}

/*******************************************************************************
function to calculate Chebyshev Highpass filter component values

@param Fcut     Cutoff freq
@param Z        Impedance
@param nOrder   order of the filter
@param Ripple   passband ripple

@return Chebyshev structure of results

@details the stucture returned is c[0] l[1] c[2]... for T (series first)
or l[0] c[1] l[2]... for PI (shunt first)

*******************************************************************************/

struct Chebyshev *Chebyshev_Highpass(
    double Fcut,
    double Z,
    unsigned int nOrder,
    double Ripple
) {

    struct Chebyshev *result;

    if (nOrder < 2)
        return NULL;

    if (NULL == (result = Chebyshev_malloc(nOrder) ))
        return NULL;

    double *A;
    double *B;
    double *Proto;

    if (NULL == (A = malloc(sizeof(double) * nOrder) )) {
        Chebyshev_free(result);
    }

    if (NULL == (B = malloc(sizeof(double) * nOrder) )) {
        free(A);
        Chebyshev_free(result);
    }
    if (NULL == (Proto = malloc(sizeof(double) * nOrder) )) {
        free(A);
        free(B);
        Chebyshev_free(result);
    }

    result->nOrder = nOrder;
    double Y = Chebyshev_Y (nOrder, Ripple);

    int i;
    for (i = 0 ; i < nOrder ; i++) {
        A[i]=Chebyshev_A((i + 1), nOrder);
        B[i]=Chebyshev_B(Y, (i + 1), nOrder);
        if (i == 0)
            Proto[i] = (2.0*A[i])/Y;
        else
            Proto[i] = (4.0*A[i-1]*A[i])/(B[i-1]*Proto[i-1]);

        result->C[i]=HIGHPASS_C(Proto[i], Z, Fcut);
        result->L[i]=HIGHPASS_L(Proto[i], Z, Fcut);
    }
    
    free(A);
    free(B);
    free(Proto);

    return result;
}

/*******************************************************************************
function to calculate Chebyshev bandpass filter component values

@param Fcenter  Center freq
@param Fwidth   width freq
@param Z        Impedance
@param nOrder   order of the filter
@param Ripple   passband ripple
@param series   non zero for for T (series first) or 0 for PI (shunt first)

@return Chebyshev structure of results

@details the stucture returned is c[0] l[0] c[1] l[1]...

*******************************************************************************/

struct Chebyshev *Chebyshev_Bandpass(
    double Fcenter,
    double Fwidth,
    double Z,
    unsigned int nOrder,
    double Ripple,
    int series
) {

    struct Chebyshev *result;

    if (nOrder < 2)
        return NULL;

    if (NULL == (result = Chebyshev_malloc(nOrder) ))
        return NULL;

    double *A;
    double *B;
    double *Proto;

    if (NULL == (A = malloc(sizeof(double) * nOrder) )) {
        Chebyshev_free(result);
    }

    if (NULL == (B = malloc(sizeof(double) * nOrder) )) {
        free(A);
        Chebyshev_free(result);
    }

    if (NULL == (Proto = malloc(sizeof(double) * nOrder) )) {
        free(A);
        free(B);
        Chebyshev_free(result);
    }

    result->nOrder = nOrder;
    double Y = Chebyshev_Y (nOrder, Ripple);

    int i;
    for (i = 0 ; i < nOrder ; i++) {
        A[i]=Chebyshev_A((i + 1), nOrder);
        B[i]=Chebyshev_B(Y, (i + 1), nOrder);
        if (i == 0)
            Proto[i] = (2.0*A[i])/Y;
        else
            Proto[i] = (4.0*A[i-1]*A[i])/(B[i-1]*Proto[i-1]);

        /***** series *****/
  
        if (series) {
            if (ODD(i)) {
                result->C[i] = HIGHPASS_C(Proto[i], Z, (Fcenter*Fcenter/Fwidth));
                result->L[i] = LOWPASS_L(Proto[i], Z, Fwidth);
            }
            else {
                result->C[i] = LOWPASS_C(Proto[i], Z, Fwidth);
                result->L[i] = HIGHPASS_L(Proto[i], Z, (Fcenter*Fcenter/Fwidth));
            }
        }
        
        /***** shunt *****/

        else {
            if (ODD(i)) {
                result->C[i] = LOWPASS_C(Proto[i], Z, Fwidth);
                result->L[i] = HIGHPASS_L(Proto[i], Z, (Fcenter*Fcenter/Fwidth));
            }
            else {
                result->C[i] = HIGHPASS_C(Proto[i], Z, (Fcenter*Fcenter/Fwidth));
                result->L[i] = LOWPASS_L(Proto[i], Z, Fwidth);
            }
        }
    }
    
    free(A);
    free(B);
    free(Proto);

    return result;
}


/*******************************************************************************
function to calculate Chebyshev bandstop filter component values

@param Fcenter  Center freq
@param Fwidth   width freq
@param Z        Impedance
@param nOrder   order of the filter
@param Ripple   passband ripple
@param series   non zero for for T (series first) or 0 for PI (shunt first)

@return Chebyshev structure of results

@details the stucture returned is c[0] l[0] c[1] l[1]...

*******************************************************************************/

struct Chebyshev *Chebyshev_Bandstop(
    double Fcenter,
    double Fwidth,
    double Z,
    unsigned int nOrder,
    double Ripple,
    int series
) {

    struct Chebyshev *result;

    if (nOrder < 2)
        return NULL;

    if (NULL == (result = Chebyshev_malloc(nOrder) ))
        return NULL;

    double *A;
    double *B;
    double *Proto;

    if (NULL == (A = malloc(sizeof(double) * nOrder) )) {
        Chebyshev_free(result);
    }

    if (NULL == (B = malloc(sizeof(double) * nOrder) )) {
        free(A);
        Chebyshev_free(result);
    }

    if (NULL == (Proto = malloc(sizeof(double) * nOrder) )) {
        free(A);
        free(B);
        Chebyshev_free(result);
    }

    result->nOrder = nOrder;
    double Y = Chebyshev_Y (nOrder, Ripple);

    int i;
    for (i = 0 ; i < nOrder ; i++) {
        A[i]=Chebyshev_A((i + 1), nOrder);
        B[i]=Chebyshev_B(Y, (i + 1), nOrder);
        if (i == 0)
            Proto[i] = (2.0*A[i])/Y;
        else
            Proto[i] = (4.0*A[i-1]*A[i])/(B[i-1]*Proto[i-1]);

        /***** series *****/
  
        if (series) {
            if (ODD(i)) {
                result->C[i] = HIGHPASS_C(Proto[i], Z, (Fcenter*Fcenter/Fwidth));
                result->L[i] = LOWPASS_L(Proto[i], Z, Fwidth);
            }
            else {
                result->C[i] = LOWPASS_C(Proto[i], Z, Fwidth);
                result->L[i] = HIGHPASS_L(Proto[i], Z, (Fcenter*Fcenter/Fwidth));
            }
        }
        
        /***** shunt *****/

        else {
            if (ODD(i)) {
                result->C[i] = LOWPASS_C(Proto[i], Z, Fwidth);
                result->L[i] = HIGHPASS_L(Proto[i], Z, (Fcenter*Fcenter/Fwidth));
            }
            else {
                result->C[i] = HIGHPASS_C(Proto[i], Z, (Fcenter*Fcenter/Fwidth));
                result->L[i] = LOWPASS_L(Proto[i], Z, Fwidth);
            }
        }
    }
    
    free(A);
    free(B);
    free(Proto);

    return result;
}


