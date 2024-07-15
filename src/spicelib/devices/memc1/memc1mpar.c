/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/


#include "ngspice/ngspice.h"
#include "ngspice/const.h"
#include "ngspice/ifsim.h"
#include "memc1defs.h"
#include "ngspice/sperror.h"


int
MEMC1mParam(int param, IFvalue *value, GENmodel *inModel)
{
    MEMC1model *model = (MEMC1model *)inModel;

    switch(param) {
    
    case MEMC1_MOD_TNOM:
        model->MEMC1tnom = value->rValue+CONSTCtoK;
        model->MEMC1tnomGiven = TRUE;
        break;
		
	case MEMC1_MOD_Wf:
		model->MEMC1defWf=value->rValue;
		model->MEMC1defWfGiven=TRUE;
		break;
		
	case MEMC1_MOD_P:
		model->MEMC1defP=value->rValue;
		model->MEMC1defPGiven=TRUE;
		break;

	case MEMC1_MOD_WF: 
		model->MEMC1defWf=value->rValue; 
		model->MEMC1defWfGiven = TRUE; 
		break; 

	case MEMC1_MOD_C_min: 
		model->MEMC1defC_min = value->rValue; 
		model->MEMC1defC_minGiven = TRUE; 
		break;
		
	case MEMC1_MOD_C_max: 
		model->MEMC1defC_max = value->rValue; 
		model->MEMC1defC_maxGiven = TRUE; 
		break;
		
	case MEMC1_MOD_K: 
		model->MEMC1defK = value->rValue; 
		model->MEMC1defKGiven = TRUE; 
		break;
		
	case MEMC1_MOD_D: 
		model->MEMC1defD = value->rValue; 
		model->MEMC1defDGiven = TRUE; 
		break;		

    case MEMC1_MOD_MEMC:
        if(value->iValue) {
            model->MEMC1type = 1;
            model->MEMC1typeGiven = TRUE;
        }
        break;		

    default:
        return(E_BADPARM);
    }
    return(OK);
}
