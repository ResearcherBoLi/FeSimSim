/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/


#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"
#include "ngspice/trandefs.h"
#include "ngspice/1-f-code.h"
#include "memr1defs.h"

#define STEP(x)  ((x)<0.0 ? 0 : 1)

/* actually load the current resistance value into the sparse matrix
 * previously provided */

	/*
	BE				MNA:											RHS:
		n1		n2		i			w						
	n1| 				+1					 |			|												  |
	n2| 				-1					 |			|												  |
	ib | 1/m 	-1/m	-1			di/dw	 |			|di/dw * w^n(t) 								  |
	w | 				-dF/di	   1/h-dF/dw|			|w(t-h)/h + F^n(t) - dF/dw*w^n(t) - dF/di*i^n(t)|

	TR				MNA:													RHS:
		n1		n2		i			w						
	n1| 				+1						|			|															   |
	n2| 				-1						|			|															   |
	ib |1/m		-1/m	-1			di/dw		|			|di/dw * w^n(t) 											   |
	w | 				-h/2*dF/di	1-h/2*dF/dw |			|w(t-h)+h/2*F^n(t)-h/2*dF/dw*w^n(t)-h/2*dF/di*i^n(t)+h/2*F(t-h) |										


	*/


int
MEMR1load(GENmodel *inModel, CKTcircuit *ckt)
{

    MEMR1model *model = (MEMR1model *)inModel;
	MEMR1instance *here;

	int error;
	double simTime = ckt->CKTtime;
	double geq;
	double ceq;
//	double req;
	int cond1;
    double wvcap;

	double F;
	double wf;
	double i_last_itr; 
	
	/* check if capacitors are in the circuit or are open circuited */
    if(ckt->CKTmode & (MODETRAN|MODEAC|MODETRANOP) ) {
        /* evaluate device independent analysis conditions */
        cond1=
            ( ( (ckt->CKTmode & MODEDC) &&
                (ckt->CKTmode & MODEINITJCT) )
              || ( ( ckt->CKTmode & MODEUIC) &&
                   ( ckt->CKTmode & MODEINITTRAN) ) ) ;
        /*  loop through all the capacitor models */

	    for( ; model != NULL; model = MEMR1nextModel(model)) {

	        /* loop through all the instances of the model */
	        for (here = MEMR1instances(model); here != NULL ;
	                here=MEMR1nextInstance(here)) {

	            //if( ckt->CKTmode & MODEINITTRAN) {
				if(cond1) {
					wvcap = here->MEMR1winit;
	                //*(ckt->CKTstate0 + here->MEMqcapPseudo) = here->MEMcapacPseudo  * here->MEMwinit;
					//*(ckt->CKTstate1 + here->MEMqcapPseudo) = *(ckt->CKTstate0 + here->MEMqcapPseudo);
					//here->MEMR1conduct =1/( here->MEMR1rinit);
					here->MEMR1conduct = 1 / (model->MEMR1defRon * here->MEMR1winit + model->MEMR1defRoff * (1 - here->MEMR1winit));

					here->MEMR1_dIdW = 0;
					*(here->MEMR1wbrIbrPtr) = 0;
					
				} else {
					wvcap = *(ckt->CKTrhsOld + here->MEMR1Wbranch);
					// limiting w range
					if (wvcap >1){
						wvcap =1;
						*(ckt->CKTrhsOld + here->MEMR1Wbranch) = 1;
					} else if(wvcap < 0){
						wvcap = 0;
						*(ckt->CKTrhsOld + here->MEMR1Wbranch) = 0;
					}
	                // *(ckt->CKTstate0 + here->MEMqcapPseudo) = here->MEMcapacPseudo * (*(ckt->CKTrhsOld + here->MEMWbranch));
					here->MEMR1conduct=1/(model->MEMR1defRoff+(model->MEMR1defRon-model->MEMR1defRoff)*(*(ckt->CKTrhsOld + (here->MEMR1Wbranch))));

					i_last_itr = *(ckt->CKTrhsOld + (here->MEMR1Ibranch)); 
					
					
					here->MEMR1_dIdW = -1*(here->MEMR1conduct)*(model->MEMR1defRon-model->MEMR1defRoff)*i_last_itr;

					wf = 1-pow(wvcap-STEP(-1*i_last_itr), 2*(here->MEMR1p) );
					F = i_last_itr * wf;
					here->MEMR1_dFdI = wf ;
					//window_diff_w = -2*p*(w - stp(-1*i)) ** (2*p-1)	
					here->MEMR1_dFdW = i_last_itr * (-2) * here->MEMR1p*pow(wvcap-STEP(-1*i_last_itr), 2*(here->MEMR1p) -1);

	            }

				if ((ckt->CKTmode & (MODEDCTRANCURVE | MODETRAN | MODEAC | MODEINITSMSIG)) ||
									((ckt->CKTmode & MODETRANOP) && (ckt->CKTmode & MODEUIC))) {

//				if(ckt->CKTmode & (MODETRAN | MODEDCOP)) {
#ifndef PREDICTOR
					if(ckt->CKTmode & MODEINITPRED) {
						*(ckt->CKTstate0 + here->MEMR1qcapPseudo) =
							*(ckt->CKTstate1 + here->MEMR1qcapPseudo);
					} else { /* only const caps - no poly's */

#endif /* PREDICTOR */
						*(ckt->CKTstate0 + here->MEMR1qcapPseudo) = here->MEMR1capacPseudo * wvcap;
						if((ckt->CKTmode & MODEINITTRAN)) {
							*(ckt->CKTstate1+here->MEMR1qcapPseudo) =
								*(ckt->CKTstate0+here->MEMR1qcapPseudo);
						}
#ifndef PREDICTOR
					}
#endif /* PREDICTOR */


				//*(ckt->CKTstate0 + here->MEMqcapPseudo) = here->MEMcapacPseudo * wvcap;
					error=NIintegrate(ckt, &geq, &ceq, here->MEMR1capacPseudo, here->MEMR1qcapPseudo);
					if(error) return(error);
					if(ckt->CKTmode & MODEINITTRAN) {
						*(ckt->CKTstate1+here->MEMR1ccapPseudo) =
							*(ckt->CKTstate0+here->MEMR1ccapPseudo);
					}


					*(here->MEMR1posIbrPtr) = 1;
					*(here->MEMR1negIbrPtr) = -1;


                    // add tran noise
                    if (here->MEMR1doNoiseTran)
                    {
                        double value=0;
                        double time = ckt->CKTtime;
						
					#ifdef  _USE_NG_TRNOISE_
					  #ifdef  _USE_VSRC_NOISE_
						double TnoiAmp = sqrt(4*BOLTZMANN*ckt->CKTtemp /here->MEMR1conduct *  model->MEMR1freqBw);
						double FnoiAmp = model->MEMR1kf/here->MEMR1conduct*i_last_itr*i_last_itr;
						double FnoiAlpha = 1+wvcap;					  
						struct trnoise_state *state = here -> MEMR1trnoise_state;							
						double TS = state -> TS;
						double RTSAM = state->RTSAM;
						state->NA = TnoiAmp;
						state->NAMP = FnoiAmp;
						state->NALPHA= FnoiAlpha ;
				
						/* reset top (hack for repeated tran commands)
						when there is the jump from time=0 to time>0 */
						if (time == 0.0)
						 state->timezero = TRUE;
						else
						 if (state->timezero) {
							 state->top = 0;
							 state->timezero = FALSE;
						 }
				
						/* no noise or time == 0 */
						if (TS == 0.0 || time == 0.0) {
						 value = 0.0;
						}
						else {
				
						 /* 1/f and white noise */
						 size_t n1 = (size_t)floor(time / TS);
				
						 double V1 = trnoise_state_get(state, ckt, n1);
						 double V2 = trnoise_state_get(state, ckt, n1 + 1);
				
						 value = V1 + (V2 - V1) * (time / TS - (double)n1);
						}
				
						/* RTS noise */
						if (RTSAM > 0) {
						 double RTScapTime = state->RTScapTime;
						 if (time >= RTScapTime)
							 value += RTSAM;
						}

						*(here->MEMR1noiseVsrcPosIbrPtr) += 1.0 ;
			            *(here->MEMR1noiseVsrcNegIbrPtr) -= 1.0 ;
			            *(here->MEMR1noiseVsrcIbrPosPtr) += 1.0 ;
			            *(here->MEMR1noiseVsrcIbrNegPtr) -= 1.0 ;				

						*(ckt->CKTrhs + (here->MEMR1noiseVsrcIbranch)) = value;

					  #else
					  	struct trnoise_state *state = here -> MEMR1trnoise_state;
					  	double TnoiAmp=0 ;
						double FnoiAmp=0;
						double FnoiAlpha=0;
						double tmpTS; 
						if(model->MEMR1tnoiModGiven){
							TnoiAmp = sqrt(4*BOLTZMANN*ckt->CKTtemp* here->MEMR1conduct *  model->MEMR1freqBw);
						}
						
						if(model->MEMR1fnoiModGiven){
							FnoiAmp = model->MEMR1kf/here->MEMR1conduct*i_last_itr*i_last_itr;
							FnoiAlpha = 1+wvcap;
						}

						
						if(!model->MEMR1noiseTimeStepGiven){
							if(ckt->CKTdelta>0){
								tmpTS = MIN(ckt->CKTdelta,  ckt->CKTstep);
							}else{
								tmpTS = ckt->CKTstep;
							}
							state->TS = tmpTS;
						}
						
						
						double TS = state -> TS;
						double RTSAM = state->RTSAM;
						state->NA = TnoiAmp;
						state->NAMP = FnoiAmp;
						state->NALPHA= FnoiAlpha;

						/* reset top (hack for repeated tran commands)
						when there is the jump from time=0 to time>0 */
						if (time == 0.0)
						 state->timezero = TRUE;
						else
						 if (state->timezero) {
							 state->top = 0;
							 state->timezero = FALSE;
						 }

						/* no noise or time == 0 */
						if (TS == 0.0 || time == 0.0) {
						 value = 0.0;
						}
						else {

						 /* 1/f and white noise */
						 size_t n1 = (size_t)floor(time / TS);

						 double V1 = trnoise_state_get(state, ckt, n1);
						 double V2 = trnoise_state_get(state, ckt, n1 + 1);

						 value = V1 + (V2 - V1) * (time / TS - (double)n1);
						}

						/* RTS noise */
						if (RTSAM > 0) {
						 double RTScapTime = state->RTScapTime;
						 if (time >= RTScapTime)
							 value += RTSAM;
						}
	                    *(ckt->CKTrhs + (here->MEMR1posNode)) += value;
	                    *(ckt->CKTrhs + (here->MEMR1negNode)) -= value;						
					  #endif

					#else
						if (time != 0.0){
							
							double ThermalNoiseRandom = 0.01;     // thermal noise standard noise distribution
							value = ThermalNoiseAmpli * ThermalNoiseRandom;
						}
					#endif
                    }




					*(here->MEMR1ibrPosPtr) = here->MEMR1conduct;
					*(here->MEMR1ibrNegPtr) = -1*here->MEMR1conduct;
					*(here->MEMR1ibrIbrPtr) = -1;
					*(here->MEMR1ibrWbrPtr) = here->MEMR1_dIdW;			
					*(ckt->CKTrhs + (here->MEMR1Ibranch)) = (here->MEMR1_dIdW) *(* (ckt->CKTrhsOld + (here->MEMR1Wbranch)));									

					*(here->MEMR1wbrIbrPtr) = -1* here->MEMR1_dFdI;
					*(here->MEMR1wbrWbrPtr) = geq- here->MEMR1_dFdW;					
					*(ckt->CKTrhs + (here->MEMR1Wbranch)) =-1*ceq + F - here->MEMR1_dFdW * wvcap - here->MEMR1_dFdI * i_last_itr;	
				
				}else{
					*(ckt->CKTstate0 + here->MEMR1qcapPseudo) = here->MEMR1capacPseudo * wvcap;
				}			
	        }
					
	    }

    }


	else{
	//else if(ckt->CKTmode & (MODEDC|MODEDCOP|MODEINITJCT|MODEAC|MODEINITFLOAT) ) { 
		/* evaluate device independent analysis conditions */ 
		/*  loop through all the MEM2 models */ 
		for( ; model != NULL; model = MEMR1nextModel(model)) { 
			/* loop through all the instances of the model */ 
			for (here = MEMR1instances(model); here != NULL ; 
					here=MEMR1nextInstance(here)) {
				//wvcap = here->MEM2winit; 				
				//here->MEM2conduct =1/( model->MEM2defR_off * here->MEM2winit + model->MEM2defR_on* (1 - here->MEM2winit)); 
				here->MEMR1conduct = 1/( model->MEMR1defRoff * here->MEMR1winit + model->MEMR1defRon* (1 - here->MEMR1winit)); 
				/* 
				here->MEM2_dIdW = 0;
				F = 0;
				i_last_itr = 0;
				v_last_itr = 0;
				here->MEM2_dFdW = 0;
				here->MEM2_dFdV = 0;
				here->MEM2_dFdI = 0;	
				*/			

				*(here->MEMR1posIbrPtr) = 1; 
				*(here->MEMR1negIbrPtr) = -1; 
				
				*(here->MEMR1ibrPosPtr) = here->MEMR1conduct ; 
				*(here->MEMR1ibrNegPtr) = -1*here->MEMR1conduct ; 
				*(here->MEMR1ibrIbrPtr) = -1; 
				//*(here->MEM2ibrWbrPtr) = here->MEM2_dIdW;
				*(here->MEMR1ibrWbrPtr) = 0;
				
				//*(ckt->CKTrhs + (here->MEM2Ibranch)) = (here->MEM2_dIdW) *wvcap; 
				*(ckt->CKTrhs + (here->MEMR1Ibranch)) = 0;
				//*(here->MEM2wbrPosPtr) = -1* here->MEM2_dFdV; 
				//*(here->MEMwbrPosPtr) = 0;
				//*(here->MEM2wbrNegPtr) = here->MEM2_dFdV; 
				//*(here->MEMwbrNegPtr) = 0;
				//*(here->MEM2wbrIbrPtr) = -1* here->MEM2_dFdI;
				*(here->MEMR1wbrIbrPtr) = 0;
				//*(here->MEM2wbrWbrPtr) = geq- here->MEM2_dFdW;
				*(here->MEMR1wbrWbrPtr) = 1;  
				*(ckt->CKTrhs + (here->MEMR1Wbranch))= here->MEMR1winit;

			}
		}
	}

    return(OK);
}



