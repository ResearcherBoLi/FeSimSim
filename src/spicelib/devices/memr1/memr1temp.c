/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memr1defs.h"
#include "ngspice/sperror.h"

int
MEMR1temp(GENmodel *inModel, CKTcircuit *ckt)
/* perform the temperature update to the resistors
 * calculate the conductance as a function of the
 * given nominal and current temperatures - the
 * resistance given in the struct is the nominal
 * temperature resistance
 */
{
    MEMR1model *model =  (MEMR1model *)inModel;
    MEMR1instance *here;


    /*  loop through all the resistor models */
	/* loop through all the instances of the model */
	/*
    for( ; model != NULL; model = MEMnextModel(model)) {

        //
        for (here = MEMinstances(model); here != NULL ;
                here=MEMnextInstance(here)) {



            if (!here->MEMtempGiven) {
                here->MEMtemp = ckt->CKTtemp;
                if (!here->MEMdtempGiven)
                    here->MEMdtemp = 0.0;
            } else {
                here->MEMdtemp = 0.0;
                if (here->MEMdtempGiven)
                    printf("%s: Instance temperature specified, dtemp ignored\n", here->MEMname);
            }

            MEMupdate_conduct(here, TRUE);
        }
    }
	*/
    return OK;
}


void
MEMR1update_conduct(MEMR1instance *here, bool spill_warnings)
{
    MEMR1model *model = MEMR1modPtr(here);
    double factor;
    double difference;
    double tc1, tc2, tce;
/*
    if (!here->MEMresGiven) {
        if (here->MEMlength * here->MEMwidth * model->MEMsheetRes > 0.0) {
            here->MEMresist =
                (here->RESlength - 2 * model->RESshort) /
                (here->RESwidth - 2 * model->RESnarrow) *
                model->RESsheetRes;
        } else if (model->RESresGiven) {
            here->MEMresist = model->MEMres;
        } else {
            if (spill_warnings)
                SPfrontEnd->IFerrorf (ERR_WARNING,
                                      "%s: resistance to low, set to 1 mOhm", here->MEMname);
            here->MEMresist = 1e-03;
        }
    }

*/  
/*
    difference = (here->MEMtemp + here->MEMdtemp) - model->MEMtnom;
*/
return OK;

}

