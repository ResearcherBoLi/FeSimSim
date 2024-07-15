#include "ngspice/ngspice.h" 
#include "ngspice/smpdefs.h" 
#include "memr3defs.h" 
#include "ngspice/sperror.h" 

 
int 
MEMR3setup(SMPmatrix *matrix, GENmodel *inModel, CKTcircuit*ckt, int *states) 
{ 
	MEMR3model *model = (MEMR3model *)inModel; 
	MEMR3instance *here; 
	CKTnode *tmp; 
	int error; 
	NG_IGNORE(ckt); 
	/*  loop through all the memristor models */ 
	for( ; model != NULL; model = MEMR3nextModel(model)) { 
		/* Default Value Processing for Memristor Models */ 
		if(!model->MEMR3tnomGiven) model->MEMR3tnom = ckt->CKTnomTemp; 
		if(!model->MEMR3defVthpGiven) model->MEMR3defVthp = 0.887; 
		if(!model->MEMR3defVthnGiven) model->MEMR3defVthn = -1.275; 
		if(!model->MEMR3defGmaxGiven) model->MEMR3defGmax = 2.075e-4; 
		if(!model->MEMR3defGminGiven) model->MEMR3defGmin = 9.726e-6; 
		if(!model->MEMR3defApGiven) model->MEMR3defAp = 145.233; 
		if(!model->MEMR3defAnGiven) model->MEMR3defAn = 47.649; 
		if(!model->MEMR3defXpGiven) model->MEMR3defXp = 0.782; 
		if(!model->MEMR3defXnGiven) model->MEMR3defXn = 0.726; 
		if(!model->MEMR3defbGiven) model->MEMR3defb = 2.531; 
		model->MEMR3defConstK = 1; 

		/* loop through all the instances of the model */ 
		for(here = MEMR3instances(model); here != NULL ; 
				here = MEMR3nextInstance(here)){ 

			here->MEMR3capacPseudo = 1/(model->MEMR3defConstK ); 

			if(!here->MEMR3winitGiven)   here->MEMR3winit  =0; 

			if(here->MEMR3Ibranch == 0) { 
				error = CKTmkCur(ckt,&tmp,here->MEMR3name,"ibranch"); 
				if(error) return(error); 
				here->MEMR3Ibranch = tmp->number; 
			} 
			if(here->MEMR3Wbranch == 0) { 
				error = CKTmkCur(ckt,&tmp,here->MEMR3name,"wbranch"); 
				if(error) return(error); 
				here->MEMR3Wbranch = tmp->number; 
			} 
			here->MEMR3qcapPseudo  = *states; 
			*states += MEMR3numStates; 
/* macro to make elements with built in test for out of memory */ 
#define TSTALLOC(ptr,first,second) \ 
do { if((here->ptr = SMPmakeElt(matrix, here->first, here->second)) == NULL){\ 
	    return(E_NOMEM);\ 
} } while(0) 
			TSTALLOC(MEMR3posIbrPtr, MEMR3posNode, MEMR3Ibranch); 
			TSTALLOC(MEMR3negIbrPtr, MEMR3negNode, MEMR3Ibranch); 

			TSTALLOC(MEMR3ibrPosPtr, MEMR3Ibranch, MEMR3posNode); 
			TSTALLOC(MEMR3ibrNegPtr, MEMR3Ibranch, MEMR3negNode); 
			TSTALLOC(MEMR3ibrIbrPtr, MEMR3Ibranch, MEMR3Ibranch); 
			TSTALLOC(MEMR3ibrWbrPtr, MEMR3Ibranch, MEMR3Wbranch); 

			TSTALLOC(MEMR3wbrPosPtr, MEMR3Wbranch, MEMR3posNode); 
			TSTALLOC(MEMR3wbrNegPtr, MEMR3Wbranch, MEMR3negNode); 
			TSTALLOC(MEMR3wbrIbrPtr, MEMR3Wbranch, MEMR3Ibranch); 
			TSTALLOC(MEMR3wbrWbrPtr, MEMR3Wbranch, MEMR3Wbranch); 

		} 
	}
	return(OK); 
} 
