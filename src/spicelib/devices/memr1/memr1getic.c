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
MEMR1getic(GENmodel *inModel, CKTcircuit *ckt)

{

    MEMR1model *model = (MEMR1model*)inModel;
    MEMR1instance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = MEMR1nextModel(model)) {
        for(here = MEMR1instances(model); here ; here = MEMR1nextInstance(here)) {
                
            if(!here->MEMR1winitGiven) {
                here->MEMR1winit= *(ckt->CKTrhsOld + here->MEMR1Wbranch);
            }
        }
    }
    return(OK);
}

