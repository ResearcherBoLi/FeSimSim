/**** BSIM4.5.0 Released by Xuemei (Jane) Xi 07/29/2005 ****/

/**********
 * Copyright 2005 Regents of the University of California. All rights reserved.
 * File: b4cvtest.c of BSIM4.5.0.
 * Author: 2000 Weidong Liu
 * Authors: 2001- Xuemei Xi, Mohan Dunga, Ali Niknejad, Chenming Hu.
 * Project Director: Prof. Chenming Hu.
 * Modified by Xuemei Xi, 04/06/2001.
 * Modified by Xuemei Xi, 10/05/2001.
 * Modified by Xuemei Xi, 05/09/2003.
 **********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "fefet1def.h"
#include "ngspice/trandefs.h"
#include "ngspice/const.h"
#include "ngspice/devdefs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
FEFET1convTest(
GENmodel *inModel,
CKTcircuit *ckt)
{
FEFET1model *model = (FEFET1model*)inModel;
FEFET1instance *here;
double delvbd, delvbs, delvds, delvgd, delvgs;
double delvdbd, delvsbs; 
double delvbd_jct, delvbs_jct;
double vds, vgs, vgd, vgdo, vbs, vbd;
double vdbd, vdbs, vsbs;
double cbhat, cdhat, Idtot, Ibtot;
double vses, vdes, vdedo, delvses, delvded, delvdes;
double Isestot, cseshat, Idedtot, cdedhat;
double Igstot, cgshat, Igdtot, cgdhat, Igbtot, cgbhat;
double tol0, tol1, tol2, tol3, tol4, tol5, tol6;

    for (; model != NULL; model = FEFET1nextModel(model))
    {    for (here = FEFET1instances(model); here != NULL ;
              here=FEFET1nextInstance(here)) 
         {
	      vds = model->FEFET1type
                  * (*(ckt->CKTrhsOld + here->FEFET1dNodePrime)
                  - *(ckt->CKTrhsOld + here->FEFET1sNodePrime));
              vgs = model->FEFET1type
                  * (*(ckt->CKTrhsOld + here->FEFET1gNodePrime)
                  - *(ckt->CKTrhsOld + here->FEFET1sNodePrime));
              vbs = model->FEFET1type
                  * (*(ckt->CKTrhsOld + here->FEFET1bNodePrime)
                  - *(ckt->CKTrhsOld + here->FEFET1sNodePrime));
              vdbs = model->FEFET1type
                   * (*(ckt->CKTrhsOld + here->FEFET1dbNode)
                   - *(ckt->CKTrhsOld + here->FEFET1sNodePrime));
              vsbs = model->FEFET1type
                   * (*(ckt->CKTrhsOld + here->FEFET1sbNode)
                   - *(ckt->CKTrhsOld + here->FEFET1sNodePrime));              
              vses = model->FEFET1type
                   * (*(ckt->CKTrhsOld + here->FEFET1sNode)
                   - *(ckt->CKTrhsOld + here->FEFET1sNodePrime));
              vdes = model->FEFET1type
                   * (*(ckt->CKTrhsOld + here->FEFET1dNode)
                   - *(ckt->CKTrhsOld + here->FEFET1sNodePrime));
              
              vgdo = *(ckt->CKTstate0 + here->FEFET1vgs)
                    - *(ckt->CKTstate0 + here->FEFET1vds);
              vbd = vbs - vds;
              vdbd = vdbs - vds;
              vgd = vgs - vds;

              delvbd = vbd - *(ckt->CKTstate0 + here->FEFET1vbd);
              delvdbd = vdbd - *(ckt->CKTstate0 + here->FEFET1vdbd);
              delvgd = vgd - vgdo;

              delvds = vds - *(ckt->CKTstate0 + here->FEFET1vds);
              delvgs = vgs - *(ckt->CKTstate0 + here->FEFET1vgs);
              delvbs = vbs - *(ckt->CKTstate0 + here->FEFET1vbs);
              delvsbs = vsbs - *(ckt->CKTstate0 + here->FEFET1vsbs);

              delvses = vses - (*(ckt->CKTstate0 + here->FEFET1vses));
              vdedo = *(ckt->CKTstate0 + here->FEFET1vdes)
                    - *(ckt->CKTstate0 + here->FEFET1vds);
              delvdes = vdes - *(ckt->CKTstate0 + here->FEFET1vdes);
              delvded = vdes - vds - vdedo;

              delvbd_jct = (!here->FEFET1rbodyMod) ? delvbd : delvdbd;
              delvbs_jct = (!here->FEFET1rbodyMod) ? delvbs : delvsbs;

              if (here->FEFET1mode >= 0)
              {   Idtot = here->FEFET1cd + here->FEFET1csub - here->FEFET1cbd
			+ here->FEFET1Igidl;
                  cdhat = Idtot - here->FEFET1gbd * delvbd_jct
                        + (here->FEFET1gmbs + here->FEFET1gbbs + here->FEFET1ggidlb) * delvbs
                        + (here->FEFET1gm + here->FEFET1gbgs + here->FEFET1ggidlg) * delvgs
                        + (here->FEFET1gds + here->FEFET1gbds + here->FEFET1ggidld) * delvds;

                  Igstot = here->FEFET1Igs + here->FEFET1Igcs;
                  cgshat = Igstot + (here->FEFET1gIgsg + here->FEFET1gIgcsg) * delvgs
                         + here->FEFET1gIgcsd * delvds + here->FEFET1gIgcsb * delvbs;

                  Igdtot = here->FEFET1Igd + here->FEFET1Igcd;
                  cgdhat = Igdtot + here->FEFET1gIgdg * delvgd + here->FEFET1gIgcdg * delvgs
                         + here->FEFET1gIgcdd * delvds + here->FEFET1gIgcdb * delvbs;

                  Igbtot = here->FEFET1Igb;
                  cgbhat = here->FEFET1Igb + here->FEFET1gIgbg * delvgs + here->FEFET1gIgbd
                         * delvds + here->FEFET1gIgbb * delvbs;
              }
              else
               {   Idtot = here->FEFET1cd + here->FEFET1cbd - here->FEFET1Igidl; /* bugfix */
                   cdhat = Idtot + here->FEFET1gbd * delvbd_jct + here->FEFET1gmbs 
                         * delvbd + here->FEFET1gm * delvgd 
                         - (here->FEFET1gds + here->FEFET1ggidls) * delvds 
                         - here->FEFET1ggidlg * delvgs - here->FEFET1ggidlb * delvbs;

                  Igstot = here->FEFET1Igs + here->FEFET1Igcd;
                  cgshat = Igstot + here->FEFET1gIgsg * delvgs + here->FEFET1gIgcdg * delvgd
                         - here->FEFET1gIgcdd * delvds + here->FEFET1gIgcdb * delvbd;

                  Igdtot = here->FEFET1Igd + here->FEFET1Igcs;
                  cgdhat = Igdtot + (here->FEFET1gIgdg + here->FEFET1gIgcsg) * delvgd
                         - here->FEFET1gIgcsd * delvds + here->FEFET1gIgcsb * delvbd;

                  Igbtot = here->FEFET1Igb;
                  cgbhat = here->FEFET1Igb + here->FEFET1gIgbg * delvgd - here->FEFET1gIgbd
                         * delvds + here->FEFET1gIgbb * delvbd;
              }

              Isestot = here->FEFET1gstot * (*(ckt->CKTstate0 + here->FEFET1vses));
              cseshat = Isestot + here->FEFET1gstot * delvses
                      + here->FEFET1gstotd * delvds + here->FEFET1gstotg * delvgs
                      + here->FEFET1gstotb * delvbs;

              Idedtot = here->FEFET1gdtot * vdedo;
              cdedhat = Idedtot + here->FEFET1gdtot * delvded
                      + here->FEFET1gdtotd * delvds + here->FEFET1gdtotg * delvgs
                      + here->FEFET1gdtotb * delvbs;

              /*
               *  Check convergence
               */

              if ((here->FEFET1off == 0)  || (!(ckt->CKTmode & MODEINITFIX)))
	      {   tol0 = ckt->CKTreltol * MAX(fabs(cdhat), fabs(Idtot))
		       + ckt->CKTabstol;
                  tol1 = ckt->CKTreltol * MAX(fabs(cseshat), fabs(Isestot))
                       + ckt->CKTabstol;
                  tol2 = ckt->CKTreltol * MAX(fabs(cdedhat), fabs(Idedtot))
                       + ckt->CKTabstol;
                  tol3 = ckt->CKTreltol * MAX(fabs(cgshat), fabs(Igstot))
                       + ckt->CKTabstol;
                  tol4 = ckt->CKTreltol * MAX(fabs(cgdhat), fabs(Igdtot))
                       + ckt->CKTabstol;
                  tol5 = ckt->CKTreltol * MAX(fabs(cgbhat), fabs(Igbtot))
                       + ckt->CKTabstol;

                  if ((fabs(cdhat - Idtot) >= tol0) || (fabs(cseshat - Isestot) >= tol1)
                      || (fabs(cdedhat - Idedtot) >= tol2))
		  {   ckt->CKTnoncon++;
                      return(OK);
                  } 

                  if ((fabs(cgshat - Igstot) >= tol3) || (fabs(cgdhat - Igdtot) >= tol4)
                      || (fabs(cgbhat - Igbtot) >= tol5))
                  {   ckt->CKTnoncon++;
                      return(OK);
                  }

                  Ibtot = here->FEFET1cbs + here->FEFET1cbd
			- here->FEFET1Igidl - here->FEFET1Igisl - here->FEFET1csub;
                  if (here->FEFET1mode >= 0)
                  {   cbhat = Ibtot + here->FEFET1gbd * delvbd_jct
                            + here->FEFET1gbs * delvbs_jct - (here->FEFET1gbbs + here->FEFET1ggidlb)
                            * delvbs - (here->FEFET1gbgs + here->FEFET1ggidlg) * delvgs
                            - (here->FEFET1gbds + here->FEFET1ggidld) * delvds
			    - here->FEFET1ggislg * delvgd - here->FEFET1ggislb* delvbd + here->FEFET1ggisls * delvds ;
		  }
		  else
		  {   cbhat = Ibtot + here->FEFET1gbs * delvbs_jct + here->FEFET1gbd 
                         * delvbd_jct - (here->FEFET1gbbs + here->FEFET1ggislb) * delvbd
                         - (here->FEFET1gbgs + here->FEFET1ggislg) * delvgd
			 + (here->FEFET1gbds + here->FEFET1ggisld - here->FEFET1ggidls) * delvds
			 - here->FEFET1ggidlg * delvgs - here->FEFET1ggidlb * delvbs; 
		  }
                  tol6 = ckt->CKTreltol * MAX(fabs(cbhat), 
			fabs(Ibtot)) + ckt->CKTabstol;
                  if (fabs(cbhat - Ibtot) > tol6) 
		  {   ckt->CKTnoncon++;
                      return(OK);
                  }
              }
         }
    }
    return(OK);
}
