#ifndef MEMR2
#define MEMR2
#include "ngspice/ifsim.h"
#include "ngspice/cktdefs.h"
#include "ngspice/gendefs.h"
#include "ngspice/complex.h"
#include "ngspice/noisedef.h"
#include "ngspice/macros.h"
#include <math.h>

#define BOLTZMANN      1.38062259e-23  


#define _USE_NG_TRNOISE_
//#define _USE_VSRC_NOISE_


//enum
//{
//    MEMR2THNOIZ = 0, /* Thermal noise source */
//    MEMR2FLNOIZ,     /* Flicker noise source */
//    MEMR2TOTNOIZ,    /* Total noise          */
//    /* finally, the number of noise sources */
//    MEMR2NSRCS
//};

typedef struct sMEMR2instance
{

    struct GENinstance gen;

#define MEMR2modPtr(inst) ((struct sMEMR2model *)((inst)->gen.GENmodPtr))
#define MEMR2nextInstance(inst) ((struct sMEMR2instance *)((inst)->gen.GENnextInstance))
#define MEMR2name gen.GENname
#define MEMR2state gen.GENstate

    const int MEMR2posNode; /* number of positive node of memristor */
    const int MEMR2negNode; /* number of negative node of memristor */
    int MEMR2Ibranch;       /* equation number of i current branch equation added for memristor */
    int MEMR2Wbranch;       /*equation number of w width branch equation added for memristor */

    double MEMR2winit;
    double MEMR2rinit;
 
    unsigned MEMR2winitGiven : 1; /* flag to indicate initial width was specified */
    unsigned MEMR2rinitGiven : 1; /* flag to indicate initial resistance was specified */

    int MEMR2doNoiseTran;
    unsigned MEMR2noiseTranGiven : 1;
    struct trnoise_state *MEMR2trnoise_state; 

    double MEMR2conduct; /* conductance at current analysis temperature */
    double MEMR2resist;  /* resistance at temperature Tnom */
    double MEMR2current; /* The dc current in the memsistor */

    double MEMR2_dIdV;
    double MEMR2_dIdW; /*  partial derivative of dI/dw */
    double MEMR2_dFdI; /*  partial derivative of dF/di */
    double MEMR2_dFdV;
    double MEMR2_dFdW; /*  partial derivative of dF/dw */

    double MEMR2capacPseudo; /*mem psudo capitance equals the reciprocal of the model constant coeff  uRon/D */

    double *MEMR2posIbrPtr; /* pointer to sparse matrix diagonal at (positive node, added current i branch) */

    double *MEMR2negIbrPtr; /* pointer to sparse matrix diagonal at (negative, added current i branch) */

    double *MEMR2ibrPosPtr; /* pointer to sparse matrix offdiagonal at (added current i branch,positive) */

    double *MEMR2ibrNegPtr; /* pointer to sparse matrix offdiagonal at (added current i branch,negative) */

    double *MEMR2ibrIbrPtr; /* pointer to sparse matrix offdiagonal at (added current i branch,added current i branch) */

    double *MEMR2ibrWbrPtr; /*pointer to sparse matrix offdiagonal at (added current i branch,added w row) */

    double *MEMR2wbrIbrPtr; /* pointer to sparse matrix offdiagonal at (added w row,added current i branch) */

    double *MEMR2wbrWbrPtr; /* pointer to sparse matrix offdiagonal at (added w row, added w row */

    double *MEMR2wbrPosPtr; /* pointer to sparse matrix offdiagonal at (added current w branch,positive) */

    double *MEMR2wbrNegPtr; /* pointer to sparse matrix offdiagonal at (added current w branch,negative) */

	// tran noise voltage source pointer//
	const int MEMR2noiseVsrcPosNode;
	const int MEMR2noiseVsrcNegNode;
	int MEMR2noiseVsrcIbranch;

    double *MEMR2noiseVsrcPosIbrPtr;  /* pointer to sparse matrix element at * (positive node, branch equation) */
    double *MEMR2noiseVsrcNegIbrPtr;  /* pointer to sparse matrix element at * (negative node, branch equation) */
    double *MEMR2noiseVsrcIbrPosPtr;  /* pointer to sparse matrix element at * (branch equation, positive node) */
    double *MEMR2noiseVsrcIbrNegPtr;  /* pointer to sparse matrix element at * (branch equation, negative node) */
    //double *MEMR2noiseVsrcIbrIbrPtr;  /* pointer to sparse matrix element at * (branch equation, branch equation) */	

    /* indices to the array of MEMR2 NOISE SOURCES */

#define MEMR2TNOIZ 0        // thermal noise
#define MEMR2SNOIZ 1        // shot noise
#define MEMR2FNOIZ 2        // flicker noise  
#define MEMR2DNOIZ 3        // diffusion noise
#define MEMR2BNOIZ 4     // Burst (random telegraph) noise

//#define MEMR2HRFLNOIZ 2


#define MEMR2TOTNOIZ 5

#define MEMR2NSRCS 6 /* Number of MEMR2 noise sources */

#ifndef NONOISE
    double MEMR2nVar[NSTATVARS][MEMR2NSRCS];
#else  /* NONOISE */
    double **MEMR2nVar;
#endif /* NONOISE */

    int Memr2IsrcFcnOrder;  /* order of the function for the source */
    double *Memr2IsrcCoeffs; /* pointer to array of coefficients */

} MEMR2instance;

