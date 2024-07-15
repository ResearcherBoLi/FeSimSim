#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memc1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
MEMC1trunc(GENmodel *inModel, CKTcircuit *ckt, double *timeStep)
{
    MEMC1model *model = (MEMC1model*)inModel;
    MEMC1instance *here;
    for( ; model!= NULL; model = MEMC1nextModel(model)) {
        for(here = MEMC1instances(model); here != NULL ;
                here = MEMC1nextInstance(here)) {

            CKTterr(here->MEMC1qcapPseudo,ckt,timeStep);
        }
    }
    return(OK);
}
