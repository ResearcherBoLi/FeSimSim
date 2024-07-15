/**********
Copyright 2024 Xidian University.	All rights reserved.
 Author: Bo Li	at Hangzhou Institute of Technology	 
 Modified: 2020/09/09  Bo Li
 Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "ngspice/complex.h"
#include "ngspice/sperror.h"
#include "fefet1def.h"
#include "ngspice/suffix.h"

int
FEFET1pzLoad(
GENmodel *inModel,
CKTcircuit *ckt,
SPcomplex *s)
{
FEFET1model *model = (FEFET1model*)inModel;
FEFET1instance *here;

double gjbd, gjbs, geltd, gcrg, gcrgg, gcrgd, gcrgs, gcrgb;
double xcggb, xcgdb, xcgsb, xcgbb, xcbgb, xcbdb, xcbsb, xcbbb;
double xcdgb, xcddb, xcdsb, xcdbb, xcsgb, xcsdb, xcssb, xcsbb;
double gds, capbd, capbs, FwdSum, RevSum, Gm, Gmbs;
double gstot, gstotd, gstotg, gstots, gstotb, gspr;
double gdtot, gdtotd, gdtotg, gdtots, gdtotb, gdpr;
double gIstotg, gIstotd, gIstots, gIstotb;
double gIdtotg, gIdtotd, gIdtots, gIdtotb;
double gIbtotg, gIbtotd, gIbtots, gIbtotb;
double gIgtotg, gIgtotd, gIgtots, gIgtotb;
double cgso, cgdo, cgbo;
double xcdbdb=0.0, xcsbsb=0.0, xcgmgmb=0.0, xcgmdb=0.0, xcgmsb=0.0, xcdgmb=0.0, xcsgmb=0.0;
double xcgmbb=0.0, xcbgmb=0.0;
double dxpart, sxpart, xgtg, xgtd, xgts, xgtb, xcqgb=0.0, xcqdb=0.0, xcqsb=0.0, xcqbb=0.0;
double gbspsp, gbbdp, gbbsp, gbspg, gbspb;
double gbspdp, gbdpdp, gbdpg, gbdpb, gbdpsp;
double ddxpart_dVd, ddxpart_dVg, ddxpart_dVb, ddxpart_dVs;
double dsxpart_dVd, dsxpart_dVg, dsxpart_dVb, dsxpart_dVs;
double T0=0.0, T1, CoxWL, qcheq, Cdg, Cdd, Cds, Csg, Csd, Css;
double ScalingFactor = 1.0e-9;
struct fefet1SizeDependParam *pParam;
double ggidld, ggidlg, ggidlb, ggislg, ggislb, ggisls;

double m;

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

              if (here->FEFET1mode >= 0) 
              {   Gm = here->FEFET1gm;
                  Gmbs = here->FEFET1gmbs;
                  FwdSum = Gm + Gmbs;
                  RevSum = 0.0;

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

                  if (here->FEFET1acnqsMod == 0)
                  {   if (here->FEFET1rgateMod == 3)
                      {   xcgmgmb = cgdo + cgso + pParam->FEFET1cgbo;
                          xcgmdb = -cgdo;
                          xcgmsb = -cgso;
                          xcgmbb = -pParam->FEFET1cgbo;

                          xcdgmb = xcgmdb;
                          xcsgmb = xcgmsb;
                          xcbgmb = xcgmbb;

                          xcggb = here->FEFET1cggb;
                          xcgdb = here->FEFET1cgdb;
                          xcgsb = here->FEFET1cgsb;
                          xcgbb = -(xcggb + xcgdb + xcgsb);

                          xcdgb = here->FEFET1cdgb;
                          xcsgb = -(here->FEFET1cggb + here->FEFET1cbgb
                                + here->FEFET1cdgb);
                          xcbgb = here->FEFET1cbgb;
                      }
                      else
                      {   xcggb = here->FEFET1cggb + cgdo + cgso
                                + pParam->FEFET1cgbo;
                          xcgdb = here->FEFET1cgdb - cgdo;
                          xcgsb = here->FEFET1cgsb - cgso;
                          xcgbb = -(xcggb + xcgdb + xcgsb);

                          xcdgb = here->FEFET1cdgb - cgdo;
                          xcsgb = -(here->FEFET1cggb + here->FEFET1cbgb
                                + here->FEFET1cdgb + cgso);
                          xcbgb = here->FEFET1cbgb - pParam->FEFET1cgbo;

                          xcdgmb = xcsgmb = xcbgmb = 0.0;
                      }
                      xcddb = here->FEFET1cddb + here->FEFET1capbd + cgdo;
                      xcdsb = here->FEFET1cdsb;

                      xcsdb = -(here->FEFET1cgdb + here->FEFET1cbdb
                            + here->FEFET1cddb);
                      xcssb = here->FEFET1capbs + cgso - (here->FEFET1cgsb
                            + here->FEFET1cbsb + here->FEFET1cdsb);

                      if (!here->FEFET1rbodyMod)
                      {   xcdbb = -(xcdgb + xcddb + xcdsb + xcdgmb);
                          xcsbb = -(xcsgb + xcsdb + xcssb + xcsgmb);
                          xcbdb = here->FEFET1cbdb - here->FEFET1capbd;
                          xcbsb = here->FEFET1cbsb - here->FEFET1capbs;
                          xcdbdb = 0.0;
                      }
                      else
                      {   xcdbb  = -(here->FEFET1cddb + here->FEFET1cdgb
                                 + here->FEFET1cdsb);
                          xcsbb = -(xcsgb + xcsdb + xcssb + xcsgmb)
                                + here->FEFET1capbs;
                          xcbdb = here->FEFET1cbdb;
                          xcbsb = here->FEFET1cbsb;

                          xcdbdb = -here->FEFET1capbd;
                          xcsbsb = -here->FEFET1capbs;
                      }
                      xcbbb = -(xcbdb + xcbgb + xcbsb + xcbgmb);

                      xgtg = xgtd = xgts = xgtb = 0.0;
		      sxpart = 0.6;
                      dxpart = 0.4;
		      ddxpart_dVd = ddxpart_dVg = ddxpart_dVb 
				  = ddxpart_dVs = 0.0;
		      dsxpart_dVd = dsxpart_dVg = dsxpart_dVb 
				  = dsxpart_dVs = 0.0;
                  }
                  else
                  {   xcggb = xcgdb = xcgsb = xcgbb = 0.0;
                      xcbgb = xcbdb = xcbsb = xcbbb = 0.0;
                      xcdgb = xcddb = xcdsb = xcdbb = 0.0;
                      xcsgb = xcsdb = xcssb = xcsbb = 0.0;

		      xgtg = here->FEFET1gtg;
                      xgtd = here->FEFET1gtd;
                      xgts = here->FEFET1gts;
                      xgtb = here->FEFET1gtb;

                      xcqgb = here->FEFET1cqgb;
                      xcqdb = here->FEFET1cqdb;
                      xcqsb = here->FEFET1cqsb;
                      xcqbb = here->FEFET1cqbb;

		      CoxWL = model->FEFET1coxe * here->pParam->FEFET1weffCV
                            * here->FEFET1nf * here->pParam->FEFET1leffCV;
		      qcheq = -(here->FEFET1qgate + here->FEFET1qbulk);
		      if (fabs(qcheq) <= 1.0e-5 * CoxWL)
		      {   if (model->FEFET1xpart < 0.5)
		          {   dxpart = 0.4;
		          }
		          else if (model->FEFET1xpart > 0.5)
		          {   dxpart = 0.0;
		          }
		          else
		          {   dxpart = 0.5;
		          }
		          ddxpart_dVd = ddxpart_dVg = ddxpart_dVb
				      = ddxpart_dVs = 0.0;
		      }
		      else
		      {   dxpart = here->FEFET1qdrn / qcheq;
		          Cdd = here->FEFET1cddb;
		          Csd = -(here->FEFET1cgdb + here->FEFET1cddb
			      + here->FEFET1cbdb);
		          ddxpart_dVd = (Cdd - dxpart * (Cdd + Csd)) / qcheq;
		          Cdg = here->FEFET1cdgb;
		          Csg = -(here->FEFET1cggb + here->FEFET1cdgb
			      + here->FEFET1cbgb);
		          ddxpart_dVg = (Cdg - dxpart * (Cdg + Csg)) / qcheq;

		          Cds = here->FEFET1cdsb;
		          Css = -(here->FEFET1cgsb + here->FEFET1cdsb
			      + here->FEFET1cbsb);
		          ddxpart_dVs = (Cds - dxpart * (Cds + Css)) / qcheq;

		          ddxpart_dVb = -(ddxpart_dVd + ddxpart_dVg 
				      + ddxpart_dVs);
		      }
		      sxpart = 1.0 - dxpart;
		      dsxpart_dVd = -ddxpart_dVd;
		      dsxpart_dVg = -ddxpart_dVg;
		      dsxpart_dVs = -ddxpart_dVs;
		      dsxpart_dVb = -(dsxpart_dVd + dsxpart_dVg + dsxpart_dVs);
                  }
              }
              else
              {   Gm = -here->FEFET1gm;
                  Gmbs = -here->FEFET1gmbs;
                  FwdSum = 0.0;
                  RevSum = -(Gm + Gmbs);

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

                  if (here->FEFET1acnqsMod == 0)
                  {   if (here->FEFET1rgateMod == 3)
                      {   xcgmgmb = cgdo + cgso + pParam->FEFET1cgbo;
                          xcgmdb = -cgdo;
                          xcgmsb = -cgso;
                          xcgmbb = -pParam->FEFET1cgbo;
   
                          xcdgmb = xcgmdb;
                          xcsgmb = xcgmsb;
                          xcbgmb = xcgmbb;

                          xcggb = here->FEFET1cggb;
                          xcgdb = here->FEFET1cgsb;
                          xcgsb = here->FEFET1cgdb;
                          xcgbb = -(xcggb + xcgdb + xcgsb);

                          xcdgb = -(here->FEFET1cggb + here->FEFET1cbgb
                                + here->FEFET1cdgb);
                          xcsgb = here->FEFET1cdgb;
                          xcbgb = here->FEFET1cbgb;
                      }
                      else
                      {   xcggb = here->FEFET1cggb + cgdo + cgso
                                + pParam->FEFET1cgbo;
                          xcgdb = here->FEFET1cgsb - cgdo;
                          xcgsb = here->FEFET1cgdb - cgso;
                          xcgbb = -(xcggb + xcgdb + xcgsb);

                          xcdgb = -(here->FEFET1cggb + here->FEFET1cbgb
                                + here->FEFET1cdgb + cgdo);
                          xcsgb = here->FEFET1cdgb - cgso;
                          xcbgb = here->FEFET1cbgb - pParam->FEFET1cgbo;

                          xcdgmb = xcsgmb = xcbgmb = 0.0;
                      }
                      xcddb = here->FEFET1capbd + cgdo - (here->FEFET1cgsb
                            + here->FEFET1cbsb + here->FEFET1cdsb);
                      xcdsb = -(here->FEFET1cgdb + here->FEFET1cbdb
                            + here->FEFET1cddb);

                      xcsdb = here->FEFET1cdsb;
                      xcssb = here->FEFET1cddb + here->FEFET1capbs + cgso;

                      if (!here->FEFET1rbodyMod)
                      {   xcdbb = -(xcdgb + xcddb + xcdsb + xcdgmb);
                          xcsbb = -(xcsgb + xcsdb + xcssb + xcsgmb);
                          xcbdb = here->FEFET1cbsb - here->FEFET1capbd;
                          xcbsb = here->FEFET1cbdb - here->FEFET1capbs;
                          xcdbdb = 0.0;
                      }
                      else
                      {   xcdbb = -(xcdgb + xcddb + xcdsb + xcdgmb)
                                + here->FEFET1capbd;
                          xcsbb = -(here->FEFET1cddb + here->FEFET1cdgb
                                + here->FEFET1cdsb);
                          xcbdb = here->FEFET1cbsb;
                          xcbsb = here->FEFET1cbdb;
                          xcdbdb = -here->FEFET1capbd;
                          xcsbsb = -here->FEFET1capbs;
                      }
                      xcbbb = -(xcbgb + xcbdb + xcbsb + xcbgmb);

                      xgtg = xgtd = xgts = xgtb = 0.0;
		      sxpart = 0.4;
                      dxpart = 0.6;
		      ddxpart_dVd = ddxpart_dVg = ddxpart_dVb 
				  = ddxpart_dVs = 0.0;
		      dsxpart_dVd = dsxpart_dVg = dsxpart_dVb 
				  = dsxpart_dVs = 0.0;
                  }
                  else
                  {   xcggb = xcgdb = xcgsb = xcgbb = 0.0;
                      xcbgb = xcbdb = xcbsb = xcbbb = 0.0;
                      xcdgb = xcddb = xcdsb = xcdbb = 0.0;
                      xcsgb = xcsdb = xcssb = xcsbb = 0.0;

		      xgtg = here->FEFET1gtg;
                      xgtd = here->FEFET1gts;
                      xgts = here->FEFET1gtd;
                      xgtb = here->FEFET1gtb;

                      xcqgb = here->FEFET1cqgb;
                      xcqdb = here->FEFET1cqsb;
                      xcqsb = here->FEFET1cqdb;
                      xcqbb = here->FEFET1cqbb;

		      CoxWL = model->FEFET1coxe * here->pParam->FEFET1weffCV
                            * here->FEFET1nf * here->pParam->FEFET1leffCV;
		      qcheq = -(here->FEFET1qgate + here->FEFET1qbulk);
		      if (fabs(qcheq) <= 1.0e-5 * CoxWL)
		      {   if (model->FEFET1xpart < 0.5)
		          {   sxpart = 0.4;
		          }
		          else if (model->FEFET1xpart > 0.5)
		          {   sxpart = 0.0;
		          }
		          else
		          {   sxpart = 0.5;
		          }
		          dsxpart_dVd = dsxpart_dVg = dsxpart_dVb
				      = dsxpart_dVs = 0.0;
		      }
		      else
		      {   sxpart = here->FEFET1qdrn / qcheq;
		          Css = here->FEFET1cddb;
		          Cds = -(here->FEFET1cgdb + here->FEFET1cddb
			      + here->FEFET1cbdb);
		          dsxpart_dVs = (Css - sxpart * (Css + Cds)) / qcheq;
		          Csg = here->FEFET1cdgb;
		          Cdg = -(here->FEFET1cggb + here->FEFET1cdgb
			      + here->FEFET1cbgb);
		          dsxpart_dVg = (Csg - sxpart * (Csg + Cdg)) / qcheq;

		          Csd = here->FEFET1cdsb;
		          Cdd = -(here->FEFET1cgsb + here->FEFET1cdsb
			      + here->FEFET1cbsb);
		          dsxpart_dVd = (Csd - sxpart * (Csd + Cdd)) / qcheq;

		          dsxpart_dVb = -(dsxpart_dVd + dsxpart_dVg 
				      + dsxpart_dVs);
		      }
		      dxpart = 1.0 - sxpart;
		      ddxpart_dVd = -dsxpart_dVd;
		      ddxpart_dVg = -dsxpart_dVg;
		      ddxpart_dVs = -dsxpart_dVs;
		      ddxpart_dVb = -(ddxpart_dVd + ddxpart_dVg + ddxpart_dVs);
                  }
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


	      T1 = *(ckt->CKTstate0 + here->FEFET1qdef) * here->FEFET1gtau;
              gds = here->FEFET1gds;

              /*
               * Loading PZ matrix
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

                  *(here->FEFET1GPgpPtr ) += m * xcggb * s->real;
                  *(here->FEFET1GPgpPtr +1) += m * xcggb * s->imag;
                  *(here->FEFET1GPgpPtr) += m * (geltd - xgtg + gIgtotg);
                  *(here->FEFET1GPdpPtr ) += m * xcgdb * s->real;
                  *(here->FEFET1GPdpPtr +1) += m * xcgdb * s->imag;
		  *(here->FEFET1GPdpPtr) -= m * (xgtd - gIgtotd);
                  *(here->FEFET1GPspPtr ) += m * xcgsb * s->real;
                  *(here->FEFET1GPspPtr +1) += m * xcgsb * s->imag;
                  *(here->FEFET1GPspPtr) -= m * (xgts - gIgtots);
                  *(here->FEFET1GPbpPtr ) += m * xcgbb * s->real;
                  *(here->FEFET1GPbpPtr +1) += m * xcgbb * s->imag;
		  *(here->FEFET1GPbpPtr) -= m * (xgtb - gIgtotb);
              }
              else if (here->FEFET1rgateMod == 2)
              {   *(here->FEFET1GEgePtr) += m * gcrg;
                  *(here->FEFET1GEgpPtr) += m * gcrgg;
                  *(here->FEFET1GEdpPtr) += m * gcrgd;
                  *(here->FEFET1GEspPtr) += m * gcrgs;
                  *(here->FEFET1GEbpPtr) += m * gcrgb;

                  *(here->FEFET1GPgePtr) -= m * gcrg;
                  *(here->FEFET1GPgpPtr ) += m * xcggb * s->real;
                  *(here->FEFET1GPgpPtr +1) += m * xcggb * s->imag;
                  *(here->FEFET1GPgpPtr) -= m * (gcrgg + xgtg - gIgtotg);
                  *(here->FEFET1GPdpPtr ) += m * xcgdb * s->real;
                  *(here->FEFET1GPdpPtr +1) += m * xcgdb * s->imag;
                  *(here->FEFET1GPdpPtr) -= m * (gcrgd + xgtd - gIgtotd);
                  *(here->FEFET1GPspPtr ) += m * xcgsb * s->real;
                  *(here->FEFET1GPspPtr +1) += m * xcgsb * s->imag;
                  *(here->FEFET1GPspPtr) -= m * (gcrgs + xgts - gIgtots);
                  *(here->FEFET1GPbpPtr ) += m * xcgbb * s->real;
                  *(here->FEFET1GPbpPtr +1) += m * xcgbb * s->imag;
                  *(here->FEFET1GPbpPtr) -= m * (gcrgb + xgtb - gIgtotb);
              }
              else if (here->FEFET1rgateMod == 3)
              {   *(here->FEFET1GEgePtr) += m * geltd;
                  *(here->FEFET1GEgmPtr) -= m * geltd;
                  *(here->FEFET1GMgePtr) -= m * geltd;
                  *(here->FEFET1GMgmPtr) += m * (geltd + gcrg);
                  *(here->FEFET1GMgmPtr ) += m * xcgmgmb * s->real;
                  *(here->FEFET1GMgmPtr +1) += m * xcgmgmb * s->imag;
  
                  *(here->FEFET1GMdpPtr) += m * gcrgd;
                  *(here->FEFET1GMdpPtr ) += m * xcgmdb * s->real;
                  *(here->FEFET1GMdpPtr +1) += m * xcgmdb * s->imag;
                  *(here->FEFET1GMgpPtr) += m * gcrgg;
                  *(here->FEFET1GMspPtr) += m * gcrgs;
                  *(here->FEFET1GMspPtr ) += m * xcgmsb * s->real;
                  *(here->FEFET1GMspPtr +1) += m * xcgmsb * s->imag;
                  *(here->FEFET1GMbpPtr) += m * gcrgb;
                  *(here->FEFET1GMbpPtr ) += m * xcgmbb * s->real;
                  *(here->FEFET1GMbpPtr +1) += m * xcgmbb * s->imag;
  
                  *(here->FEFET1DPgmPtr ) += m * xcdgmb * s->real;
                  *(here->FEFET1DPgmPtr +1) += m * xcdgmb * s->imag;
                  *(here->FEFET1GPgmPtr) -= m * gcrg;
                  *(here->FEFET1SPgmPtr ) += m * xcsgmb * s->real;
                  *(here->FEFET1SPgmPtr +1) += m * xcsgmb * s->imag;
                  *(here->FEFET1BPgmPtr ) += m * xcbgmb * s->real;
                  *(here->FEFET1BPgmPtr +1) += m * xcbgmb * s->imag;
  
                  *(here->FEFET1GPgpPtr) -= m * (gcrgg + xgtg - gIgtotg);
                  *(here->FEFET1GPgpPtr ) += m * xcggb * s->real;
                  *(here->FEFET1GPgpPtr +1) += m * xcggb * s->imag;
                  *(here->FEFET1GPdpPtr) -= m * (gcrgd + xgtd - gIgtotd);
                  *(here->FEFET1GPdpPtr ) += m * xcgdb * s->real;
                  *(here->FEFET1GPdpPtr +1) += m * xcgdb * s->imag;
                  *(here->FEFET1GPspPtr) -= m * (gcrgs + xgts - gIgtots);
                  *(here->FEFET1GPspPtr ) += m * xcgsb * s->real;
                  *(here->FEFET1GPspPtr +1) += m * xcgsb * s->imag;
                  *(here->FEFET1GPbpPtr) -= m * (gcrgb + xgtb - gIgtotb);
                  *(here->FEFET1GPbpPtr ) += m * xcgbb * s->real;
                  *(here->FEFET1GPbpPtr +1) += m * xcgbb * s->imag;
              }
              else
              {   *(here->FEFET1GPdpPtr ) += m * xcgdb * s->real;
                  *(here->FEFET1GPdpPtr +1) += m * xcgdb * s->imag;
		  *(here->FEFET1GPdpPtr) -= m * (xgtd - gIgtotd);
                  *(here->FEFET1GPgpPtr ) += m * xcggb * s->real;
                  *(here->FEFET1GPgpPtr +1) += m * xcggb * s->imag;
		  *(here->FEFET1GPgpPtr) -= m * (xgtg - gIgtotg);
                  *(here->FEFET1GPspPtr ) += m * xcgsb * s->real;
                  *(here->FEFET1GPspPtr +1) += m * xcgsb * s->imag;
                  *(here->FEFET1GPspPtr) -= m * (xgts - gIgtots);
                  *(here->FEFET1GPbpPtr ) += m * xcgbb * s->real;
                  *(here->FEFET1GPbpPtr +1) += m * xcgbb * s->imag;
		  *(here->FEFET1GPbpPtr) -= m * (xgtb - gIgtotb);
              }

              if (model->FEFET1rdsMod)
              {   (*(here->FEFET1DgpPtr) += m * gdtotg);
                  (*(here->FEFET1DspPtr) += m * gdtots);
                  (*(here->FEFET1DbpPtr) += m * gdtotb);
                  (*(here->FEFET1SdpPtr) += m * gstotd);
                  (*(here->FEFET1SgpPtr) += m * gstotg);
                  (*(here->FEFET1SbpPtr) += m * gstotb);
              }

              *(here->FEFET1DPdpPtr ) += m * xcddb * s->real;
              *(here->FEFET1DPdpPtr +1) += m * xcddb * s->imag;
              *(here->FEFET1DPdpPtr) += m * (gdpr + gds + here->FEFET1gbd
				     - gdtotd + RevSum + gbdpdp - gIdtotd
				     + dxpart * xgtd + T1 * ddxpart_dVd);
              *(here->FEFET1DPdPtr) -= m * (gdpr + gdtot);
              *(here->FEFET1DPgpPtr ) += m * xcdgb * s->real;
              *(here->FEFET1DPgpPtr +1) += m * xcdgb * s->imag;
              *(here->FEFET1DPgpPtr) += m * (Gm - gdtotg + gbdpg - gIdtotg
				     + T1 * ddxpart_dVg + dxpart * xgtg);
              *(here->FEFET1DPspPtr ) += m * xcdsb * s->real;
              *(here->FEFET1DPspPtr +1) += m * xcdsb * s->imag;
              *(here->FEFET1DPspPtr) -= m * (gds + FwdSum + gdtots - gbdpsp + gIdtots
				     - T1 * ddxpart_dVs - dxpart * xgts);
              *(here->FEFET1DPbpPtr ) += m * xcdbb * s->real;
              *(here->FEFET1DPbpPtr +1) += m * xcdbb * s->imag;
              *(here->FEFET1DPbpPtr) -= m * (gjbd + gdtotb - Gmbs - gbdpb + gIdtotb
				     - T1 * ddxpart_dVb - dxpart * xgtb);

              *(here->FEFET1DdpPtr) -= m * (gdpr - gdtotd);
              *(here->FEFET1DdPtr) += m * (gdpr + gdtot);

              *(here->FEFET1SPdpPtr ) += m * xcsdb * s->real;
              *(here->FEFET1SPdpPtr +1) += m * xcsdb * s->imag;
              *(here->FEFET1SPdpPtr) -= m * (gds + gstotd + RevSum - gbspdp + gIstotd
				     - T1 * dsxpart_dVd - sxpart * xgtd);
              *(here->FEFET1SPgpPtr ) += m * xcsgb * s->real;
              *(here->FEFET1SPgpPtr +1) += m * xcsgb * s->imag;
              *(here->FEFET1SPgpPtr) -= m * (Gm + gstotg - gbspg + gIstotg
				     - T1 * dsxpart_dVg - sxpart * xgtg);
              *(here->FEFET1SPspPtr ) += m * xcssb * s->real;
              *(here->FEFET1SPspPtr +1) += m * xcssb * s->imag;
              *(here->FEFET1SPspPtr) += m * (gspr + gds + here->FEFET1gbs - gIstots
				     - gstots + FwdSum + gbspsp
				     + sxpart * xgts + T1 * dsxpart_dVs);
              *(here->FEFET1SPsPtr) -= m * (gspr + gstot);
              *(here->FEFET1SPbpPtr ) += m * xcsbb * s->real;
              *(here->FEFET1SPbpPtr +1) += m * xcsbb * s->imag;
              *(here->FEFET1SPbpPtr) -= m * (gjbs + gstotb + Gmbs - gbspb + gIstotb
				     - T1 * dsxpart_dVb - sxpart * xgtb);

              *(here->FEFET1SspPtr) -= m * (gspr - gstots);
              *(here->FEFET1SsPtr) += m * (gspr + gstot);

              *(here->FEFET1BPdpPtr ) += m * xcbdb * s->real;
              *(here->FEFET1BPdpPtr +1) += m * xcbdb * s->imag;
              *(here->FEFET1BPdpPtr) -= m * (gjbd - gbbdp + gIbtotd);
              *(here->FEFET1BPgpPtr ) += m * xcbgb * s->real;
              *(here->FEFET1BPgpPtr +1) += m * xcbgb * s->imag;
              *(here->FEFET1BPgpPtr) -= m * (here->FEFET1gbgs + gIbtotg);
              *(here->FEFET1BPspPtr ) += m * xcbsb * s->real;
              *(here->FEFET1BPspPtr +1) += m * xcbsb * s->imag;
              *(here->FEFET1BPspPtr) -= m * (gjbs - gbbsp + gIbtots);
              *(here->FEFET1BPbpPtr ) += m * xcbbb * s->real;
              *(here->FEFET1BPbpPtr +1) += m * xcbbb * s->imag;
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
              {   (*(here->FEFET1DPdbPtr ) += m * xcdbdb * s->real);
                  (*(here->FEFET1DPdbPtr +1) += m * xcdbdb * s->imag);
                  (*(here->FEFET1DPdbPtr) -= m * here->FEFET1gbd);
                  (*(here->FEFET1SPsbPtr ) += m * xcsbsb * s->real);
                  (*(here->FEFET1SPsbPtr +1) += m * xcsbsb * s->imag);
                  (*(here->FEFET1SPsbPtr) -= m * here->FEFET1gbs);

                  (*(here->FEFET1DBdpPtr ) += m * xcdbdb * s->real);
                  (*(here->FEFET1DBdpPtr +1) += m * xcdbdb * s->imag);
                  (*(here->FEFET1DBdpPtr) -= m * here->FEFET1gbd);
                  (*(here->FEFET1DBdbPtr ) -= m * xcdbdb * s->real);
                  (*(here->FEFET1DBdbPtr +1) -= m * xcdbdb * s->imag);
                  (*(here->FEFET1DBdbPtr) += m * (here->FEFET1gbd + here->FEFET1grbpd
                                          + here->FEFET1grbdb));
                  (*(here->FEFET1DBbpPtr) -= m * here->FEFET1grbpd);
                  (*(here->FEFET1DBbPtr) -= m * here->FEFET1grbdb);

                  (*(here->FEFET1BPdbPtr) -= m * here->FEFET1grbpd);
                  (*(here->FEFET1BPbPtr) -= m * here->FEFET1grbpb);
                  (*(here->FEFET1BPsbPtr) -= m * here->FEFET1grbps);
                  (*(here->FEFET1BPbpPtr) += m * (here->FEFET1grbpd + here->FEFET1grbps
					  + here->FEFET1grbpb));
                  /* WDL: (-here->FEFET1gbbs) already added to BPbpPtr */

                  (*(here->FEFET1SBspPtr ) += m * xcsbsb * s->real);
                  (*(here->FEFET1SBspPtr +1) += m * xcsbsb * s->imag);
                  (*(here->FEFET1SBspPtr) -= m * here->FEFET1gbs);
                  (*(here->FEFET1SBbpPtr) -= m * here->FEFET1grbps);
                  (*(here->FEFET1SBbPtr) -= m * here->FEFET1grbsb);
                  (*(here->FEFET1SBsbPtr ) -= m * xcsbsb * s->real);
                  (*(here->FEFET1SBsbPtr +1) -= m * xcsbsb * s->imag);
                  (*(here->FEFET1SBsbPtr) += m * (here->FEFET1gbs
					  + here->FEFET1grbps + here->FEFET1grbsb));

                  (*(here->FEFET1BdbPtr) -= m * here->FEFET1grbdb);
                  (*(here->FEFET1BbpPtr) -= m * here->FEFET1grbpb);
                  (*(here->FEFET1BsbPtr) -= m * here->FEFET1grbsb);
                  (*(here->FEFET1BbPtr) += m * (here->FEFET1grbsb + here->FEFET1grbdb
                                        + here->FEFET1grbpb));
              }

              if (here->FEFET1acnqsMod)
              {   *(here->FEFET1QqPtr ) += m * s->real * ScalingFactor;
                  *(here->FEFET1QqPtr +1) += m * s->imag * ScalingFactor;
                  *(here->FEFET1QgpPtr ) -= m * xcqgb * s->real;
                  *(here->FEFET1QgpPtr +1) -= m * xcqgb * s->imag;
                  *(here->FEFET1QdpPtr ) -= m * xcqdb * s->real;
                  *(here->FEFET1QdpPtr +1) -= m * xcqdb * s->imag;
                  *(here->FEFET1QbpPtr ) -= m * xcqbb * s->real;
                  *(here->FEFET1QbpPtr +1) -= m * xcqbb * s->imag;
                  *(here->FEFET1QspPtr ) -= m * xcqsb * s->real;
                  *(here->FEFET1QspPtr +1) -= m * xcqsb * s->imag;

                  *(here->FEFET1GPqPtr) -= m * here->FEFET1gtau;
                  *(here->FEFET1DPqPtr) += m * dxpart * here->FEFET1gtau;
                  *(here->FEFET1SPqPtr) += m * sxpart * here->FEFET1gtau;

                  *(here->FEFET1QqPtr) += m * here->FEFET1gtau;
                  *(here->FEFET1QgpPtr) += m * xgtg;
                  *(here->FEFET1QdpPtr) += m * xgtd;
                  *(here->FEFET1QbpPtr) += m * xgtb;
                  *(here->FEFET1QspPtr) += m * xgts;
              }
         }
    }
    return(OK);
}
