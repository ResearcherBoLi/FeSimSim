/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/


#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memc1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
MEMC1findBr(CKTcircuit *ckt, GENmodel *inModel, IFuid name)
{
    MEMC1model *model = (MEMC1model *)inModel;
    MEMC1instance *here;
    int error;
    CKTnode *tmp;

    for( ; model != NULL; model = MEMC1nextModel(model)) {
        for (here = MEMC1instances(model); here != NULL;
                here = MEMC1nextInstance(here)) {
            if(here->MEMC1name == name) {
                if(here->MEMC1Ibranch == 0) {
                    error = CKTmkCur(ckt,&tmp,here->MEMC1name,"branch");
                    if(error) return(error);
                    here->MEMC1Ibranch = tmp->number;
                }
                return(here->MEMC1Ibranch);
            }
        }
    }
    return(0);
}
