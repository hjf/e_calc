/*******************************************************************************

Smith.c

Glenn Smith (Dec 1971)

From

The Proximity Effect in Systems of
Parallel Conductors and Electrically
Small Multiturn Loop Antennas

deciphered from a Horible scanned copy and ported to C by

Brian Case KU7F (2017)
rush at winkey dot org

*******************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "smith.h"

//#define DEBUG



#define MAX(A, B) (A > B ? A : B)

#define ODD(N)  ((N) % 2)
#define EVEN(N) (((N) - 1) % 2)

#define INEGONENTH(N)   (ODD(N) ? -1 : 1)
#define DNEGONENTH(N)   (ODD(N) ? -1.0 : 1.0)

#define DDARRAY(PTR, R, C) (*((PTR) + (R) * nRows + (C)))

/*
C FORTRAN~ IV PROGRAM FOR SOLUTION BY THE METHOD OF COLLOCATION
C
C
C THIS PROGRAM USES THE METHOD UF COLLCCATION TO SOLVE A SYSTEM OF EQUATIONS
C FOR THE COEFFICIENTS OF TRIGONOMETRIC SERIES. THE SERIES REPRESENT THE
C NORMALIZED SURFACE CURRENT DENSITY ON EACH WIRE IN A SYSTEM OF nWires EQUALLY
C SPACED, PARALLEL, PERFECTLY CONDUCTING WIRES. USING THIS CURRENT AN
C APPROXIMATE VALUE OF THE NORPALILED HIGH FREQUENCY RESISTANCE OF THE SYSTEM
C IS CALCULATED. THE NUMBER OF HARMONIC TERMS USED TO DESCRIBE THE CURRENT ON
C EACH WIRE IS nHarmonics. THE RATIO ca IS EQUIVALENT TO THE SPACING BETWEEN WIRE
C CENTERS DIVIDED BY THE WIRE DIAMETER. FOR nWires AND nHarmonics THE
C SIZE OF THE MATRIX T, I,J) MUST BE AT LEAST AS LARGE AS 
C       T(nWires1*nHarmonics+1,nWires1*nHarmonics)
C wHERE nWires1=nWires/2     FOR nWires EVEN
C   AND nWires1=(nWires+1)/2 FOR nWires ODD.
*/


/*
C THIS FUNCTION SUBROUTINE EVALUATES THE ANALYTIC EXPRIESSION FOR THE DEFINITE
C INTERGRAL swin.
*/

double swin(int iHarmonic, double dfTheta, double dfSep);

/*
C THIS FUNCTION SUBROUTINE SOLVES A SYSTEM OF N LINEAR EQUATIONS IN N UNKNOWNS
C BY USING GAUSSIAN ELIMINATION WITH COLUMN PIVOTING.
*/

int LSOLVE(int N, double *padfT, int nColumns);

/* function to calc the normalized resistance from the matrix */

double ResNCalc(
    int nWires,
    int nHarmonics,
    int nWires28,
    int nAug,
    int nSize,
    size_t nColumns,
    double *padfT);

/* function to print the matrix for debuging */

void MatrixPrint(size_t nRows, size_t nColumns, double *padfT);




