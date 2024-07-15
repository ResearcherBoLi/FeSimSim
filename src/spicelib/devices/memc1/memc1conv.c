#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memc1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"

int
MEMC1convTest(GENmodel *inModel, CKTcircuit *ckt)
{
    MEMC1model *model = (MEMC1model*)inModel;
    MEMC1instance *here;
    double vd, vdOld, delvd, cdhat, cd;
    double tol;
	double c1 = 0.000001;
	double c2 = 0.0001;
    /*  loop through all the diode models */
    for( ; model != NULL; model = MEMC1nextModel(model)) {

        /* loop through all the instances of the model */
        for (here = MEMC1instances(model); here != NULL ;
                here=MEMC1nextInstance(here)) {
                
            /*  
             *   initialization 
             */

            vdOld = *(ckt->CKTrhsOld+here->MEMC1posNode)-
                    *(ckt->CKTrhsOld + here->MEMC1negNode);

			vd = *(ckt->CKTrhs+here->MEMC1posNode)-
					*(ckt->CKTrhs+here->MEMC1negNode);
			
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

