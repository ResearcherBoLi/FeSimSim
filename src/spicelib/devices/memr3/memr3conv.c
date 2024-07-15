#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memr3defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"

int
MEMR3convTest(GENmodel *inModel, CKTcircuit *ckt)
{
    MEMR3model *model = (MEMR3model*)inModel;
    MEMR3instance *here;
    double vd, vdOld, delvd, cdhat, cd;
    double tol;
	double c1 = 0.000001;
	double c2 = 0.0001;
    /*  loop through all the diode models */
    for( ; model != NULL; model = MEMR3nextModel(model)) {

        /* loop through all the instances of the model */
        for (here = MEMR3instances(model); here != NULL ;
                here=MEMR3nextInstance(here)) {
                
            /*  
             *   initialization 
             */

            vdOld = *(ckt->CKTrhsOld+here->MEMR3posNode)-
                    *(ckt->CKTrhsOld + here->MEMR3negNode);

			vd = *(ckt->CKTrhs+here->MEMR3posNode)-
					*(ckt->CKTrhs+here->MEMR3negNode);
			
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

