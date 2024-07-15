/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/05/20  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memr3defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
MEMR3findBr(CKTcircuit *ckt, GENmodel *inModel, IFuid name)
{
    MEMR3model *model = (MEMR3model *)inModel;
    MEMR3instance *here;
    int error;
    CKTnode *tmp;

    for( ; model != NULL; model = MEMR3nextModel(model)) {
        for (here = MEMR3instances(model); here != NULL;
                here = MEMR3nextInstance(here)) {
            if(here->MEMR3name == name) {
                if(here->MEMR3Ibranch == 0) {
                    error = CKTmkCur(ckt, &tmp, here->MEMR3name, "branch");
                    if(error) return(error);
                    here->MEMR3Ibranch = tmp->number;
                }
                return(here->MEMR3Ibranch);
            }
        }
    }
    return(0);
}
