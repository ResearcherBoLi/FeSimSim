
/**********
Author: Bo Li
Item: adding memristor model and instance, modification is refered to RES resdefs.h 
Date: 20190609
**********/

/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/


#ifndef MEMR1
#define MEMR1

#include "ngspice/ifsim.h"
#include "ngspice/cktdefs.h"
#include "ngspice/gendefs.h"
#include "ngspice/complex.h"
#include "ngspice/noisedef.h"
#include "ngspice/macros.h"
#include <math.h>

#define BOLTZMANN      1.38062259e-23  
#define _USE_NG_TRNOISE_
/* definitions used to describe resistors */

/* indices to array of RES noise sources */

//enum {
//    MEMR1THNOIZ = 0,  /* Thermal noise source */
//    MEMR1FLNOIZ,      /* Flicker noise source */
//    MEMR1TOTNOIZ,     /* Total noise          */
    /* finally, the number of noise sources */
//    MEMR1NSRCS
//};

/* information used to describe a single instance */

typedef struct sMEMR1instance {

    struct GENinstance gen;

#define MEMR1modPtr(inst) ((struct sMEMR1model *)((inst)->gen.GENmodPtr))
#define MEMR1nextInstance(inst) ((struct sMEMR1instance *)((inst)->gen.GENnextInstance))
#define MEMR1name gen.GENname
#define MEMR1state gen.GENstate

    const int MEMR1posNode;     /* number of positive node of memristor */
    const int MEMR1negNode;     /* number of negative node of memristor */
	int MEMR1Ibranch;  	/* equation number of i current branch equation added for memristor */
	int MEMR1Wbranch;		/*equation number of w width branch equation added for memristor */    
    //double MEMwidth;
    //double MEMdepth;

    double MEMR1winit;
    double MEMR1rinit;
    double MEMR1p;
    double MEMR1wf;

	unsigned MEMR1winitGiven      : 1;    /* flag to indicate initial width was specified */   
    unsigned MEMR1rinitGiven      : 1;    /* flag to indicate initial resistance was specified */
    unsigned MEMR1pGiven        : 1;    /* flag to indicate polynomial idex of window function was specified */
    unsigned MEMR1wfGiven       : 1;    /* flag to indicate window function type was specified */

    int MEMR1doNoiseTran;
    unsigned MEMR1noiseTranGiven : 1;
    struct trnoise_state *MEMR1trnoise_state; 

    // double MEMtemp;     /* temperature at which this memristor operates */
    // double MEMdtemp;    /* delta-temperature of a particular instance  */
    double MEMR1conduct;	  	/* conductance at current analysis temperature */
    double MEMR1resist;   	/* resistance at temperature Tnom */
    double MEMR1current;		/* The dc current in the memsistor */

	double MEMR1_dIdW;			/*  partial derivative of dI/dw */ 
	double MEMR1_dFdI;  		/*  partial derivative of dF/di */ 
	double MEMR1_dFdW; 		/*  partial derivative of dF/dw */ 

	double MEMR1capacPseudo; 			/*mem psudo capitance equals the reciprocal of the model constant coeff  uRon/D * /
 /* serban */

    

    double *MEMR1posIbrPtr;           /* pointer to sparse matrix diagonal at
                                     * (positive node, added current i branch) */
    double *MEMR1negIbrPtr;           /* pointer to sparse matrix diagonal at
                                     * (negative, added current i branch) */
    double *MEMR1ibrPosPtr;           /* pointer to sparse matrix offdiagonal at
                                     * (added current i branch,positive) */
    double *MEMR1ibrNegPtr;           /* pointer to sparse matrix offdiagonal at
                                     * (added current i branch,negative) */
    double *MEMR1ibrIbrPtr;           /* pointer to sparse matrix offdiagonal at
                                     * (added current i branch,added current i branch) */
	double *MEMR1ibrWbrPtr;			/*pointer to sparse matrix offdiagonal at
									 *(added current i branch,added w row) */

    double *MEMR1wbrIbrPtr;           /* pointer to sparse matrix offdiagonal at
                                     * (added w row,added current i branch) */
    double *MEMR1wbrWbrPtr;           /* pointer to sparse matrix offdiagonal at
                                     * (added w row, added w row */


	// tran noise voltage source pointer//
	const int MEMR1noiseVsrcPosNode;
	const int MEMR1noiseVsrcNegNode;
	int MEMR1noiseVsrcIbranch;

    double *MEMR1noiseVsrcPosIbrPtr;  /* pointer to sparse matrix element at * (positive node, branch equation) */
    double *MEMR1noiseVsrcNegIbrPtr;  /* pointer to sparse matrix element at * (negative node, branch equation) */
    double *MEMR1noiseVsrcIbrPosPtr;  /* pointer to sparse matrix element at * (branch equation, positive node) */
    double *MEMR1noiseVsrcIbrNegPtr;  /* pointer to sparse matrix element at * (branch equation, negative node) */
    //double *MEMR1noiseVsrcIbrIbrPtr;  /* pointer to sparse matrix element at * (branch equation, branch equation) */	

    /* indices to the array of MEMR1 NOISE SOURCES */

#define MEMR1TNOIZ 0        // thermal noise
#define MEMR1SNOIZ 1        // shot noise
#define MEMR1FNOIZ 2        // flicker noise  
#define MEMR1DNOIZ 3        // diffusion noise
#define MEMR1BNOIZ 4     // Burst (random telegraph) noise

//#define MEMR1HRFLNOIZ 2


#define MEMR1TOTNOIZ 5

#define MEMR1NSRCS 6 /* Number of MEMR1 noise sources */

#ifndef NONOISE
    double MEMR1nVar[NSTATVARS][MEMR1NSRCS];
#else  /* NONOISE */
    double **MEMR1nVar;
#endif /* NONOISE */

    int MEMR1IsrcFcnOrder;  /* order of the function for the source */
    double *MEMR1IsrcCoeffs; /* pointer to array of coefficients */


} MEMR1instance ;


