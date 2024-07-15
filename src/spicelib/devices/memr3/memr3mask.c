#include "ngspice/ngspice.h" 
#include "ngspice/const.h" 
#include "ngspice/cktdefs.h" 
#include "ngspice/ifsim.h" 
#include "memr3defs.h" 
#include "ngspice/sperror.h" 
#include "ngspice/devdefs.h" 

int 
MEMR3modAsk(CKTcircuit *ckt, GENmodel *inModel, int which, IFvalue *value) 
{ 
	MEMR3model *model = (MEMR3model *)inModel; 
	NG_IGNORE(ckt); 
	switch(which) { 
		case MEMR3_MOD_Vthp: 
			value->rValue = model->MEMR3defVthp; 
			return(OK); 
		case MEMR3_MOD_Vthn: 
			value->rValue = model->MEMR3defVthn; 
			return(OK); 
		case MEMR3_MOD_Gmax: 
			value->rValue = model->MEMR3defGmax; 
			return(OK); 
		case MEMR3_MOD_Gmin: 
			value->rValue = model->MEMR3defGmin; 
			return(OK); 
		case MEMR3_MOD_Ap: 
			value->rValue = model->MEMR3defAp; 
			return(OK); 
		case MEMR3_MOD_An: 
			value->rValue = model->MEMR3defAn; 
			return(OK); 
		case MEMR3_MOD_Xp: 
			value->rValue = model->MEMR3defXp; 
			return(OK); 
		case MEMR3_MOD_Xn: 
			value->rValue = model->MEMR3defXn; 
			return(OK); 
		case MEMR3_MOD_b: 
			value->rValue = model->MEMR3defb; 
			return(OK); 
		default: 
			return(E_BADPARM); 
	} 
}