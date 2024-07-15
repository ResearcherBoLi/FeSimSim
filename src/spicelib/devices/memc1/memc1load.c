/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
Modified: Apr 2000 - Paolo Nenzi
**********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memc1defs.h"
#include "ngspice/sperror.h"
#include <math.h>

#define STEP(x)  ((x)<0.0 ? 0 : 1)

/* actually load the current resistance value into the sparse matrix
 * previously provided */
int
MEMC1load(GENmodel *inModel, CKTcircuit *ckt)
{



    MEMC1model *model = (MEMC1model *)inModel;
	MEMC1instance *here;
	int error; 
	double geq; 
	double ceq; 
	double geq_pseudo; 
	double ceq_pseudo; 
	int cond1; 
	double vcap;
	double w_memc; 
	double F; 
	double window_func; 
	double dwindow_func_dw;
	double charge_memc;
	double delta_recip_cap;
	double dC_dw;
	double v_last_itr; 
	double i_last_itr; 
	if(ckt->CKTmode & (MODETRAN|MODEAC|MODETRANOP) ) { 
		/* evaluate device independent analysis conditions */ 
		cond1 = 
			( ( (ckt->CKTmode & MODEDC) && 
				(ckt->CKTmode & MODEINITJCT) ) 
			||( (ckt->CKTmode & MODEUIC) && 
				(ckt->CKTmode & MODEINITTRAN) ) ); 
		/*	loop through all the MEMCap1 models */ 
		for( ; model != NULL; model = MEMC1nextModel(model)) { 
			/* loop through all the instances of the model */ 
			for (here = MEMC1instances(model); here != NULL ; 
					here=MEMC1nextInstance(here)) {
				if(cond1) { 
					
					vcap =here->MEMC1initCond;
					w_memc = here->MEMC1winit; 
					i_last_itr = 0;
					v_last_itr = 0; 
					delta_recip_cap = (1/model->MEMC1defC_min - 1/model->MEMC1defC_max);
					here->MEMC1capac   = 1/(delta_recip_cap *w_memc+1/model->MEMC1defC_max);
					dC_dw =-1* here->MEMC1capac*here->MEMC1capac*delta_recip_cap;
					here->MEMC1_dIdW =0;
					// F= K*Q*fw=K*C*V*fw
					F=0;
					here->MEMC1_dFdV=0;
					here->MEMC1_dFdW=0;
					here->MEMC1_dFdI = 0;	

					//here->MEM2conduct =1/( model->MEM2defR_on * here->MEM2winit + model->MEM2defR_off* (1 - here->MEM2winit)); 
					//here->MEM2_dIdV = 1/( model->MEM2defR_on * here->MEM2winit + model->MEM2defR_off* (1 - here->MEM2winit)); 
					/*
					here->MEMCap1_dIdW = 0;
					F = 0;
					i_last_itr = 0;
					v_last_itr = 0;
					here->MEMCap1_dFdW = 0;
					here->MEMCap1_dFdV = 0;
					here->MEMCap1_dFdI = 0;
					*/
					//wvcap = here->MEM2winit;
					//*(here->MEM2wbrIbrPtr) = 0;
					
				} else { 
					w_memc = *(ckt->CKTrhsOld + here->MEMC1Wbranch); 
					vcap = *(ckt->CKTrhsOld + (here->MEMC1posNode)) -*(ckt->CKTrhsOld + (here->MEMC1negNode)); 
					//limiting w range 
					if (w_memc>1){ 
						w_memc = 1; 
						*(ckt->CKTrhsOld + here->MEMC1Wbranch) = 1;
					} else if (w_memc<0){ 
						w_memc = 0; 
						*(ckt->CKTrhsOld + here->MEMC1Wbranch) = 0; 
					} 

					i_last_itr = *(ckt->CKTrhsOld + (here->MEMC1Ibranch)); 
					v_last_itr = *(ckt->CKTrhsOld + (here->MEMC1posNode)) -*(ckt->CKTrhsOld + (here->MEMC1negNode)); 
					delta_recip_cap = (1/model->MEMC1defC_min - 1/model->MEMC1defC_max);
					here->MEMC1capac   = 1/(delta_recip_cap *w_memc+1/model->MEMC1defC_max);
					dC_dw =-1* here->MEMC1capac*here->MEMC1capac*delta_recip_cap;

					//here->MEMCap1_dIdV =	  here->MEMCap1capac *ckt->CKTag[0] ;					
					here->MEMC1_dIdW = dC_dw *v_last_itr*ckt->CKTag[0];


					if (model->MEMC1defWf =2){ 
						// take care of wf form from original paper due to the w selection part and current reference direction  
						window_func = 1-pow(w_memc-STEP(-1*v_last_itr), 2*(model->MEMC1defP) ); 
						dwindow_func_dw = -2*model->MEMC1defP*pow(w_memc-STEP(-1*v_last_itr),2*(model->MEMC1defP)-1);
					} 

					charge_memc = model->MEMC1defK*here->MEMC1capac*v_last_itr;
					// F= K*Q*fw=K*C*V*fw
					F= charge_memc*window_func;
					here->MEMC1_dFdV = F/v_last_itr;								
					here->MEMC1_dFdW = charge_memc*dwindow_func_dw-F*here->MEMC1capac*delta_recip_cap ; 					
					here->MEMC1_dFdI = 0;	


			
				}
			
				
				if(ckt->CKTmode & (MODETRAN | MODEAC)) {
				//if ((ckt->CKTmode & (MODEDCTRANCURVE | MODETRAN | MODEAC | MODEINITSMSIG)) || 
				//					((ckt->CKTmode & MODETRANOP) && (ckt->CKTmode & MODEUIC))) { 
#ifndef PREDICTOR 
					if(ckt->CKTmode & MODEINITPRED) { 
						
						*(ckt->CKTstate0+here->MEMC1qcap) =
							*(ckt->CKTstate1+here->MEMC1qcap);
						
						*(ckt->CKTstate0 + here->MEMC1qcapPseudo) = 
							*(ckt->CKTstate1 + here->MEMC1qcapPseudo); 
						
					} else { /* only const caps - no poly's */ 
#endif /* PREDICTOR */  
						*(ckt->CKTstate0+here->MEMC1qcap) = here->MEMC1capac * vcap;
						*(ckt->CKTstate0 + here->MEMC1qcapPseudo) = here->MEMC1capacPseudo * w_memc ; 

						if((ckt->CKTmode & MODEINITTRAN)) { 
							*(ckt->CKTstate1+here->MEMC1qcap) = 
								*(ckt->CKTstate0+here->MEMC1qcap); 

							*(ckt->CKTstate1+here->MEMC1qcapPseudo) = 
								*(ckt->CKTstate0+here->MEMC1qcapPseudo); 
						} 
#ifndef PREDICTOR 
					} 
#endif /* PREDICTOR */ 




	

					
					error=NIintegrate(ckt, &geq, &ceq, here->MEMC1capac, here->MEMC1qcap);
					if(error) return(error); 
					error=NIintegrate(ckt, &geq_pseudo, &ceq_pseudo, here->MEMC1capacPseudo, here->MEMC1qcapPseudo);
					if(error) return(error); 
					if(ckt->CKTmode & MODEINITTRAN) { 
						*(ckt->CKTstate1+here->MEMC1ccapPseudo) = 
							*(ckt->CKTstate0+here->MEMC1ccapPseudo); 
						
						*(ckt->CKTstate1+here->MEMC1ccap) = 
							*(ckt->CKTstate0+here->MEMC1ccap); 

							
					} 
					*(here->MEMC1posIbrPtr) = 1; 
					*(here->MEMC1negIbrPtr) = -1; 

					//*(here->MEMCap1ibrPosPtr) = here->MEMCap1_dIdV; 
					*(here->MEMC1ibrPosPtr) = geq; 
					
					//*(here->MEMCap1ibrNegPtr) = -1*here->MEMCap1_dIdV; 
					*(here->MEMC1ibrNegPtr) = -1*geq; 
					
					*(here->MEMC1ibrIbrPtr) = -1; 
					*(here->MEMC1ibrWbrPtr) = here->MEMC1_dIdW; 
					*(ckt->CKTrhs + (here->MEMC1Ibranch)) = -1*ceq + dC_dw*v_last_itr*w_memc*ckt->CKTag[0]; 

					*(here->MEMC1wbrPosPtr) = -1* here->MEMC1_dFdV; 
					*(here->MEMC1wbrNegPtr) = here->MEMC1_dFdV; 
					*(here->MEMC1wbrIbrPtr) = -1* here->MEMC1_dFdI;
					*(here->MEMC1wbrWbrPtr) = geq_pseudo- here->MEMC1_dFdW; 
					*(ckt->CKTrhs + (here->MEMC1Wbranch)) =-1*ceq_pseudo + F - here->MEMC1_dFdW * w_memc- here->MEMC1_dFdV * v_last_itr - here->MEMC1_dFdI * i_last_itr;
				}else{
					*(ckt->CKTstate0 + here->MEMC1qcap) = here->MEMC1capac * vcap;
					*(ckt->CKTstate0 + here->MEMC1qcapPseudo) = here->MEMC1capacPseudo * w_memc;
				}			
			}				
		}
	}
	return(OK);
}


