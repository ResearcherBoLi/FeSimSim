#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memr3defs.h"
#include "ngspice/sperror.h"

int
MEMR3temp(GENmodel *inModel, CKTcircuit *ckt)
/* perform the temperature update to the resistors
 * calculate the conductance as a function of the
 * given nominal and current temperatures - the
 * resistance given in the struct is the nominal
 * temperature resistance
 */
{
    MEMR3model *model =  (MEMR3model *)inModel;
    MEMR3instance *here;


    /*  loop through all the resistor models */
	/* loop through all the instances of the model */
	/*
    for( ; model != NULL; model = MEMR3nextModel(model)) {

        //
        for (here = MEMR3instances(model); here != NULL ;
                here=MEMR3nextInstance(here)) {



            if (!here->MEMR3tempGiven) {
                here->MEMR3temp = ckt->CKTtemp;
                if (!here->MEMR3dtempGiven)
                    here->MEMR3dtemp = 0.0;
            } else {
                here->MEMR3dtemp = 0.0;
                if (here->MEMR3dtempGiven)
                    printf("%s: Instance temperature specified, dtemp ignored\n", here->MEMR3name);
            }

            MEMR3update_conduct(here, TRUE);
        }
    }
	*/
    return OK;
}


void
MEMR3update_conduct(MEMR3instance *here, bool spill_warnings)
{
    MEMR3model *model = MEMR3modPtr(here);
    double factor;
    double difference;
    double tc1, tc2, tce;
/*
    if (!here->MEMR3resGiven) {
        if (here->MEMR3length * here->MEMR3width * model->MEMR3sheetRes > 0.0) {
            here->MEMR3resist =
                (here->MEMR3length - 2 * model->MEMR3short) /
                (here->MEMR3width - 2 * model->MEMR3narrow) *
                model->MEMR3sheetRes;
        } else if (model->MEMR3resGiven) {
            here->MEMR3resist = model->MEMR3res;
        } else {
            if (spill_warnings)
                SPfrontEnd->IFerrorf (ERR_WARNING,
                                      "%s: resistance to low, set to 1 mOhm", here->MEMR3name);
            here->MEMR3resist = 1e-03;
        }
    }

*/  
/*
    difference = (here->MEMR3temp + here->MEMR3dtemp) - model->MEMR3tnom;
*/
return OK;

}