double SmithParallelCollocation(
    int nWires,
    int nHarmonics,
    double dfWireDia,
    double dfWireSpacing
) {


    double dfCA = dfWireSpacing / dfWireDia;

#ifdef DEBUG
    printf("nWires %i nHarmonics %i ca %lf\n", nWires, nHarmonics, dfCA);
#endif

    int nWires1 = (nWires + ODD(nWires)) / 2;

    size_t nRows = nWires1 * nHarmonics + 1;
    size_t nColumns = nWires1*nHarmonics;

    double *padfT;
    if (!(padfT = calloc( (nRows + 1) * (nColumns + 1), sizeof(double)))) {
        fprintf(stderr, "SmithParallelCollocation() could not alocate memory\n");
        exit(EXIT_FAILURE);
    }
    
    double dfTheta;
    double dfSep = 0.0;
    double dfResN = 0.0;
    
    int nWires20 = (nWires + 1)/2;
    int nWires28 = nWires / 2;
    int nSize = nWires20 * nHarmonics;
    int nAug = nHarmonics * nWires20 + 1;

#ifdef DEBUG
    printf("nWires20 %i nWires28 %i nSize %i nAug %i\n",
         nWires20, nWires28, nSize, nAug);
#endif

    /* loop over the harmonics */

    int iHarmonic;
    for (iHarmonic = 1; iHarmonic <= nHarmonics; iHarmonic++) {

        int nR1 = (nWires20 - 1) * nHarmonics + iHarmonic;

        /* SETTING COLLOCATION POINTS */

        dfTheta = M_PI * (double)iHarmonic / (double) (nHarmonics + 1);

        /* nHarmonics not even */
        if ( ODD(nHarmonics) && iHarmonic <= (nHarmonics + 1) / 2) {
            dfTheta = M_PI * (double)iHarmonic       / (double)(nHarmonics + 2);
        }
        if ( ODD(nHarmonics) && iHarmonic >  (nHarmonics + 1) / 2) {
            dfTheta = M_PI * (double)(iHarmonic + 1) / (double)(nHarmonics + 2);
        }
        
        size_t iRow;
        for (iRow = iHarmonic; iRow <= nR1; iRow += nHarmonics) {
            int nRWire = 1 + (iRow-iHarmonic) / nHarmonics;
             
            if (ODD(nWires)) {
                if (nRWire == nWires20) {
                    dfTheta /= 2.0;
                }
            }
            
            int nSWire = 1;
            int nH1 = nHarmonics;

            size_t iCol;
            for (iCol = 1; iCol <= nAug; iCol++) {
                
                if (iCol != nAug) {
                    if ((int)iCol-nH1 > 0) {
                        nH1 += nHarmonics;
                        nSWire++;
                    }
                    
                    int nSHarmonic = (int)iCol - nHarmonics * (nSWire - 1);

                    if (nRWire != nSWire) {
                        dfSep = dfCA * (double)(nSWire - nRWire);
                        if ((2 * nSWire) != (nWires + 1)) {
                            DDARRAY(padfT, iRow, iCol) =
                                -swin(nSHarmonic, dfTheta, dfSep);
                            
                            dfSep = dfCA * (double)(nWires + 1 - nSWire - nRWire);
                            DDARRAY(padfT, iRow, iCol) =
                                    DDARRAY(padfT, iRow, iCol)
                                  - DNEGONENTH(nSHarmonic)
                                  * swin(nSHarmonic, dfTheta, dfSep);
                            

                        }
                        else {
                            DDARRAY(padfT, iRow, iCol) =
                                -swin(2 * nSHarmonic, dfTheta, dfSep);
                            
                        }
                    }
                    else if ((2 * nSWire) != (nWires + 1)) {
                        dfSep = dfCA * (double)(nWires + 1 - 2 + nRWire);
                        //printf("dfSep %lf\n", dfSep);
                        DDARRAY(padfT, iRow, iCol) =
                                (cos((double)nSHarmonic * dfTheta))
                              / 2.0 - DNEGONENTH(nSHarmonic)
                              * swin(nSHarmonic, dfTheta, dfSep);
                        
                    }
                    else {
                        DDARRAY(padfT, iRow, iCol) =
                            (cos((double)(2 * nSHarmonic) * dfTheta)) / 2.0;
                        
                    }
                }
                else {
                    double dfSource=0.0;
                    
                    int nSelfHarmonic = 0;
                    
                    int LD1;
                    for (LD1 = 1; LD1 <= nWires28 ; LD1++) {
                        dfSep = dfCA * (double)(nWires + 1 - LD1 - nRWire);
                        //printf("dfSep %lf\n", dfSep);
                        dfSource += swin(nSelfHarmonic, dfTheta, dfSep);
                    }
                    
                    int LD2;
                    for (LD2=1; LD2 <= nWires20; LD2++) {
                        if (LD2 != nRWire) {
                            dfSep = dfCA * (double)(LD2 - nRWire);
                            dfSource += swin(nSelfHarmonic, dfTheta, dfSep);
                        }
                    }
                    
                    DDARRAY(padfT, iRow, iCol) = dfSource;
                    
                }
            }
        }
    }

#ifdef DEBUG
    MatrixPrint(nRows, nColumns, padfT);
#endif

    if (LSOLVE(nSize, padfT, nRows)) {
        printf("THE T MATRIX IS SINGULAR. NO UNIQUE SOLUTION EXISTS.\n");
    }

    else {

#ifdef DEBUG
        MatrixPrint(nRows, nColumns, padfT);
#endif
        /* caLCULATING THE NORMALIZED RESISTANCE */

        dfResN = ResNCalc(
            nWires,
            nHarmonics,
            nWires28,
            nAug,
            nSize,
            nRows,
            padfT);
    }

    return dfResN;

}

/*
C THIS FUNCTION SUBROUTINE EVALUATES THE ANALYTIC EXPRIESSION FOR THE DEFINITE
C INTERGRAL swin.
*/


double swin(int iHarmonic, double dfTheta, double dfSep) {

    
    double swin = 0.0;
    double S, A, B, C;
    double PSI = 0.0;

    if (dfSep < 0.0) {
        PSI =        atan( sin(dfTheta) / (-2.0 * dfSep - cos(dfTheta)));
    }

    else if (dfSep > 0.0) {
        PSI = M_PI - atan( sin(dfTheta) / ( 2.0 * dfSep + cos(dfTheta)));
    }

    else {
        printf("warning in swin PSI undefined\n");
    }

    if (dfSep != 0.0) {
        S = sqrt(4.0 * pow(dfSep, 2) + 1.0 + 4.0 * dfSep * cos(dfTheta));
        A = 0.5 * cos(dfTheta - PSI * (double)(iHarmonic - 1));
        C = 0.5 * cos(dfTheta + PSI * (double)(iHarmonic + 1));
        B = (1.0 + 2.0 * dfSep * cos(dfTheta)) * cos(PSI * (double)iHarmonic);
        if (iHarmonic != 0) {
            swin =  (A * pow(S, 2) + B * S + C)
                  / (   (1.0 - pow(S, 2)) * pow(-S, (iHarmonic+1)));
        }
        else {
            swin = -(B * S + 2.0 * C) / (S * (1.0 - pow(S, 2)));
        }
    }

    return swin;
}

/*
C THIS FUNCTION SUBROUTINE SOLVES A SYSTEM OF N LINEAR EQUATIONS IN N UNKNOWNS
C BY USING GAUSSIAN ELIMINATION WITH COLUMN PIVOTING.
*/


#define NPLUSONE (N + 1)
#define NMINUSONE (N - 1)
#define KPLUSONE (K + 1)

