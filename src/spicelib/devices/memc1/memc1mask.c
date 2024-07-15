/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/


#include "ngspice/ngspice.h"
#include "ngspice/const.h"
#include "ngspice/cktdefs.h"
#include "ngspice/ifsim.h"
#include "memc1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/devdefs.h"


int 
MEMC1modAsk(CKTcircuit *ckt, GENmodel *inModel, int which, IFvalue *value)
{

    MEMC1model *model = (MEMC1model *)inModel;

    NG_IGNORE(ckt);

    switch(which) {

		case MEMC1_MOD_WF: 
			value->rValue = model->MEMC1defWf; 
			return(OK); 
		
		case MEMC1_MOD_C_min: 
			value->rValue = model->MEMC1defC_min; 
			return(OK); 
		
		case MEMC1_MOD_C_max: 
			value->rValue = model->MEMC1defC_max; 
			return(OK); 
		
		case MEMC1_MOD_K: 
			value->rValue = model->MEMC1defK; 
			return(OK); 
		
		case MEMC1_MOD_D: 
			value->rValue = model->MEMC1defD; 
			return(OK); 
		
		case MEMC1_MOD_Wf: 
			value->rValue = model->MEMC1defWf; 
			return(OK); 
		
		case MEMC1_MOD_P: 
			value->rValue = model->MEMC1defP; 
			return(OK); 

        default:
            return(E_BADPARM);
    }
}

