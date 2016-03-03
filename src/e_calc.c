/*******************************************************************************
 *
 * Project: e_calc
 * 
 * e_calc.c
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
#include <stdio.h>
#include <string.h>

#include "butterworth.h"
#include "chebyshev.h"
#include "capacitor.h"
#include "inductor.h"
#include "resister.h"
#include "resister.h"

void help (char *argv0) {
    
    printf("%s butterworth <calc> <calc args>\n", argv0); 
    printf("%s chebyshev <calc> <calc args>\n", argv0);
    printf("%s capacitor <calc> <calc args>\n", argv0);
    printf("%s inductor <calc> <calc args>\n", argv0);
    printf("%s resister <calc> <calc args>\n", argv0);
    printf("%s match_network <calc> <calc args>\n", argv0);

}



int main (int argc, char *argv[]) {

    if (argc < 2 ) {
        help(argv[0]);
        return EXIT_FAILURE;
    }

    if (0 == strcasecmp(argv[1], "butterworth") ) {
        butterworth_parse(argc, argv);
    }

    if (0 == strcasecmp(argv[1], "chebyshev") ) {
        chebyshev_parse(argc, argv);
    }

    if (0 == strcasecmp(argv[1], "capacitor") ) {
        capacitor_parse(argc, argv);
    }
    if (0 == strcasecmp(argv[1], "inductor") ) {
        inductor_parse(argc, argv);
    }
    if (0 == strcasecmp(argv[1], "resister") ) {
        resister_parse(argc, argv);
    }

    if (0 == strcasecmp(argv[1], "match_network") ) {
        match_network_parse(argc, argv);
    }

    else {

        help(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

}
