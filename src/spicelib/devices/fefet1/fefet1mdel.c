/**** BSIM4.5.0 Released by Xuemei (Jane) Xi 07/29/2005 ****/

/**********
 * Copyright 2005 Regents of the University of California. All rights reserved.
 * File: b4mdel.c of BSIM4.5.0.
 * Author: 2000 Weidong Liu
 * Authors: 2001- Xuemei Xi, Mohan Dunga, Ali Niknejad, Chenming Hu.
 * Project Director: Prof. Chenming Hu.
 **********/

#include "ngspice/ngspice.h"
#include "fefet1def.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
FEFET1mDelete(GENmodel *gen_model)
{
    FEFET1model *model = (FEFET1model*) gen_model;

#ifdef USE_OMP
    FREE(model->FEFET1InstanceArray);
#endif

    struct fefet1SizeDependParam *p = model->pSizeDependParamKnot;
    while (p) {
        struct fefet1SizeDependParam *next_p = p->pNext;
        FREE(p);
        p = next_p;
    }

    FREE(model->FEFET1version);

    return OK;
}
