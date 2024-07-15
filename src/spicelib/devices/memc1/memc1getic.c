#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memc1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"

int
MEMC1getic(GENmodel *inModel, CKTcircuit *ckt)

{

    MEMC1model *model = (MEMC1model*)inModel;
    MEMC1instance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = MEMC1nextModel(model)) {
        for(here = MEMC1instances(model); here ; here = MEMC1nextInstance(here)) {
                
            if(!here->MEMC1winitGiven) {
                here->MEMC1winit= *(ckt->CKTrhsOld + here->MEMC1Wbranch);
            }
        }
    }
    return(OK);
}

