/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memr1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
MEMR1trunc(GENmodel *inModel, CKTcircuit *ckt, double *timeStep)
{
    MEMR1model *model = (MEMR1model*)inModel;
    MEMR1instance *here;
    for( ; model!= NULL; model = MEMR1nextModel(model)) {
        for(here = MEMR1instances(model); here != NULL ;
                here = MEMR1nextInstance(here)) {
	
            CKTterr(here->MEMR1qcapPseudo, ckt, timeStep);
			//CKTterr(here->MEMinternalState, ckt, timeStep);
        }
    }
    return(OK);
}
