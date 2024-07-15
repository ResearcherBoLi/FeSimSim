#ifndef MEMI1 
#define MEMI1 
#include "ngspice/ifsim.h" 
#include "ngspice/cktdefs.h" 
#include "ngspice/gendefs.h" 
#include "ngspice/complex.h" 
#include "ngspice/noisedef.h" 

enum { 
	MEMI1THNOIZ = 0,  /* Thermal noise source */ 
	MEMI1FLNOIZ,      /* Flicker noise source */ 
	MEMI1TOTNOIZ,     /* Total noise          */ 
	/* finally, the number of noise sources */ 
	MEMI1NSRCS 
}; 

typedef struct sMEMI1instance { 

	struct GENinstance gen; 

#define MEMI1modPtr(inst) ((struct sMEMI1model *)((inst)->gen.GENmodPtr)) 
#define MEMI1nextInstance(inst) ((struct sMEMI1instance *)((inst)->gen.GENnextInstance)) 
#define MEMI1name gen.GENname 
#define MEMI1state gen.GENstate 

	const int MEMI1posNode;     /* number of positive node of memristor */ 
	const int MEMI1negNode;     /* number of negative node of memristor */ 
	int MEMI1Ibranch;  	/* equation number of i current branch equation added for memristor */ 
	int MEMI1Wbranch;	/*equation number of w width branch equation added for memristor */ 

	double MEMI1winit; 
	double MEMI1rinit; 

	double MEMI1initCond; 
	unsigned MEMI1icGiven			:1; /* flag to indicate initial voltage was specified */ 	
     
	unsigned MEMI1winitGiven      : 1;    /* flag to indicate initial width was specified */ 
	unsigned MEMI1rinitGiven      : 1;    /* flag to indicate initial resistance was specified */ 

	double MEMI1current;		/* The dc current in the memsistor */ 

	double MEMI1_dIdV; 
	double MEMI1_dIdW;		/*  partial derivative of dI/dw */ 
 	double MEMI1_dFdI;  	/*  partial derivative of dF/di */  
	double MEMI1_dFdV; 
	double MEMI1_dFdW;		/*  partial derivative of dF/dw */ 

	double MEMI1induc;
    double MEMI1capacPseudo; 			/*mem psudo capitance equals the reciprocal of the model constant coeff  uRon/D */ 

	double *MEMI1posIbrPtr;           /* pointer to sparse matrix diagonal at (positive node, added current i branch) */ 

	double *MEMI1negIbrPtr;           /* pointer to sparse matrix diagonal at (negative, added current i branch) */ 

	double *MEMI1ibrPosPtr;           /* pointer to sparse matrix offdiagonal at (added current i branch,positive) */ 

	double *MEMI1ibrNegPtr;           /* pointer to sparse matrix offdiagonal at (added current i branch,negative) */ 

	double *MEMI1ibrIbrPtr;           /* pointer to sparse matrix offdiagonal at (added current i branch,added current i branch) */ 

	double *MEMI1ibrWbrPtr;			/*pointer to sparse matrix offdiagonal at (added current i branch,added w row) */ 

	double *MEMI1wbrIbrPtr;           /* pointer to sparse matrix offdiagonal at (added w row,added current i branch) */ 

	double *MEMI1wbrWbrPtr;           /* pointer to sparse matrix offdiagonal at (added w row, added w row */ 

	double *MEMI1wbrPosPtr; 		  /* pointer to sparse matrix offdiagonal at (added current w branch,positive) */ 

	double *MEMI1wbrNegPtr; 		  /* pointer to sparse matrix offdiagonal at (added current w branch,negative) */ 
#ifndef NONOISE 
	double MEMI1nVar[NSTATVARS][MEMI1NSRCS]; 
#else /* NONOISE */ 
	double **MEMI1nVar; 
#endif /* NONOISE */ 
} MEMI1instance ; 

#define MEMI1qcapPseudo MEMI1state 			/* pseudo charge on the w branch of  memristor */ 
#define MEMI1ccapPseudo MEMI1state+1		/* pseudo current through the w branch of  memristor */ 
#define MEMI1currentState MEMI1state+2		/* Memristor current state used for convergence test */ 
#define MEMI1voltageState MEMI1state+3		/* Memristor voltage state used for convergence test */ 
#define MEMI1conductState MEMI1state+4		/* Memristor conduct state used for convergence test */ 
#define MEMI1flux MEMI1state+5
#define MEMI1volt MEMI1state+6
#define MEMI1numStates 7 

/* per model data */ 

typedef struct sMEMI1model {       /* model structure for a memristor */ 

	struct GENmodel gen; 

#define MEMI1modType gen.GENmodType 
#define MEMI1nextModel(inst) ((struct sMEMI1model *)((inst)->gen.GENnextModel)) 
#define MEMI1instances(inst) ((MEMI1instance *)((inst)->gen.GENinstances)) 
#define MEMI1modName gen.GENmodName 

double MEMI1tnom;         /* temperature at which memristance measured */ 
unsigned MEMI1tnomGiven		:1; /* flag to indicate nominal temp. was given */ 

int MEMI1type; 
unsigned MEMI1typeGiven		:1; 

double MEMI1defConstK; 

/*model parameterL_min*/ 
double MEMI1defL_min; 
unsigned MEMI1defL_minGiven	:1; 

/*model parameterL_max*/ 
double MEMI1defL_max; 
unsigned MEMI1defL_maxGiven	:1; 

/*model parameterK*/ 
double MEMI1defK; 
unsigned MEMI1defKGiven	:1; 

/*model parameterD*/ 
double MEMI1defD; 
unsigned MEMI1defDGiven	:1; 

/*model parameterWf*/ 
double MEMI1defWf; 
unsigned MEMI1defWfGiven	:1; 

/*model parameterP*/ 
double MEMI1defP; 
unsigned MEMI1defPGiven	:1; 

}MEMI1model; 

enum {  
	MEMI1_WINIT, 
    MEMI1_IC,
	MEMI1_WFLUXPSEUDO, 
	MEMI1_WVOLTPSEUDO, 
}; 
 
enum { 
	MEMI1_MOD_L_min, 
	MEMI1_MOD_L_max, 
	MEMI1_MOD_K, 
	MEMI1_MOD_D, 
	MEMI1_MOD_Wf, 
	MEMI1_MOD_P, 
	MEMI1_MOD_MEM, 
	MEMI1_MOD_WF, 
	MEMI1_MOD_TNOM, 
}; 

#include "memi1ext.h" 

extern void MEMI1update_conduct(MEMI1instance *, bool spill_warnings); 

#endif /*MEMI1*/ 
