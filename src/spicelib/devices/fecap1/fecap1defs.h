/**********
Copyright 2024 Xidian University.	All rights reserved.
 Author: Bo Li	at Hangzhou Institute of Technology	 
 Modified: 2020/09/09  Bo Li
 Refered to NgSPICE Res/Cap related file
**********/

#ifndef FECAP1
#define FECAP1


#include "ngspice/ifsim.h"
#include "ngspice/complex.h"
#include "ngspice/gendefs.h"
#include "ngspice/cktdefs.h"

/* structures used to describe capacitors */


/* information to describe each instance */

typedef struct sFECAP1instance {

    struct GENinstance gen;

#define FECAP1modPtr(inst) ((struct sFECAP1model *)((inst)->gen.GENmodPtr))
#define FECAP1nextInstance(inst) ((struct sFECAP1instance *)((inst)->gen.GENnextInstance))
#define FECAP1name gen.GENname
#define FECAP1state gen.GENstate

    const int FECAP1posNode; /* number of positive node of FECAP1acitor */
    const int FECAP1negNode; /* number of negative node of FECAP1acitor */

    double FECAP1temp;     /* temperature at which this FECAP1acitor operates */
    double FECAP1dtemp;    /* delta-temperature of this instance */
	
    double FECAP1capac;    /* capacitance */
	double FECAP1capacPseudo; 
	
    double FECAP1initCondV; /* initial capacitor  voltage if specified */
    double FECAP1initCondP; /* initial capacitor P if specified */

    //double FECAP1scale;    /* scale factor */
    double FECAP1m;        /* parallel multiplier */

	double FECAP1_Area;
	
	
    //double FECAP1tc1;      /* first temperature coefficient of capacitors */
    //double FECAP1tc2;      /* second temperature coefficient of capacitors */
    //double FECAP1bv_max;   /* Maximum capacitor voltage */

	int FECAP1Ibranch;
	int FECAP1Pbranch;


	double FECAP1_dF1dP;

	double FECAP1_dF2dV;

	double FECAP1_dF2dP;
	

    double *FECAP1posPosPtr;    /* pointer to sparse matrix diagonal at  * (positive,positive) */

    double *FECAP1posNegPtr;    /* pointer to sparse matrix offdiagonal at  * (positive,negative) */
							  	
    double *FECAP1posIbrPtr;    /* pointer to sparse matrix offdiagonal at  * (positive,negative) */


    double *FECAP1negPosPtr;    /* pointer to sparse matrix offdiagonal at  * (negative,positive) */

    double *FECAP1negNegPtr;    /* pointer to sparse matrix diagonal at  * (negative,negative) */

    double *FECAP1negIbrPtr;    /* pointer to sparse matrix offdiagonal at  * (negative,positive) */


    double *FECAP1ibrPosPtr; /* pointer to sparse matrix offdiagonal at (added current i branch,added current i branch) */

    double *FECAP1ibrNegPtr; /*pointer to sparse matrix offdiagonal at (added current i branch,added w row) */

    double *FECAP1ibrIbrPtr; /* pointer to sparse matrix offdiagonal at (added current i branch,added current i branch) */

    double *FECAP1ibrPbrPtr; /*pointer to sparse matrix offdiagonal at (added current i branch,added w row) */


	double *FECAP1pbrPosPtr; /* pointer to sparse matrix offdiagonal at (added current w branch,positive) */

    double *FECAP1pbrNegPtr; /* pointer to sparse matrix offdiagonal at (added current w branch,negative) */

    double *FECAP1pbrIbrPtr; /* pointer to sparse matrix offdiagonal at (added p row,added current i branch) */

    double *FECAP1pbrPbrPtr; /* pointer to sparse matrix offdiagonal at (added p row, added w row */


							  
    unsigned FECAP1_AreaGiven    : 1;   /* flag to indicate area was specified */
    unsigned FECAP1_icGiven     : 1;   /* flag to indicate init. cond. was specified */
    //unsigned CAPwidthGiven  : 1;   /* flag to indicate capacitor width given */
    //unsigned CAPlengthGiven : 1;   /* flag to indicate capacitor length given*/
    unsigned FECAP1tempGiven   : 1;   /* flag to indicate operating temp given */
    // unsigned CAPdtempGiven  : 1;   /* flag to indicate delta temp given */
    // unsigned CAPscaleGiven  : 1;   /* flag to indicate scale factor given */
    unsigned FECAP1mGiven      : 1;   /* flag to indicate parallel multiplier given */
    //unsigned CAPtc1Given    : 1;    /* flag indicates tc1 was specified */
    //unsigned CAPtc2Given    : 1;    /* flag indicates tc2 was specified */
    //unsigned CAPbv_maxGiven : 1;    /* flags indicates maximum voltage is given */
    //int    CAPsenParmNo;         /* parameter # for sensitivity use;set equal to  0 if not a design parameter*/

				

} FECAP1instance;

