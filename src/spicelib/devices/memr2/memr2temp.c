#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memr2defs.h"
#include "ngspice/sperror.h"

int
MEMR2temp(GENmodel *inModel, CKTcircuit *ckt)
/* perform the temperature update to the resistors
 * calculate the conductance as a function of the
 * given nominal and current temperatures - the
 * resistance given in the struct is the nominal
 * temperature resistance
 */
{
    MEMR2model *model =  (MEMR2model *)inModel;
    MEMR2instance *here;


    /*  loop through all the resistor models */
	/* loop through all the instances of the model */
	/*
    for( ; model != NULL; model = MEM2nextModel(model)) {

        //
        for (here = MEM2instances(model); here != NULL ;
                here=MEM2nextInstance(here)) {



            if (!here->MEM2tempGiven) {
                here->MEM2temp = ckt->CKTtemp;
                if (!here->MEM2dtempGiven)
                    here->MEM2dtemp = 0.0;
            } else {
                here->MEM2dtemp = 0.0;
                if (here->MEM2dtempGiven)
                    printf("%s: Instance temperature specified, dtemp ignored\n", here->MEM2name);
            }

            MEM2update_conduct(here, TRUE);
        }
    }
	*/
    return OK;
}


void
MEMR2update_conduct(MEMR2instance *here, bool spill_warnings)
{
    MEMR2model *model = MEMR2modPtr(here);
    double factor;
    double difference;
    double tc1, tc2, tce;
/*
    if (!here->MEM2resGiven) {
        if (here->MEM2length * here->MEM2width * model->MEM2sheetRes > 0.0) {
            here->MEM2resist =
                (here->MEM2length - 2 * model->MEM2short) /
                (here->MEM2width - 2 * model->MEM2narrow) *
                model->MEM2sheetRes;
        } else if (model->MEM2resGiven) {
            here->MEM2resist = model->MEM2res;
        } else {
            if (spill_warnings)
                SPfrontEnd->IFerrorf (ERR_WARNING,
                                      "%s: resistance to low, set to 1 mOhm", here->MEM2name);
            here->MEM2resist = 1e-03;
        }
    }

*/  
/*
    difference = (here->MEM2temp + here->MEM2dtemp) - model->MEM2tnom;
*/
return OK;

}

