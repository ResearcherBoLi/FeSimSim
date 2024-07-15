/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memr2defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"

int
MEMR2convTest(GENmodel *inModel, CKTcircuit *ckt)
{
    MEMR2model *model = (MEMR2model*)inModel;
    MEMR2instance *here;
	// vnp node p of memristor voltage vnpOld node p of memristor last iteration voltage
	// cdhat& cd branch current of memristor last iteration value and  present value
    double vnp, vnpOld, vnn, vnnOld, delvpd, delvnd, cdhat, cd;
    double tolvp, tolvn, tol;
    /*  loop through all the diode models */
    for( ; model != NULL; model = MEMR2nextModel(model)) {

        /* loop through all the instances of the model */
        for (here = MEMR2instances(model); here != NULL ;
                here=MEMR2nextInstance(here)) {
                
            /*  
             *   initialization 
             */

            vnpOld  =  *(ckt->CKTrhsOld+here->MEMR2posNode);
            vnnOld  =  *(ckt->CKTrhsOld + here->MEMR2negNode);

			vnp =  *(ckt->CKTrhs+here->MEMR2posNode);
			vnn = *(ckt->CKTrhs+here->MEMR2negNode);
			
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
					cd = *(ckt->CKTrhs+here->MEMR2Ibranch);
					cdhat = *(ckt->CKTrhsOld+here->MEMR2Ibranch);

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

