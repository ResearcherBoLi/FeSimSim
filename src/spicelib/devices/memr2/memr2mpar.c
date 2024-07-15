/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h" 
#include "ngspice/const.h" 
#include "ngspice/ifsim.h" 
#include "memr2defs.h" 
#include "ngspice/sperror.h" 

int 
MEMR2mParam(int param, IFvalue *value, GENmodel *inModel) 
{ 
	MEMR2model *model = (MEMR2model *)inModel; 
 
	switch(param) { 
 
	case MEMR2_MOD_TNOM: 
		model->MEMR2tnom = value->rValue+CONSTCtoK; 
		model->MEMR2tnomGiven = TRUE;
		break;
		
	case MEMR2_MOD_WF: 
		model->MEMR2defWf=value->rValue; 
		model->MEMR2defWfGiven = TRUE; 
		break;
		
	case MEMR2_MOD_P: 
		model->MEMR2defP=value->rValue; 
		model->MEMR2defPGiven=TRUE; 
		break;
		
	case MEMR2_MOD_MEMR: 
		if(value->iValue){ 
			model->MEMR2type = 1; 
			model->MEMR2typeGiven = TRUE; 
		}
		break;
		
	case MEMR2_MOD_Alpha_on: 
		model->MEMR2defAlpha_on = value->rValue; 
		model->MEMR2defAlpha_onGiven = TRUE; 
		break;
		
	case MEMR2_MOD_Alpha_off: 
		model->MEMR2defAlpha_off = value->rValue; 
		model->MEMR2defAlpha_offGiven = TRUE; 
		break;
		
	case MEMR2_MOD_K_on: 
		model->MEMR2defK_on = value->rValue; 
		model->MEMR2defK_onGiven = TRUE; 
		break;
		
	case MEMR2_MOD_K_off: 
		model->MEMR2defK_off = value->rValue; 
		model->MEMR2defK_offGiven = TRUE; 
		break;
		
	case MEMR2_MOD_Vt_on: 
		model->MEMR2defVt_on = value->rValue; 
		model->MEMR2defVt_onGiven = TRUE; 
		break;
		
	case MEMR2_MOD_Vt_off: 
		model->MEMR2defVt_off = value->rValue; 
		model->MEMR2defVt_offGiven = TRUE; 
		break;
		
	case MEMR2_MOD_R_on: 
		model->MEMR2defR_on = value->rValue; 
		model->MEMR2defR_onGiven = TRUE; 
		break;
		
	case MEMR2_MOD_R_off: 
		model->MEMR2defR_off = value->rValue; 
		model->MEMR2defR_offGiven = TRUE; 
		break;
		
	case MEMR2_MOD_D: 
		model->MEMR2defD = value->rValue; 
		model->MEMR2defDGiven = TRUE; 
		break;

	case MEMR2_MOD_TNOIMOD: 
		model->MEMR2tnoiMod = value->iValue; 
		model->MEMR2tnoiModGiven = TRUE; 
		break;

	case MEMR2_MOD_FNOIMOD: 
		model->MEMR2fnoiMod = value->iValue; 
		model->MEMR2fnoiModGiven = TRUE; 
		break;

	case MEMR2_MOD_SNOIMOD: 
		model->MEMR2snoiMod = value->iValue; 
		model->MEMR2snoiModGiven = TRUE; 
		break;

	case MEMR2_MOD_BNOIMOD: 
		model->MEMR2bnoiMod = value->iValue; 
		model->MEMR2bnoiModGiven = TRUE; 
		break;

	case MEMR2_MOD_DNOIMOD: 
		model->MEMR2dnoiMod = value->iValue; 
		model->MEMR2dnoiModGiven = TRUE; 
		break;

	case MEMR2_MOD_AF: 
		model->MEMR2af = value->rValue; 
		model->MEMR2afGiven = TRUE; 
		break;

	case MEMR2_MOD_EF: 
		model->MEMR2ef = value->rValue; 
		model->MEMR2efGiven = TRUE; 
		break;

	case MEMR2_MOD_EFH: 
		model->MEMR2efh = value->rValue; 
		model->MEMR2efhGiven = TRUE; 
		break;

	case MEMR2_MOD_KF: 
		model->MEMR2kf = value->rValue; 
		model->MEMR2kfGiven = TRUE; 
		break;

	case MEMR2_MOD_FreqBW: 
		model->MEMR2freqBw = value->rValue; 
		model->MEMR2freqBwGiven = TRUE; 
		break;

	case MEMR2_MOD_NoiseTimeStep: 
		model->MEMR2noiseTimeStep = value->rValue; 
		model->MEMR2noiseTimeStepGiven = TRUE; 
		break;

	case MEMR2_MOD_RTN_AMP: 
		model->MEMR2RTNamplitude = value->rValue; 
		model->MEMR2RTNamplitudeGiven = TRUE; 
		break;

	case MEMR2_MOD_RTN_TCAPT: 
		model->MEMR2RTNtimeCapt = value->rValue; 
		model->MEMR2RTNtimeCaptGiven = TRUE; 
		break;
		
	case MEMR2_MOD_RTN_TEMIT: 
		model->MEMR2RTNtimeEmit = value->rValue; 
		model->MEMR2RTNtimeEmitGiven = TRUE; 
		break;
		


	default: 
		return(E_BADPARM); 
	} 
	return(OK); 
} 
