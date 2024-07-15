/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/05/20  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

 //////////////////////////////////
// TCAD 2020 Yakopcic
// Yakopcic
// Memristor Model Optimization Based on Parameter Extraction From Device Characterization Data
// device equations
// I-V
// i(t) = h1(V(t))x(t)+h2(V(t))(1-x(t))
// option 1:  h1= y * sinh(kV(t))  h2= a(1-exp(-bV(t))) 
// option 2:  h1=GmaxV(t)          h2= Gmin*sinh(bV(t))   
// state variable equation
// dx/dt = ng(V(t))f(x(t))
//           [ Ap(exp(V(t))-exp(Vp))  ; V(t)>Vp
// g(V(t)) = [-An(exp(-V(t))-exp(Vn)) ; V(t)<-Vn
//           [ 0                      ; -Vn<=V(t)<=Vp
// n = 1 or -1
// when nV(t) > 0
// f (x) = [ exp(-(x-xp))wp(x,xp)   ; x>=xp
//         [ 1                      ; x< xp
// when nV(t) <= 0
// f (x) = [ exp(-(1-x-xn))wn(x,xn) ; x<=1-xn
//         [ 1                      ; x> 1-xn
// 0 < x <1
// wp(x,xp) = (xp-x)/(1-xp)+1 = (1-x)/(1-xp)
// wn(x,xn) = x/(1-xn)
//
// parameters : n Ap An Vp Vn xp xn 

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memr3defs.h"
#include "ngspice/sperror.h"
#include <math.h>

#define STEP(x)  ((x)<0.0 ? 0 : 1)

/* actually load the current resistance value into the sparse matrix
 * previously provided */
