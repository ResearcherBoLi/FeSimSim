/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/10  Bo Li
Refered to NgSPICE Res/Cap related file
**********/


#include "ngspice/ngspice.h"
#include "ngspice/const.h"
#include "memr3defs.h"
#include "ngspice/ifsim.h"
#include "ngspice/cktdefs.h"
#include "ngspice/sperror.h"


/* TODO : there are "double" value compared with 0 (eg: vm == 0)
 *        Need to substitute this check with a suitable eps.
 *        PN 2003
 */

int
MEMR3ask(CKTcircuit *ckt, GENinstance *inst, int which, IFvalue *value,
       IFvalue *select)
{
    MEMR3instance *fast = (MEMR3instance *)inst;
	/*
    double vr;
    double vi;
    double sr;
    double si;
    double vm;
    static char *msg = "Current and power not available for ac analysis";
	*/
    switch(which) {
        case MEMR3_WFLUXPSEUDO:
            value->rValue = *(ckt->CKTstate0+fast->MEMR3qcapPseudo);
            return(OK);
        case MEMR3_WVOLTPSEUDO:
            value->rValue = *(ckt->CKTstate0+fast->MEMR3ccapPseudo);
			return(OK);
	
        case MEMR3_RINIT:
            value->rValue = fast->MEMR3rinit;
            return(OK);
        case MEMR3_WINIT:
            value->rValue = fast->MEMR3winit;
            return(OK);		
        
		default:
			return(E_BADPARM);
    }
    /* NOTREACHED */
}
