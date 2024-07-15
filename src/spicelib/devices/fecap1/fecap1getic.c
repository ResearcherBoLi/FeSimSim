/**********
Copyright 2024 Xidian University.	All rights reserved.
 Author: Bo Li	at Hangzhou Institute of Technology	 
 Modified: 2020/09/09  Bo Li
 Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "fecap1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"

int
FECAP1getic(GENmodel *inModel, CKTcircuit *ckt)

{

    FECAP1model *model = (FECAP1model*)inModel;
    FECAP1instance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = FECAP1nextModel(model)) {
        for(here = FECAP1instances(model); here ; here = FECAP1nextInstance(here)) {
                
            /*
            if(!here->FECAP1icVGiven) {
                here->FECAP1initCondV = 
                        *(ckt->CKTrhs + here->FECAP1posNode) - 
                        *(ckt->CKTrhs + here->FECAP1negNode);
            }*/
            if(!here->FECAP1_icGiven) {
                here->FECAP1initCondP = 
                        *(ckt->CKTrhs + here->FECAP1Pbranch);
            }

        }
    }
    return(OK);
}
