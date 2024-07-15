#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memi1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
MEMI1trunc(GENmodel *inModel, CKTcircuit *ckt, double *timeStep)
{
    MEMI1model *model = (MEMI1model*)inModel;
    MEMI1instance *here;
    for( ; model!= NULL; model = MEMI1nextModel(model)) {
        for(here = MEMI1instances(model); here != NULL ;
                here = MEMI1nextInstance(here)) {

            CKTterr(here->MEMI1qcapPseudo,ckt,timeStep);
        }
    }
    return(OK);
}
