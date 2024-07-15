/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h" 
#include "ngspice/cktdefs.h" 
#include "memr2defs.h" 
#include "ngspice/sperror.h" 
#include "ngspice/suffix.h"

#include <math.h> 
#define STEP(x)  ((x)<0.0 ? 0 : 1) 
int 
MEMR2acLoad(GENmodel *inModel, CKTcircuit *ckt) 
{ 


	MEMR2model *model = (MEMR2model *)inModel; 
	MEMR2instance *here; 
	NG_IGNORE(ckt);
	for( ; model != NULL; model = MEMR2nextModel(model)) { 
		/* loop through all the instances of the model */ 
		for (here = MEMR2instances(model); here != NULL ; 
				here=MEMR2nextInstance(here)) {
			//wvcap = here->MEM2winit;				
			//here->MEM2conduct =1/( model->MEM2defR_off * here->MEM2winit + model->MEM2defR_on* (1 - here->MEM2winit)); 
			here->MEMR2_dIdV = 1/( model->MEMR2defR_off * here->MEMR2winit + model->MEMR2defR_on* (1 - here->MEMR2winit)); 
			/* 
			here->MEM2_dIdW = 0;
			F = 0;
			i_last_itr = 0;
			v_last_itr = 0;
			here->MEM2_dFdW = 0;
			here->MEM2_dFdV = 0;
			here->MEM2_dFdI = 0;	
			*/			
	
			*(here->MEMR2posIbrPtr) = 1; 
			*(here->MEMR2negIbrPtr) = -1; 
			
			*(here->MEMR2ibrPosPtr) = here->MEMR2_dIdV; 
			*(here->MEMR2ibrNegPtr) = -1*here->MEMR2_dIdV; 
			*(here->MEMR2ibrIbrPtr) = -1; 
			//*(here->MEM2ibrWbrPtr) = here->MEM2_dIdW;
			*(here->MEMR2ibrWbrPtr) = 0;
			
			//*(ckt->CKTrhs + (here->MEM2Ibranch)) = (here->MEM2_dIdW) *wvcap; 
			*(ckt->CKTrhs + (here->MEMR2Ibranch)) = 0;
			//*(here->MEM2wbrPosPtr) = -1* here->MEM2_dFdV; 
			*(here->MEMR2wbrPosPtr) = 0;
			//*(here->MEM2wbrNegPtr) = here->MEM2_dFdV; 
			*(here->MEMR2wbrNegPtr) = 0;
			//*(here->MEM2wbrIbrPtr) = -1* here->MEM2_dFdI;
			*(here->MEMR2wbrIbrPtr) = 0;
			//*(here->MEM2wbrWbrPtr) = geq- here->MEM2_dFdW;
			*(here->MEMR2wbrWbrPtr)	=1;
			*(ckt->CKTrhs + (here->MEMR2Wbranch)) = here->MEMR2winit;
	
		}
	}

    return(OK);
}	
