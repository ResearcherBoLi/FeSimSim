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
MEMR2findBr(CKTcircuit *ckt, GENmodel *inModel, IFuid name)
{
    MEMR2model *model = (MEMR2model *)inModel;
    MEMR2instance *here;
    int error;
    CKTnode *tmp;

    for( ; model != NULL; model = MEMR2nextModel(model)) {
        for (here = MEMR2instances(model); here != NULL;
                here = MEMR2nextInstance(here)) {
            if(here->MEMR2name == name) {
                if(here->MEMR2Ibranch == 0) {
                    error = CKTmkCur(ckt, &tmp, here->MEMR2name, "branch");
                    if(error) return(error);
                    here->MEMR2Ibranch = tmp->number;
                }
                return(here->MEMR2Ibranch);
            }
        }
    }
    return(0);
}
