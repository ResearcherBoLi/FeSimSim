/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/


#include "ngspice/ngspice.h"
#include "ngspice/const.h"
#include "memc1defs.h"
#include "ngspice/ifsim.h"
#include "ngspice/cktdefs.h"
#include "ngspice/sperror.h"


/* TODO : there are "double" value compared with 0 (eg: vm == 0)
 *        Need to substitute this check with a suitable eps.
 *        PN 2003
 */

int
MEMC1ask(CKTcircuit *ckt, GENinstance *inst, int which, IFvalue *value,
       IFvalue *select)
{
    MEMC1instance *fast = (MEMC1instance *)inst;
	/*
    double vr;
    double vi;
    double sr;
    double si;
    double vm;
    static char *msg = "Current and power not available for ac analysis";
	*/
    switch(which) {

		case MEMC1_WFLUXPSEUDO:
			value->rValue = *(ckt->CKTstate0+fast->MEMC1qcapPseudo);
			return(OK);
		
		case MEMC1_WVOLTPSEUDO:
			value->rValue = *(ckt->CKTstate0+fast->MEMC1ccapPseudo);
			return(OK);
		
		case MEMC1_RINIT:
			value->rValue = fast->MEMC1rinit;
			return(OK);

		case MEMC1_WINIT:
			value->rValue = fast->MEMC1winit;
			return(OK); 	
		
		case MEMC1_IC:
			value->rValue = fast->MEMC1initCond;
			return(OK);																						        
    default:
        return(E_BADPARM);
    }
    /* NOTREACHED */
}