int LSOLVE(int N, double *padfT, int nRows) {
    
    int LSOLVE = 0;

    double SUM = 0.0;

    int I;
    int J;
    for(I = 1; I <= N; I++) {
        for(J = 1; J <= N; J++) {
            SUM = SUM +fabs(DDARRAY(padfT, I, J));
        }
    }

    /* this apears to be for some tolerance. statistical? */

    double Tolerance = (SUM / pow(N, 2)) * 1e-12;
    
    int K;
    for (K=1; K <= NMINUSONE; K++) {

        double TEMP = fabs(DDARRAY(padfT, K, K));
        int ITEMP = K;
        
        for (I=KPLUSONE; I <= N; I++) {

            if (fabs(DDARRAY(padfT, I, K)) > TEMP) {
                TEMP = fabs(DDARRAY(padfT, I, K));
                ITEMP = I;
                
            }
        }

        if (TEMP <= Tolerance ) {
            LSOLVE = 1;
            return LSOLVE;
        }
        if (ITEMP != K) {
            for (I=K ; I <= NPLUSONE ; I++) {

                TEMP = DDARRAY(padfT, K, I);
                
                DDARRAY(padfT, K, I) = DDARRAY(padfT, ITEMP, I);
                
                DDARRAY(padfT, ITEMP, I) = TEMP;
            }
        }

        for (I=KPLUSONE; I <= N; I++) {
            DDARRAY(padfT, I, K) =    DDARRAY(padfT, I, K) 
                                    / DDARRAY(padfT, K, K);
            
            for (J=KPLUSONE; J <= NPLUSONE; J++) {
            
                DDARRAY(padfT, I, J) =   DDARRAY(padfT, I, J)
                                       - DDARRAY(padfT, I, K)
                                       * DDARRAY(padfT, K, J);
            
            }
        }
    }

    if (fabs(DDARRAY(padfT, N, N)) > Tolerance) {

        DDARRAY(padfT, N, NPLUSONE) =     DDARRAY(padfT, N, NPLUSONE)
                                        / DDARRAY(padfT, N, N);

        for (I = 1; I <= NMINUSONE; I++) {

            K = N - I;
            for (J = 1; J <= I; J++) {

                int L = NPLUSONE - J;
                DDARRAY(padfT, K, NPLUSONE) =   DDARRAY(padfT, K, NPLUSONE)
                                              - DDARRAY(padfT, K, L)
                                              * DDARRAY(padfT, L, NPLUSONE);
            }
            DDARRAY(padfT, K, NPLUSONE) =    DDARRAY(padfT, K, NPLUSONE) 
                                           / DDARRAY(padfT, K, K);
        }
        LSOLVE = 0;
    }
    
    else {
        LSOLVE = 1;
    }

    return LSOLVE; 
}

/* function to calc the normalized resistance from the matrix */

double ResNCalc(
    int nWires,
    int nHarmonics,
    int nWires28,
    int nAug,
    int nSize,
    size_t nRows,
    double *padfT)
{

    //FIXME why is dfResN always a min of one this must not be Rp/Ro
    // Smith has a table with Rp/Ro less than 1
    // i get the right values if i use 0
    double dfResN = 0.0;

    int nCS = nWires28 * nHarmonics;

    /* assume a even number of wires and sum the results */        
    int iC;
    for (iC=1; iC <= nCS ; iC++) {
        dfResN += pow(DDARRAY(padfT, iC, nAug), 2) / (double)nWires;
    }

#ifdef DEBUG
    printf("resn %.12lf\n", dfResN);
#endif

    /* sum in the remaning results if its od */

    if (ODD(nWires)) {
        nCS += 1;

        for (iC = nCS; iC <= nSize ; iC++) {
            dfResN += pow(DDARRAY(padfT, iC, nAug), 2)/(double)(2 * nWires);
        }

#ifdef DEBUG
    printf("resn %.12lf\n", dfResN);
#endif
    }

    
    return dfResN;
}

/* function to print the matrix for debuging */

void MatrixPrint(size_t nRows, size_t nColumns, double *padfT) {

    size_t iRow, iCol;
    for (iRow = 1; iRow <=nRows ; iRow++) {
        printf("\n");
        for (iCol = 1; iCol <= nColumns; iCol++) {
            printf("% 4.6lf ", DDARRAY(padfT, iRow, iCol));
        }
    }
    printf("\n");

    return;
}


/*
C FORTRAN~ IV PROGRAM FOR SOLUTION BY THE METHOD OF LEAST SQUARES
C
C
C THIS PROGRAM USES THE METHOD OF LEAST SQUARES TO SOLVE A SYSTEM OF EQUATIONS
C FOR THE COEFFICIENTS OF TRIGONOMETRIC SERIES. THE SERIES REPRESENT THE
C NORMALIZED SURFACE CURRENT DENSITY ON EACH WIRE IN A SYSTEM OF nWires EQUALLY
C SPACED, PARALLEL, PERFECTLY CONDUCTING WIRES. USING THIS CURRENT AN
C APPROXIMATE VALUE OF THE NORPALILED HIGH FREQUENCY RESISTANCE OF THE SYSTEM
C IS CALCULATED. THE NUMBER OF HARMONIC TERMS USED TO DESCRIBE THE CURRENT ON
C EACH WIRE IS nHarmonics. THE RATIO ca IS EQUIVALENT TO THE SPACING BETWEEN WIRE
C CENTERS DIVIDED BY THE WIRE DIAMETER. FOR nWires AND nHarmonics THE
C SIZE OF THE MATRIX T, I,J) MUST BE AT LEAST AS LARGE AS 
C       T(nWires1*nHarmonics+1,nWires1*nHarmonics)
C wHERE nWires1=nWires/2     FOR nWires EVEN
C   AND nWires1=(nWires+1)/2 FOR nWires ODD.
*/

