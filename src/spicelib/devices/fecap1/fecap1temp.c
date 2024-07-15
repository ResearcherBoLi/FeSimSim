/**********
Copyright 2024 Xidian University
Author: 2024 Bo Li
Modified: 2024/01/27  Bo Li
Refered to NgSPICE Res/Cap related file
**********/
/*
 */

/* load the capacitor structure with those pointers needed later
 * for fast matrix loading
 */

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "fecap1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


/*ARGSUSED*/
int
FECAP1temp(GENmodel *inModel, CKTcircuit *ckt)

{
    FECAP1model *model = (FECAP1model*)inModel;
    FECAP1instance *here;
    double difference;
    double factor;
    double tc1, tc2;

    return(OK);
}

