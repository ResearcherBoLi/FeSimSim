/**** BSIM4.5.0 Released by Xuemei (Jane) Xi 07/29/2005 ****/

/**********
 * Copyright 2005 Regents of the University of California. All rights reserved.
 * File: b4acld.c of BSIM4.5.0.
 * Author: 2000 Weidong Liu
 * Authors: 2001- Xuemei Xi, Mohan Dunga, Ali Niknejad, Chenming Hu.
 * Project Director: Prof. Chenming Hu.
 * Modified by Xuemei Xi, 10/05/2001.
 **********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "fefet1def.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
FEFET1acLoad(
GENmodel *inModel,
CKTcircuit *ckt)
{
FEFET1model *model = (FEFET1model*)inModel;
FEFET1instance *here;

double gjbd, gjbs, geltd, gcrg, gcrgg, gcrgd, gcrgs, gcrgb;
double xcbgb, xcbdb, xcbsb, xcbbb;
double xcggbr, xcgdbr, xcgsbr, xcgbbr, xcggbi, xcgdbi, xcgsbi, xcgbbi;
double Cggr, Cgdr, Cgsr, Cgbr, Cggi, Cgdi, Cgsi, Cgbi;
double xcddbr, xcdgbr, xcdsbr, xcdbbr, xcsdbr, xcsgbr, xcssbr, xcsbbr;
double xcddbi, xcdgbi, xcdsbi, xcdbbi, xcsdbi, xcsgbi, xcssbi, xcsbbi;
double xcdbdb, xcsbsb=0.0, xcgmgmb=0.0, xcgmdb=0.0, xcgmsb=0.0, xcdgmb, xcsgmb;
double xcgmbb=0.0, xcbgmb;
double capbd, capbs, omega;
double gstot, gstotd, gstotg, gstots, gstotb, gspr;
double gdtot, gdtotd, gdtotg, gdtots, gdtotb, gdpr;
double gIstotg, gIstotd, gIstots, gIstotb;
double gIdtotg, gIdtotd, gIdtots, gIdtotb;
double gIbtotg, gIbtotd, gIbtots, gIbtotb;
double gIgtotg, gIgtotd, gIgtots, gIgtotb;
double cgso, cgdo, cgbo;
double gbspsp, gbbdp, gbbsp, gbspg, gbspb;
double gbspdp, gbdpdp, gbdpg, gbdpb, gbdpsp;
double T0=0.0, T1, T2, T3;
double Csg, Csd, Css;
double Cdgr, Cddr, Cdsr, Cdbr, Csgr, Csdr, Cssr, Csbr;
double Cdgi, Cddi, Cdsi, Cdbi, Csgi, Csdi, Cssi, Csbi;
double gmr, gmi, gmbsr, gmbsi, gdsr, gdsi;
double FwdSumr, RevSumr, Gmr, Gmbsr;
double FwdSumi, RevSumi, Gmi, Gmbsi;
struct fefet1SizeDependParam *pParam;
double ggidld, ggidlg, ggidlb, ggislg, ggislb, ggisls;

double m;

    omega = ckt->CKTomega;
    for (; model != NULL; model = FEFET1nextModel(model)) 
    {    for (here = FEFET1instances(model); here!= NULL;
              here = FEFET1nextInstance(here)) 
         {
	            pParam = here->pParam;
              capbd = here->FEFET1capbd;
              capbs = here->FEFET1capbs;
              cgso = here->FEFET1cgso;
              cgdo = here->FEFET1cgdo;
              cgbo = pParam->FEFET1cgbo;

              Csd = -(here->FEFET1cddb + here->FEFET1cgdb + here->FEFET1cbdb);
              Csg = -(here->FEFET1cdgb + here->FEFET1cggb + here->FEFET1cbgb);
              Css = -(here->FEFET1cdsb + here->FEFET1cgsb + here->FEFET1cbsb);

              if (here->FEFET1acnqsMod)
              {   T0 = omega * here->FEFET1taunet;
                  T1 = T0 * T0;
                  T2 = 1.0 / (1.0 + T1);
                  T3 = T0 * T2;

                  gmr = here->FEFET1gm * T2;
                  gmbsr = here->FEFET1gmbs * T2;
                  gdsr = here->FEFET1gds * T2;

                  gmi = -here->FEFET1gm * T3;
                  gmbsi = -here->FEFET1gmbs * T3;
                  gdsi = -here->FEFET1gds * T3;

                  Cddr = here->FEFET1cddb * T2;
                  Cdgr = here->FEFET1cdgb * T2;
                  Cdsr = here->FEFET1cdsb * T2;
                  Cdbr = -(Cddr + Cdgr + Cdsr);

		  /* WDLiu: Cxyi mulitplied by jomega below, and actually to be of conductance */
                  Cddi = here->FEFET1cddb * T3 * omega;
                  Cdgi = here->FEFET1cdgb * T3 * omega;
                  Cdsi = here->FEFET1cdsb * T3 * omega;
                  Cdbi = -(Cddi + Cdgi + Cdsi);

                  Csdr = Csd * T2;
                  Csgr = Csg * T2;
                  Cssr = Css * T2;
                  Csbr = -(Csdr + Csgr + Cssr);

                  Csdi = Csd * T3 * omega;
                  Csgi = Csg * T3 * omega;
                  Cssi = Css * T3 * omega;
                  Csbi = -(Csdi + Csgi + Cssi);

		  Cgdr = -(Cddr + Csdr + here->FEFET1cbdb);
		  Cggr = -(Cdgr + Csgr + here->FEFET1cbgb);
		  Cgsr = -(Cdsr + Cssr + here->FEFET1cbsb);
		  Cgbr = -(Cgdr + Cggr + Cgsr);

		  Cgdi = -(Cddi + Csdi);
		  Cggi = -(Cdgi + Csgi);
		  Cgsi = -(Cdsi + Cssi);
		  Cgbi = -(Cgdi + Cggi + Cgsi);
              }
              else /* QS */
              {   gmr = here->FEFET1gm;
                  gmbsr = here->FEFET1gmbs;
                  gdsr = here->FEFET1gds;
                  gmi = gmbsi = gdsi = 0.0;

                  Cddr = here->FEFET1cddb;
                  Cdgr = here->FEFET1cdgb;
                  Cdsr = here->FEFET1cdsb;
                  Cdbr = -(Cddr + Cdgr + Cdsr);
                  Cddi = Cdgi = Cdsi = Cdbi = 0.0;

                  Csdr = Csd;
                  Csgr = Csg;
                  Cssr = Css;
                  Csbr = -(Csdr + Csgr + Cssr);
                  Csdi = Csgi = Cssi = Csbi = 0.0;

                  Cgdr = here->FEFET1cgdb;
                  Cggr = here->FEFET1cggb;
                  Cgsr = here->FEFET1cgsb;
                  Cgbr = -(Cgdr + Cggr + Cgsr);
                  Cgdi = Cggi = Cgsi = Cgbi = 0.0;
              }


              if (here->FEFET1mode >= 0) 
	      {   Gmr = gmr;
                  Gmbsr = gmbsr;
                  FwdSumr = Gmr + Gmbsr;
                  RevSumr = 0.0;
		  Gmi = gmi;
                  Gmbsi = gmbsi;
                  FwdSumi = Gmi + Gmbsi;
                  RevSumi = 0.0;

                  gbbdp = -(here->FEFET1gbds);
                  gbbsp = here->FEFET1gbds + here->FEFET1gbgs + here->FEFET1gbbs;
                  gbdpg = here->FEFET1gbgs;
                  gbdpdp = here->FEFET1gbds;
                  gbdpb = here->FEFET1gbbs;
                  gbdpsp = -(gbdpg + gbdpdp + gbdpb);

                  gbspdp = 0.0;
                  gbspg = 0.0;
                  gbspb = 0.0;
                  gbspsp = 0.0;

                  if (model->FEFET1igcMod)
                  {   gIstotg = here->FEFET1gIgsg + here->FEFET1gIgcsg;
                      gIstotd = here->FEFET1gIgcsd;
                      gIstots = here->FEFET1gIgss + here->FEFET1gIgcss;
                      gIstotb = here->FEFET1gIgcsb;

                      gIdtotg = here->FEFET1gIgdg + here->FEFET1gIgcdg;
                      gIdtotd = here->FEFET1gIgdd + here->FEFET1gIgcdd;
                      gIdtots = here->FEFET1gIgcds;
                      gIdtotb = here->FEFET1gIgcdb;
                  }
                  else
                  {   gIstotg = gIstotd = gIstots = gIstotb = 0.0;
                      gIdtotg = gIdtotd = gIdtots = gIdtotb = 0.0;
                  }

                  if (model->FEFET1igbMod)
                  {   gIbtotg = here->FEFET1gIgbg;
                      gIbtotd = here->FEFET1gIgbd;
                      gIbtots = here->FEFET1gIgbs;
                      gIbtotb = here->FEFET1gIgbb;
                  }
                  else
                      gIbtotg = gIbtotd = gIbtots = gIbtotb = 0.0;

                  if ((model->FEFET1igcMod != 0) || (model->FEFET1igbMod != 0))
                  {   gIgtotg = gIstotg + gIdtotg + gIbtotg;
                      gIgtotd = gIstotd + gIdtotd + gIbtotd ;
                      gIgtots = gIstots + gIdtots + gIbtots;
                      gIgtotb = gIstotb + gIdtotb + gIbtotb;
                  }
                  else
                      gIgtotg = gIgtotd = gIgtots = gIgtotb = 0.0;

                  if (here->FEFET1rgateMod == 2)
                      T0 = *(ckt->CKTstates[0] + here->FEFET1vges)
                         - *(ckt->CKTstates[0] + here->FEFET1vgs);
                  else if (here->FEFET1rgateMod == 3)
                      T0 = *(ckt->CKTstates[0] + here->FEFET1vgms)
                         - *(ckt->CKTstates[0] + here->FEFET1vgs);
                  if (here->FEFET1rgateMod > 1)
                  {   gcrgd = here->FEFET1gcrgd * T0;
                      gcrgg = here->FEFET1gcrgg * T0;
                      gcrgs = here->FEFET1gcrgs * T0;
                      gcrgb = here->FEFET1gcrgb * T0;
                      gcrgg -= here->FEFET1gcrg;
                      gcrg = here->FEFET1gcrg;
                  }
                  else
                      gcrg = gcrgd = gcrgg = gcrgs = gcrgb = 0.0;

                  if (here->FEFET1rgateMod == 3)
                  {   xcgmgmb = (cgdo + cgso + pParam->FEFET1cgbo) * omega;
                      xcgmdb = -cgdo * omega;
                      xcgmsb = -cgso * omega;
                      xcgmbb = -pParam->FEFET1cgbo * omega;
    
                      xcdgmb = xcgmdb;
                      xcsgmb = xcgmsb;
                      xcbgmb = xcgmbb;
    
                      xcggbr = Cggr * omega;
                      xcgdbr = Cgdr * omega;
                      xcgsbr = Cgsr * omega;
                      xcgbbr = -(xcggbr + xcgdbr + xcgsbr);
    
                      xcdgbr = Cdgr * omega;
                      xcsgbr = Csgr * omega;
                      xcbgb = here->FEFET1cbgb * omega;
                  }
                  else
                  {   xcggbr = (Cggr + cgdo + cgso + pParam->FEFET1cgbo ) * omega;
                      xcgdbr = (Cgdr - cgdo) * omega;
                      xcgsbr = (Cgsr - cgso) * omega;
                      xcgbbr = -(xcggbr + xcgdbr + xcgsbr);
    
                      xcdgbr = (Cdgr - cgdo) * omega;
                      xcsgbr = (Csgr - cgso) * omega;
                      xcbgb = (here->FEFET1cbgb - pParam->FEFET1cgbo) * omega;
    
                      xcdgmb = xcsgmb = xcbgmb = 0.0;
                  }
                  xcddbr = (Cddr + here->FEFET1capbd + cgdo) * omega;
                  xcdsbr = Cdsr * omega;
                  xcsdbr = Csdr * omega;
                  xcssbr = (here->FEFET1capbs + cgso + Cssr) * omega;
    
                  if (!here->FEFET1rbodyMod)
                  {   xcdbbr = -(xcdgbr + xcddbr + xcdsbr + xcdgmb);
                      xcsbbr = -(xcsgbr + xcsdbr + xcssbr + xcsgmb);

                      xcbdb = (here->FEFET1cbdb - here->FEFET1capbd) * omega;
                      xcbsb = (here->FEFET1cbsb - here->FEFET1capbs) * omega;
                      xcdbdb = 0.0;
                  }
                  else
                  {   xcdbbr = Cdbr * omega;
                      xcsbbr = -(xcsgbr + xcsdbr + xcssbr + xcsgmb)
			     + here->FEFET1capbs * omega;

                      xcbdb = here->FEFET1cbdb * omega;
                      xcbsb = here->FEFET1cbsb * omega;
    
                      xcdbdb = -here->FEFET1capbd * omega;
                      xcsbsb = -here->FEFET1capbs * omega;
                  }
                  xcbbb = -(xcbdb + xcbgb + xcbsb + xcbgmb);

                  xcdgbi = Cdgi;
                  xcsgbi = Csgi;
                  xcddbi = Cddi;
                  xcdsbi = Cdsi;
                  xcsdbi = Csdi;
                  xcssbi = Cssi;
                  xcdbbi = Cdbi;
                  xcsbbi = Csbi;
                  xcggbi = Cggi;
                  xcgdbi = Cgdi;
                  xcgsbi = Cgsi;
                  xcgbbi = Cgbi;
              } 
              else /* Reverse mode */
              {   Gmr = -gmr;
                  Gmbsr = -gmbsr;
                  FwdSumr = 0.0;
                  RevSumr = -(Gmr + Gmbsr);
                  Gmi = -gmi;
                  Gmbsi = -gmbsi;
                  FwdSumi = 0.0;
                  RevSumi = -(Gmi + Gmbsi);

                  gbbsp = -(here->FEFET1gbds);
                  gbbdp = here->FEFET1gbds + here->FEFET1gbgs + here->FEFET1gbbs;

                  gbdpg = 0.0;
                  gbdpsp = 0.0;
                  gbdpb = 0.0;
                  gbdpdp = 0.0;

                  gbspg = here->FEFET1gbgs;
                  gbspsp = here->FEFET1gbds;
                  gbspb = here->FEFET1gbbs;
                  gbspdp = -(gbspg + gbspsp + gbspb);

                  if (model->FEFET1igcMod)
                  {   gIstotg = here->FEFET1gIgsg + here->FEFET1gIgcdg;
                      gIstotd = here->FEFET1gIgcds;
                      gIstots = here->FEFET1gIgss + here->FEFET1gIgcdd;
                      gIstotb = here->FEFET1gIgcdb;

                      gIdtotg = here->FEFET1gIgdg + here->FEFET1gIgcsg;
                      gIdtotd = here->FEFET1gIgdd + here->FEFET1gIgcss;
                      gIdtots = here->FEFET1gIgcsd;
                      gIdtotb = here->FEFET1gIgcsb;
                  }
                  else
                  {   gIstotg = gIstotd = gIstots = gIstotb = 0.0;
                      gIdtotg = gIdtotd = gIdtots = gIdtotb  = 0.0;
                  }

                  if (model->FEFET1igbMod)
                  {   gIbtotg = here->FEFET1gIgbg;
                      gIbtotd = here->FEFET1gIgbs;
                      gIbtots = here->FEFET1gIgbd;
                      gIbtotb = here->FEFET1gIgbb;
                  }
                  else
                      gIbtotg = gIbtotd = gIbtots = gIbtotb = 0.0;

                  if ((model->FEFET1igcMod != 0) || (model->FEFET1igbMod != 0))
                  {   gIgtotg = gIstotg + gIdtotg + gIbtotg;
                      gIgtotd = gIstotd + gIdtotd + gIbtotd ;
                      gIgtots = gIstots + gIdtots + gIbtots;
                      gIgtotb = gIstotb + gIdtotb + gIbtotb;
                  }
                  else
                      gIgtotg = gIgtotd = gIgtots = gIgtotb = 0.0;

                  if (here->FEFET1rgateMod == 2)
                      T0 = *(ckt->CKTstates[0] + here->FEFET1vges)
                         - *(ckt->CKTstates[0] + here->FEFET1vgs);
                  else if (here->FEFET1rgateMod == 3)
                      T0 = *(ckt->CKTstates[0] + here->FEFET1vgms)
                         - *(ckt->CKTstates[0] + here->FEFET1vgs);
                  if (here->FEFET1rgateMod > 1)
                  {   gcrgd = here->FEFET1gcrgs * T0;
                      gcrgg = here->FEFET1gcrgg * T0;
                      gcrgs = here->FEFET1gcrgd * T0;
                      gcrgb = here->FEFET1gcrgb * T0;
                      gcrgg -= here->FEFET1gcrg;
                      gcrg = here->FEFET1gcrg;
                  }
                  else
                      gcrg = gcrgd = gcrgg = gcrgs = gcrgb = 0.0;

                  if (here->FEFET1rgateMod == 3)
                  {   xcgmgmb = (cgdo + cgso + pParam->FEFET1cgbo) * omega;
                      xcgmdb = -cgdo * omega;
                      xcgmsb = -cgso * omega;
                      xcgmbb = -pParam->FEFET1cgbo * omega;
    
                      xcdgmb = xcgmdb;
                      xcsgmb = xcgmsb;
                      xcbgmb = xcgmbb;
   
                      xcggbr = Cggr * omega;
                      xcgdbr = Cgsr * omega;
                      xcgsbr = Cgdr * omega;
                      xcgbbr = -(xcggbr + xcgdbr + xcgsbr);
    
                      xcdgbr = Csgr * omega;
                      xcsgbr = Cdgr * omega;
                      xcbgb = here->FEFET1cbgb * omega;
                  }
                  else
                  {   xcggbr = (Cggr + cgdo + cgso + pParam->FEFET1cgbo ) * omega;
                      xcgdbr = (Cgsr - cgdo) * omega;
                      xcgsbr = (Cgdr - cgso) * omega;
                      xcgbbr = -(xcggbr + xcgdbr + xcgsbr);
    
                      xcdgbr = (Csgr - cgdo) * omega;
                      xcsgbr = (Cdgr - cgso) * omega;
                      xcbgb = (here->FEFET1cbgb - pParam->FEFET1cgbo) * omega;
    
                      xcdgmb = xcsgmb = xcbgmb = 0.0;
                  }
                  xcddbr = (here->FEFET1capbd + cgdo + Cssr) * omega;
                  xcdsbr = Csdr * omega;
                  xcsdbr = Cdsr * omega;
                  xcssbr = (Cddr + here->FEFET1capbs + cgso) * omega;
    
                  if (!here->FEFET1rbodyMod)
                  {   xcdbbr = -(xcdgbr + xcddbr + xcdsbr + xcdgmb);
                      xcsbbr = -(xcsgbr + xcsdbr + xcssbr + xcsgmb);

                      xcbdb = (here->FEFET1cbsb - here->FEFET1capbd) * omega;
                      xcbsb = (here->FEFET1cbdb - here->FEFET1capbs) * omega;
                      xcdbdb = 0.0;
                  }
                  else
                  {   xcdbbr = -(xcdgbr + xcddbr + xcdsbr + xcdgmb)
                             + here->FEFET1capbd * omega;
                      xcsbbr = Cdbr * omega;

                      xcbdb = here->FEFET1cbsb * omega;
                      xcbsb = here->FEFET1cbdb * omega;
                      xcdbdb = -here->FEFET1capbd * omega;
                      xcsbsb = -here->FEFET1capbs * omega;
                  }
                  xcbbb = -(xcbgb + xcbdb + xcbsb + xcbgmb);

                  xcdgbi = Csgi;
                  xcsgbi = Cdgi;
                  xcddbi = Cssi;
                  xcdsbi = Csdi;
                  xcsdbi = Cdsi;
                  xcssbi = Cddi;
                  xcdbbi = Csbi;
                  xcsbbi = Cdbi;
                  xcggbi = Cggi;
                  xcgdbi = Cgsi;
                  xcgsbi = Cgdi;
                  xcgbbi = Cgbi;
              }

              if (model->FEFET1rdsMod == 1)
              {   gstot = here->FEFET1gstot;
                  gstotd = here->FEFET1gstotd;
                  gstotg = here->FEFET1gstotg;
                  gstots = here->FEFET1gstots - gstot;
                  gstotb = here->FEFET1gstotb;

                  gdtot = here->FEFET1gdtot;
                  gdtotd = here->FEFET1gdtotd - gdtot;
                  gdtotg = here->FEFET1gdtotg;
                  gdtots = here->FEFET1gdtots;
                  gdtotb = here->FEFET1gdtotb;
              }
              else
              {   gstot = gstotd = gstotg = gstots = gstotb = 0.0;
                  gdtot = gdtotd = gdtotg = gdtots = gdtotb = 0.0;
              }


              /*
               * Loading AC matrix
               */

   	          m = here->FEFET1m;

              if (!model->FEFET1rdsMod)
              {   gdpr = here->FEFET1drainConductance;
                  gspr = here->FEFET1sourceConductance;
              }
              else
                  gdpr = gspr = 0.0;

              if (!here->FEFET1rbodyMod)
              {   gjbd = here->FEFET1gbd;
                  gjbs = here->FEFET1gbs;
              }
              else
                  gjbd = gjbs = 0.0;

              geltd = here->FEFET1grgeltd;

              if (here->FEFET1rgateMod == 1)
              {   *(here->FEFET1GEgePtr) += m * geltd;
                  *(here->FEFET1GPgePtr) -= m * geltd;
                  *(here->FEFET1GEgpPtr) -= m * geltd;

                  *(here->FEFET1GPgpPtr +1) += m * xcggbr;
		  *(here->FEFET1GPgpPtr) += m * (geltd + xcggbi + gIgtotg);
                  *(here->FEFET1GPdpPtr +1) += m * xcgdbr;
                  *(here->FEFET1GPdpPtr) += m * (xcgdbi + gIgtotd);
                  *(here->FEFET1GPspPtr +1) += m * xcgsbr;
                  *(here->FEFET1GPspPtr) += m * (xcgsbi + gIgtots);
                  *(here->FEFET1GPbpPtr +1) += m * xcgbbr;
                  *(here->FEFET1GPbpPtr) += m * (xcgbbi + gIgtotb);
              } /* WDLiu: gcrg already subtracted from all gcrgg below */
              else if (here->FEFET1rgateMod == 2)
              {   *(here->FEFET1GEgePtr) += m * gcrg;
                  *(here->FEFET1GEgpPtr) += m * gcrgg;
                  *(here->FEFET1GEdpPtr) += m * gcrgd;
                  *(here->FEFET1GEspPtr) += m * gcrgs;
                  *(here->FEFET1GEbpPtr) += m * gcrgb;

                  *(here->FEFET1GPgePtr) -= m * gcrg;
                  *(here->FEFET1GPgpPtr +1) += m * xcggbr;
		  *(here->FEFET1GPgpPtr) -= m * (gcrgg - xcggbi - gIgtotg);
                  *(here->FEFET1GPdpPtr +1) += m * xcgdbr;
		  *(here->FEFET1GPdpPtr) -= m * (gcrgd - xcgdbi - gIgtotd);
                  *(here->FEFET1GPspPtr +1) += m * xcgsbr;
		  *(here->FEFET1GPspPtr) -= m * (gcrgs - xcgsbi - gIgtots);
                  *(here->FEFET1GPbpPtr +1) += m * xcgbbr;
		  *(here->FEFET1GPbpPtr) -= m * (gcrgb - xcgbbi - gIgtotb);
              }
              else if (here->FEFET1rgateMod == 3)
              {   *(here->FEFET1GEgePtr) += m * geltd;
                  *(here->FEFET1GEgmPtr) -= m * geltd;
                  *(here->FEFET1GMgePtr) -= m * geltd;
                  *(here->FEFET1GMgmPtr) += m * (geltd + gcrg);
                  *(here->FEFET1GMgmPtr +1) += m * xcgmgmb;
   
                  *(here->FEFET1GMdpPtr) += m * gcrgd;
                  *(here->FEFET1GMdpPtr +1) += m * xcgmdb;
                  *(here->FEFET1GMgpPtr) += m * gcrgg;
                  *(here->FEFET1GMspPtr) += m * gcrgs;
                  *(here->FEFET1GMspPtr +1) += m * xcgmsb;
                  *(here->FEFET1GMbpPtr) += m * gcrgb;
                  *(here->FEFET1GMbpPtr +1) += m * xcgmbb;
   
                  *(here->FEFET1DPgmPtr +1) += m * xcdgmb;
                  *(here->FEFET1GPgmPtr) -= m * gcrg;
                  *(here->FEFET1SPgmPtr +1) += m * xcsgmb;
                  *(here->FEFET1BPgmPtr +1) += m * xcbgmb;
   
                  *(here->FEFET1GPgpPtr) -= m * (gcrgg - xcggbi - gIgtotg);
                  *(here->FEFET1GPgpPtr +1) += m * xcggbr;
                  *(here->FEFET1GPdpPtr) -= m * (gcrgd - xcgdbi - gIgtotd);
                  *(here->FEFET1GPdpPtr +1) += m * xcgdbr;
                  *(here->FEFET1GPspPtr) -= m * (gcrgs - xcgsbi - gIgtots);
                  *(here->FEFET1GPspPtr +1) += m * xcgsbr;
                  *(here->FEFET1GPbpPtr) -= m * (gcrgb - xcgbbi - gIgtotb);
                  *(here->FEFET1GPbpPtr +1) += m * xcgbbr;
              }
              else
              {   *(here->FEFET1GPgpPtr +1) += m * xcggbr;
                  *(here->FEFET1GPgpPtr) += m * (xcggbi + gIgtotg);
                  *(here->FEFET1GPdpPtr +1) += m * xcgdbr;
                  *(here->FEFET1GPdpPtr) += m * (xcgdbi + gIgtotd);
                  *(here->FEFET1GPspPtr +1) += m * xcgsbr;
                  *(here->FEFET1GPspPtr) += m * (xcgsbi + gIgtots);
                  *(here->FEFET1GPbpPtr +1) += m * xcgbbr;
                  *(here->FEFET1GPbpPtr) += m * (xcgbbi + gIgtotb);
              }

              if (model->FEFET1rdsMod)
              {   (*(here->FEFET1DgpPtr) += m * gdtotg);
                  (*(here->FEFET1DspPtr) += m * gdtots);
                  (*(here->FEFET1DbpPtr) += m * gdtotb);
                  (*(here->FEFET1SdpPtr) += m * gstotd);
                  (*(here->FEFET1SgpPtr) += m * gstotg);
                  (*(here->FEFET1SbpPtr) += m * gstotb);
              }

              *(here->FEFET1DPdpPtr +1) += m * (xcddbr + gdsi + RevSumi);
              *(here->FEFET1DPdpPtr) += m * (gdpr + xcddbi + gdsr + here->FEFET1gbd 
				     - gdtotd + RevSumr + gbdpdp - gIdtotd);
              *(here->FEFET1DPdPtr) -= m * (gdpr + gdtot);
              *(here->FEFET1DPgpPtr +1) += m * (xcdgbr + Gmi);
              *(here->FEFET1DPgpPtr) += m * (Gmr + xcdgbi - gdtotg + gbdpg - gIdtotg);
              *(here->FEFET1DPspPtr +1) += m * (xcdsbr - gdsi - FwdSumi);
              *(here->FEFET1DPspPtr) -= m * (gdsr - xcdsbi + FwdSumr + gdtots - gbdpsp + gIdtots);
              *(here->FEFET1DPbpPtr +1) += m * (xcdbbr + Gmbsi);
              *(here->FEFET1DPbpPtr) -= m * (gjbd + gdtotb - xcdbbi - Gmbsr - gbdpb + gIdtotb);

              *(here->FEFET1DdpPtr) -= m * (gdpr - gdtotd);
              *(here->FEFET1DdPtr) += m * (gdpr + gdtot);

              *(here->FEFET1SPdpPtr +1) += m * (xcsdbr - gdsi - RevSumi);
              *(here->FEFET1SPdpPtr) -= m * (gdsr - xcsdbi + gstotd + RevSumr - gbspdp + gIstotd);
              *(here->FEFET1SPgpPtr +1) += m * (xcsgbr - Gmi);
              *(here->FEFET1SPgpPtr) -= m * (Gmr - xcsgbi + gstotg - gbspg + gIstotg);
              *(here->FEFET1SPspPtr +1) += m * (xcssbr + gdsi + FwdSumi);
              *(here->FEFET1SPspPtr) += m * (gspr + xcssbi + gdsr + here->FEFET1gbs
				     - gstots + FwdSumr + gbspsp - gIstots);
              *(here->FEFET1SPsPtr) -= m * (gspr + gstot);
              *(here->FEFET1SPbpPtr +1) += m * (xcsbbr - Gmbsi);
              *(here->FEFET1SPbpPtr) -= m * (gjbs + gstotb - xcsbbi + Gmbsr - gbspb + gIstotb);

              *(here->FEFET1SspPtr) -= m * (gspr - gstots);
              *(here->FEFET1SsPtr) += m * (gspr + gstot);

              *(here->FEFET1BPdpPtr +1) += m * xcbdb;
              *(here->FEFET1BPdpPtr) -= m * (gjbd - gbbdp + gIbtotd);
              *(here->FEFET1BPgpPtr +1) += m * xcbgb;
              *(here->FEFET1BPgpPtr) -= m * (here->FEFET1gbgs + gIbtotg);
              *(here->FEFET1BPspPtr +1) += m * xcbsb;
              *(here->FEFET1BPspPtr) -= m * (gjbs - gbbsp + gIbtots);
              *(here->FEFET1BPbpPtr +1) += m * xcbbb;
              *(here->FEFET1BPbpPtr) += m * (gjbd + gjbs - here->FEFET1gbbs
				     - gIbtotb);
           ggidld = here->FEFET1ggidld;
           ggidlg = here->FEFET1ggidlg;
           ggidlb = here->FEFET1ggidlb;
           ggislg = here->FEFET1ggislg;
           ggisls = here->FEFET1ggisls;
           ggislb = here->FEFET1ggislb;

           /* stamp gidl */
           (*(here->FEFET1DPdpPtr) += m * ggidld);
           (*(here->FEFET1DPgpPtr) += m * ggidlg);
           (*(here->FEFET1DPspPtr) -= m * ((ggidlg + ggidld) + ggidlb));
           (*(here->FEFET1DPbpPtr) += m * ggidlb);
           (*(here->FEFET1BPdpPtr) -= m * ggidld);
           (*(here->FEFET1BPgpPtr) -= m * ggidlg);
           (*(here->FEFET1BPspPtr) += m * ((ggidlg + ggidld) + ggidlb));
           (*(here->FEFET1BPbpPtr) -= m * ggidlb);
            /* stamp gisl */
           (*(here->FEFET1SPdpPtr) -= m * ((ggisls + ggislg) + ggislb));
           (*(here->FEFET1SPgpPtr) += m * ggislg);
           (*(here->FEFET1SPspPtr) += m * ggisls);
           (*(here->FEFET1SPbpPtr) += m * ggislb);
           (*(here->FEFET1BPdpPtr) += m * ((ggislg + ggisls) + ggislb));
           (*(here->FEFET1BPgpPtr) -= m * ggislg);
           (*(here->FEFET1BPspPtr) -= m * ggisls);
           (*(here->FEFET1BPbpPtr) -= m * ggislb);

              if (here->FEFET1rbodyMod)
              {   (*(here->FEFET1DPdbPtr +1) += m * xcdbdb);
                  (*(here->FEFET1DPdbPtr) -= m * here->FEFET1gbd);
                  (*(here->FEFET1SPsbPtr +1) += m * xcsbsb);
                  (*(here->FEFET1SPsbPtr) -= m * here->FEFET1gbs);

                  (*(here->FEFET1DBdpPtr +1) += m * xcdbdb);
                  (*(here->FEFET1DBdpPtr) -= m * here->FEFET1gbd);
                  (*(here->FEFET1DBdbPtr +1) -= m * xcdbdb);
                  (*(here->FEFET1DBdbPtr) += m * (here->FEFET1gbd + here->FEFET1grbpd 
                                          + here->FEFET1grbdb));
                  (*(here->FEFET1DBbpPtr) -= m * here->FEFET1grbpd);
                  (*(here->FEFET1DBbPtr) -= m * here->FEFET1grbdb);

                  (*(here->FEFET1BPdbPtr) -= m * here->FEFET1grbpd);
                  (*(here->FEFET1BPbPtr) -= m * here->FEFET1grbpb);
                  (*(here->FEFET1BPsbPtr) -= m * here->FEFET1grbps);
                  (*(here->FEFET1BPbpPtr) += m * (here->FEFET1grbpd + here->FEFET1grbps 
					  + here->FEFET1grbpb));
		  /* WDLiu: (-here->FEFET1gbbs) already added to BPbpPtr */

                  (*(here->FEFET1SBspPtr +1) += m * xcsbsb);
                  (*(here->FEFET1SBspPtr) -= m * here->FEFET1gbs);
                  (*(here->FEFET1SBbpPtr) -= m * here->FEFET1grbps);
                  (*(here->FEFET1SBbPtr) -= m * here->FEFET1grbsb);
                  (*(here->FEFET1SBsbPtr +1) -= m * xcsbsb);
                  (*(here->FEFET1SBsbPtr) += m * (here->FEFET1gbs
					  + here->FEFET1grbps + here->FEFET1grbsb));

                  (*(here->FEFET1BdbPtr) -= m * here->FEFET1grbdb);
                  (*(here->FEFET1BbpPtr) -= m * here->FEFET1grbpb);
                  (*(here->FEFET1BsbPtr) -= m * here->FEFET1grbsb);
                  (*(here->FEFET1BbPtr) += m * (here->FEFET1grbsb + here->FEFET1grbdb
                                        + here->FEFET1grbpb));
              }


	   /*
	    * WDLiu: The internal charge node generated for transient NQS is not needed for
	    *        AC NQS. The following is not doing a real job, but we have to keep it;
	    *        otherwise a singular AC NQS matrix may occur if the transient NQS is on.
	    *        The charge node is isolated from the instance.
	    */
           if (here->FEFET1trnqsMod)
           {   (*(here->FEFET1QqPtr) += m * 1.0);
               (*(here->FEFET1QgpPtr) += 0.0);
               (*(here->FEFET1QdpPtr) += 0.0);
               (*(here->FEFET1QspPtr) += 0.0);
               (*(here->FEFET1QbpPtr) += 0.0);

               (*(here->FEFET1DPqPtr) += 0.0);
               (*(here->FEFET1SPqPtr) += 0.0);
               (*(here->FEFET1GPqPtr) += 0.0);
           }
         }
    }
    return(OK);
}
