#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memr2defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
MEMR2trunc(GENmodel *inModel, CKTcircuit *ckt, double *timeStep)
{
    MEMR2model *model = (MEMR2model*)inModel;
    MEMR2instance *here;
    for( ; model!= NULL; model = MEMR2nextModel(model)) {
        for(here = MEMR2instances(model); here != NULL ;
                here = MEMR2nextInstance(here)) {

            CKTterr(here->MEMR2qcapPseudo,ckt,timeStep);
        }
    }
    return(OK);
}
