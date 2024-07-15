/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "fecap1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"

int
FECAP1convTest(GENmodel *inModel, CKTcircuit *ckt)
{
    FECAP1model *model = (FECAP1model*)inModel;
    FECAP1instance *here;
    double vd, vdOld, delvd, cdhat, vnp, vnpOld, vnn, vnnOld, delvpd, delvnd,cd;
    double tolvp, tolvn, tol;
	double c1 = 0.000001;
	double c2 = 0.0001;

    /*  loop through all the diode models */
    for( ; model != NULL; model = FECAP1nextModel(model)) {

        /* loop through all the instances of the model */
        for (here = FECAP1instances(model); here != NULL ;
                here=FECAP1nextInstance(here)) {
                
            /*  
             *   initialization 
             */

            vdOld = *(ckt->CKTrhsOld+here->FECAP1posNode)-
                    *(ckt->CKTrhsOld + here->FECAP1negNode);

			vd = *(ckt->CKTrhs+here->FECAP1posNode)-
					*(ckt->CKTrhs+here->FECAP1negNode);
			
			delvd = fabs(vd-vdOld);
			tol = MIN(c1, c2*fabs(vdOld));
            vnpOld  =  *(ckt->CKTrhsOld + here->FECAP1posNode);
            vnnOld  =  *(ckt->CKTrhsOld + here->FECAP1negNode);

			vnp =  *(ckt->CKTrhs + here->FECAP1posNode);
			vnn = *(ckt->CKTrhs + here->FECAP1negNode);
			
			delvpd = fabs(vnp-vnpOld);
			//tol = MIN(c1, c2*fabs(vdOld));
            tolvp = ckt->CKTreltol*MAX(fabs(vnp),fabs(vnpOld))
            	  +ckt->CKTvoltTol;
            /*
             *   check convergence
             */
            /*
            tol=ckt->CKTreltol*
                    MAX(fabs(cdhat),fabs(cd))+ckt->CKTabstol;
            if (fabs(vd-vdOld)> tol) {
                ckt->CKTnoncon++;
				ckt->CKTtroubleElt = (GENinstance *) here;
                return(OK); 
            }
            */

            if (delvpd > tolvp) {
                ckt->CKTnoncon++;
				ckt->CKTtroubleElt = (GENinstance *) here;
                return(OK); /* don't need to check any more device */
            }else{
				delvnd = fabs(vnn-vnnOld);
           		tolvn = ckt->CKTreltol* MAX(fabs(vnn),fabs(vnnOld))
					 +ckt->CKTvoltTol;				
				if (delvnd > tolvn) {
					ckt->CKTnoncon++;
					ckt->CKTtroubleElt = (GENinstance *) here;
					return(OK); /* don't need to check any more device */
				}else{
					cd = *(ckt->CKTrhs+here->FECAP1Pbranch);
					cdhat = *(ckt->CKTrhsOld+here->FECAP1Pbranch);

					tol = ckt->CKTreltol*MAX(fabs(cdhat),fabs(cd))
					   +ckt->CKTabstol;
					if (delvnd > tolvn) {
						ckt->CKTnoncon++;
						ckt->CKTtroubleElt = (GENinstance *) here;
						return(OK); /* don't need to check any more device */
					}					   
				}
            }


        }
    }
    return(OK);


}

