/**********
Copyright 2024 Xidian University.	All rights reserved.
 Author: Bo Li	at Hangzhou Institute of Technology	 
 Modified: 2024/1/27  Bo Li
 Refered to NgSPICE Res/Cap related file
**********/

/*
 */

#include "ngspice/ngspice.h"
#include "ngspice/ifsim.h"
#include "fecap1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
FECAP1mParam(int param, IFvalue *value, GENmodel *inModel)
{
    FECAP1model *mod = (FECAP1model*)inModel;
    switch(param) {

	case FECAP1_MOD_FECAP: 
		if(value->iValue){ 
			mod->FECAP1type = 1; 
			mod->FECAP1typeGiven = TRUE; 
		}
		break;

    case FECAP1_MOD_TNOM:
        mod->FECAP1_Tnom = value->rValue+CONSTCtoK;
        mod->FECAP1_TnomGiven = TRUE;
        break;
	/*	
	case FECAP1_MOD_N: 
		mod->FECAP1defN=value->rValue; 
		mod->FECAP1defNGiven=TRUE; 
		break;
    */

    case FECAP1_MOD_Thickness: 
		mod->FECAP1_Thickness = value->rValue; 
		mod->FECAP1_ThicknessGiven = TRUE; 
		break;

    case FECAP1_MOD_Ec: 
		mod->FECAP1_Ec = value->rValue; 
		mod->FECAP1_EcGiven = TRUE; 
		break;

    case FECAP1_MOD_Ps: 
		mod->FECAP1_Ps = value->rValue; 
		mod->FECAP1_PsGiven = TRUE; 
		break;

    case FECAP1_MOD_Alpha: 
		mod->FECAP1_Alpha = value->rValue; 
		mod->FECAP1_AlphaGiven = TRUE; 
		break;     

    case FECAP1_MOD_Beta: 
		mod->FECAP1_Beta = value->rValue; 
		mod->FECAP1_BetaGiven = TRUE; 
		break;     

    case FECAP1_MOD_Gamma: 
		mod->FECAP1_Gamma = value->rValue; 
		mod->FECAP1_GammaGiven = TRUE; 
		break;     		

    case FECAP1_MOD_Rho: 
		mod->FECAP1_Rho = value->rValue; 
		mod->FECAP1_RhoGiven = TRUE; 
		break;          

    default:
        return(E_BADPARM);
    }
    return(OK);
}