#define MEMR2qcapPseudo MEMR2state       /* pseudo charge on the w branch of  memristor */
#define MEMR2ccapPseudo MEMR2state + 1   /* pseudo current through the w branch of  memristor */
#define MEMR2currentState MEMR2state + 2 /* Memristor current state used for convergence test */
#define MEMR2voltageState MEMR2state + 3 /* Memristor voltage state used for convergence test */
#define MEMR2conductState MEMR2state + 4 /* Memristor conduct state used for convergence test */
#define MEMR2numStates 5

/* per model data */

typedef struct sMEMR2model
{ /* model structure for a memristor */

    struct GENmodel gen;

#define MEMR2modType gen.GENmodType
#define MEMR2nextModel(inst) ((struct sMEMR2model *)((inst)->gen.GENnextModel))
#define MEMR2instances(inst) ((MEMR2instance *)((inst)->gen.GENinstances))
#define MEMR2modName gen.GENmodName

    double MEMR2tnom;            /* temperature at which memristance measured */
    unsigned MEMR2tnomGiven : 1; /* flag to indicate nominal temp. was given */
    int MEMR2type;
    unsigned MEMR2typeGiven : 1;

    double MEMR2defConstK;

    /*model parameterAlpha_on*/
    double MEMR2defAlpha_on;
    unsigned MEMR2defAlpha_onGiven : 1;

    /*model parameterAlpha_off*/
    double MEMR2defAlpha_off;
    unsigned MEMR2defAlpha_offGiven : 1;

    /*model parameterK_on*/
    double MEMR2defK_on;
    unsigned MEMR2defK_onGiven : 1;

    /*model parameterK_off*/
    double MEMR2defK_off;
    unsigned MEMR2defK_offGiven : 1;

    /*model parameterVt_on*/
    double MEMR2defVt_on;
    unsigned MEMR2defVt_onGiven : 1;

    /*model parameterVt_off*/
    double MEMR2defVt_off;
    unsigned MEMR2defVt_offGiven : 1;

    /*model parameterR_on*/
    double MEMR2defR_on;
    unsigned MEMR2defR_onGiven : 1;

    /*model parameterR_off*/
    double MEMR2defR_off;
    unsigned MEMR2defR_offGiven : 1;

    /*model parameterD*/
    double MEMR2defD;
    unsigned MEMR2defDGiven : 1;

    /*model parameterWf*/
    double MEMR2defWf;
    unsigned MEMR2defWfGiven : 1;

    /*model parameterP*/
    double MEMR2defP;
    unsigned MEMR2defPGiven : 1;

    /* Flags */
	bool MEMR2hasNoiseMod;
	
    int MEMR2tnoiMod;
    unsigned MEMR2tnoiModGiven : 1;

    int MEMR2fnoiMod;
    unsigned MEMR2fnoiModGiven : 1;

    int MEMR2snoiMod;
    unsigned MEMR2snoiModGiven : 1;

    int MEMR2bnoiMod;
    unsigned MEMR2bnoiModGiven : 1;

    int MEMR2dnoiMod;
    unsigned MEMR2dnoiModGiven : 1;

    double MEMR2af;
    unsigned MEMR2afGiven : 1;

    double MEMR2ef;
    unsigned MEMR2efGiven : 1;

    double MEMR2efh;
    unsigned MEMR2efhGiven : 1;


    double MEMR2kf;
    unsigned MEMR2kfGiven : 1;

	double MEMR2freqBw;
	unsigned MEMR2freqBwGiven : 1;

	double MEMR2noiseTimeStep;
	unsigned MEMR2noiseTimeStepGiven : 1;

	double MEMR2RTNamplitude;
	unsigned MEMR2RTNamplitudeGiven : 1;

	double MEMR2RTNtimeCapt;
	unsigned MEMR2RTNtimeCaptGiven : 1;

	double MEMR2RTNtimeEmit;
	unsigned MEMR2RTNtimeEmitGiven : 1;	

} MEMR2model;

enum
{
    MEMR2_RINIT,
    MEMR2_WINIT,
    MEMR2_WFLUXPSEUDO,
    MEMR2_WVOLTPSEUDO,
    MEMR2_TRNOISE
};

enum
{
    MEMR2_MOD_Alpha_on,
    MEMR2_MOD_Alpha_off,
    MEMR2_MOD_K_on,
    MEMR2_MOD_K_off,
    MEMR2_MOD_Vt_on,
    MEMR2_MOD_Vt_off,
    MEMR2_MOD_R_on,
    MEMR2_MOD_R_off,
    MEMR2_MOD_D,
    MEMR2_MOD_Wf,
    MEMR2_MOD_P,
    MEMR2_MOD_MEMR,
    MEMR2_MOD_WF,
    MEMR2_MOD_TNOM,
    MEMR2_MOD_TNOIMOD,
    MEMR2_MOD_FNOIMOD,
    MEMR2_MOD_SNOIMOD,
    MEMR2_MOD_BNOIMOD,
    MEMR2_MOD_DNOIMOD,
    MEMR2_MOD_AF,
    MEMR2_MOD_EF,
    MEMR2_MOD_EFH,    
    MEMR2_MOD_KF,  
    MEMR2_MOD_FreqBW,
 	MEMR2_MOD_NoiseTimeStep,
 	MEMR2_MOD_RTN_AMP,
 	MEMR2_MOD_RTN_TCAPT,
 	MEMR2_MOD_RTN_TEMIT
};

#include "memr2ext.h"

extern void MEMR2update_conduct(MEMR2instance *, bool spill_warnings);

#endif /*MEM2*/