typedef struct data_struct {
    double dfSepMim;
    double dfSepMut;
    double dfSepSim;
    int nWires;
    int nWires20;
    int nWires28;
    int iSWire;
    int iSWireH;
    int nSWireH2;
    int iRWire;
    int iHarmonic;
    double dfCA;

}data_t;

typedef double(*func_t)(double, data_t);


/*
C   THIS FUNCTION SUBROUTINE EVALUATES THE ANALYTIC EXPRESSION FOR THE DEFINITE
C   INTEGRAL sintgl.

note this is the same function swin() used in the first program and is 
replaced by a macro to call swin

*/

#define sintgl swin

/*
C   THIS FUNCTION SUBROUTINE EVALUATES THE DEFINITE INTEGRALS WHICH ARISE IN THE
C   ELEMENTS CF THE MATRIX T.
*/

double alsqi (
    int iHarmonic,
    int iSWireH,
    func_t f,
    data_t d
);

/*
C   F1, F2, F3, F4 ANO F5 ARE AUXILIARY FUNCTION SUBRoUTINES USED TO SIMPLIFY THE
C   INTEGRANUS OF THE DEFINITE INTEGRALS WHICH ARE EVALUATED NUMERICALLY.
*/


double F1(double dfTheta, data_t d);
double F2(double dfTheta, data_t d);
double F3(double dfTheta, data_t d);
double F4(double dfTheta, data_t d);
double F5(double dfTheta, data_t d);

/*
C   GAUSS6 IS A FUNCTICN SUBUUUTINE WHICH COMPUTES AN APPROXIMATE VALUE OF THE
C   INTEGRAL OF F(X) OVER THE INTERVAL FRCP X=XL TO X=XU.  EVALUATION IS DONE BY
C   MEANS OF A 6- POINT GAUSS QUAURATURE FORMULA.
*/

double GAUSS6(
    double dfXL,
    double dfXU,
    func_t F,
    data_t d
);

/*
C THIS FUNCTION SUBROUTINE SOLVES A SYSTEM OF N LINEAR EQUATIONS IN N UNKNOWNS
C BY USING GAUSSIAN ELIMINATION WITH COLUMN PIVOTING.

note we reuse the fuction from the first app
*/




double SmithParallelLeastSquares(
    int nWires,
    int nHarmonics,
    double dfWireDia,
    double dfWireSpacing
) {

    data_t d;

    d.dfCA = dfWireSpacing / dfWireDia;
    d.nWires = nWires;
    
#ifdef DEBUG
    printf("nWires %i nHarmonics %i ca %lf\n", d.nWires, nHarmonics, d.dfCA);
#endif

    int nWires1 = (nWires + ODD(nWires)) / 2;

    size_t nRows = nWires1 * nHarmonics + 1;
    size_t nColumns = nWires1 * nHarmonics ;

#ifdef DEBUG
    printf("nRows %i nColumns %i\n", nRows, nColumns);
#endif

    double *padfT;
    if (!(padfT = calloc( (nRows + 1) * (nColumns + 1), sizeof(double)))) {
        fprintf(stderr, "SmithParallelLeastSquares() could not alocate memory\n");
        exit(EXIT_FAILURE);
    }
    
    
    double dfResN = 0.0;
    
    d.nWires20 = (nWires + 1) / 2;
    d.nWires28 = nWires / 2;
    int nSize = d.nWires20 * nHarmonics;
    int nAug = nSize + 1;

#ifdef DEBUG
    printf("d.nWires20 %i d.nWires28 %i nSize %i nAug %i\n", d.nWires20, d.nWires28, nSize, nAug);
#endif
    
    
    int nHarmonic1 = 0;

    for (d.iRWire = 1; d.iRWire <= d.nWires20; d.iRWire++) {
        d.dfSepSim = d.dfCA * (double)(d.nWires + 1 - 2 * d.iRWire);

        /* loop over the harmonics */
        for (d.iHarmonic = 1; d.iHarmonic <= nHarmonics; d.iHarmonic++) {

            size_t nRow=(d.iRWire - 1) * nHarmonics + d.iHarmonic;
            size_t nCol = 0;
            
            for (d.iSWire = 1; d.iSWire <= d.nWires20; d.iSWire++) {
                d.dfSepMut = d.dfCA * (double)(d.iSWire - d.iRWire);
                d.dfSepMim = d.dfCA * (double)(d.nWires + 1 - d.iSWire - d.iRWire);

                for (d.iSWireH = 1; d.iSWireH <= nHarmonics; d.iSWireH++) {
                    d.nSWireH2 = 2 * d.iSWireH;
                    nCol = (d.iSWire - 1) * nHarmonics + d.iSWireH;

                    double DEL=0.0;
                    nHarmonic1 = d.iHarmonic;

                    if (ODD(d.nWires) && d.iRWire == d.nWires20) {
                        nHarmonic1 *= 2;
                    }
                    if (d.iHarmonic == d.iSWireH) {
                        DEL = M_PI / 8.0;
                    }

                    
                    if (d.iRWire == d.nWires20 && ODD(d.nWires)) {
                        if (d.iRWire != d.iSWire) {
                            DDARRAY(padfT, nRow, nCol) =
                                -0.5 * alsqi(nHarmonic1, d.iSWireH, &F4, d);
                        }
                        else {
                            DDARRAY(padfT, nRow, nCol)= DEL;
                        }
                    }
                    else if (d.iRWire == d.iSWire) {
                        DDARRAY(padfT, nRow, nCol) =
                            DEL - 0.5 * alsqi(d.iHarmonic, d.iSWireH, &F3, d);
                    }
                    else if (d.iSWire == d.nWires20 && ODD(d.nWires)) {
                        DDARRAY(padfT, nRow, nCol) =
                            -alsqi(d.iHarmonic, d.nSWireH2, &F2, d);
                    }
                    else {
                        
                        DDARRAY(padfT, nRow, nCol) =
                            -alsqi(d.iHarmonic, d.iSWireH, &F1, d);
                    }
                    
                }
            }

            DDARRAY(padfT, nRow, nAug) = alsqi(nHarmonic1, d.iHarmonic, &F5, d);

        }

    }

#ifdef DEBUG
    MatrixPrint(nRows, nColumns, padfT);
#endif

    if (LSOLVE(nSize, padfT, nRows)) {
        printf("THE T MATRIX IS SINGULAR. NO UNIQUE SOLUTION EXISTS.\n");
    }


    else {

#ifdef DEBUG
        MatrixPrint(nRows, nColumns, padfT);
#endif

        dfResN = ResNCalc(
            nWires,
            nHarmonics,
            d.nWires28,
            nAug,
            nSize,
            nRows,
            padfT);
    }


    return dfResN;

}

