/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h" 
#include "ngspice/smpdefs.h" 
#include "memr2defs.h" 
#include "ngspice/sperror.h" 
#include "ngspice/1-f-code.h"


int 
MEMR2setup(SMPmatrix *matrix, GENmodel *inModel, CKTcircuit*ckt, int *states) 
{ 
	MEMR2model *model = (MEMR2model *)inModel; 
	MEMR2instance *here; 
	CKTnode *tmp; 
	int error; 
	NG_IGNORE(ckt); 
	/*  loop through all the memristor models */ 
	for( ; model != NULL; model = MEMR2nextModel(model)) { 
		/* Default Value Processing for Memristor Models */ 
		if(!model->MEMR2tnomGiven) model->MEMR2tnom = ckt->CKTnomTemp; 
		if(!model->MEMR2defAlpha_onGiven) model->MEMR2defAlpha_on = 4; 
		if(!model->MEMR2defAlpha_offGiven) model->MEMR2defAlpha_off = 4; 
		if(!model->MEMR2defK_onGiven) model->MEMR2defK_on = -216.2; 
		if(!model->MEMR2defK_offGiven) model->MEMR2defK_off = 0.091; 
		if(!model->MEMR2defVt_onGiven) model->MEMR2defVt_on = -1.5; 
		if(!model->MEMR2defVt_offGiven) model->MEMR2defVt_off = 0.3; 
		if(!model->MEMR2defR_onGiven) model->MEMR2defR_on = 1000; 
		if(!model->MEMR2defR_offGiven) model->MEMR2defR_off = 300000; 
		if(!model->MEMR2defDGiven) model->MEMR2defD = 3e-9; 
		if(!model->MEMR2defWfGiven) model->MEMR2defWf = 2; 
		if(!model->MEMR2defPGiven) model->MEMR2defP = 10; 
        if(!model->MEMR2tnoiModGiven) model->MEMR2tnoiMod = 0;
		if(!model->MEMR2fnoiModGiven) model->MEMR2fnoiMod = 0;
        if(!model->MEMR2snoiModGiven) model->MEMR2snoiMod = 0;
        if(!model->MEMR2bnoiModGiven) model->MEMR2bnoiMod = 0;
        if(!model->MEMR2dnoiModGiven) model->MEMR2dnoiMod = 0;
        if(!model->MEMR2freqBwGiven) model->MEMR2freqBw =1e9;
		if(!model->MEMR2noiseTimeStepGiven) model->MEMR2noiseTimeStep=ckt->CKTstep;		
		if(!model->MEMR2RTNamplitudeGiven) model->MEMR2RTNamplitude = 0;
		if(!model->MEMR2RTNtimeCaptGiven) model->MEMR2RTNtimeCapt = 1e-6;
		if(!model->MEMR2RTNtimeEmitGiven) model->MEMR2RTNtimeEmit = 1e-6;
        model->MEMR2defConstK = 1; 

		/* loop through all the instances of the model */ 
		for(here = MEMR2instances(model); here != NULL ; 
				here = MEMR2nextInstance(here)){ 

			here->MEMR2capacPseudo = 1/(model->MEMR2defConstK ); 

			if(!here->MEMR2winitGiven)   here->MEMR2winit = 0.5; 
            
            if(!here->MEMR2noiseTranGiven)   here->MEMR2doNoiseTran = 0; 

            if(here->MEMR2doNoiseTran){
				double TnoiAmp=0;  // thermal noise amplitude
				double FnoiAmp=0;  // flicker noise amplitude
				double FnoiAlpha=0; 
				double TS=model->MEMR2noiseTimeStep;
				here->MEMR2conduct = 1 / (model->MEMR2defR_off * here->MEMR2winit + model->MEMR2defR_on * (1 - here->MEMR2winit));				

				if(model->MEMR2tnoiMod){

					TnoiAmp = sqrt(2*BOLTZMANN*ckt->CKTtemp* here->MEMR2conduct *  model->MEMR2freqBw);
					}
				
				if(model->MEMR2fnoiMod){
					FnoiAmp = model->MEMR2kf/here->MEMR2conduct*1e-18;
					FnoiAlpha = 1+here->MEMR2winit;
					}
				
				if(!model->MEMR2noiseTimeStepGiven){
					if(ckt->CKTdelta>0){
						TS = MIN(ckt->CKTdelta,	ckt->CKTstep);
					}else{
						TS = ckt->CKTstep;
					}
				}else{
					TS=model->MEMR2noiseTimeStep;
				}
				
                double NA = TnoiAmp;
                double NALPHA = FnoiAlpha;
                double NAMP = FnoiAmp;   			
                double RTSAM = 0;
                double RTSCAPT = 0;
                double RTSEMT = 0;      
				if(model->MEMR2bnoiMod){
					here->MEMR2conduct = 1 / (model->MEMR2defR_off * here->MEMR2winit + model->MEMR2defR_on * (1 - here->MEMR2winit));                  
					RTSAM = model->MEMR2RTNamplitude*here->MEMR2winit;
					RTSCAPT = model->MEMR2RTNtimeCapt;
					RTSEMT = model->MEMR2RTNtimeEmit;
				}
				
                trnoise_state_free(here->MEMR2trnoise_state);
                here->MEMR2trnoise_state =
                    trnoise_state_init(NA, TS, NALPHA, NAMP, RTSAM, RTSCAPT, RTSEMT);
            }

			if(here->MEMR2Ibranch == 0) { 
				error = CKTmkCur(ckt,&tmp,here->MEMR2name,"ibranch"); 
				if(error) return(error); 
				here->MEMR2Ibranch = tmp->number; 
			} 
			if(here->MEMR2Wbranch == 0) { 
				error = CKTmkCur(ckt,&tmp,here->MEMR2name,"wbranch"); 
				if(error) return(error); 
				here->MEMR2Wbranch = tmp->number; 
			} 
#ifdef  _USE_VSRC_NOISE_
			if(here->MEMR2noiseVsrcIbranch == 0){
				error = CKTmkCur(ckt,&tmp,here->MEMR2name,"VNibranch"); 
				if(error) return(error); 
				here->MEMR2noiseVsrcIbranch = tmp->number; 
			}
#endif			
			here->MEMR2qcapPseudo  = *states; 
			*states += MEMR2numStates; 
/* macro to make elements with built in test for out of memory */ 
#define TSTALLOC(ptr,first,second) \ 
do { if((here->ptr = SMPmakeElt(matrix, here->first, here->second)) == NULL){\ 
	    return(E_NOMEM);\ 
} } while(0) 
			TSTALLOC(MEMR2posIbrPtr, MEMR2posNode, MEMR2Ibranch); 
			TSTALLOC(MEMR2negIbrPtr, MEMR2negNode, MEMR2Ibranch); 

			TSTALLOC(MEMR2ibrPosPtr, MEMR2Ibranch, MEMR2posNode); 
			TSTALLOC(MEMR2ibrNegPtr, MEMR2Ibranch, MEMR2negNode); 
			TSTALLOC(MEMR2ibrIbrPtr, MEMR2Ibranch, MEMR2Ibranch); 
			TSTALLOC(MEMR2ibrWbrPtr, MEMR2Ibranch, MEMR2Wbranch); 

			TSTALLOC(MEMR2wbrPosPtr, MEMR2Wbranch, MEMR2posNode); 
			TSTALLOC(MEMR2wbrNegPtr, MEMR2Wbranch, MEMR2negNode); 
			TSTALLOC(MEMR2wbrIbrPtr, MEMR2Wbranch, MEMR2Ibranch); 
			TSTALLOC(MEMR2wbrWbrPtr, MEMR2Wbranch, MEMR2Wbranch); 

#ifdef  _USE_VSRC_NOISE_
			TSTALLOC(MEMR2noiseVsrcPosIbrPtr, MEMR2noiseVsrcPosNode, MEMR2noiseVsrcIbranch);
			TSTALLOC(MEMR2noiseVsrcNegIbrPtr, MEMR2noiseVsrcNegNode, MEMR2noiseVsrcIbranch);
			TSTALLOC(MEMR2noiseVsrcIbrNegPtr, MEMR2noiseVsrcIbranch, MEMR2noiseVsrcNegNode);
			TSTALLOC(MEMR2noiseVsrcIbrPosPtr, MEMR2noiseVsrcIbranch, MEMR2noiseVsrcPosNode);
#endif

		} 
	}
	return(OK); 
} 
