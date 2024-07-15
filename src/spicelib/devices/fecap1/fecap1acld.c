/**********
Copyright 2024 Xidian University.	All rights reserved.
 Author: Bo Li	at Hangzhou Institute of Technology	 
 Modified: 2020/09/09  Bo Li
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
FECAP1acLoad(GENmodel *inModel, CKTcircuit *ckt)
{
    FECAP1model *model = (FECAP1model*)inModel;
    double val;
    double m;
    FECAP1instance *here;

    for( ; model != NULL; model = FECAP1nextModel(model)) {
        for( here = FECAP1instances(model); here != NULL;
                here = FECAP1nextInstance(here)) {

            m = here->FECAP1m;

            val = ckt->CKTomega * here->FECAP1capac;

            *(here->FECAP1posPosPtr +1) += m * val;
            *(here->FECAP1negNegPtr +1) += m * val;
            *(here->FECAP1posNegPtr +1) -= m * val;
            *(here->FECAP1negPosPtr +1) -= m * val;
        }
    }
    return(OK);

}