/*
C   THIS FUNCTION SUBROUTINE EVALUATES THE DEFINITE INTEGRALS WHICH ARISE IN THE
C   ELEMENTS CF THE MATRIX T.
*/

#define MAX(A, B) (A > B ? A : B)

double alsqi (
    int iHarmonic,
    int iSWireH,
    func_t f,
    data_t d
) {
    
    int MP=MAX(iHarmonic, iSWireH);

    double dfTheta1 = 0.0;
    double dfTheta2 = M_PI / (double)(2 * MP);
    double dfTheta3 = M_PI - dfTheta2;
    double dfDTheta = dfTheta2 * 2.0;
    double alsqi = 0.0;

    int iP;
    for (iP = 1; iP <= MP; iP++) {
        alsqi += GAUSS6(dfTheta1, dfTheta2, f, d);
        dfTheta1 = dfTheta2;
        dfTheta2 += dfDTheta;
    }

    alsqi += GAUSS6(dfTheta3, M_PI, f, d);
    
    return alsqi;
}

 
/*
C   F1, F2, F3, F4 ANO F5 ARE AUXILIARY FUNCTION SUBRoUTINES USED TO SIMPLIFY THE
C   INTEGRANUS OF THE DEFINITE INTEGRALS WHICH ARE EVALUATED NUMERICALLY.
*/


double F1(double dfTheta, data_t d) {
    //printf("f1");
    double F1 =   (   0.5 
                    * cos((double)d.iHarmonic * dfTheta)
                    - DNEGONENTH(d.iHarmonic) 
                    * sintgl(d.iHarmonic, dfTheta, d.dfSepSim)
                  )
                * (   sintgl(d.iSWireH, dfTheta, d.dfSepMut) 
                    + DNEGONENTH(d.iSWireH)
                    * sintgl(d.iSWireH, dfTheta, d.dfSepMim)
                  );

    return F1;
}

double F2(double dfTheta, data_t d) {

    //printf("f3");
    double F2 =   (   0.5
                    * cos((double)d.iHarmonic * dfTheta)
                    - DNEGONENTH(d.iHarmonic) 
                    * sintgl(d.iHarmonic, dfTheta, d.dfSepSim)
                  )
                * sintgl(d.nSWireH2, dfTheta, d.dfSepMut);

    return F2;
}

double F3(double dfTheta, data_t d) {

    //printf("f3");
    double F3 =   DNEGONENTH(d.iHarmonic)
                * sintgl(d.iHarmonic, dfTheta, d.dfSepSim)
                * cos((double)d.iSWireH * dfTheta)
                + DNEGONENTH(d.iSWireH)
                * sintgl(d.iSWireH, dfTheta, d.dfSepSim)
                * cos((double)d.iHarmonic * dfTheta)
                - 2.0
                * DNEGONENTH(d.iHarmonic + d.iSWireH)
                * sintgl(d.iHarmonic, dfTheta, d.dfSepSim)
                * sintgl(d.iSWireH, dfTheta, d.dfSepSim);

    return F3;
}

double F4(double dfTheta, data_t d) {

    //printf("f4");
    double F4 =    cos(2.0 * (double)d.iHarmonic * dfTheta)
                *  (   sintgl(d.iSWireH, dfTheta, d.dfSepMut)
                     + DNEGONENTH(d.iSWireH)
                     * sintgl(d.iSWireH, dfTheta, d.dfSepMim)
                   );

    return F4;
}

