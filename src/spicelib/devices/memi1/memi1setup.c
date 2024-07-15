#include "ngspice/ngspice.h" 
#include "ngspice/smpdefs.h" 
#include "memi1defs.h" 
#include "ngspice/sperror.h" 

 
int 
MEMI1setup(SMPmatrix *matrix, GENmodel *inModel, CKTcircuit*ckt, int *states) 
{ 
	MEMI1model *model = (MEMI1model *)inModel; 
	MEMI1instance *here; 
	CKTnode *tmp; 
	int error; 
	NG_IGNORE(ckt); 
	/*  loop through all the memristor models */ 
	for( ; model != NULL; model = MEMI1nextModel(model)) { 
		/* Default Value Processing for Memristor Models */ 
		if(!model->MEMI1tnomGiven) model->MEMI1tnom = ckt->CKTnomTemp; 
		if(!model->MEMI1defL_minGiven) model->MEMI1defL_min = 10e-9; 
		if(!model->MEMI1defL_maxGiven) model->MEMI1defL_max = 10e-6; 
		if(!model->MEMI1defKGiven) model->MEMI1defK = 10e6; 
		if(!model->MEMI1defDGiven) model->MEMI1defD = 10e-9; 
		if(!model->MEMI1defWfGiven) model->MEMI1defWf = 2; 
		if(!model->MEMI1defPGiven) model->MEMI1defP = 10; 
		model->MEMI1defConstK = 1; 

		/* loop through all the instances of the model */ 
		for(here = MEMI1instances(model); here != NULL ; 
				here = MEMI1nextInstance(here)){ 

			here->MEMI1capacPseudo = 1/(model->MEMI1defConstK ); 
			here->MEMI1induc =(model->MEMI1defL_min - model->MEMI1defL_max)*here->MEMI1winit+model->MEMI1defL_max;		

			if(!here->MEMI1winitGiven)   here->MEMI1winit  =0.5; 
			if(!here->MEMI1icGiven)   here->MEMI1initCond =0;

			if(here->MEMI1Ibranch == 0) { 
				error = CKTmkCur(ckt,&tmp,here->MEMI1name,"ibranch"); 
				if(error) return(error); 
				here->MEMI1Ibranch = tmp->number; 
			} 
			if(here->MEMI1Wbranch == 0) { 
				error = CKTmkCur(ckt,&tmp,here->MEMI1name,"wbranch"); 
				if(error) return(error); 
				here->MEMI1Wbranch = tmp->number; 
			} 
			here->MEMI1qcapPseudo  = *states; 
			*states += MEMI1numStates; 
/* macro to make elements with built in test for out of memory */ 
#define TSTALLOC(ptr,first,second) \ 
do { if((here->ptr = SMPmakeElt(matrix, here->first, here->second)) == NULL){\ 
	    return(E_NOMEM);\ 
} } while(0) 
			TSTALLOC(MEMI1posIbrPtr, MEMI1posNode, MEMI1Ibranch); 
			TSTALLOC(MEMI1negIbrPtr, MEMI1negNode, MEMI1Ibranch); 

			TSTALLOC(MEMI1ibrPosPtr, MEMI1Ibranch, MEMI1posNode); 
			TSTALLOC(MEMI1ibrNegPtr, MEMI1Ibranch, MEMI1negNode); 
			TSTALLOC(MEMI1ibrIbrPtr, MEMI1Ibranch, MEMI1Ibranch); 
			TSTALLOC(MEMI1ibrWbrPtr, MEMI1Ibranch, MEMI1Wbranch); 

			TSTALLOC(MEMI1wbrPosPtr, MEMI1Wbranch, MEMI1posNode); 
			TSTALLOC(MEMI1wbrNegPtr, MEMI1Wbranch, MEMI1negNode); 
			TSTALLOC(MEMI1wbrIbrPtr, MEMI1Wbranch, MEMI1Ibranch); 
			TSTALLOC(MEMI1wbrWbrPtr, MEMI1Wbranch, MEMI1Wbranch); 

		} 
	}
	return(OK); 
} 
