
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
#include "ngspice/suffix.h"

int
MEMR1convTest(GENmodel *inModel, CKTcircuit *ckt)
{
    MEMR1model *model = (MEMR1model*)inModel;
    MEMR1instance *here;
	// vnp node p of memristor voltage vnpOld node p of memristor last iteration voltage
	// cdhat& cd branch current of memristor last iteration value and  present value
    double vnp, vnpOld, vnn, vnnOld, delvpd, delvnd, cdhat, cd;
    double tolvp, tolvn, tol;
    /*  loop through all the diode models */
    for( ; model != NULL; model = MEMR1nextModel(model)) {

        /* loop through all the instances of the model */
        for (here = MEMR1instances(model); here != NULL ;
                here=MEMR1nextInstance(here)) {
                
            /*  
             *   initialization 
             */

            vnpOld  =  *(ckt->CKTrhsOld+here->MEMR1posNode);
            vnnOld  =  *(ckt->CKTrhsOld + here->MEMR1negNode);

			vnp =  *(ckt->CKTrhs+here->MEMR1posNode);
			vnn = *(ckt->CKTrhs+here->MEMR1negNode);
			
			delvpd = fabs(vnp-vnpOld);
			//tol = MIN(c1, c2*fabs(vdOld));
            tolvp = ckt->CKTreltol*MAX(fabs(vnp),fabs(vnpOld))
            	  +ckt->CKTvoltTol;
	
            /*
             *   check convergence
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
					cd = *(ckt->CKTrhs+here->MEMR1Ibranch);
					cdhat = *(ckt->CKTrhsOld+here->MEMR1Ibranch);

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

