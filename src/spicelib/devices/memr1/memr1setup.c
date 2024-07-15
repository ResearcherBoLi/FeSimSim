/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/


#include "ngspice/ngspice.h"
#include "ngspice/smpdefs.h"
#include "memr1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/1-f-code.h"

int
MEMR1setup(SMPmatrix *matrix, GENmodel *inModel, CKTcircuit*ckt, int *states)
        /* load the memristor structure with those pointers needed later
         * for fast matrix loading
         */
{
    MEMR1model *model = (MEMR1model *)inModel;
    MEMR1instance *here;
    CKTnode *tmp;
    int error;

 
    NG_IGNORE(ckt);

    /*  loop through all the memristor models */
    for( ; model != NULL; model = MEMR1nextModel(model)) {

        /* Default Value Processing for Memristor Models */
        if(!model->MEMR1tnomGiven) model->MEMR1tnom         = ckt->CKTnomTemp;
        if(!model->MEMR1defDepthGiven) model->MEMR1defDepth = 10e-9;
        if(!model->MEMR1defRonGiven) model->MEMR1defRon = 100; 
        if(!model->MEMR1defRoffGiven ) model->MEMR1defRoff = 1e5;
        if(!model->MEMR1defMuGiven) model->MEMR1defMu    =1e-15;
		if (!model->MEMR1defWfGiven) model->MEMR1defWf    =2;
        // initial tran noise model paras
        if(!model->MEMR1tnoiModGiven) model->MEMR1tnoiMod = 0;
		if(!model->MEMR1fnoiModGiven) model->MEMR1fnoiMod = 0;
        if(!model->MEMR1snoiModGiven) model->MEMR1snoiMod = 0;
        if(!model->MEMR1bnoiModGiven) model->MEMR1bnoiMod = 0;
        if(!model->MEMR1dnoiModGiven) model->MEMR1dnoiMod = 0;
        if(!model->MEMR1freqBwGiven) model->MEMR1freqBw =1e9;
		if(!model->MEMR1noiseTimeStepGiven) model->MEMR1noiseTimeStep=ckt->CKTstep;		
		if(!model->MEMR1RTNamplitudeGiven) model->MEMR1RTNamplitude = 0;
		if(!model->MEMR1RTNtimeCaptGiven) model->MEMR1RTNtimeCapt = 1e-6;
		if(!model->MEMR1RTNtimeEmitGiven) model->MEMR1RTNtimeEmit = 1e-6;        

		model->MEMR1defConstK = (model->MEMR1defMu * model->MEMR1defRon)/(model->MEMR1defDepth *model->MEMR1defDepth);

        /* loop through all the instances of the model */
        for (here = MEMR1instances(model); here != NULL ;
                here=MEMR1nextInstance(here)) {
					
			here->MEMR1capacPseudo = 1/(model->MEMR1defConstK );
            if(!here->MEMR1wfGiven)   here->MEMR1wf  = model->MEMR1defWf;
            if(!here->MEMR1winitGiven)   here->MEMR1winit  =0.5;
			here->MEMR1rinit = model->MEMR1defRon * here->MEMR1winit + model->MEMR1defRoff* (1 - here->MEMR1winit);
            if(!here->MEMR1pGiven)       here->MEMR1p      = 10;

            if(!here->MEMR1noiseTranGiven)   here->MEMR1doNoiseTran = 0; 

            if(here->MEMR1doNoiseTran){
				double TnoiAmp=0;  // thermal noise amplitude
				double FnoiAmp=0;  // flicker noise amplitude
				double FnoiAlpha=0; 
				double TS=model->MEMR1noiseTimeStep;
				if(model->MEMR1tnoiModGiven){
					here->MEMR1conduct = 1 / (model->MEMR1defRon * here->MEMR1winit + model->MEMR1defRoff * (1 - here->MEMR1winit));
					TnoiAmp = sqrt(2*BOLTZMANN*ckt->CKTtemp* here->MEMR1conduct *  model->MEMR1freqBw);
				}
				
				if(model->MEMR1fnoiModGiven){
					FnoiAmp = model->MEMR1kf/here->MEMR1conduct*1e-18;
					FnoiAlpha = 1+here->MEMR1winit;
				}

				
				if(!model->MEMR1noiseTimeStepGiven){
					if(ckt->CKTdelta>0){
						TS = MIN(ckt->CKTdelta,	ckt->CKTstep);
					}else{
						TS = ckt->CKTstep;
					}
				}else{
					TS=model->MEMR1noiseTimeStep;
				}
				
                double NA = TnoiAmp;
                double NALPHA = FnoiAlpha;
                double NAMP = FnoiAmp;   			
                double RTSAM = 0;
                double RTSCAPT = 0;
                double RTSEMT = 0;      
				if(model->MEMR1bnoiModGiven){
					RTSAM = model->MEMR1RTNamplitude;
					RTSCAPT = model->MEMR1RTNtimeCapt;
					RTSEMT = model->MEMR1RTNtimeEmit;
				}
				
                trnoise_state_free(here->MEMR1trnoise_state);
                here->MEMR1trnoise_state =
                    trnoise_state_init(NA, TS, NALPHA, NAMP, RTSAM, RTSCAPT, RTSEMT);
            }

            if(here->MEMR1Ibranch == 0) {
                error = CKTmkCur(ckt,&tmp,here->MEMR1name,"ibranch");
                if(error) return(error);
                here->MEMR1Ibranch = tmp->number;
			}
			if(here->MEMR1Wbranch == 0) {
					error = CKTmkCur(ckt,&tmp,here->MEMR1name,"wbranch");
					if(error) return(error);
					here->MEMR1Wbranch = tmp->number;
			}
			
            here->MEMR1qcapPseudo  = *states;
            *states += MEMR1numStates;
			
/* macro to make elements with built in test for out of memory */
#define TSTALLOC(ptr,first,second) \
do { if((here->ptr = SMPmakeElt(matrix, here->first, here->second)) == NULL){\
    return(E_NOMEM);\
} } while(0)

            TSTALLOC(MEMR1posIbrPtr, MEMR1posNode, MEMR1Ibranch);
            TSTALLOC(MEMR1negIbrPtr, MEMR1negNode, MEMR1Ibranch);

            TSTALLOC(MEMR1ibrPosPtr, MEMR1Ibranch, MEMR1posNode);
            TSTALLOC(MEMR1ibrNegPtr, MEMR1Ibranch, MEMR1negNode);
            TSTALLOC(MEMR1ibrIbrPtr, MEMR1Ibranch, MEMR1Ibranch);
            TSTALLOC(MEMR1ibrWbrPtr, MEMR1Ibranch, MEMR1Wbranch);

            TSTALLOC(MEMR1wbrIbrPtr, MEMR1Wbranch, MEMR1Ibranch);
            TSTALLOC(MEMR1wbrWbrPtr, MEMR1Wbranch, MEMR1Wbranch);

			

        }

    }
    return(OK);
}
