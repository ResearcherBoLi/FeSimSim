#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memi1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"

int
MEMI1getic(GENmodel *inModel, CKTcircuit *ckt)

{

    MEMI1model *model = (MEMI1model*)inModel;
    MEMI1instance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = MEMI1nextModel(model)) {
        for(here = MEMI1instances(model); here ; here = MEMI1nextInstance(here)) {
                
            if(!here->MEMI1winitGiven) {
                here->MEMI1winit= *(ckt->CKTrhsOld + here->MEMI1Wbranch);
            }
        }
    }
    return(OK);
}

