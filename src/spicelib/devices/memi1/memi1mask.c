#include "ngspice/ngspice.h" 
#include "ngspice/const.h" 
#include "ngspice/cktdefs.h" 
#include "ngspice/ifsim.h" 
#include "memi1defs.h" 
#include "ngspice/sperror.h" 
#include "ngspice/devdefs.h" 

int 
MEMI1modAsk(CKTcircuit *ckt, GENmodel *inModel, int which, IFvalue *value) 
{ 
	MEMI1model *model = (MEMI1model *)inModel; 
	NG_IGNORE(ckt); 
	switch(which) { 
		case MEMI1_MOD_WF: 
			value->rValue = model->MEMI1defWf; 
			return(OK); 
		case MEMI1_MOD_L_min: 
			value->rValue = model->MEMI1defL_min; 
			return(OK); 
		case MEMI1_MOD_L_max: 
			value->rValue = model->MEMI1defL_max; 
			return(OK); 
		case MEMI1_MOD_K: 
			value->rValue = model->MEMI1defK; 
			return(OK); 
		case MEMI1_MOD_D: 
			value->rValue = model->MEMI1defD; 
			return(OK); 
		case MEMI1_MOD_Wf: 
			value->rValue = model->MEMI1defWf; 
			return(OK); 
		case MEMI1_MOD_P: 
			value->rValue = model->MEMI1defP; 
			return(OK); 
		default: 
			return(E_BADPARM); 
	} 
}