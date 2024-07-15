#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memr3defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
MEMR3trunc(GENmodel *inModel, CKTcircuit *ckt, double *timeStep)
{
    MEMR3model *model = (MEMR3model*)inModel;
    MEMR3instance *here;
    for( ; model!= NULL; model = MEMR3nextModel(model)) {
        for(here = MEMR3instances(model); here != NULL ;
                here = MEMR3nextInstance(here)) {

            CKTterr(here->MEMR3qcapPseudo,ckt,timeStep);
        }
    }
    return(OK);
}
