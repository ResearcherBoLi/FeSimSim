#include "ngspice/ngspice.h" 
#include "ngspice/const.h" 
#include "ngspice/ifsim.h" 
#include "memr3defs.h" 
#include "ngspice/sperror.h" 

int 
MEMR3mParam(int param, IFvalue *value, GENmodel *inModel) 
{ 
	MEMR3model *model = (MEMR3model *)inModel; 
 
	switch(param) { 
 
	case MEMR3_MOD_TNOM: 
		model->MEMR3tnom = value->rValue+CONSTCtoK; 
		model->MEMR3tnomGiven = TRUE;
		break;

	case MEMR3_MOD_MEM: 
		if(value->iValue){ 
			model->MEMR3type = 1; 
			model->MEMR3typeGiven = TRUE; 
		}
		break; 
	case MEMR3_MOD_Vthp: 
		model->MEMR3defVthp = value->rValue; 
		model->MEMR3defVthpGiven = TRUE; 
		break;
	case MEMR3_MOD_Vthn: 
		model->MEMR3defVthn = value->rValue; 
		model->MEMR3defVthnGiven = TRUE; 
		break;
	case MEMR3_MOD_Gmax: 
		model->MEMR3defGmax = value->rValue; 
		model->MEMR3defGmaxGiven = TRUE; 
		break;
	case MEMR3_MOD_Gmin: 
		model->MEMR3defGmin = value->rValue; 
		model->MEMR3defGminGiven = TRUE; 
		break;
	case MEMR3_MOD_Ap: 
		model->MEMR3defAp = value->rValue; 
		model->MEMR3defApGiven = TRUE; 
		break;
	case MEMR3_MOD_An: 
		model->MEMR3defAn = value->rValue; 
		model->MEMR3defAnGiven = TRUE; 
		break;
	case MEMR3_MOD_Xp: 
		model->MEMR3defXp = value->rValue; 
		model->MEMR3defXpGiven = TRUE; 
		break;
	case MEMR3_MOD_Xn: 
		model->MEMR3defXn = value->rValue; 
		model->MEMR3defXnGiven = TRUE; 
		break;
	case MEMR3_MOD_b: 
		model->MEMR3defb = value->rValue; 
		model->MEMR3defbGiven = TRUE; 
		break;
	default: 
		return(E_BADPARM); 
	} 
	return(OK); 
} 
