/*******************************************************************************
 *
 * Project: e_calc
 * 
 * match_network.h
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


#ifndef MATCH_NETWORK_H
#define MATCH_NETWORK_H

struct match_network {
    int series;
    double *L;
    double *C;
    double *Zi;
    double BW;
    int nOrder;
};

void match_network_free(struct match_network * m);

/*******************************************************************************
    function to calculate a lowpass impedance match

*******************************************************************************/

struct match_network *match_network_l_lowpass(
    double Zgen,
    double Zload,
    double Fcut,
    int nOrder
);

/*******************************************************************************
    function to calculate a highpass impedance match

*******************************************************************************/

struct match_network *match_network_l_highpass(
    double Zgen,
    double Zload,
    double Fcut,
    int nOrder
);

struct match_network *match_network_pi(
    double Rg,
    double Rl,
    double Fcut,
    double Fbw
);

struct match_network *match_network_pi2(
    double Rg,
    double Rl,
    double Fcut
);

struct match_network *match_network_t(
    double Rg,
    double Rl,
    double Fcut,
    double Fbw
);


#endif
