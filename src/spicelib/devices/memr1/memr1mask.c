
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
#include "memr1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/devdefs.h"


int 
MEMR1modAsk(CKTcircuit *ckt, GENmodel *inModel, int which, IFvalue *value)
{

    MEMR1model *model = (MEMR1model *)inModel;

    NG_IGNORE(ckt);

    switch(which) {
        case MEMR1_MOD_RON:
            value->rValue = model->MEMR1defRon;
            return(OK);

        case MEMR1_MOD_ROFF:
            value->rValue = model->MEMR1defRoff;
            return(OK);

        case MEMR1_MOD_MU:
            value->rValue = model->MEMR1defMu;
            return(OK);

        case MEMR1_MOD_DEPTH:
            value->rValue = model->MEMR1defDepth;
            return(OK);

		case MEMR1_MOD_TNOIMOD: 
			value->rValue = model->MEMR1tnoiMod; 
			return(OK); 

		case MEMR1_MOD_FNOIMOD: 
			value->rValue = model->MEMR1fnoiMod; 
			return(OK); 

		case MEMR1_MOD_SNOIMOD: 
			value->rValue = model->MEMR1snoiMod; 
			return(OK); 

		case MEMR1_MOD_BNOIMOD: 
			value->rValue = model->MEMR1bnoiMod; 
			return(OK); 

		case MEMR1_MOD_DNOIMOD: 
			value->rValue = model->MEMR1dnoiMod; 
			return(OK); 

		case MEMR1_MOD_AF: 
			value->rValue = model->MEMR1af; 
			return(OK); 

		case MEMR1_MOD_EF: 
			value->rValue = model->MEMR1ef; 
			return(OK); 

		case MEMR1_MOD_EFH: 
			value->rValue = model->MEMR1efh; 
			return(OK);             

		case MEMR1_MOD_KF: 
			value->rValue = model->MEMR1kf; 
			return(OK); 

		case MEMR1_MOD_FreqBW: 
			value->rValue = model->MEMR1freqBw; 
			return(OK); 

		case MEMR1_MOD_NoiseTimeStep: 
			value->rValue = model->MEMR1noiseTimeStep; 
			return(OK); 
		
		case MEMR1_MOD_RTN_AMP: 
			value->rValue = model->MEMR1RTNamplitude; 
			return(OK); 

		case MEMR1_MOD_RTN_TCAPT: 
			value->rValue = model->MEMR1RTNtimeCapt; 
			return(OK); 

		case MEMR1_MOD_RTN_TEMIT: 
			value->rValue = model->MEMR1RTNtimeEmit; 
			return(OK);


        default:
            return(E_BADPARM);
    }
}

