#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memi1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"

int
MEMI1convTest(GENmodel *inModel, CKTcircuit *ckt)
{
    MEMI1model *model = (MEMI1model*)inModel;
    MEMI1instance *here;
    double vd, vdOld, delvd, cdhat, cd;
    double tol;
	double c1 = 0.000001;
	double c2 = 0.0001;
    /*  loop through all the diode models */
    for( ; model != NULL; model = MEMI1nextModel(model)) {

        /* loop through all the instances of the model */
        for (here = MEMI1instances(model); here != NULL ;
                here=MEMI1nextInstance(here)) {
                
            /*  
             *   initialization 
             */

            vdOld = *(ckt->CKTrhsOld+here->MEMI1posNode)-
                    *(ckt->CKTrhsOld + here->MEMI1negNode);

			vd = *(ckt->CKTrhs+here->MEMI1posNode)-
					*(ckt->CKTrhs+here->MEMI1negNode);
			
			delvd = fabs(vd-vdOld);
			tol = MIN(c1, c2*fabs(vdOld));


            /*
             *   check convergence
             */
            tol=ckt->CKTreltol*
                    MAX(fabs(cdhat),fabs(cd))+ckt->CKTabstol;
            if (fabs(vd-vdOld)> tol) {
                ckt->CKTnoncon++;
				ckt->CKTtroubleElt = (GENinstance *) here;
                return(OK); /* don't need to check any more device */
            }
        }
    }
    return(OK);


}

