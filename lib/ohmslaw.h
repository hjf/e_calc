/*******************************************************************************
 *
 * Project: e_calc
 * 
 * ohmslaw.h
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


#ifndef REACTANCE_H
#define REACTANCE_H

#include <math.h>

#define I_VR(V, R) ( (V)/(R) )
#define I_PV(P, V) ( (P)/(V) )
#define I_PR(P, R) ( sqrt((P)/(R)) )

#define V_IR(I, R) ( (I)*(R) )
#define V_PI(P, I) ( (P)/(I) )
#define V_PR(P, R) ( sqrt((P)*(R)) )

#define R_VI(V, I) ( (V)/(I) )
#define R_PI(P, I) ( (P)/(I)*(I) )
#define R_VP(V, P) ( ((V)*(V))/P )

#define P_VI(V, I) ( (V)*(I) )
#define P_RI(R, I) ( (R)*(I)*(I) )
#define P_VI(V, R) ( ((V)*(V))/(R) )

#endif

