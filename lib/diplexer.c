/*******************************************************************************
 *
 * Project: e_calc
 * 
 * diplexer.c
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

#include "diplexer.h"
#include "reactance.h"
#include "lowpass.h"
#include "highpass.h"
#include "q.h"


struct Diplexer Diplexer_bandpass(
    double Fcenter,
    double Fwidth,
    double Z
) {

    struct Tank result;

    double Q;
    double X;

    Q = Q_FbwFcut(Fwidth, Fcenter);

    result.L[0] = LOWPASS_L(Q, Z, Fcenter);
    result.C[0] = HIGHPASS_C(Q, Z, Fcenter);

    result.L[1] = HIGHPASS_L(Q, Z, Fcut);
    result.C[1] = LOWPASS_C(Q, Z, Fcut);

    return result;
}

struct Diplexer Diplexer_lowpass(
    double Fcut,
    double Fwidth,
    double Z
) {

    struct Tank result;

    double Q;
    double X;

    Q = Q_FbwFcut(Fwidth, Fcenter);

    result.C[0] = HIGHPASS_C(Q, Z, Fcut);
    result.L[0] = HIGHPASS_L(Q, Z, Fcut);

    return result;
}





