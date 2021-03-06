/*******************************************************************************
 *
 * Project: e_calc
 * 
 * reactance.h
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


#define XC(F, c)     ( 1 / (2 * M_PI * (F)  * (c ) ) )
#define C_XcF(Xc, F) ( 1 / (2 * M_PI * (F)  * (Xc) ) )
#define F_XcC(Xc, c) ( 1 / (2 * M_PI * (Xc) * (c ) ) )



#define XL(f, l)     ( 2 * M_PI * (f) * (l) )
#define L_XlF(Xl, F) ( (Xl) / ( 2 * M_PI * (F) ) )



#define XSeries_QR(Q, R) ((Q)*(R))

#define XShunt_QR(Q, R) ((R)/(Q))

#endif

