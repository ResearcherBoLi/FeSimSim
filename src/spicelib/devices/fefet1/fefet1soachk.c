/**********
Copyright 2024 Xidian University.	All rights reserved.
 Author: Bo Li	at Hangzhou Institute of Technology	 
 Modified: 2020/09/09  Bo Li
 Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "fefet1def.h"
#include "ngspice/trandefs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"
#include "ngspice/cpdefs.h"


int
FEFET1soaCheck(CKTcircuit *ckt, GENmodel *inModel)
{
    FEFET1model *model = (FEFET1model *) inModel;
    FEFET1instance *here;
    double vgs, vgd, vgb, vds, vbs, vbd;    /* actual mos voltages */
    int maxwarns;
    static int warns_vgs = 0, warns_vgd = 0, warns_vgb = 0, warns_vds = 0, warns_vbs = 0, warns_vbd = 0;

    if (!ckt) {
        warns_vgs = 0;
        warns_vgd = 0;
        warns_vgb = 0;
        warns_vds = 0;
        warns_vbs = 0;
        warns_vbd = 0;
        return OK;
    }

    maxwarns = ckt->CKTsoaMaxWarns;

    for (; model; model = FEFET1nextModel(model)) {

        for (here = FEFET1instances(model); here; here = FEFET1nextInstance(here)) {

            vgs = ckt->CKTrhsOld [here->FEFET1gNodePrime] -
                  ckt->CKTrhsOld [here->FEFET1sNodePrime];

            vgd = ckt->CKTrhsOld [here->FEFET1gNodePrime] -
                  ckt->CKTrhsOld [here->FEFET1dNodePrime];

            vgb = ckt->CKTrhsOld [here->FEFET1gNodePrime] -
                  ckt->CKTrhsOld [here->FEFET1bNodePrime];

            vds = ckt->CKTrhsOld [here->FEFET1dNodePrime] -
                  ckt->CKTrhsOld [here->FEFET1sNodePrime];

            vbs = ckt->CKTrhsOld [here->FEFET1bNodePrime] -
                  ckt->CKTrhsOld [here->FEFET1sNodePrime];

            vbd = ckt->CKTrhsOld [here->FEFET1bNodePrime] -
                  ckt->CKTrhsOld [here->FEFET1dNodePrime];

            if (!model->FEFET1vgsrMaxGiven) {
                if (fabs(vgs) > model->FEFET1vgsMax)
                    if (warns_vgs < maxwarns) {
                        soa_printf(ckt, (GENinstance*) here,
                                   "Vgs=%g has exceeded Vgs_max=%g\n",
                                   vgs, model->FEFET1vgsMax);
                        warns_vgs++;
                    }
                if (!model->FEFET1vgbMaxGiven) {
                    if (fabs(vgb) > model->FEFET1vgsMax)
                        if (warns_vgb < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vgb=%g has exceeded Vgs_max=%g\n",
                                       vgb, model->FEFET1vgsMax);
                            warns_vgb++;
                        }
                } else {
                    if (fabs(vgb) > model->FEFET1vgbMax)
                        if (warns_vgb < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vgb=%g has exceeded Vgb_max=%g\n",
                                       vgb, model->FEFET1vgbMax);
                            warns_vgb++;
                        }
                }
            } else {
                if (model->FEFET1type > 0) {
                    if (vgs > model->FEFET1vgsMax)
                        if (warns_vgs < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vgs=%g has exceeded Vgs_max=%g\n",
                                       vgs, model->FEFET1vgsMax);
                            warns_vgs++;
                        }
                    if (-1*vgs > model->FEFET1vgsrMax)
                        if (warns_vgs < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vgs=%g has exceeded Vgsr_max=%g\n",
                                       vgs, model->FEFET1vgsrMax);
                            warns_vgs++;
                        }
                } else {
                    if (vgs > model->FEFET1vgsrMax)
                        if (warns_vgs < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vgs=%g has exceeded Vgsr_max=%g\n",
                                       vgs, model->FEFET1vgsrMax);
                            warns_vgs++;
                        }
                    if (-1*vgs > model->FEFET1vgsMax)
                        if (warns_vgs < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vgs=%g has exceeded Vgs_max=%g\n",
                                       vgs, model->FEFET1vgsMax);
                            warns_vgs++;
                        }
                }
            }

            if (!model->FEFET1vgdrMaxGiven) {
                if (fabs(vgd) > model->FEFET1vgdMax)
                    if (warns_vgd < maxwarns) {
                        soa_printf(ckt, (GENinstance*) here,
                                   "Vgd=%g has exceeded Vgd_max=%g\n",
                                   vgd, model->FEFET1vgdMax);
                        warns_vgd++;
                    }
            } else {
                if (model->FEFET1type > 0) {
                    if (vgd > model->FEFET1vgdMax)
                        if (warns_vgd < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vgd=%g has exceeded Vgd_max=%g\n",
                                       vgd, model->FEFET1vgdMax);
                            warns_vgd++;
                        }
                    if (-1*vgd > model->FEFET1vgdrMax)
                        if (warns_vgd < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vgd=%g has exceeded Vgdr_max=%g\n",
                                       vgd, model->FEFET1vgdrMax);
                            warns_vgd++;
                        }
                } else {
                    if (vgd > model->FEFET1vgdrMax)
                        if (warns_vgd < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vgd=%g has exceeded Vgdr_max=%g\n",
                                       vgd, model->FEFET1vgdrMax);
                            warns_vgd++;
                        }
                    if (-1*vgd > model->FEFET1vgdMax)
                        if (warns_vgd < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vgd=%g has exceeded Vgd_max=%g\n",
                                       vgd, model->FEFET1vgdMax);
                            warns_vgd++;
                        }
                }
            }

            if (fabs(vds) > model->FEFET1vdsMax)
                if (warns_vds < maxwarns) {
                    soa_printf(ckt, (GENinstance*) here,
                               "Vds=%g has exceeded Vds_max=%g\n",
                               vds, model->FEFET1vdsMax);
                    warns_vds++;
                }

            if (!model->FEFET1vgbrMaxGiven) {
                if (fabs(vgb) > model->FEFET1vgbMax)
                    if (warns_vgb < maxwarns) {
                        soa_printf(ckt, (GENinstance*) here,
                                   "Vgb=%g has exceeded Vgb_max=%g\n",
                                   vgb, model->FEFET1vgbMax);
                        warns_vgb++;
                    }
            } else {
                if (model->FEFET1type > 0) {
                    if (vgb > model->FEFET1vgbMax)
                        if (warns_vgb < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vgb=%g has exceeded Vgb_max=%g\n",
                                       vgb, model->FEFET1vgbMax);
                            warns_vgb++;
                        }
                    if (-1*vgb > model->FEFET1vgbrMax)
                        if (warns_vgb < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vgb=%g has exceeded Vgbr_max=%g\n",
                                       vgb, model->FEFET1vgbrMax);
                            warns_vgb++;
                        }
                } else {
                    if (vgb > model->FEFET1vgbrMax)
                        if (warns_vgb < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vgb=%g has exceeded Vgbr_max=%g\n",
                                       vgb, model->FEFET1vgbrMax);
                            warns_vgb++;
                        }
                    if (-1*vgb > model->FEFET1vgbMax)
                        if (warns_vgb < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vgb=%g has exceeded Vgb_max=%g\n",
                                       vgb, model->FEFET1vgbMax);
                            warns_vgb++;
                        }
                }
            }

            if (!model->FEFET1vbsrMaxGiven) {
                if (!model->FEFET1vbsMaxGiven) {
                    if (fabs(vbs) > model->FEFET1vbdMax)
                        if (warns_vbs < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vbs=%g has exceeded Vbd_max=%g\n",
                                       vbs, model->FEFET1vbdMax);
                            warns_vbs++;
                        }
                } else {
                    if (fabs(vbs) > model->FEFET1vbsMax)
                        if (warns_vbs < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vbs=%g has exceeded Vbs_max=%g\n",
                                       vbs, model->FEFET1vbsMax);
                            warns_vbs++;
                        }
                }
            } else {
                if (!model->FEFET1vbsMaxGiven) {
                    if (model->FEFET1type > 0) {
                        if (vbs > model->FEFET1vbdMax)
                            if (warns_vbs < maxwarns) {
                                soa_printf(ckt, (GENinstance*) here,
                                           "Vbs=%g has exceeded Vbd_max=%g\n",
                                           vbs, model->FEFET1vbdMax);
                                warns_vbs++;
                            }
                        if (-1*vbs > model->FEFET1vbsrMax)
                            if (warns_vbs < maxwarns) {
                                soa_printf(ckt, (GENinstance*) here,
                                           "Vbs=%g has exceeded Vbsr_max=%g\n",
                                           vbs, model->FEFET1vbsrMax);
                                warns_vbs++;
                            }
                    } else {
                        if (vbs > model->FEFET1vbsrMax)
                            if (warns_vbs < maxwarns) {
                                soa_printf(ckt, (GENinstance*) here,
                                           "Vbs=%g has exceeded Vbsr_max=%g\n",
                                           vbs, model->FEFET1vbsrMax);
                                warns_vbs++;
                            }
                        if (-1*vbs > model->FEFET1vbdMax)
                            if (warns_vbs < maxwarns) {
                                soa_printf(ckt, (GENinstance*) here,
                                           "Vbs=%g has exceeded Vbd_max=%g\n",
                                           vbs, model->FEFET1vbdMax);
                                warns_vbs++;
                            }
                    }
                } else {
                    if (model->FEFET1type > 0) {
                        if (vbs > model->FEFET1vbsMax)
                            if (warns_vbs < maxwarns) {
                                soa_printf(ckt, (GENinstance*) here,
                                           "Vbs=%g has exceeded Vbs_max=%g\n",
                                           vbs, model->FEFET1vbsMax);
                                warns_vbs++;
                            }
                        if (-1*vbs > model->FEFET1vbsrMax)
                            if (warns_vbs < maxwarns) {
                                soa_printf(ckt, (GENinstance*) here,
                                           "Vbs=%g has exceeded Vbsr_max=%g\n",
                                           vbs, model->FEFET1vbsrMax);
                                warns_vbs++;
                            }
                    } else {
                        if (vbs > model->FEFET1vbsrMax)
                            if (warns_vbs < maxwarns) {
                                soa_printf(ckt, (GENinstance*) here,
                                           "Vbs=%g has exceeded Vbsr_max=%g\n",
                                           vbs, model->FEFET1vbsrMax);
                                warns_vbs++;
                            }
                        if (-1*vbs > model->FEFET1vbsMax)
                            if (warns_vbs < maxwarns) {
                                soa_printf(ckt, (GENinstance*) here,
                                           "Vbs=%g has exceeded Vbs_max=%g\n",
                                           vbs, model->FEFET1vbsMax);
                                warns_vbs++;
                            }
                    }
                }
            }

            if (!model->FEFET1vbdrMaxGiven) {
                if (fabs(vbd) > model->FEFET1vbdMax)
                    if (warns_vbd < maxwarns) {
                        soa_printf(ckt, (GENinstance*) here,
                                   "Vbd=%g has exceeded Vbd_max=%g\n",
                                   vbd, model->FEFET1vbdMax);
                        warns_vbd++;
                    }
            } else {
                if (model->FEFET1type > 0) {
                    if (vbd > model->FEFET1vbdMax)
                        if (warns_vbd < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vbd=%g has exceeded Vbd_max=%g\n",
                                       vbd, model->FEFET1vbdMax);
                            warns_vbd++;
                        }
                    if (-1*vbd > model->FEFET1vbdrMax)
                        if (warns_vbd < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vbd=%g has exceeded Vbdr_max=%g\n",
                                       vbd, model->FEFET1vbdrMax);
                            warns_vbd++;
                        }
                } else {
                    if (vbd > model->FEFET1vbdrMax)
                        if (warns_vbd < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vbd=%g has exceeded Vbdr_max=%g\n",
                                       vbd, model->FEFET1vbdrMax);
                            warns_vbd++;
                        }
                    if (-1*vbd > model->FEFET1vbdMax)
                        if (warns_vbd < maxwarns) {
                            soa_printf(ckt, (GENinstance*) here,
                                       "Vbd=%g has exceeded Vbd_max=%g\n",
                                       vbd, model->FEFET1vbdMax);
                            warns_vbd++;
                        }
                }
            }

        }
    }

    return OK;
}
