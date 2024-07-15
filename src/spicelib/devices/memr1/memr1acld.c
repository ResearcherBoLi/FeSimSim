/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab  
Modified: 2020/09/09  Bo Li
Refered to NgSPICE  Res/Ind/Cap related file
**********/



#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memr1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"
#include <math.h>

#define STEP(x)  ((x)<0.0 ? 0 : 1)

/* actually load the current resistance value into the sparse matrix
 * previously provided */
int
MEMR1acLoad(GENmodel *inModel, CKTcircuit *ckt)
{

    MEMR1model *model = (MEMR1model *)inModel;
	MEMR1instance *here;
	NG_IGNORE(ckt);
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

    return(OK);
}



