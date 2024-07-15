/**********
Author: Bo Li

20191019  implement new model----this model is proposed in  Prof. Lu Wei group in Univ. of  Michigan
		  recently this model is employed in the paper published in Nature Electronics(2019, Vol7) 
		   the device and chip is manufactured 
adding memristor model and instance, modification is refered to RES resdefs.h 
20190609
**********/

/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/


#ifndef MEMC1
#define MEMC1

#include "ngspice/ifsim.h"
#include "ngspice/cktdefs.h"
#include "ngspice/gendefs.h"
#include "ngspice/complex.h"
#include "ngspice/noisedef.h"

/* definitions used to describe resistors */

/* indices to array of RES noise sources */

enum {
    MEMC1THNOIZ = 0,  /* Thermal noise source */
    MEMC1FLNOIZ,      /* Flicker noise source */
    MEMC1TOTNOIZ,     /* Total noise          */
    /* finally, the number of noise sources */
    MEMC1NSRCS
};

/* information used to describe a single instance */

typedef struct sMEMC1instance {

    struct GENinstance gen;

#define MEMC1modPtr(inst) ((struct sMEMC1model *)((inst)->gen.GENmodPtr))
#define MEMC1nextInstance(inst) ((struct sMEMC1instance *)((inst)->gen.GENnextInstance))
#define MEMC1name gen.GENname
#define MEMC1state gen.GENstate

    const int MEMC1posNode;     /* number of positive node of memristor */
    const int MEMC1negNode;     /* number of negative node of memristor */
	int MEMC1Ibranch;  	/* equation number of i current branch equation added for memristor */
	int MEMC1Wbranch;		/*equation number of w width branch equation added for memristor */    

    double MEMC1winit;
    double MEMC1rinit;
	double MEMC1initCond; 
	unsigned MEMC1icGiven			:1; /* flag to indicate initial voltage was specified */ 	

	unsigned MEMC1winitGiven      : 1;    /* flag to indicate initial width was specified */   
    unsigned MEMC1rinitGiven      : 1;    /* flag to indicate initial resistance was specified */

	double MEMC1_dIdV;
	double MEMC1_dIdW;		/*  partial derivative of dI/dw */

	double MEMC1_dFdI;  		/*  partial derivative of dF/di */ 
	double MEMC1_dFdV;
	double MEMC1_dFdW; 		/*  partial derivative of dF/dw */ 
	
	double MEMC1capac;
	double MEMC1capacPseudo;  	   

    double *MEMC1posIbrPtr;           /* pointer to sparse matrix diagonal at
 									 * (positive node, added current i branch) */
    double *MEMC1negIbrPtr;           /* pointer to sparse matrix diagonal at
                                     * (negative, added current i branch) */
    double *MEMC1ibrPosPtr;           /* pointer to sparse matrix offdiagonal at
                                     * (added current i branch,positive) */
    double *MEMC1ibrNegPtr;           /* pointer to sparse matrix offdiagonal at
                                     * (added current i branch,negative) */
    double *MEMC1ibrIbrPtr;           /* pointer to sparse matrix offdiagonal at
                                     * (added current i branch,added current i branch) */
	double *MEMC1ibrWbrPtr;			/*pointer to sparse matrix offdiagonal at
									 *(added current i branch,added w row) */

    double *MEMC1wbrIbrPtr;           /* pointer to sparse matrix offdiagonal at
                                     * (added w row,added current i branch) */
    double *MEMC1wbrWbrPtr;           /* pointer to sparse matrix offdiagonal at
                                     * (added w row, added w row */

	double *MEMC1wbrPosPtr; 		  /* pointer to sparse matrix offdiagonal at
									  * (added current w branch,positive) */
	double *MEMC1wbrNegPtr; 		  /* pointer to sparse matrix offdiagonal at
									  * (added current w branch,negative) */


#ifndef NONOISE
    double MEMC1nVar[NSTATVARS][MEMC1NSRCS];
#else /* NONOISE */
    double **MEMC1nVar;
#endif /* NONOISE */

} MEMC1instance ;


//#define MEMwFluxPseudo MEMstate    /* pseudo charge on the w branch of  memristor */
//#define MEMwbrCurPseudo MEMstate+1  /* pseudo current through the w branch of memristor */
#define MEMC1qcapPseudo MEMC1state 			/* pseudo charge on the w branch of  memristor */
#define MEMC1ccapPseudo MEMC1state+1		/* pseudo current through the w branch of  memristor */
#define MEMC1currentState MEMC1state+2		/* Memristor current state used for convergence test */
#define MEMC1voltageState MEMC1state+3		/* Memristor voltage state used for convergence test */
#define MEMC1conductState MEMC1state+4		/* Memristor conduct state used for convergence test */
#define MEMC1qcap MEMC1state+5
#define MEMC1ccap MEMC1state+6
#define MEMC1numStates 7

/* per model data */

typedef struct sMEMC1model {       /* model structure for a resistor */

    struct GENmodel gen;

#define MEMC1modType gen.GENmodType
#define MEMC1nextModel(inst) ((struct sMEMC1model *)((inst)->gen.GENnextModel))
#define MEMC1instances(inst) ((MEMC1instance *)((inst)->gen.GENinstances))
#define MEMC1modName gen.GENmodName

    double MEMC1tnom;         /* temperature at which memristance measured */
	unsigned MEMC1tnomGiven:1;

 
	int MEMC1type; 
	unsigned MEMC1typeGiven 	:1; 
	
	double MEMC1defConstK; 
	
	/*model parameterC_min*/ 
	double MEMC1defC_min; 
	unsigned MEMC1defC_minGiven :1; 
	
	/*model parameterC_max*/ 
	double MEMC1defC_max; 
	unsigned MEMC1defC_maxGiven :1; 
	
	/*model parameterK*/ 
	double MEMC1defK; 
	unsigned MEMC1defKGiven :1; 
	
	/*model parameterD*/ 
	double MEMC1defD; 
	unsigned MEMC1defDGiven :1; 
	
	/*model parameterWf*/ 
	double MEMC1defWf; 
	unsigned MEMC1defWfGiven	:1; 
	
	/*model parameterP*/ 
	double MEMC1defP; 
	unsigned MEMC1defPGiven :1; 

} MEMC1model;

/* device parameters */
enum {

    MEMC1_RINIT,
    MEMC1_WINIT,
    MEMC1_IC,

    MEMC1_WFLUXPSEUDO,
    MEMC1_WVOLTPSEUDO,
};



/* model parameters */
enum {
	MEMC1_MOD_C_min, 
	MEMC1_MOD_C_max, 
	MEMC1_MOD_K, 
	MEMC1_MOD_D, 
	MEMC1_MOD_Wf, 
	MEMC1_MOD_P, 
	MEMC1_MOD_MEMC, 
	MEMC1_MOD_WF, 
	MEMC1_MOD_TNOM, 

};


/* model questions */

#include "memc1ext.h"

extern void MEMC1update_conduct(MEMC1instance *, bool spill_warnings);

#endif /*MEM*/
