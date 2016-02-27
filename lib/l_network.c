/*******************************************************************************
 *
 * Project: e_calc
 * 
 * l_network.c
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

#include "l_network.h"
#include "reactance.h"


#define MIN(a, b) ( ((a) < (b)) ? (a): (b) )
#define MAX(a, b) ( ((a) > (b)) ? (a): (b) )


#define Q(R1, R2) (sqrt((R1)/(R2) -1.0))


struct l_network l_network(
    double Rg,
    double Rl,
    double Fcut
) {

    struct l_network result;

    double Xl;
    double Xc;
    double Q;

    if (Rg < Rl) {

        Q = Q(Rl, Rg);
        Xl = Xl_QR(Q, Rg);
        Xc = Xc_QR(Q, Rl);
        result.pi = 0;
    }
    else {
        Q = Q(Rg, Rl);
        Xl = Xl_QR(Q, Rl);
        Xc = Xc_QR(Q, Rg);
        result.pi = 1;

    }
    
    result.L = C_XlF(Xl, Fcut);
    result.C = C_XcF(Xc, Fcut);

    return result;

}