double F5(double dfTheta, data_t d) {

    //printf("f5");
    double F5 = 0.0;

    int K;
    for (K = 1; K <= d.nWires28; K++) {
        
        d.dfSepMut = d.dfCA * (double)(K - d.iRWire);
        d.dfSepMim = d.dfCA * (double)(d.nWires + 1 - K - d.iRWire);
        if (K != d.iRWire) {
            F5 +=   sintgl(0, dfTheta, d.dfSepMut)
                  + sintgl(0, dfTheta, d.dfSepMim);
        }
    }
    d.dfSepMut = d.dfCA * (double)(d.nWires20 - d.iRWire);
    
    if (ODD(d.nWires)) {

        if (d.iRWire == d.nWires20) {   
            F5 *= (   0.5 
                    * cos((double)d.iHarmonic * 2.0 * dfTheta)
                  );
        }

        else {
            F5 =   ( F5 
                     + sintgl(0, dfTheta, d.dfSepMut)
                     + sintgl(0, dfTheta, d.dfSepSim)
                   )
                 * (   0.5
                     * cos((double)d.iHarmonic * dfTheta)
                     - DNEGONENTH(d.iHarmonic)
                     * sintgl(d.iHarmonic, dfTheta, d.dfSepSim)
                   );
        }
    }

    else {
        F5 =   ( F5 
                 + sintgl(0, dfTheta, d.dfSepSim)
               )
             * (   0.5
                 * cos((double)d.iHarmonic * dfTheta)
                 - DNEGONENTH(d.iHarmonic)
                 * sintgl(d.iHarmonic, dfTheta, d.dfSepSim)
               );
    }

    return F5;
}



/*
C   GAUSS6 IS A FUNCTICN SUBUUUTINE WHICH COMPUTES AN APPROXIMATE VALUE OF THE
C   INTEGRAL OF F(X) OVER THE INTERVAL FROM X=XL TO X=XU.  EVALUATION IS DONE BY
C   MEANS OF A 6- POINT GAUSS QUAURATURE FORMULA.
*/

double GAUSS6(
    double dfXL,
    double dfXU,
    func_t F,
    data_t d
) {

    double A = 0.5 * (dfXU + dfXL);
    double B = dfXU - dfXL;
    double C = 0.4662348 * B;
    double GAUSS6 = 0.08566225 * ( F(A + C, d) + F(A - C, d));
    C=0.3306047 * B;
    GAUSS6 = GAUSS6 + 0.1803808 * ( F(A + C, d) + F(A - C, d));
    C=0.1193096 * B;
    GAUSS6 = B * (GAUSS6 + 0.2339570 * (F(A + C, d) + F(A - C, d)));
    
    return GAUSS6;
}


/*
FORTRAN IV PROGRAM FOR CALCULATING THE CURRENT DISTRIBUTIONS CAUSED
BY INTERCONNECTING WIRES IN THE EXPERIMERTAL MODEL.

THIS PROGRAM USES THE METHOD OF COLLOCATION T3 SOLVE A SYSTEM OF
EQUATIONS FOR THE COEFFICIENTS OF TRIGONOMETRIC SERIES. THE SERIES
REPRESENT THE NORNALIZED SURFACE CURRENT DENSITY ON EACH WIRE IN
SYSTEM OF NW WIRES. THE NUMBER OF HARMONIC TERMS USED TO DESCRIBE
THE CURRENT ON EACH WIRE IS NH. THE RATIO CA IS EQUIVALENT TO THE
SPACING BETWEEN WIRE CENTERS DIVIDED BY THE WIRE DIAMETER. THE
NORMALIZED DIMENSIONS CAH2 AND CHL2 ARE THE TUBE RADIUS DIVIDED BY
THE TUBE HALF HEIGHT AND THE TUBE HALF HEIGHT DIVIDED BY THE LENGTH
OF THE INTERCONNECTING WIRES.

*/

/*
C THIS FUNCTION SUBROUTINE EVALUATES THE ANALYTIC EXPRIESSION FOR THE DEFINITE
C INTERGRAL AINE.
*/

double aine(int iHarmonic, double dfTheta, double dfSep);

/*
C THIS FUNCTION SUBROUTINE EVALUATES THE ANALYTIC EXPRIESSION FOR THE DEFINITE
C INTERGRAL AINES.
*/

double aines(int iHarmonic, double dfTheta, double dfSep);


