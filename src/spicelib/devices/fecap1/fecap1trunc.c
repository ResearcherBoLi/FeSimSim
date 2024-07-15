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
#include "fecap1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
FECAP1trunc(GENmodel *inModel, CKTcircuit *ckt, double *timeStep)
{
    FECAP1model *model = (FECAP1model*)inModel;
    FECAP1instance *here;

    for( ; model!= NULL; model = FECAP1nextModel(model)) {
        for(here = FECAP1instances(model); here != NULL ;
                here = FECAP1nextInstance(here)) {

            CKTterr(here->FECAP1qcap,ckt,timeStep);
        }
    }
    return(OK);
}
