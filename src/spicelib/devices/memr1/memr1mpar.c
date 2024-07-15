
/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/


#include "ngspice/ngspice.h"
#include "ngspice/const.h"
#include "ngspice/ifsim.h"
#include "memr1defs.h"
#include "ngspice/sperror.h"


int
MEMR1mParam(int param, IFvalue *value, GENmodel *inModel)
{
    MEMR1model *model = (MEMR1model *)inModel;

    switch(param) {
    
    case MEMR1_MOD_TNOM:
        model->MEMR1tnom = value->rValue+CONSTCtoK;
        model->MEMR1tnomGiven = TRUE;
        break;
	case MEMR1_MOD_WF:
		model->MEMR1defWf=value->rValue;
		model->MEMR1defWfGiven=TRUE;
		break;

    case MEMR1_MOD_RON:
        model->MEMR1defRon = value->rValue;
        model->MEMR1defRonGiven = TRUE;
        break;
    case MEMR1_MOD_ROFF:
        model->MEMR1defRoff = value->rValue;
        model->MEMR1defRoffGiven = TRUE;
        break;
    case MEMR1_MOD_MU:
        model->MEMR1defMu = value->rValue;
        model->MEMR1defMuGiven = TRUE;
        break;

    case MEMR1_MOD_DEPTH:
        model->MEMR1defDepth = value->rValue;
        model->MEMR1defDepthGiven = TRUE;
        break;

    case MEMR1_MOD_MEMR:
        if(value->iValue) {
            model->MEMR1type = 1;
            model->MEMR1typeGiven = TRUE;
        }
        break;		

	case MEMR1_MOD_TNOIMOD: 
		model->MEMR1tnoiMod = value->iValue; 
		model->MEMR1tnoiModGiven = TRUE; 
		break;

	case MEMR1_MOD_FNOIMOD: 
		model->MEMR1fnoiMod = value->iValue; 
		model->MEMR1fnoiModGiven = TRUE; 
		break;

	case MEMR1_MOD_SNOIMOD: 
		model->MEMR1snoiMod = value->iValue; 
		model->MEMR1snoiModGiven = TRUE; 
		break;

	case MEMR1_MOD_BNOIMOD: 
		model->MEMR1bnoiMod = value->iValue; 
		model->MEMR1bnoiModGiven = TRUE; 
		break;

	case MEMR1_MOD_DNOIMOD: 
		model->MEMR1dnoiMod = value->iValue; 
		model->MEMR1dnoiModGiven = TRUE; 
		break;

	case MEMR1_MOD_AF: 
		model->MEMR1af = value->rValue; 
		model->MEMR1afGiven = TRUE; 
		break;

	case MEMR1_MOD_EF: 
		model->MEMR1ef = value->rValue; 
		model->MEMR1efGiven = TRUE; 
		break;

	case MEMR1_MOD_EFH: 
		model->MEMR1efh = value->rValue; 
		model->MEMR1efhGiven = TRUE; 
		break;

	case MEMR1_MOD_KF: 
		model->MEMR1kf = value->rValue; 
		model->MEMR1kfGiven = TRUE; 
		break;

	case MEMR1_MOD_FreqBW: 
		model->MEMR1freqBw = value->rValue; 
		model->MEMR1freqBwGiven = TRUE; 
		break;

	case MEMR1_MOD_NoiseTimeStep: 
		model->MEMR1noiseTimeStep = value->rValue; 
		model->MEMR1noiseTimeStepGiven = TRUE; 
		break;

	case MEMR1_MOD_RTN_AMP: 
		model->MEMR1RTNamplitude = value->rValue; 
		model->MEMR1RTNamplitudeGiven = TRUE; 
		break;

	case MEMR1_MOD_RTN_TCAPT: 
		model->MEMR1RTNtimeCapt = value->rValue; 
		model->MEMR1RTNtimeCaptGiven = TRUE; 
		break;
		
	case MEMR1_MOD_RTN_TEMIT: 
		model->MEMR1RTNtimeEmit = value->rValue; 
		model->MEMR1RTNtimeEmitGiven = TRUE; 
		break;
		


    default:
        return(E_BADPARM);
    }
    return(OK);
}
