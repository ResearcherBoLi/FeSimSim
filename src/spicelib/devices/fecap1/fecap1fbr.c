

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "fecap1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
FECAP1findBr(CKTcircuit *ckt, GENmodel *inModel, IFuid name)
{
    FECAP1model *model = (FECAP1model *)inModel;
    FECAP1instance *here;
    int error;
    CKTnode *tmp;

    for( ; model != NULL; model = FECAP1nextModel(model)) {
        for (here = FECAP1instances(model); here != NULL;
                here = FECAP1nextInstance(here)) {
            if(here->FECAP1name == name) {
                if(here->FECAP1Ibranch == 0) {
                    error = CKTmkCur(ckt, &tmp, here->FECAP1name, "branch");
                    if(error) return(error);
                    here->FECAP1Ibranch = tmp->number;
                }
                return(here->FECAP1Ibranch);
            }
        }
    }
    return(0);
}

