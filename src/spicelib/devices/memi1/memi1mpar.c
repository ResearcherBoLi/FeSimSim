#include "ngspice/ngspice.h" 
#include "ngspice/const.h" 
#include "ngspice/ifsim.h" 
#include "memi1defs.h" 
#include "ngspice/sperror.h" 

int 
MEMI1mParam(int param, IFvalue *value, GENmodel *inModel) 
{ 
	MEMI1model *model = (MEMI1model *)inModel; 
 
	switch(param) { 
 
	case MEMI1_MOD_TNOM: 
		model->MEMI1tnom = value->rValue+CONSTCtoK; 
		model->MEMI1tnomGiven = TRUE;
		break;
	case MEMI1_MOD_Wf: 
		model->MEMI1defWf=value->rValue; 
		model->MEMI1defWfGiven = TRUE; 
		break; 
	case MEMI1_MOD_P: 
		model->MEMI1defP=value->rValue; 
		model->MEMI1defPGiven=TRUE; 
		break; 
	case MEMI1_MOD_MEM: 
		if(value->iValue){ 
			model->MEMI1type = 1; 
			model->MEMI1typeGiven = TRUE; 
		}
		break; 
	case MEMI1_MOD_L_min: 
		model->MEMI1defL_min = value->rValue; 
		model->MEMI1defL_minGiven = TRUE; 
		break;
	case MEMI1_MOD_L_max: 
		model->MEMI1defL_max = value->rValue; 
		model->MEMI1defL_maxGiven = TRUE; 
		break;
	case MEMI1_MOD_K: 
		model->MEMI1defK = value->rValue; 
		model->MEMI1defKGiven = TRUE; 
		break;
	case MEMI1_MOD_D: 
		model->MEMI1defD = value->rValue; 
		model->MEMI1defDGiven = TRUE; 
		break;
	case MEMI1_MOD_WF: 
		model->MEMI1defWf = value->rValue; 
		model->MEMI1defWfGiven = TRUE; 
		break;

	default: 
		return(E_BADPARM); 
	} 
	return(OK); 
} 
