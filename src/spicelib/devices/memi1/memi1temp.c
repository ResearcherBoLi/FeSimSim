#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memi1defs.h"
#include "ngspice/sperror.h"

int
MEMI1temp(GENmodel *inModel, CKTcircuit *ckt)
/* perform the temperature update to the resistors
 * calculate the conductance as a function of the
 * given nominal and current temperatures - the
 * resistance given in the struct is the nominal
 * temperature resistance
 */
{
    MEMI1model *model =  (MEMI1model *)inModel;
    MEMI1instance *here;


    /*  loop through all the resistor models */
	/* loop through all the instances of the model */
	/*
    for( ; model != NULL; model = MEMI1nextModel(model)) {

        //
        for (here = MEMI1instances(model); here != NULL ;
                here=MEMI1nextInstance(here)) {



            if (!here->MEMI1tempGiven) {
                here->MEMI1temp = ckt->CKTtemp;
                if (!here->MEMI1dtempGiven)
                    here->MEMI1dtemp = 0.0;
            } else {
                here->MEMI1dtemp = 0.0;
                if (here->MEMI1dtempGiven)
                    printf("%s: Instance temperature specified, dtemp ignored\n", here->MEMI1name);
            }

            MEMI1update_conduct(here, TRUE);
        }
    }
	*/
    return OK;
}


void
MEMI1update_conduct(MEMI1instance *here, bool spill_warnings)
{
    MEMI1model *model = MEMI1modPtr(here);
    double factor;
    double difference;
    double tc1, tc2, tce;
/*
    if (!here->MEMI1resGiven) {
        if (here->MEMI1length * here->MEMI1width * model->MEMI1sheetRes > 0.0) {
            here->MEMI1resist =
                (here->MEMI1length - 2 * model->MEMI1short) /
                (here->MEMI1width - 2 * model->MEMI1narrow) *
                model->MEMI1sheetRes;
        } else if (model->MEMI1resGiven) {
            here->MEMI1resist = model->MEMI1res;
        } else {
            if (spill_warnings)
                SPfrontEnd->IFerrorf (ERR_WARNING,
                                      "%s: resistance to low, set to 1 mOhm", here->MEMI1name);
            here->MEMI1resist = 1e-03;
        }
    }

*/  
/*
    difference = (here->MEMI1temp + here->MEMI1dtemp) - model->MEMI1tnom;
*/
return OK;

}

