/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/


#include "ngspice/ngspice.h"
#include "ngspice/const.h"
#include "memr2defs.h"
#include "ngspice/ifsim.h"
#include "ngspice/cktdefs.h"
#include "ngspice/sperror.h"


int
MEMR2ask(CKTcircuit *ckt, GENinstance *inst, int which, IFvalue *value,
       IFvalue *select)
{
    MEMR2instance *fast = (MEMR2instance *)inst;

    switch(which) {
        case MEMR2_WFLUXPSEUDO:
            value->rValue = *(ckt->CKTstate0+fast->MEMR2qcapPseudo);			
            return(OK);
		
        case MEMR2_WVOLTPSEUDO:
            value->rValue = *(ckt->CKTstate0+fast->MEMR2ccapPseudo);
			return(OK);
	
        case MEMR2_RINIT:
            value->rValue = fast->MEMR2rinit;
            return(OK);
		
        case MEMR2_WINIT:
            value->rValue = fast->MEMR2winit;
            return(OK);	
        case MEMR2_TRNOISE:
            value->iValue = fast->MEMR2doNoiseTran;
            return(OK);	                	
        /*
        case MEMR2_TRNOISE:
            temp = value->v.numValue = here->ISRCfunctionOrder;
            v = value->v.vec.rVec = TMALLOC(double, here->ISRCfunctionOrder);
            w = here->ISRCcoeffs;
            while (temp--)
                *v++ = *w++;
            return (OK);
        */            
	    default:
	        return(E_BADPARM);
    }
    /* NOTREACHED */
}
