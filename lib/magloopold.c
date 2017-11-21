


#define LAMBA (freq) (double) 3e8/freq

#define Permfs (4*M_PI*10e-7)



/***** permeability of free space in henrys per meter *****/

#define Uo 1.257e-6

/***** permittivity of free space in farads per meter *****/
#define Eo 18.85e-12

/***** impedance of free space *****/

#define Z0 sqrt(Uo/Eo)

/*******************************************************************************

@param bSquare  0 for a circular loop, non 0 for square
@param nLoops   number of paralell loops
@param U        Permability of free space
@param Ur       relative permibility of the conductor
@param LoopR    loop radius
@param LoopD    loop diameter
@param CondR    conductor radius
@param K        ???

@return the inductance of the loop
*******************************************************************************/

double loop_Inductance (
    int bSquare,
    int nLoops,
    double U,
    double Ur,
    double LoopR,
    double LoopD,
    double CondR,
    double K
) {

    double L;

    if (bSquare)
        L=((2.0 * U * Ur * LoopD) / M_PI ) * (log ( LoopD / CondR)-0.77401);
    else
        L= LoopR * U * Ur * (log((8.0 * LoopR ) / CondR) - 2.0 );

    if ( nLoops > 1)
        L= L/ ((double) nLoops * K);

    return L;

}

/*******************************************************************************

@param bSquare  0 for a circular loop, non 0 for square
@param nLoops   number of paralell loops
@param U        Permability of free space
@param Ur       relative permibility of the conductor
@param Area     loop Area
@param W        wavelength

@return the radiation resiatance of the loop
*******************************************************************************/

double loop_RadiationResistance(
    int bSquare,
    int nLoops,
    double Area,
    double Wl,
) {

    
    double RadResistance;
    
    if (bSquare)
        RadResistance = ((31171·((Area * Area) / (Wl * Wl * Wl * Wl)) ) / 1.621) * nLoops;
    else
        RadResistance = (31171·((Area * Area) / (Wl * Wl * Wl * Wl)) * nLoops;

    return RadResistance;
}

/*******************************************************************************

@param nLoops       number of paralell loops
@param U            Permability of free space
@param Ur           relative permibility of the conductor
@param Conductivity Conductivity of the conductor
@param Freq         Freq 

@return the skin effect resistance of the loop
*******************************************************************************/

double loop_SkinEffect(
    int nLoops,
    double U,
    double Ur,
    double Conductivity,
    double Freq
) {

    double Rskin;

    Rskin = sqrt(( M_PI * Freq *U * Ur) / Conductivity);

    Ro=(CIR/cir)·Rs

    Rhf=Ro/N d



Proximity Effect

Glenn Smith (1971) did an exhaustive examination of proximity effect in multiple parallel conductors. To make it simple, if you space the center of the conductors by 3 times the diameter of a single conductor, proximity effect will be minimized to less than 1.2 percent for a small number of parallel conductors (N > 0, N<=2). However, there is a limit, and at around three or four parallel conductors it is no longer possible to adequately compensate for proximity effect, and there is little to gain from adding additional parallel conductors. The model is complex and a suitable simplified equation for proximity effect in a magnetic loop antenna does not yet exist. With this problem in mind an approximation is used here which assumes that proximity effect increases by N cubed as the number of parallel loops is increased. In practice, this is far from being a perfect approximation, but the gross behavior of the approximation is correct, and comparisons of predicted vs measured change in antenna radiation due to a change in N are within 1 dB of predicted values in the test antenna with N=4 (four loops) and are more strongly in agreement at N=2.

Rpo=0.003·N3

Rp=Ro·Rpo

Rhf2=(Rp+Ro)·CIR 