//#define MEMwFluxPseudo MEMstate    /* pseudo charge on the w branch of  memristor */
//#define MEMwbrCurPseudo MEMstate+1  /* pseudo current through the w branch of memristor */
#define MEMR1qcapPseudo MEMR1state 			/* pseudo charge on the w branch of  memristor */
#define MEMR1ccapPseudo MEMR1state+1		/* pseudo current through the w branch of  memristor */
#define MEMR1currentState MEMR1state+2		/* Memristor current state used for convergence test */
#define MEMR1voltageState MEMR1state+3		/* Memristor voltage state used for convergence test */
#define MEMR1conductState MEMR1state+4		/* Memristor conduct state used for convergence test */
#define MEMR1numStates 5

/* per model data */

typedef struct sMEMR1model {       /* model structure for a resistor */

    struct GENmodel gen;

#define MEMR1modType gen.GENmodType
#define MEMR1nextModel(inst) ((struct sMEMR1model *)((inst)->gen.GENnextModel))
#define MEMR1instances(inst) ((MEMR1instance *)((inst)->gen.GENinstances))
#define MEMR1modName gen.GENmodName

    double MEMR1tnom;         /* temperature at which memristance measured */
    unsigned MEMR1tnomGiven       :1; /* flag to indicate nominal temp. was given */

	int MEMR1type;			/*1, -1 memristor type , used for digital or analog, maybe modified later*/
 	unsigned MEMR1typeGiven	  	:1; 	/* flag to indicate default memristor work mode*/ 

    double MEMR1defDepth;     /* default depth of a memristor */
    unsigned MEMR1defDepthGiven   :1;     /* flag to indicate default depth given */ 
	
    double MEMR1defRon;       /* default Ron of a memristor */
    unsigned MEMR1defRonGiven     :1;     /* flag to indicate default Ron given */
	
    double MEMR1defRoff;      /* default Roff of a memristor */
    unsigned MEMR1defRoffGiven    :1;     /* flag to indicate default Roff given */

	double MEMR1defMu;        /* default mobility u of a memristor */
    unsigned MEMR1defMuGiven      :1;     /* flag to indicate default ion mobility u given */

	double MEMR1defConstK;	/*default constant value of mu*Ron/(D*D) */

	int MEMR1defWf;			/* default window function for a memristor */
	unsigned MEMR1defWfGiven		:1;		/* flag to indicate default memristor window function */

    //unsigned MEMdefWidthGiven   :1;     /* flag to indicate default width given */

    /* Flags */
	bool MEMR1hasNoiseMod;
	
    int MEMR1tnoiMod;
    unsigned MEMR1tnoiModGiven : 1;

    int MEMR1fnoiMod;
    unsigned MEMR1fnoiModGiven : 1;

    int MEMR1snoiMod;
    unsigned MEMR1snoiModGiven : 1;

    int MEMR1bnoiMod;
    unsigned MEMR1bnoiModGiven : 1;

    int MEMR1dnoiMod;
    unsigned MEMR1dnoiModGiven : 1;

    double MEMR1af;
    unsigned MEMR1afGiven : 1;

    double MEMR1ef;
    unsigned MEMR1efGiven : 1;

    double MEMR1efh;
    unsigned MEMR1efhGiven : 1;


    double MEMR1kf;
    unsigned MEMR1kfGiven : 1;

	double MEMR1freqBw;
	unsigned MEMR1freqBwGiven : 1;

	double MEMR1noiseTimeStep;
	unsigned MEMR1noiseTimeStepGiven : 1;

	double MEMR1RTNamplitude;
	unsigned MEMR1RTNamplitudeGiven : 1;

	double MEMR1RTNtimeCapt;
	unsigned MEMR1RTNtimeCaptGiven : 1;

	double MEMR1RTNtimeEmit;
	unsigned MEMR1RTNtimeEmitGiven : 1;	

} MEMR1model;

/* device parameters */
enum {

    MEMR1_RINIT,
    MEMR1_WINIT,
    MEMR1_P,
    MEMR1_WF,
    MEMR1_WFLUXPSEUDO,
    MEMR1_WVOLTPSEUDO,
    MEMR1_TRNOISE
};



/* model parameters */
enum {

    MEMR1_MOD_RON,
    MEMR1_MOD_ROFF,
    MEMR1_MOD_MU,
    //MEM_MOD_DEFWIDTH,
    MEMR1_MOD_DEPTH,
    MEMR1_MOD_MEMR,
    MEMR1_MOD_WF,
    MEMR1_MOD_TNOM,
    // below paras for tran noise
    MEMR1_MOD_TNOIMOD,
    MEMR1_MOD_FNOIMOD,
    MEMR1_MOD_SNOIMOD,
    MEMR1_MOD_BNOIMOD,
    MEMR1_MOD_DNOIMOD,
    MEMR1_MOD_AF,
    MEMR1_MOD_EF,
    MEMR1_MOD_EFH,    
    MEMR1_MOD_KF,  
    MEMR1_MOD_FreqBW,
 	MEMR1_MOD_NoiseTimeStep,
 	MEMR1_MOD_RTN_AMP,
 	MEMR1_MOD_RTN_TCAPT,
 	MEMR1_MOD_RTN_TEMIT   

};


#include "memr1ext.h"

extern void MEMR1update_conduct(MEMR1instance *, bool spill_warnings);

#endif /*MEM*/
