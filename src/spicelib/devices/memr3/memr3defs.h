/**********
Author: Bo Li

20191019  implement new model----this model is proposed in  Prof. C. Yakopcic group in Univ. of  Dayton
		  recently this model is employed in the paper published in IEEE TCAD(2020) 
		   the device and chip is manufactured 
adding memristor model and instance, modification is refered to RES resdefs.h 
20200520

**********/
/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/05/20  Bo Li
Refered to NgSPICE Res/Cap related file
**********/
#ifndef MEMR3 
#define MEMR3 

#include "ngspice/ifsim.h" 
#include "ngspice/cktdefs.h" 
#include "ngspice/gendefs.h" 
#include "ngspice/complex.h" 
#include "ngspice/noisedef.h" 

enum { 
	MEMR3THNOIZ = 0,  /* Thermal noise source */ 
	MEMR3FLNOIZ,      /* Flicker noise source */ 
	MEMR3TOTNOIZ,     /* Total noise          */ 
	/* finally, the number of noise sources */ 
	MEMR3NSRCS 
}; 

typedef struct sMEMR3instance { 

	struct GENinstance gen; 

#define MEMR3modPtr(inst) ((struct sMEMR3model *)((inst)->gen.GENmodPtr)) 
#define MEMR3nextInstance(inst) ((struct sMEMR3instance *)((inst)->gen.GENnextInstance)) 
#define MEMR3name gen.GENname 
#define MEMR3state gen.GENstate 

	const int MEMR3posNode;     /* number of positive node of memristor */ 
	const int MEMR3negNode;     /* number of negative node of memristor */ 
	int MEMR3Ibranch;  	/* equation number of i current branch equation added for memristor */ 
	int MEMR3Wbranch;	/*equation number of w width branch equation added for memristor */ 

		double MEMR3winit; 
	double MEMR3rinit; 
	unsigned MEMR3winitGiven      : 1;    /* flag to indicate initial width was specified */ 
	unsigned MEMR3rinitGiven      : 1;    /* flag to indicate initial resistance was specified */ 

	double MEMR3conduct;		/* conductance at current analysis temperature */ 
	double MEMR3resist;		/* resistance at temperature Tnom */ 
	double MEMR3current;		/* The dc current in the memsistor */ 

	double MEMR3_dIdV; 
	double MEMR3_dIdW;		/*  partial derivative of dI/dw */ 
 	double MEMR3_dFdI;  	/*  partial derivative of dF/di */  
	double MEMR3_dFdV; 
	double MEMR3_dFdW;		/*  partial derivative of dF/dw */ 

	double MEMR3capacPseudo; 			/*mem psudo capitance equals the reciprocal of the model constant coeff  uRon/D */ 

	double *MEMR3posIbrPtr;           /* pointer to sparse matrix diagonal at (positive node, added current i branch) */ 

	double *MEMR3negIbrPtr;           /* pointer to sparse matrix diagonal at (negative, added current i branch) */ 

	double *MEMR3ibrPosPtr;           /* pointer to sparse matrix offdiagonal at (added current i branch,positive) */ 

	double *MEMR3ibrNegPtr;           /* pointer to sparse matrix offdiagonal at (added current i branch,negative) */ 

	double *MEMR3ibrIbrPtr;           /* pointer to sparse matrix offdiagonal at (added current i branch,added current i branch) */ 

	double *MEMR3ibrWbrPtr;			/*pointer to sparse matrix offdiagonal at (added current i branch,added w row) */ 

	double *MEMR3wbrIbrPtr;           /* pointer to sparse matrix offdiagonal at (added w row,added current i branch) */ 

	double *MEMR3wbrWbrPtr;           /* pointer to sparse matrix offdiagonal at (added w row, added w row */ 

	double *MEMR3wbrPosPtr; 		  /* pointer to sparse matrix offdiagonal at (added current w branch,positive) */ 

	double *MEMR3wbrNegPtr; 		  /* pointer to sparse matrix offdiagonal at (added current w branch,negative) */ 
#ifndef NONOISE 
	double MEMR3nVar[NSTATVARS][MEMR3NSRCS]; 
#else /* NONOISE */ 
	double **MEMR3nVar; 
#endif /* NONOISE */ 
} MEMR3instance ; 

#define MEMR3qcapPseudo MEMR3state 			/* pseudo charge on the w branch of  memristor */ 
#define MEMR3ccapPseudo MEMR3state+1		/* pseudo current through the w branch of  memristor */ 
#define MEMR3currentState MEMR3state+2		/* Memristor current state used for convergence test */ 
#define MEMR3voltageState MEMR3state+3		/* Memristor voltage state used for convergence test */ 
#define MEMR3conductState MEMR3state+4		/* Memristor conduct state used for convergence test */ 
#define MEMR3numStates 5 

/* per model data */ 

typedef struct sMEMR3model {       /* model structure for a memristor */ 

	struct GENmodel gen; 

#define MEMR3modType gen.GENmodType 
#define MEMR3nextModel(inst) ((struct sMEMR3model *)((inst)->gen.GENnextModel)) 
#define MEMR3instances(inst) ((MEMR3instance *)((inst)->gen.GENinstances)) 
#define MEMR3modName gen.GENmodName 

double MEMR3tnom;         /* temperature at which memristance measured */ 
unsigned MEMR3tnomGiven		:1; /* flag to indicate nominal temp. was given */ 
int MEMR3type; 
unsigned MEMR3typeGiven		:1; 

double MEMR3defConstK; 

/*model parameterVthp*/ 
double MEMR3defVthp; 
unsigned MEMR3defVthpGiven	:1; 

/*model parameterVthn*/ 
double MEMR3defVthn; 
unsigned MEMR3defVthnGiven	:1; 

/*model parameterGmax*/ 
double MEMR3defGmax; 
unsigned MEMR3defGmaxGiven	:1; 

/*model parameterGmin*/ 
double MEMR3defGmin; 
unsigned MEMR3defGminGiven	:1; 

/*model parameterAp*/ 
double MEMR3defAp; 
unsigned MEMR3defApGiven	:1; 

/*model parameterAn*/ 
double MEMR3defAn; 
unsigned MEMR3defAnGiven	:1; 

/*model parameterXp*/ 
double MEMR3defXp; 
unsigned MEMR3defXpGiven	:1; 

/*model parameterXn*/ 
double MEMR3defXn; 
unsigned MEMR3defXnGiven	:1; 

/*model parameterb*/ 
double MEMR3defb; 
unsigned MEMR3defbGiven	:1; 

}MEMR3model; 

enum { 
	MEMR3_RINIT, 
	MEMR3_WINIT, 
	MEMR3_WFLUXPSEUDO, 
	MEMR3_WVOLTPSEUDO, 
}; 
 
enum { 
	MEMR3_MOD_Vthp, 
	MEMR3_MOD_Vthn, 
	MEMR3_MOD_Gmax, 
	MEMR3_MOD_Gmin, 
	MEMR3_MOD_Ap, 
	MEMR3_MOD_An, 
	MEMR3_MOD_Xp, 
	MEMR3_MOD_Xn, 
	MEMR3_MOD_b, 
	MEMR3_MOD_MEM, 
	MEMR3_MOD_WF, 
	MEMR3_MOD_TNOM, 
}; 

#include "memr3ext.h" 

extern void MEMR3update_conduct(MEMR3instance *, bool spill_warnings); 

#endif /*MEMR3*/ 
