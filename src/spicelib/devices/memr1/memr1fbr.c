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
MEMR1findBr(CKTcircuit *ckt, GENmodel *inModel, IFuid name)
{
    MEMR1model *model = (MEMR1model *)inModel;
    MEMR1instance *here;
    int error;
    CKTnode *tmp;

    for( ; model != NULL; model = MEMR1nextModel(model)) {
        for (here = MEMR1instances(model); here != NULL;
                here = MEMR1nextInstance(here)) {
            if(here->MEMR1name == name) {
                if(here->MEMR1Ibranch == 0) {
                    error = CKTmkCur(ckt,&tmp,here->MEMR1name,"branch");
                    if(error) return(error);
                    here->MEMR1Ibranch = tmp->number;
                }
                return(here->MEMR1Ibranch);
            }
        }
    }
    return(0);
}
