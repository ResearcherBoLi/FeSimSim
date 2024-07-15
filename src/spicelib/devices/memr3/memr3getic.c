#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memr3defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"

int
MEMR3getic(GENmodel *inModel, CKTcircuit *ckt)

{

    MEMR3model *model = (MEMR3model*)inModel;
    MEMR3instance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = MEMR3nextModel(model)) {
        for(here = MEMR3instances(model); here ; here = MEMR3nextInstance(here)) {
                
            if(!here->MEMR3winitGiven) {
                here->MEMR3winit= *(ckt->CKTrhsOld + here->MEMR3Wbranch);
            }
        }
    }
    return(OK);
}