int
MEMR3load(GENmodel *inModel, CKTcircuit *ckt)
{



    MEMR3model *model = (MEMR3model *)inModel;
	MEMR3instance *here;

	int error; 
	double geq; 
	double ceq; 
	int cond1; 
	double wvcap; 
	double F; 
	double wf;   //window function term 
	double ff;
	double gf;        
	double v_last_itr; 
	double i_last_itr; 
	if(ckt->CKTmode & (MODETRAN|MODEAC|MODETRANOP) ) { 
		/* evaluate device independent analysis conditions */ 
		cond1 = 
			( ( (ckt->CKTmode & MODEDC) && 
				(ckt->CKTmode & MODEINITJCT) ) 
			||( (ckt->CKTmode & MODEUIC) && 
				(ckt->CKTmode & MODEINITTRAN) ) ) ;
		/*	loop through all the MEM3 models */ 
		for( ; model != NULL; model = MEMR3nextModel(model)) { 
			/* loop through all the instances of the model */ 
			for (here = MEMR3instances(model); here != NULL ; 
					here=MEMR3nextInstance(here)) {
				if(cond1) { 
					wvcap = here->MEMR3winit; 
					
					//here->MEMR3conduct =1/( model->MEMR2defR_off * here->MEMR2winit + model->MEMR2defR_on* (1 - here->MEMR2winit)); 
					here->MEMR3_dIdV = model->MEMR3defGmax*wvcap+ model->MEMR3defGmin *(1-wvcap)*model->MEMR3defb ; 
					here->MEMR3_dIdW = 0;
					F = 0;
					i_last_itr = 0;
					v_last_itr = 0;
					here->MEMR3_dFdW = 0;
					here->MEMR3_dFdV = 0;
					here->MEMR3_dFdI = 0;
					//wvcap = here->MEM2winit;
					//*(here->MEM2wbrIbrPtr) = 0;
					
				} else { 
					wvcap = *(ckt->CKTrhsOld + here->MEMR3Wbranch); 
					//limiting w range 
					if (wvcap>1){ 
						wvcap = 1; 
						*(ckt->CKTrhsOld + here->MEMR3Wbranch) = 1;
					} else if (wvcap<0){ 
						wvcap = 0; 
						*(ckt->CKTrhsOld + here->MEMR3Wbranch) = 0; 
					} 
					i_last_itr = *(ckt->CKTrhsOld + (here->MEMR3Ibranch)); 
					v_last_itr = *(ckt->CKTrhsOld + (here->MEMR3posNode)) -*(ckt->CKTrhsOld + (here->MEMR3negNode)); 
					here->MEMR3_dIdV=  model->MEMR3defGmax*wvcap \
									   +model->MEMR3defGmin *(1-wvcap)*model->MEMR3defb *cosh(model->MEMR3defb*v_last_itr);
					here->MEMR3_dIdW = model->MEMR3defGmax *v_last_itr-model->MEMR3defGmin*sinh(model->MEMR3defb*v_last_itr);

					if (v_last_itr > model->MEMR3defVthp){
						gf = model->MEMR3defAp *(exp(v_last_itr)-exp(model->MEMR3defVthp));
						if(wvcap >= model->MEMR3defXp){
							ff = exp(-1*(wvcap-model->MEMR3defXp));
							wf =ff *(1-wvcap) /(1-model->MEMR3defXp); 

							F = gf * wf;						
							here->MEMR3_dFdV = model->MEMR3defAp *exp(v_last_itr) *wf;						
							here->MEMR3_dFdW = gf* (-1*wf -ff/(1-model->MEMR3defXp));					
							here->MEMR3_dFdI = 0;
						} else {
							//ff = 1;
							wf =1; 

							F = gf * wf;						
							here->MEMR3_dFdV = model->MEMR3defAp *exp(v_last_itr) ;						
							here->MEMR3_dFdW = 0;					
							here->MEMR3_dFdI = 0;						
						}
					}else if (v_last_itr < model->MEMR3defVthn){
						gf =-1* model->MEMR3defAn *(exp(-1*v_last_itr)-exp(-1*model->MEMR3defVthn));
						if(wvcap <= (1-model->MEMR3defXn)){
							ff = exp(-1*(1-model->MEMR3defXn-wvcap));
							wf =ff *wvcap /(1-model->MEMR3defXn); 
						
							F = gf * wf;						
							here->MEMR3_dFdV = model->MEMR3defAn *exp(-1*v_last_itr) *wf;						
							here->MEMR3_dFdW = gf* (wf +ff/(1-model->MEMR3defXn));					
							here->MEMR3_dFdI = 0;
						} else {
							//ff = 1;
							wf =1; 
						
							F = gf * wf;						
							here->MEMR3_dFdV = model->MEMR3defAn *exp(-1*v_last_itr);						
							here->MEMR3_dFdW = 0;					
							here->MEMR3_dFdI = 0;
						}

					}else{
						F= 0;
						here->MEMR3_dFdW = 0  ;
						here->MEMR3_dFdV = 0 ;
						here->MEMR3_dFdI = 0;					
					}
				} 
				
				if(ckt->CKTmode & (MODETRAN | MODEAC)) {
				//if ((ckt->CKTmode & (MODEDCTRANCURVE | MODETRAN | MODEAC | MODEINITSMSIG)) || 
				//					((ckt->CKTmode & MODETRANOP) && (ckt->CKTmode & MODEUIC))) { 
#ifndef PREDICTOR 
					if(ckt->CKTmode & MODEINITPRED) { 
						*(ckt->CKTstate0 + here->MEMR3qcapPseudo) = 
							*(ckt->CKTstate1 + here->MEMR3qcapPseudo); 
					} else { /* only const caps - no poly's */ 
#endif /* PREDICTOR */  
							*(ckt->CKTstate0 + here->MEMR3qcapPseudo) = here->MEMR3capacPseudo * wvcap; 
						if((ckt->CKTmode & MODEINITTRAN)) { 
							*(ckt->CKTstate1+here->MEMR3qcapPseudo) = 
								*(ckt->CKTstate0+here->MEMR3qcapPseudo); 
						} 
#ifndef PREDICTOR 
					} 
#endif /* PREDICTOR */ 
					error=NIintegrate(ckt, &geq, &ceq, here->MEMR3capacPseudo, here->MEMR3qcapPseudo);
					if(error) return(error); 
					if(ckt->CKTmode & MODEINITTRAN) { 
						*(ckt->CKTstate1+here->MEMR3ccapPseudo) = 
							*(ckt->CKTstate0+here->MEMR3ccapPseudo); 
					} 
					*(here->MEMR3posIbrPtr) = 1; 
					*(here->MEMR3negIbrPtr) = -1; 

					*(here->MEMR3ibrPosPtr) = here->MEMR3_dIdV; 
					*(here->MEMR3ibrNegPtr) = -1*here->MEMR3_dIdV; 
					*(here->MEMR3ibrIbrPtr) = -1; 
					*(here->MEMR3ibrWbrPtr) = here->MEMR3_dIdW; 
					*(ckt->CKTrhs + (here->MEMR3Ibranch)) = (here->MEMR3_dIdW) *wvcap+(here->MEMR3_dIdV) *v_last_itr-i_last_itr; 

					*(here->MEMR3wbrPosPtr) = -1* here->MEMR3_dFdV; 
					*(here->MEMR3wbrNegPtr) = here->MEMR3_dFdV; 
					*(here->MEMR3wbrIbrPtr) = -1* here->MEMR3_dFdI;
					*(here->MEMR3wbrWbrPtr) = geq- here->MEMR3_dFdW; 
					*(ckt->CKTrhs + (here->MEMR3Wbranch)) =-1*ceq + F - here->MEMR3_dFdW * wvcap - here->MEMR3_dFdV * v_last_itr - here->MEMR3_dFdI * i_last_itr;
				}else{
					*(ckt->CKTstate0 + here->MEMR3qcapPseudo) = here->MEMR3capacPseudo * wvcap;
				}			
			}
					
		}

	}
	else{
	//else if(ckt->CKTmode & (MODEDC|MODEDCOP|MODEINITJCT|MODEAC|MODEINITFLOAT) ) { 
		/* evaluate device independent analysis conditions */ 
		/*	loop through all the MEM2 models */ 
		for( ; model != NULL; model = MEMR3nextModel(model)) { 
			/* loop through all the instances of the model */ 
			for (here = MEMR3instances(model); here != NULL ; 
				here=MEMR3nextInstance(here)) {
				wvcap = here->MEMR3winit; 
				
				//here->MEMR3conduct =1/( model->MEMR2defR_off * here->MEMR2winit + model->MEMR2defR_on* (1 - here->MEMR2winit)); 
				here->MEMR3_dIdV = model->MEMR3defGmax*wvcap+ model->MEMR3defGmin *(1-wvcap)*model->MEMR3defb ; 		
				//here->MEMR3_dIdW = 0;





				*(here->MEMR3posIbrPtr) = 1; 
				*(here->MEMR3negIbrPtr) = -1; 
				
				*(here->MEMR3ibrPosPtr) = here->MEMR3_dIdV; 
				*(here->MEMR3ibrNegPtr) = -1*here->MEMR3_dIdV; 
				*(here->MEMR3ibrIbrPtr) = -1; 
				//*(here->MEMR3ibrWbrPtr) = here->MEMR3_dIdW;
				*(here->MEMR3ibrWbrPtr) = 0;
				
				//*(ckt->CKTrhs + (here->MEM2Ibranch)) = (here->MEM2_dIdW) *wvcap; 
				*(ckt->CKTrhs + (here->MEMR3Ibranch)) = 0;
				//*(here->MEM2wbrPosPtr) = -1* here->MEM2_dFdV; 
				*(here->MEMR3wbrPosPtr) = 0;
				//*(here->MEM2wbrNegPtr) = here->MEM2_dFdV; 
				*(here->MEMR3wbrNegPtr) = 0;
				//*(here->MEM2wbrIbrPtr) = -1* here->MEM2_dFdI;
				*(here->MEMR3wbrIbrPtr) = 0;
				//*(here->MEM2wbrWbrPtr) = geq- here->MEM2_dFdW;
				*(here->MEMR3wbrWbrPtr)  =1;
				*(ckt->CKTrhs + (here->MEMR3Wbranch)) = here->MEMR3winit;

		}
	}
}
	return(OK);

}



