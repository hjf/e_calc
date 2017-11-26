/*******************************************************************************
*******************************************************************************/

#ifndef MAGLOOP_H
#define MAGLOOP_H

/***** some macros for square, cube, etc... *****/

#define SQUARED(N) ((N)*(N))
#define CUBED(N) ((N)*(N)*(N))
#define FOURTH(N)  ((N)*(N)*(N)*(N))

typedef struct magloop_out_struct { 
    double Frequency;
    double RadiationResistance;
    double SkinDepth;
    double RfResistanceLoss2;
    double Efficiency;
    double LoopInductance;
    double C;
    double Xl;
    double LoopSeriesImpedance;
    double Q;
    double SixDbBandwidth;
    double SwrBandwidth;
    double LcDynamicResistance;
    double VMAX;
    double LoopDiamater;
    double PickupLoopArea;
} magloop_out_t;

/*******************************************************************************
single loop calc
*******************************************************************************/

magloop_out_t *magloop_single_calc (
    double LoopCircumference,
    double LoopConductorDiameter,
    double Resistivity,
    double RelativePermeabilityConductor,
    /*double K,*/
    double TxPower,
    double FrequencyLowLimit,
    double FrequencyHighLimit,
    double FrequencyStep,
    int isSquare,
    size_t *nOuts);

/*******************************************************************************
multi loop calc
*******************************************************************************/

magloop_out_t *magloop_multi_calc (
    double LoopCircumference,
    double LoopConductorDiameter,
    double Resistivity,
    double RelativePermeabilityConductor,
    /*double K,*/
    double TxPower,
    double FrequencyLowLimit,
    double FrequencyHighLimit,
    double FrequencyStep,
    int isSquare,
    int nLoops,
    size_t *nOuts);


/*******************************************************************************
@brief Fuction to calculate radiation resistance

@param LoopArea     area of the loop
@param Wavelength
@param isSquare     non zero if the loop is square
@param NumLoops     number of parallel loops
@param NumTurns     number of turns in a multiturn loop
@returns radiation resistance

*******************************************************************************/

double CalcRadiationResistance(
    double LoopArea,
    double Wavelength,
    int isSquare,
    int NumLoops,
    int NumTurns);


/*******************************************************************************
@brief Fuction to calculate combined rf loss resistance for skin effect and
       proximity effect

@param RfResistanceSkinEffect   rf loss resistance for skin effect
@param LoopCircumference        circumference of the loop
@param NumLoops                 number of paralell loops

@returns rf loss resistance for skin effect and proximity effect

*******************************************************************************/

double CalcProximity(
    double RfResistanceSkinEffect,
    double LoopCircumference,
    double NumLoops);

/*******************************************************************************
@brief Fuction to calculate loop Efficiency

@param RadiationResistance  radiation resistance
@param RfResistanceLoss2    combined rf loss resistance for skin effect and
                            proximity effect

@returns loop Efficiency

*******************************************************************************/

double CalcEfficiency(
    double RadiationResistance,
    double RfResistanceLoss2);

/*******************************************************************************
@brief Fuction to calculate loop Inductance

@param LoopRadius                       radius of the loop
@param RelativePermeabilityConductor    relative permeability of the conductor
@param LoopConductorRadius              radius of the loop conductor
@param NumLoops                         number of paralell loops
@param K                                coefficient of coupling

@returns loop inductance

*******************************************************************************/

double CalcLoopInductance(
    double LoopRadius,
    double RelativePermeabilityConductor,
    double LoopConductorRadius,
    double NumLoops/*,
    double K*/);

/*******************************************************************************
@brief Fuction to calculate square loop Inductance

@param LoopDiamater                     diameter of the loop
@param RelativePermeabilityConductor    relative permeability of the conductor
@param LoopConductorRadius              radius of the loop conductor
@param NumLoops                         number of paralell loops
@param K                                coefficient of coupling


@returns loop inductance

*******************************************************************************/

double CalcSquareLoopInductance(
    double LoopDiamater,
    double RelativePermeabilityConductor,
    double LoopConductorRadius,
    double NumLoops/*,
    double K*/);

/*******************************************************************************
@brief Fuction to calculate pickup loop area

@param LoopArea             area of the loop
@param LoopSeriesImpedance  loop series impedance
@param Zin                  series input impedance

@returns pickup loop area

*******************************************************************************/

double CalcPickupLoopArea(
    double LoopArea,
    double LoopSeriesImpedance,
    double Zin);

///// for referance untill the design is over

/*
int isSquare;

// operating frequency
double Frequency;

// dynamic resistance of the LC circuit
double LcDynamicResistance;

// transmitter power
double TxPower;

// voltage on the capacitor at 100 watts
double VMAX;

// wavelength
double Wavelength;

// Q of the antenna
double Q;

// 2:1 SWR bandwidth of the antenna
double SwrBandwidth;

// 6 dB bandwidth of the antenna
double SixDbBandwidth;

// estimated efficiency
double Efficiency;

// resistivity of 6061-T6 aluminum alloy
double Resistivity;

// reciprocal of the resistivity
// conductivity
double Conductivity;

// resistivity of skin effect
double ResistivitySkinEffect;

// rf loss resistance for skin effect
double RfResistanceSkinEffect;

// combined rf loss resistance for 
// skin effect and proximity effect
double RfResistanceLoss2;

// relative permeability of the conductor
double RelativePermeabilityConductor;

// diameter of the loop
double LoopDiamater;

// radius of the loop
double LoopRadius;

// circumference of the loop
double LoopCircumference;

// area of the loop
double LoopArea;

// width of the loop
double LoopWidth;

// height of the loop
double LoopHeight;

// diameter of the loop conductor
double LoopConductorDiameter;

// cross-sectional area of the loop conductor
double a;

// radius of the loop conductor
double LoopConductorRadius;

// circumference of the loop conductor
double LoopConductorCircumference;

// series input impedance
double Zin;

// rf resistance loss
double RfResistanceLoss;

// inductance of a loop
double LoopInductance;

// conductor inductance
double Lc;

// inductive reactance of a loop
double Xl;

// Capacitance 
double C;

// capacative reactance
double Xc;

// radiation resistance
double RadiationResistance;

// loop series impedance
double LoopSeriesImpedance;

// area of pickup loop
double PickupLoopArea;

// number of paralell loops
double NumLoops;

// coefficient of coupling
double K;


// low frequency limit
double FrequencyLowLimit;

// high frequency limit
double FrequencyHighLimit;

// frequency step
double FrequencyStep;
*/
#endif

