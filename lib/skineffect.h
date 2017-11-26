/*******************************************************************************
 *
 * Project: e_calc
 * 
 * skineffect.h
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

#ifndef SKINEFFECT_H
#define SKINEFFECT_H

#include <math.h>
#include "permeability.h"

#define SKIN_R(FREQ, PERMEABILITY, CONDUCTIVITY)                            (sqrt(( M_PI * (FREQ) * PERMEABILITY_FS * (PERMEABILITY)) / (CONDUCTIVITY)))

#define SKIN_DEPTH_RAP(RESISTIVITY, ANGULARFREQ, PERMEABILITY)               (sqrt((2 * (RESISTIVITY)) / ((ANGULARFREQ) * PERMEABILITY_FS * (PERMEABILITY)))

#define SKIN_DEPTH_RFP(RESISTIVITY, FREQ, PERMEABILITY)               (sqrt((RESISTIVITY) / (M_PI * (FREQ) * PERMEABILITY_FS * (PERMEABILITY))))

#define SKIN_RESISTANCE_THICK(LENGTH, RESISTIVITY, DIAMETER, SKIN_DEPTH)    (((LENGTH) * (RESISTIVITY)) / (M_PI  * (DIAMETER) * (SKIN_DEPTH)))

#endif



