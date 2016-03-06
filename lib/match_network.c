/*******************************************************************************
 *
 * Project: e_calc
 * 
 * match_network.c
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
#include "match_network.h"
#include "reactance.h"
#include "q.h"

#define MIN(a, b) ( ((a) < (b)) ? (a): (b) )
#define MAX(a, b) ( ((a) > (b)) ? (a): (b) )

#define RV_RhQ(Rh, Q) ( Rh / ((Q * Q)  + 1) )
#define RV_RlQ(Zload, Q) ( Zload * ((Q * Q)  + 1) )

#define GEOMETRIC_MEAN(R1, R2) (sqrt((R1)*(R2)))
/***** geometricly divide the impedance mismatch into nOrder *****/
/***** sections                                              *****/
/***** (50*50*50*1000)^(1/4)     = 105.737126344             *****/
/***** (50*50*1000*1000)^(1/4)   = 223.60679775              *****/
/***** (50*1000*1000*1000)^(1/4) = 472.870804502             *****/

#define GEOMETRIC_DIVIDE(a, b, n, i) ( pow ( pow( (a), ((double)(n)-(double)(i)) ) * pow( (b), ((double)(i)) )  , (1.0/(double)(n))) )

void match_network_free(struct match_network * m) {
    free(m->C);
    free(m->L);
    free(m);
}

struct match_network *match_network_malloc(unsigned int nOrder) {

    struct match_network *result;

    if (NULL == (result = malloc(sizeof(struct match_network)) ))
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

    if (NULL == (result->Zi = malloc(sizeof(double) * nOrder) )) {
        free(result->C);
        free(result->L);
        free(result);
        return NULL;
    }

    return result;
}


/*******************************************************************************
    function to calculate a lowpass impedance match

*******************************************************************************/

struct match_network *match_network_l_lowpass(
    double Zgen,
    double Zload,
    double Fcut,
    int nOrder
) {

    struct match_network *result;

    double Xseries;
    double Xshunt;
    double Q;
    double lastZ;
    double Zi;
    int i;

    if (NULL == (result = match_network_malloc(nOrder) ))
        return NULL;


    if (Zgen < Zload) {
        
        i = 1;        
        lastZ = Zgen;
        result->series = 1;

        do {

            if (i < nOrder)
                Zi = GEOMETRIC_DIVIDE(Zgen, Zload, nOrder, i);
            else
                Zi = Zload;

            Q = Q_R1_R2(Zi, lastZ);
            Xseries = XSeries_QR(Q, lastZ);
            Xshunt = XShunt_QR(Q, Zi);

            result->L[i - 1] = L_XlF(Xseries, Fcut);
            result->C[i - 1] = C_XcF(Xshunt, Fcut);
            result->Zi[i-1] = Zi;

            lastZ=Zi;
            i++;
        } while (i <= nOrder);
    
    }
    else {

        i = 1;        
        lastZ = Zgen;
        result->series = 0;

        do {

            if (i < nOrder)
                Zi = GEOMETRIC_DIVIDE(Zgen, Zload, nOrder, i);
            else
                Zi = Zload;

            Q = Q_R1_R2(lastZ, Zi);
            Xseries = XSeries_QR(Q, Zi);
            Xshunt = XShunt_QR(Q, lastZ);

            result->L[i - 1] = L_XlF(Xseries, Fcut);
            result->C[i - 1] = C_XcF(Xshunt, Fcut);
            result->Zi[i-1] = Zi;

            lastZ=Zi;
            i++;
        } while (i <= nOrder);
    

    }
    
    result->nOrder = nOrder;
    result->BW = Fcut/Q;

    return result;
}

/*******************************************************************************
    function to calculate a highpass impedance match

*******************************************************************************/

struct match_network *match_network_l_highpass(
    double Zgen,
    double Zload,
    double Fcut,
    int nOrder
) {

    struct match_network *result;

    double Xseries;
    double Xshunt;
    double Q;
    double lastZ;
    double Zi;
    int i;

    if (NULL == (result = match_network_malloc(nOrder) ))
        return NULL;


    if (Zgen < Zload) {
        
        i = 1;        
        lastZ = Zgen;
        result->series = 1;

        do {

            if (i < nOrder)
                Zi = GEOMETRIC_DIVIDE(Zgen, Zload, nOrder, i);
            else
                Zi = Zload;

            Q = Q_R1_R2(Zi, lastZ);
            Xseries = XSeries_QR(Q, lastZ);
            Xshunt = XShunt_QR(Q, Zi);

            result->L[i - 1] = L_XlF(Xshunt, Fcut);
            result->C[i - 1] = C_XcF(Xseries, Fcut);
            result->Zi[i-1] = Zi;

            lastZ=Zi;
            i++;
        } while (i <= nOrder);
    
    }
    else {

        i = 1;        
        lastZ = Zgen;
        result->series = 0;

        do {

            if (i < nOrder)
                Zi = GEOMETRIC_DIVIDE(Zgen, Zload, nOrder, i);
            else
                Zi = Zload;

            Q = Q_R1_R2(lastZ, Zi);
            Xseries = XSeries_QR(Q, Zi);
            Xshunt = XShunt_QR(Q, lastZ);

            result->L[i - 1] = L_XlF(Xshunt, Fcut);
            result->C[i - 1] = C_XcF(Xseries, Fcut);
            result->Zi[i-1] = Zi;

            lastZ=Zi;
            i++;
        } while (i <= nOrder);
    

    }
    
    result->nOrder = nOrder;
    result->BW = Fcut/Q;

    return result;
}