double SmithMultiturnCollocation(
    int nTurns,
    int nHarmonics,
    double dfWireDia,
    double dfWireSpacing
) {

    double dfCAH2 = 0.048645;
    double dfCHL2 = 0.340824;

    double dfCA = dfWireSpacing / dfWireDia;
    
#ifdef DEBUG
    printf("nTurns %i nHarmonics %i ca %lf\n", nTurns, nHarmonics, dfCA);
#endif

    int nTurns1 = (nTurns + ODD(nTurns)) / 2;

    size_t nRows = nTurns1 * nHarmonics + 1;
    size_t nColumns = nTurns1 * nHarmonics ;

#ifdef DEBUG
    printf("nRows %i nColumns %i\n", nRows, nColumns);
#endif

    double *padfA;
    if (!(padfA = calloc( (nRows + 1) * (nColumns + 1), sizeof(double)))) {
        fprintf(stderr, "SmithMultiturnCollocation() could not alocate memory\n");
        exit(EXIT_FAILURE);
    }
    
    double dfTheta;
    double dfSep;    
    double dfResN = 0.0;
    
    int nTurns21 = (nTurns + 1) / 2;
    int nTurns2 = nTurns / 2;
    int nSize = nTurns21 * nHarmonics;
    int nAug = nSize + 1;

#ifdef DEBUG
    printf("nTurns21 %i nTurns2 %i nSize %i nAug %i\n",
        nTurns21, nTurns2, nSize, nAug);
#endif
    
    /* loop over the harmonics */

    int iHarmonic;
    for (iHarmonic = 1; iHarmonic <= nHarmonics; iHarmonic++) {

        int nR1 = (nTurns21 - 1) * nHarmonics + iHarmonic;

        /* SETTING COLLOCATION POINTS */

        dfTheta = M_PI * 2.0 * ((double)iHarmonic / (double) (nHarmonics + 1));

        size_t iRow;
        for (iRow = iHarmonic; iRow <= nR1; iRow += nHarmonics) {
            int nRWire = 1 + (iRow-iHarmonic) / nHarmonics;
             
            if (ODD(nTurns)) {
                if (nRWire == nTurns21) {
                    dfTheta = (dfTheta - M_PI) / 2.0;
                }
                if (dfTheta <= 0.0) {
                    dfTheta = 2.0 * M_PI + dfTheta;
                }
            }
            
            int nSWire = 1;
            int nH1 = nHarmonics;

            size_t iCol;
            for (iCol = 1; iCol <= nAug; iCol++) {
                
                if (iCol != nAug) {
                    if ((int)iCol-nH1 > 0) {
                        nH1 += nHarmonics;
                        nSWire++;
                    }
                   
                    int nSHarmonic = (int)iCol - nHarmonics * (nSWire - 1);

                    if (!ODD(iCol)) {
                        nSHarmonic /= 2;
                    }
                    else {
                       nSHarmonic = (nSHarmonic +1) / 2;
                    }

                    if (nRWire != nSWire) {
                        dfSep = dfCA * (double)(nSWire - nRWire);
                        if ((2 * nSWire) != (nTurns + 1)) {
                            DDARRAY(padfA, iRow, iCol) =
                                    -aine(nSHarmonic, dfTheta, dfSep);
                            
                            dfSep = dfCA * (double)(nTurns + 1 - nSWire - nRWire);
                                
                            if (ODD(iCol)) {
                                DDARRAY(padfA, iRow, iCol) =
                                        DDARRAY(padfA, iRow, iCol)
                                      - DNEGONENTH(nSHarmonic)
                                      * aine(nSHarmonic, dfTheta, dfSep);
                            }
                            else {
                                    DDARRAY(padfA, iRow, iCol) =
                                        DDARRAY(padfA, iRow, iCol)
                                      + DNEGONENTH(nSHarmonic)
                                      * aines(nSHarmonic, dfTheta, dfSep);
                            }

                        }
                        else if (ODD(iCol)) {
                            DDARRAY(padfA, iRow, iCol) =
                                -aine(2 * nSHarmonic, dfTheta, dfSep);
                        }
                        else {
                            DDARRAY(padfA, iRow, iCol) =
                                -aines(2 * nSHarmonic - 1, dfTheta, dfSep);
                        }
                    }
                    else if ((2 * nSWire) != (nTurns + 1)) {
                        dfSep = dfCA * (double)(nTurns + 1 - 2 * nRWire);
                        if (ODD(iCol)) {
                            DDARRAY(padfA, iRow, iCol) =
                                    (cos((double)nSHarmonic * dfTheta))
                                  / 2.0
                                  - DNEGONENTH(nSHarmonic)
                                  * aine(nSHarmonic, dfTheta, dfSep);
                        }
                        else {
                            DDARRAY(padfA, iRow, iCol) =
                                    (sin((double)nSHarmonic * dfTheta))
                                  / 2.0
                                  + DNEGONENTH(nSHarmonic)
                                  * aines(nSHarmonic, dfTheta, dfSep);
                        }
                        
                    }
                    else if (ODD(iCol)) {
                        DDARRAY(padfA, iRow, iCol) =
                            (cos((double)(2 * nSHarmonic) * dfTheta)) / 2.0;
                        
                    }
                    else {
                        DDARRAY(padfA, iRow, iCol) =
                            (sin((double)(2 * nSHarmonic - 1) * dfTheta)) / 2.0;
                    }
                }
                else {
                    double dfSource = 0.0;
                    double dfSource1 = 0.0;
                    
                    
                    int LD1;
                    for (LD1 = 1; LD1 <= nTurns ; LD1++) {
                        dfSep = dfCA * (double)(LD1 - nRWire);
                        
                        dfSource1 =  (  dfCAH2
                                      / sqrt(  1.0 
                                             + pow(dfCHL2, 2)
                                             + 4.0
                                             * pow(dfSep * dfCAH2 * dfCHL2, 2)
                                            )
                                     )
                                   * (  pow(dfCHL2, 2)
                                      * (  sin(dfTheta) 
                                         - 2.0
                                         * dfSep
                                         * dfCAH2
                                         * dfCHL2
                                         * cos(dfTheta)
                                        )
                                      / (  1.0
                                         + 4.0
                                         * pow(dfSep * dfCAH2 * dfCHL2, 2)
                                        )
                                      + sin(dfTheta)
                                      / (  1.0
                                         + 4.0
                                         * pow(dfSep * dfCAH2, 2)
                                        )
                                     )
                                   - (
                                        (  1.0
                                         + 2.0
                                         * dfSep
                                         * cos(dfTheta)
                                        )
                                      / (  1.0
                                         + 4.0
                                         * pow(dfSep, 2)
                                         + 4.0
                                         * dfSep
                                         * cos(dfTheta)
                                        )
                                     )
                                   * (  1.0
                                      - 1.0
                                      / sqrt(  1.0 
                                             + 4.0
                                             * pow(dfCAH2, 2)
                                             * pow(dfSep, 2)
                                            )
                                      );

                        dfSource += dfSource1;
                    } 
                    
                    DDARRAY(padfA, iRow, iCol) = dfSource;
                    
                }
            }
        }
    }

#ifdef DEBUG
    MatrixPrint(nRows, nColumns, padfA);
#endif

    if (LSOLVE(nSize, padfA, nRows)) {
        printf("THE T MATRIX IS SINGULAR. NO UNIQUE SOLUTION EXISTS.\n");
    }

    else {

#ifdef DEBUG
        MatrixPrint(nRows, nColumns, padfA);
#endif
        /* caLCULATING THE NORMALIZED RESISTANCE */

        dfResN = ResNCalc(
            nTurns,
            nHarmonics,
            nTurns2,
            nAug,
            nSize,
            nRows,
            padfA);
    }

    return dfResN;

}
    
