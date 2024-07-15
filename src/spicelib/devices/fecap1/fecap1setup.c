/**********
Copyright 2024 Xidian University
Author: 2024 Bo Li
Modified: 2024/01/27  Bo Li
Refered to NgSPICE Res/Cap related file
**********/
/*
 */

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "ngspice/smpdefs.h" 
#include "fecap1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"
#include "ngspice/1-f-code.h"
#include <math.h>


/*ARGSUSED*/
int
FECAP1setup(SMPmatrix *matrix, GENmodel *inModel, CKTcircuit *ckt, int *states)
        /* load the capacitor structure with those pointers needed later
         * for fast matrix loading
         */

{
    FECAP1model *model = (FECAP1model*)inModel;
    FECAP1instance *here;
	CKTnode *tmp; 
	int error; 
	NG_IGNORE(ckt);

	double epsn = 29 * (8.854e-12);
	double k = 8.987e9;
	double pi = M_PI;
    /*  loop through all the capacitor models */
    for( ; model != NULL; model = FECAP1nextModel(model)) {

        /*Default Value Processing for Model Parameters */

        
        if (!model->FECAP1_ThicknessGiven) model->FECAP1_Thickness = 1e-9;
				
        if (!model->FECAP1_PsGiven) model->FECAP1_Ps = 27.54;	
		
        if (!model->FECAP1_EcGiven) model->FECAP1_Ec = 1.175;

		if (!model->FECAP1_AlphaGiven) model->FECAP1_Alpha =18;

        if (!model->FECAP1_BetaGiven) model->FECAP1_Beta = 0.0;
		
        if (!model->FECAP1_GammaGiven) model->FECAP1_Gamma = 0.0;

        if (!model->FECAP1_RhoGiven) model->FECAP1_Rho = 0.0;

        if (!model->FECAP1_TnomGiven)  model->FECAP1_Tnom = ckt->CKTnomTemp;


        /* loop through all the instances of the model */
        for (here = FECAP1instances(model); here != NULL ;
                here=FECAP1nextInstance(here)) {

			if (!here->FECAP1_AreaGiven) here->FECAP1_Area = 1e-14;
			if (!here->FECAP1mGiven) here->FECAP1m = 1;
			if (!here->FECAP1_icGiven) here->FECAP1initCondP = -20e-2;
			//if(!here->FECAP1temp)
			here->FECAP1capac =  epsn * here->FECAP1_Area/(4*k*pi*model->FECAP1_Thickness);
			here->FECAP1capacPseudo = 1;
			if(here->FECAP1Ibranch == 0) { 
				error = CKTmkCur(ckt,&tmp,here->FECAP1name,"ibranch"); 
				if(error) return(error); 
				here->FECAP1Ibranch = tmp->number; 
			} 


			if(here->FECAP1Pbranch == 0) { 
				error = CKTmkCur(ckt,&tmp,here->FECAP1name,"pbranch"); 
				if(error) return(error); 
				here->FECAP1Pbranch = tmp->number; 
			} 

            here->FECAP1qcap = *states;          
			//here->FECAP1pcap = *states; 
            *states += FECAP1numStates;
			//*states += FECAP1numStates; 


/* macro to make elements with built in test for out of memory */
#define TSTALLOC(ptr,first,second) \
do { if((here->ptr = SMPmakeElt(matrix, here->first, here->second)) == NULL){\
    return(E_NOMEM);\
} } while(0)

            TSTALLOC(FECAP1posPosPtr,FECAP1posNode,FECAP1posNode);
			TSTALLOC(FECAP1posNegPtr,FECAP1posNode,FECAP1negNode);
			TSTALLOC(FECAP1posIbrPtr, FECAP1posNode, FECAP1Ibranch); 

            TSTALLOC(FECAP1negPosPtr,FECAP1negNode,FECAP1posNode);
            TSTALLOC(FECAP1negNegPtr,FECAP1negNode,FECAP1negNode);            
			TSTALLOC(FECAP1negIbrPtr, FECAP1negNode, FECAP1Ibranch); 
			
			TSTALLOC(FECAP1ibrPosPtr, FECAP1Ibranch, FECAP1posNode); 
			TSTALLOC(FECAP1ibrNegPtr, FECAP1Ibranch, FECAP1negNode); 
			TSTALLOC(FECAP1ibrIbrPtr, FECAP1Ibranch, FECAP1Ibranch); 
			TSTALLOC(FECAP1ibrPbrPtr, FECAP1Ibranch, FECAP1Pbranch); 
			
			TSTALLOC(FECAP1pbrPosPtr, FECAP1Pbranch, FECAP1posNode); 
			TSTALLOC(FECAP1pbrNegPtr, FECAP1Pbranch, FECAP1negNode); 
			TSTALLOC(FECAP1pbrIbrPtr, FECAP1Pbranch, FECAP1Ibranch); 
			TSTALLOC(FECAP1pbrPbrPtr, FECAP1Pbranch,FECAP1Pbranch); 


        }
    }
    return(OK);
}

