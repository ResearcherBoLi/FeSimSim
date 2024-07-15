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

int
MEMR2getic(GENmodel *inModel, CKTcircuit *ckt)

{

    MEMR2model *model = (MEMR2model*)inModel;
    MEMR2instance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = MEMR2nextModel(model)) {
        for(here = MEMR2instances(model); here ; here = MEMR2nextInstance(here)) {
                
            if(!here->MEMR2winitGiven) {
                here->MEMR2winit= *(ckt->CKTrhsOld + here->MEMR2Wbranch);
            }
        }
    }
    return(OK);
}