/*
C THIS FUNCTION SUBROUTINE EVALUATES THE ANALYTIC EXPRIESSION FOR THE DEFINITE
C INTERGRAL AINE.
*/

double aine(int iHarmonic, double dfTheta, double dfSep) {

    
    double aine = 0.0;
    double H, B, C, E, F, G;
    double PSI = 0.0;

    if (dfSep < 0.0) {
        if (dfTheta <= M_PI) {
            PSI =       -atan( sin(dfTheta) / (-2.0 * dfSep - cos(dfTheta)));
        }
        //FIXME note smiths code was orignaly this way not sure what is correct
        // if theta == PI
        if (dfTheta >= M_PI) {
            PSI =        atan(-sin(dfTheta) / (-2.0 * dfSep - cos(dfTheta)));
        }
    }
    else if (dfSep > 0.0) {
        if (dfTheta <= M_PI) {
            PSI = M_PI + atan( sin(dfTheta) / ( 2.0 * dfSep - cos(dfTheta)));
        }
        //FIXME note smiths code was orignaly this way not sure what is correct
        // if theta == PI
        if (dfTheta >= M_PI) {
            PSI = M_PI - atan(-sin(dfTheta) / ( 2.0 * dfSep - cos(dfTheta)));
        }
    }
    else {
        printf("warning in aine() PSI undefined\n");
    }

    if (dfSep != 0.0) {
        H = (1.0 + 2.0 * dfSep * cos(dfTheta)) * cos(PSI * (double)iHarmonic);
        B = 0.5 * cos(dfTheta - PSI * (double)(iHarmonic + 1));
        C = 0.5 * cos(dfTheta + PSI * (double)(iHarmonic - 1));
        E = 4.0 * pow(dfSep, 2) + 2.0 + 4.0 * dfSep * cos(dfTheta);
        F = (2.0 * pow(E - 1.0, 0.5)) / E;
        G = (pow ((1.0 - pow(F, 2)), 0.5) - 1.0) / F;
        
        aine =   (pow(G, iHarmonic - 1))
               * (H * G + B * pow(G, 2) + C)
               / (E * pow(1.0 - pow(F, 2), 0.5));

    }

    return aine;
}


/*
C THIS FUNCTION SUBROUTINE EVALUATES THE ANALYTIC EXPRIESSION FOR THE DEFINITE
C INTERGRAL AINES.
*/

double aines(int iHarmonic, double dfTheta, double dfSep) {

    
    double aines = 0.0;
    double H1, B1, C1, E, F, G;
    double PSI = 0.0;

    if (dfSep < 0.0) {
        if (dfTheta <= M_PI) {
            PSI =       -atan( sin(dfTheta) / (-2.0 * dfSep - cos(dfTheta)));
        }
        //FIXME note smiths code was orignaly this way not sure what is correct
        // if theta == PI
        if (dfTheta >= M_PI) {
            PSI =        atan(-sin(dfTheta) / (-2.0 * dfSep - cos(dfTheta)));
        }
    }
    else if (dfSep > 0.0) {
        if (dfTheta <= M_PI) {
            PSI = M_PI + atan( sin(dfTheta) / ( 2.0 * dfSep - cos(dfTheta)));
        }
        //FIXME note smiths code was orignaly this way not sure what is correct
        // if theta == PI
        if (dfTheta >= M_PI) {
            PSI = M_PI - atan(-sin(dfTheta) / ( 2.0 * dfSep - cos(dfTheta)));
        }
    }
    else {
        printf("warning in aines() PSI undefined\n");
    }

    if (dfSep != 0.0) {
        H1 = (1.0 + 2.0 * dfSep * cos(dfTheta)) * sin(PSI * (double)iHarmonic);
        B1 = -0.5 * sin(PSI * (double)(iHarmonic + 1) - dfTheta);
        C1 = -0.5 * sin(PSI * (double)(iHarmonic - 1) + dfTheta);
        E = 4.0 * pow(dfSep, 2) + 2.0 + 4.0 * dfSep * cos(dfTheta);
        F = (2.0 * pow(E - 1.0, 0.5)) / E;
        G = (pow ((1.0 - pow(F, 2)), 0.5) - 1.0) / F;
        
        aines =  (pow(G, iHarmonic - 1))
               * (H1 * G + B1 * pow(G, 2) + C1)
               / (E * pow(1.0 - pow(F, 2), 0.5));

    }

    return aines;
}



