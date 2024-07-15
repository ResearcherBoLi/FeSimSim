#include "ngspice/ngspice.h" 
#include "ngspice/cktdefs.h" 
#include "memi1defs.h" 
#include "ngspice/sperror.h" 
#include <math.h> 
#define STEP(x)  ((x)<0.0 ? 0 : 1) 
int 
MEMI1load(GENmodel *inModel, CKTcircuit *ckt) 
{ 


	MEMI1model *model = (MEMI1model *)inModel; 
	MEMI1instance *here; 
	
	int error; 
	double geq; 
	double ceq; 
	double req; 
	double veq; 
	int cond1; 
	double iind; 
    double w_meml;
	double F; 
	double window_func; 
    double dwindow_func_dw;
    double flux_meml;
    double dL_dw;
	double v_last_itr; 
	double i_last_itr;     
	if(ckt->CKTmode & (MODETRAN|MODEAC|MODETRANOP) ) { 
		/* evaluate device independent analysis conditions */ 
		cond1 = 
			( ( (ckt->CKTmode & MODEDC) && 
			    (ckt->CKTmode & MODEINITJCT) ) 
			||( (ckt->CKTmode & MODEUIC) && 
			    (ckt->CKTmode & MODEINITTRAN) ) ); 
		/*  loop through all the MEMI1 models */ 
		for( ; model != NULL; model = MEMI1nextModel(model)) { 
			/* loop through all the instances of the model */ 
			for (here = MEMI1instances(model); here != NULL ; 
					here=MEMI1nextInstance(here)) {
				if(cond1) { 
					iind = here->MEMI1initCond; 
					w_meml = here->MEMI1winit;
                    i_last_itr = 0;
                    v_last_itr = 0;                    
                    //here->MEMI1induc =(model->MEMI1defL_min - model->MEMI1defC_max)*here->MEMI1winit+model->MEMI1defC_max; 
					dL_dw = model->MEMI1defL_min - model->MEMI1defL_max;
                    here->MEMI1_dIdW = 0; 
                    F = 0;
                    here->MEMI1_dFdV = 0;
                    here->MEMI1_dFdI = 0;
                    here->MEMI1_dFdW = 0;

				} else { 
                    w_meml = *(ckt->CKTrhsOld + here->MEMI1Wbranch); 
					iind = *(ckt->CKTrhsOld + here->MEMI1Ibranch); 
					//limiting w range 
					if (w_meml>1){ 
						w_meml = 1; 
						*(ckt->CKTrhsOld + here->MEMI1Wbranch) = 1;
					} else if (w_meml<0){ 
						w_meml = 0; 
						*(ckt->CKTrhsOld + here->MEMI1Wbranch) = 0; 
					} 
					i_last_itr = *(ckt->CKTrhsOld + (here->MEMI1Ibranch)); 
					v_last_itr = *(ckt->CKTrhsOld + (here->MEMI1posNode)) -*(ckt->CKTrhsOld + (here->MEMI1negNode)); 
					here->MEMI1induc = (model->MEMI1defL_min - model->MEMI1defL_max)*w_meml+model->MEMI1defL_max;
                    dL_dw = model->MEMI1defL_min - model->MEMI1defL_max; 
					here->MEMI1_dIdW = dL_dw * i_last_itr * ckt->CKTag[0];
					//here->MEMI1_dIdW = -1*i_last_itr/here->MEMI1induc*dL_dw;
					if (model->MEMI1defWf =2){ 
						// take care of wf form from original paper due to the w selection part and current reference direction  
						window_func = 1-pow(w_meml-STEP(-1*v_last_itr), 2*(model->MEMI1defP) ); 
						dwindow_func_dw = -2*model->MEMI1defP*pow(w_meml-STEP(-1*v_last_itr),2*(model->MEMI1defP)-1);
					} 
                    flux_meml = model->MEMI1defK*here->MEMI1induc*i_last_itr;
					// F = K*L(w)*I*fw
                    F= flux_meml*window_func;
                    here->MEMI1_dFdV = 0;
					here->MEMI1_dFdW = flux_meml*dwindow_func_dw + F/here->MEMI1induc*dL_dw;
					here->MEMI1_dFdI = F/i_last_itr; 
				} 
				if(ckt->CKTmode & (MODETRAN | MODEAC)) {
				//if ((ckt->CKTmode & (MODEDCTRANCURVE | MODETRAN | MODEAC | MODEINITSMSIG)) || 
				//					((ckt->CKTmode & MODETRANOP) && (ckt->CKTmode & MODEUIC))) { 
#ifndef PREDICTOR 
					if(ckt->CKTmode & MODEINITPRED) { 

						*(ckt->CKTstate0+here->MEMI1flux) =
							*(ckt->CKTstate1+here->MEMI1flux);
						
						*(ckt->CKTstate0 + here->MEMI1qcapPseudo) = 
							*(ckt->CKTstate1 + here->MEMI1qcapPseudo); 

					} else { /* only const caps - no poly's */ 
#endif /* PREDICTOR */  
						*(ckt->CKTstate0+here->MEMI1flux) = here->MEMI1induc * iind;
						*(ckt->CKTstate0 + here->MEMI1qcapPseudo) = here->MEMI1capacPseudo * w_meml; 

						if((ckt->CKTmode & MODEINITTRAN)) { 
							*(ckt->CKTstate3+here->MEMI1flux) = 
								*(ckt->CKTstate0+here->MEMI1flux); 

							*(ckt->CKTstate1+here->MEMI1qcapPseudo) = 
								*(ckt->CKTstate0+here->MEMI1qcapPseudo); 
						} 
#ifndef PREDICTOR 
					} 
#endif /* PREDICTOR */ 
                    error=NIintegrate(ckt, &req, &veq, here->MEMI1induc, here->MEMI1flux);
                    if(error) return(error);
				    error=NIintegrate(ckt, &geq, &ceq, here->MEMI1capacPseudo, here->MEMI1qcapPseudo);
                    if(error) return(error); 
                    if(ckt->CKTmode & MODEINITTRAN) { 
                        *(ckt->CKTstate1+here->MEMI1ccapPseudo) = 
                            *(ckt->CKTstate0+here->MEMI1ccapPseudo); 
                        
                        *(ckt->CKTstate1+here->MEMI1volt) = 
                            *(ckt->CKTstate0+here->MEMI1volt); 
                    } 
                    *(here->MEMI1posIbrPtr) = 1; 
                    *(here->MEMI1negIbrPtr) = -1; 

                    *(here->MEMI1ibrPosPtr) = 1; 
                    
                    //*(here->MEMCap1ibrNegPtr) = -1*here->MEMCap1_dIdV; 
                    *(here->MEMI1ibrNegPtr) = -1; 
                    
                    *(here->MEMI1ibrIbrPtr) = -1*req; 
                    *(here->MEMI1ibrWbrPtr) = -1*here->MEMI1_dIdW; 
                    *(ckt->CKTrhs + (here->MEMI1Ibranch)) = veq - dL_dw*i_last_itr*w_meml*ckt->CKTag[0]; 

                    *(here->MEMI1wbrPosPtr) = -1* here->MEMI1_dFdV; 
                    *(here->MEMI1wbrNegPtr) = here->MEMI1_dFdV; 
                    *(here->MEMI1wbrIbrPtr) = -1* here->MEMI1_dFdI;
                    *(here->MEMI1wbrWbrPtr) = geq- here->MEMI1_dFdW; 
                    *(ckt->CKTrhs + (here->MEMI1Wbranch)) =-1*ceq + F - here->MEMI1_dFdW * w_meml- here->MEMI1_dFdV * v_last_itr - here->MEMI1_dFdI * i_last_itr;
                }else{
                    *(ckt->CKTstate0 + here->MEMI1flux) = here->MEMI1induc * iind;
                    *(ckt->CKTstate0 + here->MEMI1qcapPseudo) = here->MEMI1capacPseudo * w_meml;
                }			
			}				
		}
	}
	return(OK);
}