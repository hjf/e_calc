/*******************************************************************************
 *
 * Project: e_calc
 * 
 * chebyshev.h
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

#ifndef CHEBYSHEV_H
#define CHEBYSHEV_H

struct Chebyshev {
    unsigned int nOrder; 
    double *C;
    double *L;
};

/*******************************************************************************
function to free the Chebyshev struct

@param c    Chebyshev struct to free

@return nothing

*******************************************************************************/

void Chebyshev_free(struct Chebyshev * b);

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
);

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
);

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
);

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
);

#endif