#define FECAP1qcap FECAP1state    /* charge on the capacitor */
#define FECAP1ccap FECAP1state+1  /* current through the capacitor */

#define FECAP1pcap FECAP1state+2
#define FECAP1pccap FECAP1state+3

#define FECAP1numStates 4

//#define CAPsensxp CAPstate+2 
/* charge sensitivities and their derivatives.
                              * +3 for the derivatives - pointer to the
                              * beginning of the array */

//#define CAPnumSenStates 2


/* data per model */

typedef struct sFECAP1model {      /* model structure for a capacitor */

    struct GENmodel gen;

#define FECAP1modType gen.GENmodType
#define FECAP1nextModel(inst) ((struct sFECAP1model *)((inst)->gen.GENnextModel))
#define FECAP1instances(inst) ((FECAP1instance *)((inst)->gen.GENinstances))
#define FECAP1modName gen.GENmodName

    double FECAP1_Tnom;       /* temperature at which capacitance measured */
    //double CAPtempCoeff1; /* linear temperature coefficient */
    //double CAPtempCoeff2; /* quadratic temperature coefficient */
    //double CAPmCap;       /* Model default capacitance */
    //double CAPcj;         /* Unit Area Capacitance ( F/ M**2 ) */
   //double CAPcjsw;       /* Unit Length Sidewall Capacitance ( F / M ) */
    //double CAPdefWidth;   /* the default width of a capacitor */
    //double CAPdefLength;  /* the default length of a capacitor */
    //double CAPnarrow;     /* amount by which width are less than drawn */
    //double CAPshort;      /* amount by which length are less than drawn */
    //double CAPdel;        /* amount by which length and width are less than drawn */
    double FECAP1_Alpha;         /* Relative dielectric constant */
	double FECAP1_Beta;
	double FECAP1_Gamma;
	double FECAP1_Rho;
	double FECAP1_Thickness;      /* Insulator thickness */
	double FECAP1_Ec;
	double FECAP1_Ps;

    //double CAPbv_max;     /* Maximum capacitor voltage */
    unsigned FECAP1_AlphaGiven      : 1;    /* flag indicates default capacitance given */
    unsigned FECAP1_BetaGiven        : 1;    /* Unit Area Capacitance ( F/ M**2 ) */
    unsigned FECAP1_GammaGiven      : 1;    /* Unit Length Sidewall Capacitance( F/M )*/
    unsigned FECAP1_RhoGiven  : 1;    /* flag indicates default width given*/
    unsigned FECAP1_ThicknessGiven : 1;    /* flag indicates deafult lenght given */
    unsigned FECAP1_EcGiven  : 1;  
    unsigned FECAP1_PsGiven  : 1;  
    unsigned FECAP1_TnomGiven :1; 
        
        
    int FECAP1type;
    unsigned FECAP1typeGiven : 1;


} FECAP1model;

/* device parameters */
enum {
    FECAP1_AREA,
    FECAP1_IC,
    FECAP1_POWER,
    FECAP1_TEMP,
    FECAP1_M,

};

/* model parameters */
enum {
    FECAP1_MOD_FECAP,
    FECAP1_MOD_Thickness,
    FECAP1_MOD_Alpha,
    FECAP1_MOD_Beta,
    FECAP1_MOD_Gamma,
    FECAP1_MOD_Rho,
    FECAP1_MOD_TNOM,
    FECAP1_MOD_Ec,
    FECAP1_MOD_Ps,    
};

/* device questions */
/*
enum {
    CAP_QUEST_SENS_REAL = 201,
    CAP_QUEST_SENS_IMAG,
    CAP_QUEST_SENS_MAG,
    CAP_QUEST_SENS_PH,
    CAP_QUEST_SENS_CPLX,
    CAP_QUEST_SENS_DC,
};
*/
#include "fecap1ext.h"

#endif /*CAP*/
