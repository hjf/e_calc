/*******************************************************************************
 *
 * Project: e_calc
 * 
 * butterworth.h
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

#ifndef BUTERWORTH_H
#define BUTERWORTH_H

struct Butterworth {
    unsigned int nOrder; 
    double *C;
    double *L;
};

/*******************************************************************************
function to free the Butterworth struct

@param b    Butterworth struct to free

@return nothing

*******************************************************************************/

void Butterworth_free(struct Butterworth * b);


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
);

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
);

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
);

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
);

#endif

