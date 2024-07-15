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
#include "memr2defs.h" 
#include "ngspice/sperror.h" 
#include "ngspice/devdefs.h" 

int 
MEMR2modAsk(CKTcircuit *ckt, GENmodel *inModel, int which, IFvalue *value) 
{ 
	MEMR2model *model = (MEMR2model *)inModel; 
	NG_IGNORE(ckt); 
	switch(which) { 
		case MEMR2_MOD_WF: 
			value->rValue = model->MEMR2defWf; 
			return(OK); 
		
		case MEMR2_MOD_P: 
			value->rValue = model->MEMR2defP; 
			return(OK); 
		
		case MEMR2_MOD_Alpha_on: 
			value->rValue = model->MEMR2defAlpha_on; 
			return(OK); 
		
		case MEMR2_MOD_Alpha_off: 
			value->rValue = model->MEMR2defAlpha_off; 
			return(OK); 
		
		case MEMR2_MOD_K_on: 
			value->rValue = model->MEMR2defK_on; 
			return(OK); 
		
		case MEMR2_MOD_K_off: 
			value->rValue = model->MEMR2defK_off; 
			return(OK); 
		
		case MEMR2_MOD_Vt_on: 
			value->rValue = model->MEMR2defVt_on; 
			return(OK);
		
		case MEMR2_MOD_Vt_off: 
			value->rValue = model->MEMR2defVt_off; 
			return(OK);
		
		case MEMR2_MOD_R_on: 
			value->rValue = model->MEMR2defR_on; 
			return(OK); 
		
		case MEMR2_MOD_R_off: 
			value->rValue = model->MEMR2defR_off; 
			return(OK);
		
		case MEMR2_MOD_D: 
			value->rValue = model->MEMR2defD; 
			return(OK); 

		case MEMR2_MOD_TNOIMOD: 
			value->rValue = model->MEMR2tnoiMod; 
			return(OK); 

		case MEMR2_MOD_FNOIMOD: 
			value->rValue = model->MEMR2fnoiMod; 
			return(OK); 

		case MEMR2_MOD_SNOIMOD: 
			value->rValue = model->MEMR2snoiMod; 
			return(OK); 

		case MEMR2_MOD_BNOIMOD: 
			value->rValue = model->MEMR2bnoiMod; 
			return(OK); 

		case MEMR2_MOD_DNOIMOD: 
			value->rValue = model->MEMR2dnoiMod; 
			return(OK); 

		case MEMR2_MOD_AF: 
			value->rValue = model->MEMR2af; 
			return(OK); 

		case MEMR2_MOD_EF: 
			value->rValue = model->MEMR2ef; 
			return(OK); 

		case MEMR2_MOD_EFH: 
			value->rValue = model->MEMR2efh; 
			return(OK);             

		case MEMR2_MOD_KF: 
			value->rValue = model->MEMR2kf; 
			return(OK); 

		case MEMR2_MOD_FreqBW: 
			value->rValue = model->MEMR2freqBw; 
			return(OK); 

		case MEMR2_MOD_NoiseTimeStep: 
			value->rValue = model->MEMR2noiseTimeStep; 
			return(OK); 
		
		case MEMR2_MOD_RTN_AMP: 
			value->rValue = model->MEMR2RTNamplitude; 
			return(OK); 

		case MEMR2_MOD_RTN_TCAPT: 
			value->rValue = model->MEMR2RTNtimeCapt; 
			return(OK); 

		case MEMR2_MOD_RTN_TEMIT: 
			value->rValue = model->MEMR2RTNtimeEmit; 
			return(OK); 
			
		default: 
			return(E_BADPARM) ;
	} 
}
