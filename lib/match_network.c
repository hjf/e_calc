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

#include "match_network.h"
#include "reactance.h"
#include "q.h"

#define MIN(a, b) ( ((a) < (b)) ? (a): (b) )
#define MAX(a, b) ( ((a) > (b)) ? (a): (b) )

#define RV(Rh, Q) ( Rh / ((Q * Q)  + 1) )

#define GEOMETRIC_MEAN(R1, R2) (sqrt((R1)*(R2)))


struct match_network match_network_l(
    double Rg,
    double Rl,
    double Fcut
) {

    struct match_network result;

    double Xl;
    double Xc;
    double Q;

    if (Rg < Rl) {

        Q = Q_R1_R2(Rl, Rg);
        Xl = Xl_QR(Q, Rg);
        Xc = Xc_QR(Q, Rl);
        result.series = 1;
    }
    else {
        Q = Q_R1_R2(Rg, Rl);
        Xl = Xl_QR(Q, Rl);
        Xc = Xc_QR(Q, Rg);
        result.series = 0;

    }
    
    result.L[0] = C_XlF(Xl, Fcut);
    result.C[0] = C_XcF(Xc, Fcut);
    result.BW = Fcut/Q;

    return result;

}

struct match_network match_network_pi(
    double Rg,
    double Rl,
    double Fcut,
    double Fbw
) {

    struct match_network result;

    double Xl;
    double Xc;
    double Q;
    double Rv;

        
    if (Rg < Rl) {

        Q = Q_FcutBw(Fcut, Fbw);
        Rv = RV(Rl, Q);

        Xl = Xl_QR(Q, Rv);
        Xc = Xc_QR(Q, Rl);

        result.L[0] = C_XlF(Xl, Fcut);
        result.C[0] = C_XcF(Xc, Fcut);

        Q = Q_R1_R2(Rv, Rg);

        Xl = Xl_QR(Q, Rg);
        Xc = Xc_QR(Q, Rv);

        result.L[1] = C_XlF(Xl, Fcut);
        result.C[1] = C_XcF(Xc, Fcut);

    }
    else {
        
        Q = Q_FcutBw(Fcut, Fbw);
        Rv = RV(Rg, Q);
        
        Xl = Xl_QR(Q, Rv);
        Xc = Xc_QR(Q, Rg);

        result.L[0] = C_XlF(Xl, Fcut);
        result.C[0] = C_XcF(Xc, Fcut);

        Q = Q_R1_R2(Rv, Rl);

        Xl = Xl_QR(Q, Rl);
        Xc = Xc_QR(Q, Rv);

        result.L[1] = C_XlF(Xl, Fcut);
        result.C[1] = C_XcF(Xc, Fcut);

    }
    
    result.BW = Fbw;

    return result;

}

struct match_network match_network_pi2(
    double Rg,
    double Rl,
    double Fcut
) {

    struct match_network result;

    double Xl;
    double Xc;
    double Q;
    double Rv;

    Rv = GEOMETRIC_MEAN(Rg, Rl);

    if (Rg < Rl) {

        Q = Q_R1_R2(Rl, Rv);

        Xl = Xl_QR(Q, Rv);
        Xc = Xc_QR(Q, Rl);

        result.L[0] = C_XlF(Xl, Fcut);
        result.C[0] = C_XcF(Xc, Fcut);

        Q = Q_R1_R2(Rv, Rg);

        Xl = Xl_QR(Q, Rg);
        Xc = Xc_QR(Q, Rv);

        result.L[1] = C_XlF(Xl, Fcut);
        result.C[1] = C_XcF(Xc, Fcut);

    }
    else {
        
        
        Q = Q_R1_R2(Rg, Rv);

        Xl = Xl_QR(Q, Rv);
        Xc = Xc_QR(Q, Rg);

        result.L[0] = C_XlF(Xl, Fcut);
        result.C[0] = C_XcF(Xc, Fcut);

        Q = Q_R1_R2(Rv, Rl);

        Xl = Xl_QR(Q, Rl);
        Xc = Xc_QR(Q, Rv);

        result.L[1] = C_XlF(Xl, Fcut);
        result.C[1] = C_XcF(Xc, Fcut);

    }
    
    result.BW = Fcut/Q;

    return result;

}


