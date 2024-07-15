

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memi1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
MEMI1findBr(CKTcircuit *ckt, GENmodel *inModel, IFuid name)
{
    MEMI1model *model = (MEMI1model *)inModel;
    MEMI1instance *here;
    int error;
    CKTnode *tmp;

    for( ; model != NULL; model = MEMI1nextModel(model)) {
        for (here = MEMI1instances(model); here != NULL;
                here = MEMI1nextInstance(here)) {
            if(here->MEMI1name == name) {
                if(here->MEMI1Ibranch == 0) {
                    error = CKTmkCur(ckt, &tmp, here->MEMI1name, "branch");
                    if(error) return(error);
                    here->MEMI1Ibranch = tmp->number;
                }
                return(here->MEMI1Ibranch);
            }
        }
    }
    return(0);
}
