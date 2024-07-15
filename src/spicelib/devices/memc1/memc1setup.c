/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
Modified: Apr 2000 Paolo Nenzi
**********/

#include "ngspice/ngspice.h"
#include "ngspice/smpdefs.h"
#include "memc1defs.h"
#include "ngspice/sperror.h"


int
MEMC1setup(SMPmatrix *matrix, GENmodel *inModel, CKTcircuit*ckt, int *states)
        /* load the memristor structure with those pointers needed later
         * for fast matrix loading
         */
{

    MEMC1model *model = (MEMC1model *)inModel;
    MEMC1instance *here;
    CKTnode *tmp;
    int error;

 
    NG_IGNORE(ckt);

    /*  loop through all the memristor models */
    for( ; model != NULL; model = MEMC1nextModel(model)) {

        /* Default Value Processing for Memristor Models */
        if(!model->MEMC1tnomGiven) model->MEMC1tnom         = ckt->CKTnomTemp;
		if(!model->MEMC1defWfGiven) model->MEMC1defWf  = 2;
		if(!model->MEMC1defPGiven) model->MEMC1defP  = 10;


		if(!model->MEMC1defC_minGiven) model->MEMC1defC_min = 10e-9; 
		if(!model->MEMC1defC_maxGiven) model->MEMC1defC_max = 10e-6; 
		if(!model->MEMC1defKGiven) model->MEMC1defK = 10e6; 
		if(!model->MEMC1defDGiven) model->MEMC1defD = 10e-9; 

		model->MEMC1defConstK = 1; 


        /* loop through all the instances of the model */
        for (here = MEMC1instances(model); here != NULL ;
                here=MEMC1nextInstance(here)) {
					
			here->MEMC1capacPseudo = 1/(model->MEMC1defConstK );
			here->MEMC1capac =1/( (1/model->MEMC1defC_min - 1/model->MEMC1defC_max)*here->MEMC1winit+1/model->MEMC1defC_max);		
			

       
            if(!here->MEMC1winitGiven)   here->MEMC1winit  =0.5;
			if(!here->MEMC1icGiven)   here->MEMC1initCond =0;
			//here->MEM1rinit = model->MEM1defRon * here->MEM1winit + model->MEM1defRoff* (1 - here->MEM1winit);
            //if(!here->MEM1pGiven)       here->MEM1p      = 10;

            if(here->MEMC1Ibranch == 0) {
                error = CKTmkCur(ckt,&tmp,here->MEMC1name,"ibranch");
                if(error) return(error);
                here->MEMC1Ibranch = tmp->number;
			}
			if(here->MEMC1Wbranch == 0) {
					error = CKTmkCur(ckt,&tmp,here->MEMC1name,"wbranch");
					if(error) return(error);
					here->MEMC1Wbranch = tmp->number;
			}
			
            here->MEMC1qcapPseudo  = *states;
            *states += MEMC1numStates;
			
/* macro to make elements with built in test for out of memory */
#define TSTALLOC(ptr,first,second) \
do { if((here->ptr = SMPmakeElt(matrix, here->first, here->second)) == NULL){\
    return(E_NOMEM);\
} } while(0)

            TSTALLOC(MEMC1posIbrPtr, MEMC1posNode, MEMC1Ibranch);
            TSTALLOC(MEMC1negIbrPtr, MEMC1negNode, MEMC1Ibranch);
			
            TSTALLOC(MEMC1ibrPosPtr, MEMC1Ibranch, MEMC1posNode);
            TSTALLOC(MEMC1ibrNegPtr, MEMC1Ibranch, MEMC1negNode);
            TSTALLOC(MEMC1ibrIbrPtr, MEMC1Ibranch, MEMC1Ibranch);
            TSTALLOC(MEMC1ibrWbrPtr, MEMC1Ibranch, MEMC1Wbranch);
			
            
            TSTALLOC(MEMC1wbrPosPtr, MEMC1Wbranch, MEMC1posNode);
            TSTALLOC(MEMC1wbrNegPtr, MEMC1Wbranch, MEMC1negNode);
			TSTALLOC(MEMC1wbrIbrPtr, MEMC1Wbranch, MEMC1Ibranch);
            TSTALLOC(MEMC1wbrWbrPtr, MEMC1Wbranch, MEMC1Wbranch);

			

        }

    }
    return(OK);
}
