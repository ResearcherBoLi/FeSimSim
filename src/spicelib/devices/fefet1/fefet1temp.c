/**** BSIM4.5.0 Released by Xuemei (Jane) Xi 07/29/2005 ****/
/**********
Copyright 2024 Xidian University.	All rights reserved.
 Author: Bo Li	at Hangzhou Institute of Technology	 
 Modified: 2020/09/09  Bo Li
 Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/smpdefs.h"
#include "ngspice/cktdefs.h"
#include "fefet1def.h"
#include "ngspice/const.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"

#define Kb 1.3806226e-23
#define KboQ 8.617087e-5
#define EPS0 8.85418e-12
#define EPSSI 1.03594e-10
#define PI 3.141592654
#define MAX_EXP 5.834617425e14
#define MIN_EXP 1.713908431e-15
#define EXP_THRESHOLD 34.0
#define Charge_q 1.60219e-19
#define DELTA  1.0E-9
#define DEXP(A,B) {                                                        \
        if (A > EXP_THRESHOLD) {                                           \
            B = MAX_EXP*(1.0+(A)-EXP_THRESHOLD);                           \
        } else if (A < -EXP_THRESHOLD)  {                                  \
            B = MIN_EXP;                                                   \
        } else   {                                                         \
            B = exp(A);                                                    \
        }                                                                  \
    }



static int
FEFET1DioIjthVjmEval(
double Nvtm, double Ijth, double Isb, double XExpBV,
double *Vjm)
{
double Tb, Tc, EVjmovNv;

       Tc = XExpBV;
       Tb = 1.0 + Ijth / Isb - Tc;
       EVjmovNv = 0.5 * (Tb + sqrt(Tb * Tb + 4.0 * Tc));
       *Vjm = Nvtm * log(EVjmovNv);

return 0;
}


int
FEFET1temp(
GENmodel *inModel,
CKTcircuit *ckt)
{
FEFET1model *model = (FEFET1model*) inModel;
FEFET1instance *here;
struct fefet1SizeDependParam *pSizeDependParamKnot, *pLastKnot, *pParam = NULL;
double tmp, tmp1, tmp2, Eg, Eg0, ni;
double T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, Lnew=0.0, Wnew;
double delTemp, Temp, TRatio, Inv_L, Inv_W, Inv_LW, Vtm0, Tnom;
double dumPs, dumPd, dumAs, dumAd, PowWeffWr;
double DMCGeff, DMCIeff, DMDGeff;
double Nvtms, Nvtmd, SourceSatCurrent, DrainSatCurrent;
double T10;
double Inv_saref, Inv_sbref, Inv_sa, Inv_sb, rho, Ldrn, dvth0_lod;
double W_tmp, Inv_ODeff, OD_offset, dk2_lod, deta0_lod;
double lnl, lnw, lnnf, rbpbx, rbpby, rbsbx, rbsby, rbdbx, rbdby,bodymode;
double kvsat, wlod, sceff, Wdrn;

int Size_Not_Found, i;

    /*  loop through all the FEFET1 device models */
    for (; model != NULL; model = FEFET1nextModel(model))
    {    Temp = ckt->CKTtemp;
         if (model->FEFET1SbulkJctPotential < 0.1)
         {   model->FEFET1SbulkJctPotential = 0.1;
             fprintf(stderr, "Given pbs is less than 0.1. Pbs is set to 0.1.\n");
         }
         if (model->FEFET1SsidewallJctPotential < 0.1)
         {   model->FEFET1SsidewallJctPotential = 0.1;
             fprintf(stderr, "Given pbsws is less than 0.1. Pbsws is set to 0.1.\n");
         }
         if (model->FEFET1SGatesidewallJctPotential < 0.1)
         {   model->FEFET1SGatesidewallJctPotential = 0.1;
             fprintf(stderr, "Given pbswgs is less than 0.1. Pbswgs is set to 0.1.\n");
         }

         if (model->FEFET1DbulkJctPotential < 0.1)
         {   model->FEFET1DbulkJctPotential = 0.1;
             fprintf(stderr, "Given pbd is less than 0.1. Pbd is set to 0.1.\n");
         }
         if (model->FEFET1DsidewallJctPotential < 0.1)
         {   model->FEFET1DsidewallJctPotential = 0.1;
             fprintf(stderr, "Given pbswd is less than 0.1. Pbswd is set to 0.1.\n");
         }
         if (model->FEFET1DGatesidewallJctPotential < 0.1)
         {   model->FEFET1DGatesidewallJctPotential = 0.1;
             fprintf(stderr, "Given pbswgd is less than 0.1. Pbswgd is set to 0.1.\n");
         }

         if ((model->FEFET1toxeGiven) && (model->FEFET1toxpGiven) && (model->FEFET1dtoxGiven)
             && (model->FEFET1toxe != (model->FEFET1toxp + model->FEFET1dtox)))
             printf("Warning: toxe, toxp and dtox all given and toxe != toxp + dtox; dtox ignored.\n");
         else if ((model->FEFET1toxeGiven) && (!model->FEFET1toxpGiven))
             model->FEFET1toxp = model->FEFET1toxe - model->FEFET1dtox;
         else if ((!model->FEFET1toxeGiven) && (model->FEFET1toxpGiven))
             model->FEFET1toxe = model->FEFET1toxp + model->FEFET1dtox;

         model->FEFET1coxe = model->FEFET1epsrox * EPS0 / model->FEFET1toxe;
         model->FEFET1coxp = model->FEFET1epsrox * EPS0 / model->FEFET1toxp;

         if (!model->FEFET1cgdoGiven)
         {   if (model->FEFET1dlcGiven && (model->FEFET1dlc > 0.0))
                 model->FEFET1cgdo = model->FEFET1dlc * model->FEFET1coxe
                                  - model->FEFET1cgdl ;
             else
                 model->FEFET1cgdo = 0.6 * model->FEFET1xj * model->FEFET1coxe;
         }
         if (!model->FEFET1cgsoGiven)
         {   if (model->FEFET1dlcGiven && (model->FEFET1dlc > 0.0))
                 model->FEFET1cgso = model->FEFET1dlc * model->FEFET1coxe
                                  - model->FEFET1cgsl ;
             else
                 model->FEFET1cgso = 0.6 * model->FEFET1xj * model->FEFET1coxe;
         }
         if (!model->FEFET1cgboGiven)
             model->FEFET1cgbo = 2.0 * model->FEFET1dwc * model->FEFET1coxe;

         struct fefet1SizeDependParam *p = model->pSizeDependParamKnot;
         while (p) {
             struct fefet1SizeDependParam *next_p = p->pNext;
             FREE(p);
             p = next_p;
         }
         model->pSizeDependParamKnot = NULL;
         pLastKnot = NULL;

         Tnom = model->FEFET1tnom;
         TRatio = Temp / Tnom;

         model->FEFET1vcrit = CONSTvt0 * log(CONSTvt0 / (CONSTroot2 * 1.0e-14));
         model->FEFET1factor1 = sqrt(EPSSI / (model->FEFET1epsrox * EPS0)
                             * model->FEFET1toxe);

         Vtm0 = model->FEFET1vtm0 = KboQ * Tnom;
         Eg0 = 1.16 - 7.02e-4 * Tnom * Tnom / (Tnom + 1108.0);
         ni = 1.45e10 * (Tnom / 300.15) * sqrt(Tnom / 300.15)
            * exp(21.5565981 - Eg0 / (2.0 * Vtm0));

         model->FEFET1vtm = KboQ * Temp;
         Eg = 1.16 - 7.02e-4 * Temp * Temp / (Temp + 1108.0);
         if (Temp != Tnom)
         {   T0 = Eg0 / Vtm0 - Eg / model->FEFET1vtm;
             T1 = log(Temp / Tnom);
             T2 = T0 + model->FEFET1SjctTempExponent * T1;
             T3 = exp(T2 / model->FEFET1SjctEmissionCoeff);
             model->FEFET1SjctTempSatCurDensity = model->FEFET1SjctSatCurDensity
                                               * T3;
             model->FEFET1SjctSidewallTempSatCurDensity
                         = model->FEFET1SjctSidewallSatCurDensity * T3;
             model->FEFET1SjctGateSidewallTempSatCurDensity
                         = model->FEFET1SjctGateSidewallSatCurDensity * T3;

             T2 = T0 + model->FEFET1DjctTempExponent * T1;
             T3 = exp(T2 / model->FEFET1DjctEmissionCoeff);
             model->FEFET1DjctTempSatCurDensity = model->FEFET1DjctSatCurDensity
                                               * T3;
             model->FEFET1DjctSidewallTempSatCurDensity
                         = model->FEFET1DjctSidewallSatCurDensity * T3;
             model->FEFET1DjctGateSidewallTempSatCurDensity
                         = model->FEFET1DjctGateSidewallSatCurDensity * T3;
         }
         else
         {   model->FEFET1SjctTempSatCurDensity = model->FEFET1SjctSatCurDensity;
             model->FEFET1SjctSidewallTempSatCurDensity
                        = model->FEFET1SjctSidewallSatCurDensity;
             model->FEFET1SjctGateSidewallTempSatCurDensity
                        = model->FEFET1SjctGateSidewallSatCurDensity;
             model->FEFET1DjctTempSatCurDensity = model->FEFET1DjctSatCurDensity;
             model->FEFET1DjctSidewallTempSatCurDensity
                        = model->FEFET1DjctSidewallSatCurDensity;
             model->FEFET1DjctGateSidewallTempSatCurDensity
                        = model->FEFET1DjctGateSidewallSatCurDensity;
         }

         if (model->FEFET1SjctTempSatCurDensity < 0.0)
             model->FEFET1SjctTempSatCurDensity = 0.0;
         if (model->FEFET1SjctSidewallTempSatCurDensity < 0.0)
             model->FEFET1SjctSidewallTempSatCurDensity = 0.0;
         if (model->FEFET1SjctGateSidewallTempSatCurDensity < 0.0)
             model->FEFET1SjctGateSidewallTempSatCurDensity = 0.0;
         if (model->FEFET1DjctTempSatCurDensity < 0.0)
             model->FEFET1DjctTempSatCurDensity = 0.0;
         if (model->FEFET1DjctSidewallTempSatCurDensity < 0.0)
             model->FEFET1DjctSidewallTempSatCurDensity = 0.0;
         if (model->FEFET1DjctGateSidewallTempSatCurDensity < 0.0)
             model->FEFET1DjctGateSidewallTempSatCurDensity = 0.0;

         /* Temperature dependence of D/B and S/B diode capacitance begins */
         delTemp = ckt->CKTtemp - model->FEFET1tnom;
         T0 = model->FEFET1tcj * delTemp;
         if (T0 >= -1.0)
         {   model->FEFET1SunitAreaTempJctCap = model->FEFET1SunitAreaJctCap *(1.0 + T0); /*bug_fix -JX */
             model->FEFET1DunitAreaTempJctCap = model->FEFET1DunitAreaJctCap *(1.0 + T0);
         }
         else
         {   if (model->FEFET1SunitAreaJctCap > 0.0)
             {   model->FEFET1SunitAreaTempJctCap = 0.0;
                 fprintf(stderr, "Temperature effect has caused cjs to be negative. Cjs is clamped to zero.\n");
             }
             if (model->FEFET1DunitAreaJctCap > 0.0)
             {   model->FEFET1DunitAreaTempJctCap = 0.0;
                 fprintf(stderr, "Temperature effect has caused cjd to be negative. Cjd is clamped to zero.\n");
             }
         }
         T0 = model->FEFET1tcjsw * delTemp;
         if (T0 >= -1.0)
         {   model->FEFET1SunitLengthSidewallTempJctCap = model->FEFET1SunitLengthSidewallJctCap *(1.0 + T0);
             model->FEFET1DunitLengthSidewallTempJctCap = model->FEFET1DunitLengthSidewallJctCap *(1.0 + T0);
         }
         else
         {   if (model->FEFET1SunitLengthSidewallJctCap > 0.0)
             {   model->FEFET1SunitLengthSidewallTempJctCap = 0.0;
                 fprintf(stderr, "Temperature effect has caused cjsws to be negative. Cjsws is clamped to zero.\n");
             }
             if (model->FEFET1DunitLengthSidewallJctCap > 0.0)
             {   model->FEFET1DunitLengthSidewallTempJctCap = 0.0;
                 fprintf(stderr, "Temperature effect has caused cjswd to be negative. Cjswd is clamped to zero.\n");
             }
         }
         T0 = model->FEFET1tcjswg * delTemp;
         if (T0 >= -1.0)
         {   model->FEFET1SunitLengthGateSidewallTempJctCap = model->FEFET1SunitLengthGateSidewallJctCap *(1.0 + T0);
             model->FEFET1DunitLengthGateSidewallTempJctCap = model->FEFET1DunitLengthGateSidewallJctCap *(1.0 + T0);
         }
         else
         {   if (model->FEFET1SunitLengthGateSidewallJctCap > 0.0)
             {   model->FEFET1SunitLengthGateSidewallTempJctCap = 0.0;
                 fprintf(stderr, "Temperature effect has caused cjswgs to be negative. Cjswgs is clamped to zero.\n");
             }
             if (model->FEFET1DunitLengthGateSidewallJctCap > 0.0)
             {   model->FEFET1DunitLengthGateSidewallTempJctCap = 0.0;
                 fprintf(stderr, "Temperature effect has caused cjswgd to be negative. Cjswgd is clamped to zero.\n");
             }
         }

         model->FEFET1PhiBS = model->FEFET1SbulkJctPotential
                           - model->FEFET1tpb * delTemp;
         if (model->FEFET1PhiBS < 0.01)
         {   model->FEFET1PhiBS = 0.01;
             fprintf(stderr, "Temperature effect has caused pbs to be less than 0.01. Pbs is clamped to 0.01.\n");
         }
         model->FEFET1PhiBD = model->FEFET1DbulkJctPotential
                           - model->FEFET1tpb * delTemp;
         if (model->FEFET1PhiBD < 0.01)
         {   model->FEFET1PhiBD = 0.01;
             fprintf(stderr, "Temperature effect has caused pbd to be less than 0.01. Pbd is clamped to 0.01.\n");
         }

         model->FEFET1PhiBSWS = model->FEFET1SsidewallJctPotential
                             - model->FEFET1tpbsw * delTemp;
         if (model->FEFET1PhiBSWS <= 0.01)
         {   model->FEFET1PhiBSWS = 0.01;
             fprintf(stderr, "Temperature effect has caused pbsws to be less than 0.01. Pbsws is clamped to 0.01.\n");
         }
         model->FEFET1PhiBSWD = model->FEFET1DsidewallJctPotential
                             - model->FEFET1tpbsw * delTemp;
         if (model->FEFET1PhiBSWD <= 0.01)
         {   model->FEFET1PhiBSWD = 0.01;
             fprintf(stderr, "Temperature effect has caused pbswd to be less than 0.01. Pbswd is clamped to 0.01.\n");
         }

         model->FEFET1PhiBSWGS = model->FEFET1SGatesidewallJctPotential
                              - model->FEFET1tpbswg * delTemp;
         if (model->FEFET1PhiBSWGS <= 0.01)
         {   model->FEFET1PhiBSWGS = 0.01;
             fprintf(stderr, "Temperature effect has caused pbswgs to be less than 0.01. Pbswgs is clamped to 0.01.\n");
         }
         model->FEFET1PhiBSWGD = model->FEFET1DGatesidewallJctPotential
                              - model->FEFET1tpbswg * delTemp;
         if (model->FEFET1PhiBSWGD <= 0.01)
         {   model->FEFET1PhiBSWGD = 0.01;
             fprintf(stderr, "Temperature effect has caused pbswgd to be less than 0.01. Pbswgd is clamped to 0.01.\n");
         } /* End of junction capacitance */


         if (model->FEFET1ijthdfwd <= 0.0)
         {   model->FEFET1ijthdfwd = 0.1;
             fprintf(stderr, "Ijthdfwd reset to %g.\n", model->FEFET1ijthdfwd);
         }
         if (model->FEFET1ijthsfwd <= 0.0)
         {   model->FEFET1ijthsfwd = 0.1;
             fprintf(stderr, "Ijthsfwd reset to %g.\n", model->FEFET1ijthsfwd);
         }
         if (model->FEFET1ijthdrev <= 0.0)
         {   model->FEFET1ijthdrev = 0.1;
             fprintf(stderr, "Ijthdrev reset to %g.\n", model->FEFET1ijthdrev);
         }
         if (model->FEFET1ijthsrev <= 0.0)
         {   model->FEFET1ijthsrev = 0.1;
             fprintf(stderr, "Ijthsrev reset to %g.\n", model->FEFET1ijthsrev);
         }

         if ((model->FEFET1xjbvd <= 0.0) && (model->FEFET1dioMod == 2))
         {   model->FEFET1xjbvd = 1.0;
             fprintf(stderr, "Xjbvd reset to %g.\n", model->FEFET1xjbvd);
         }
         else if ((model->FEFET1xjbvd < 0.0) && (model->FEFET1dioMod == 0))
         {   model->FEFET1xjbvd = 1.0;
             fprintf(stderr, "Xjbvd reset to %g.\n", model->FEFET1xjbvd);
         }

         if (model->FEFET1bvd <= 0.0)
         {   model->FEFET1bvd = 10.0;
             fprintf(stderr, "BVD reset to %g.\n", model->FEFET1bvd);
         }

         if ((model->FEFET1xjbvs <= 0.0) && (model->FEFET1dioMod == 2))
         {   model->FEFET1xjbvs = 1.0;
             fprintf(stderr, "Xjbvs reset to %g.\n", model->FEFET1xjbvs);
         }
         else if ((model->FEFET1xjbvs < 0.0) && (model->FEFET1dioMod == 0))
         {   model->FEFET1xjbvs = 1.0;
             fprintf(stderr, "Xjbvs reset to %g.\n", model->FEFET1xjbvs);
         }

         if (model->FEFET1bvs <= 0.0)
         {   model->FEFET1bvs = 10.0;
             fprintf(stderr, "BVS reset to %g.\n", model->FEFET1bvs);
         }


         /* loop through all the instances of the model */
         for (here = FEFET1instances(model); here != NULL;
              here = FEFET1nextInstance(here))
            {
              pSizeDependParamKnot = model->pSizeDependParamKnot;
              Size_Not_Found = 1;
              while ((pSizeDependParamKnot != NULL) && Size_Not_Found)
              {   if ((here->FEFET1l == pSizeDependParamKnot->Length)
                      && (here->FEFET1w == pSizeDependParamKnot->Width)
                      && (here->FEFET1nf == pSizeDependParamKnot->NFinger))
                  {   Size_Not_Found = 0;
                      here->pParam = pSizeDependParamKnot;
                      pParam = here->pParam; /*bug-fix  */
                  }
                  else
                  {   pLastKnot = pSizeDependParamKnot;
                      pSizeDependParamKnot = pSizeDependParamKnot->pNext;
                  }
              }

              /* stress effect */
              Ldrn = here->FEFET1l;
              Wdrn = here->FEFET1w / here->FEFET1nf;

              if (Size_Not_Found)
              {   pParam = TMALLOC(struct fefet1SizeDependParam, 1);
                  if (pLastKnot == NULL)
                      model->pSizeDependParamKnot = pParam;
                  else
                      pLastKnot->pNext = pParam;
                  pParam->pNext = NULL;
                  here->pParam = pParam;

                  pParam->Length = here->FEFET1l;
                  pParam->Width = here->FEFET1w;
                  pParam->NFinger = here->FEFET1nf;
                  Lnew = here->FEFET1l  + model->FEFET1xl ;
                  Wnew = here->FEFET1w / here->FEFET1nf + model->FEFET1xw;

                  T0 = pow(Lnew, model->FEFET1Lln);
                  T1 = pow(Wnew, model->FEFET1Lwn);
                  tmp1 = model->FEFET1Ll / T0 + model->FEFET1Lw / T1
                       + model->FEFET1Lwl / (T0 * T1);
                  pParam->FEFET1dl = model->FEFET1Lint + tmp1;
                  tmp2 = model->FEFET1Llc / T0 + model->FEFET1Lwc / T1
                       + model->FEFET1Lwlc / (T0 * T1);
                  pParam->FEFET1dlc = model->FEFET1dlc + tmp2;

                  T2 = pow(Lnew, model->FEFET1Wln);
                  T3 = pow(Wnew, model->FEFET1Wwn);
                  tmp1 = model->FEFET1Wl / T2 + model->FEFET1Ww / T3
                       + model->FEFET1Wwl / (T2 * T3);
                  pParam->FEFET1dw = model->FEFET1Wint + tmp1;
                  tmp2 = model->FEFET1Wlc / T2 + model->FEFET1Wwc / T3
                       + model->FEFET1Wwlc / (T2 * T3);
                  pParam->FEFET1dwc = model->FEFET1dwc + tmp2;
                  pParam->FEFET1dwj = model->FEFET1dwj + tmp2;

                  pParam->FEFET1leff = Lnew - 2.0 * pParam->FEFET1dl;
                  if (pParam->FEFET1leff <= 0.0)
                  {
                      SPfrontEnd->IFerrorf (ERR_FATAL,
                      "FEFET1: mosfet %s, model %s: Effective channel length <= 0",
                       model->FEFET1modName, here->FEFET1name);
                      return(E_BADPARM);
                  }

                  pParam->FEFET1weff = Wnew - 2.0 * pParam->FEFET1dw;
                  if (pParam->FEFET1weff <= 0.0)
                  {
                      SPfrontEnd->IFerrorf (ERR_FATAL,
                      "FEFET1: mosfet %s, model %s: Effective channel width <= 0",
                       model->FEFET1modName, here->FEFET1name);
                      return(E_BADPARM);
                  }

                  pParam->FEFET1leffCV = Lnew - 2.0 * pParam->FEFET1dlc;
                  if (pParam->FEFET1leffCV <= 0.0)
                  {
                      SPfrontEnd->IFerrorf (ERR_FATAL,
                      "FEFET1: mosfet %s, model %s: Effective channel length for C-V <= 0",
                       model->FEFET1modName, here->FEFET1name);
                      return(E_BADPARM);
                  }

                  pParam->FEFET1weffCV = Wnew - 2.0 * pParam->FEFET1dwc;
                  if (pParam->FEFET1weffCV <= 0.0)
                  {
                      SPfrontEnd->IFerrorf (ERR_FATAL,
                      "FEFET1: mosfet %s, model %s: Effective channel width for C-V <= 0",
                       model->FEFET1modName, here->FEFET1name);
                      return(E_BADPARM);
                  }

                  pParam->FEFET1weffCJ = Wnew - 2.0 * pParam->FEFET1dwj;
                  if (pParam->FEFET1weffCJ <= 0.0)
                  {
                      SPfrontEnd->IFerrorf (ERR_FATAL,
                      "FEFET1: mosfet %s, model %s: Effective channel width for S/D junctions <= 0",
                       model->FEFET1modName, here->FEFET1name);
                      return(E_BADPARM);
                  }


                  if (model->FEFET1binUnit == 1)
                  {   Inv_L = 1.0e-6 / pParam->FEFET1leff;
                      Inv_W = 1.0e-6 / pParam->FEFET1weff;
                      Inv_LW = 1.0e-12 / (pParam->FEFET1leff
                             * pParam->FEFET1weff);
                  }
                  else
                  {   Inv_L = 1.0 / pParam->FEFET1leff;
                      Inv_W = 1.0 / pParam->FEFET1weff;
                      Inv_LW = 1.0 / (pParam->FEFET1leff
                             * pParam->FEFET1weff);
                  }
                  pParam->FEFET1cdsc = model->FEFET1cdsc
                                    + model->FEFET1lcdsc * Inv_L
                                    + model->FEFET1wcdsc * Inv_W
                                    + model->FEFET1pcdsc * Inv_LW;
                  pParam->FEFET1cdscb = model->FEFET1cdscb
                                     + model->FEFET1lcdscb * Inv_L
                                     + model->FEFET1wcdscb * Inv_W
                                     + model->FEFET1pcdscb * Inv_LW;

                      pParam->FEFET1cdscd = model->FEFET1cdscd
                                     + model->FEFET1lcdscd * Inv_L
                                     + model->FEFET1wcdscd * Inv_W
                                     + model->FEFET1pcdscd * Inv_LW;

                  pParam->FEFET1cit = model->FEFET1cit
                                   + model->FEFET1lcit * Inv_L
                                   + model->FEFET1wcit * Inv_W
                                   + model->FEFET1pcit * Inv_LW;
                  pParam->FEFET1nfactor = model->FEFET1nfactor
                                       + model->FEFET1lnfactor * Inv_L
                                       + model->FEFET1wnfactor * Inv_W
                                       + model->FEFET1pnfactor * Inv_LW;
                  pParam->FEFET1xj = model->FEFET1xj
                                  + model->FEFET1lxj * Inv_L
                                  + model->FEFET1wxj * Inv_W
                                  + model->FEFET1pxj * Inv_LW;
                  pParam->FEFET1vsat = model->FEFET1vsat
                                    + model->FEFET1lvsat * Inv_L
                                    + model->FEFET1wvsat * Inv_W
                                    + model->FEFET1pvsat * Inv_LW;
                  pParam->FEFET1at = model->FEFET1at
                                  + model->FEFET1lat * Inv_L
                                  + model->FEFET1wat * Inv_W
                                  + model->FEFET1pat * Inv_LW;
                  pParam->FEFET1a0 = model->FEFET1a0
                                  + model->FEFET1la0 * Inv_L
                                  + model->FEFET1wa0 * Inv_W
                                  + model->FEFET1pa0 * Inv_LW;

                  pParam->FEFET1ags = model->FEFET1ags
                                  + model->FEFET1lags * Inv_L
                                  + model->FEFET1wags * Inv_W
                                  + model->FEFET1pags * Inv_LW;

                  pParam->FEFET1a1 = model->FEFET1a1
                                  + model->FEFET1la1 * Inv_L
                                  + model->FEFET1wa1 * Inv_W
                                  + model->FEFET1pa1 * Inv_LW;
                  pParam->FEFET1a2 = model->FEFET1a2
                                  + model->FEFET1la2 * Inv_L
                                  + model->FEFET1wa2 * Inv_W
                                  + model->FEFET1pa2 * Inv_LW;
                  pParam->FEFET1keta = model->FEFET1keta
                                    + model->FEFET1lketa * Inv_L
                                    + model->FEFET1wketa * Inv_W
                                    + model->FEFET1pketa * Inv_LW;
                  pParam->FEFET1nsub = model->FEFET1nsub
                                    + model->FEFET1lnsub * Inv_L
                                    + model->FEFET1wnsub * Inv_W
                                    + model->FEFET1pnsub * Inv_LW;
                  pParam->FEFET1ndep = model->FEFET1ndep
                                    + model->FEFET1lndep * Inv_L
                                    + model->FEFET1wndep * Inv_W
                                    + model->FEFET1pndep * Inv_LW;
                  pParam->FEFET1nsd = model->FEFET1nsd
                                   + model->FEFET1lnsd * Inv_L
                                   + model->FEFET1wnsd * Inv_W
                                   + model->FEFET1pnsd * Inv_LW;
                  pParam->FEFET1phin = model->FEFET1phin
                                    + model->FEFET1lphin * Inv_L
                                    + model->FEFET1wphin * Inv_W
                                    + model->FEFET1pphin * Inv_LW;
                  pParam->FEFET1ngate = model->FEFET1ngate
                                     + model->FEFET1lngate * Inv_L
                                     + model->FEFET1wngate * Inv_W
                                     + model->FEFET1pngate * Inv_LW;
                  pParam->FEFET1gamma1 = model->FEFET1gamma1
                                      + model->FEFET1lgamma1 * Inv_L
                                      + model->FEFET1wgamma1 * Inv_W
                                      + model->FEFET1pgamma1 * Inv_LW;
                  pParam->FEFET1gamma2 = model->FEFET1gamma2
                                      + model->FEFET1lgamma2 * Inv_L
                                      + model->FEFET1wgamma2 * Inv_W
                                      + model->FEFET1pgamma2 * Inv_LW;
                  pParam->FEFET1vbx = model->FEFET1vbx
                                   + model->FEFET1lvbx * Inv_L
                                   + model->FEFET1wvbx * Inv_W
                                   + model->FEFET1pvbx * Inv_LW;
                  pParam->FEFET1vbm = model->FEFET1vbm
                                   + model->FEFET1lvbm * Inv_L
                                   + model->FEFET1wvbm * Inv_W
                                   + model->FEFET1pvbm * Inv_LW;
                  pParam->FEFET1xt = model->FEFET1xt
                                   + model->FEFET1lxt * Inv_L
                                   + model->FEFET1wxt * Inv_W
                                   + model->FEFET1pxt * Inv_LW;
                  pParam->FEFET1vfb = model->FEFET1vfb
                                   + model->FEFET1lvfb * Inv_L
                                   + model->FEFET1wvfb * Inv_W
                                   + model->FEFET1pvfb * Inv_LW;
                  pParam->FEFET1k1 = model->FEFET1k1
                                  + model->FEFET1lk1 * Inv_L
                                  + model->FEFET1wk1 * Inv_W
                                  + model->FEFET1pk1 * Inv_LW;
                  pParam->FEFET1kt1 = model->FEFET1kt1
                                   + model->FEFET1lkt1 * Inv_L
                                   + model->FEFET1wkt1 * Inv_W
                                   + model->FEFET1pkt1 * Inv_LW;
                  pParam->FEFET1kt1l = model->FEFET1kt1l
                                    + model->FEFET1lkt1l * Inv_L
                                    + model->FEFET1wkt1l * Inv_W
                                    + model->FEFET1pkt1l * Inv_LW;
                  pParam->FEFET1k2 = model->FEFET1k2
                                  + model->FEFET1lk2 * Inv_L
                                  + model->FEFET1wk2 * Inv_W
                                  + model->FEFET1pk2 * Inv_LW;
                  pParam->FEFET1kt2 = model->FEFET1kt2
                                   + model->FEFET1lkt2 * Inv_L
                                   + model->FEFET1wkt2 * Inv_W
                                   + model->FEFET1pkt2 * Inv_LW;
                  pParam->FEFET1k3 = model->FEFET1k3
                                  + model->FEFET1lk3 * Inv_L
                                  + model->FEFET1wk3 * Inv_W
                                  + model->FEFET1pk3 * Inv_LW;
                  pParam->FEFET1k3b = model->FEFET1k3b
                                   + model->FEFET1lk3b * Inv_L
                                   + model->FEFET1wk3b * Inv_W
                                   + model->FEFET1pk3b * Inv_LW;
                  pParam->FEFET1w0 = model->FEFET1w0
                                  + model->FEFET1lw0 * Inv_L
                                  + model->FEFET1ww0 * Inv_W
                                  + model->FEFET1pw0 * Inv_LW;
                  pParam->FEFET1lpe0 = model->FEFET1lpe0
                                    + model->FEFET1llpe0 * Inv_L
                                     + model->FEFET1wlpe0 * Inv_W
                                    + model->FEFET1plpe0 * Inv_LW;
                  pParam->FEFET1lpeb = model->FEFET1lpeb
                                    + model->FEFET1llpeb * Inv_L
                                    + model->FEFET1wlpeb * Inv_W
                                    + model->FEFET1plpeb * Inv_LW;
                  pParam->FEFET1dvtp0 = model->FEFET1dvtp0
                                     + model->FEFET1ldvtp0 * Inv_L
                                     + model->FEFET1wdvtp0 * Inv_W
                                     + model->FEFET1pdvtp0 * Inv_LW;
                  pParam->FEFET1dvtp1 = model->FEFET1dvtp1
                                     + model->FEFET1ldvtp1 * Inv_L
                                     + model->FEFET1wdvtp1 * Inv_W
                                     + model->FEFET1pdvtp1 * Inv_LW;
                  pParam->FEFET1dvt0 = model->FEFET1dvt0
                                    + model->FEFET1ldvt0 * Inv_L
                                    + model->FEFET1wdvt0 * Inv_W
                                    + model->FEFET1pdvt0 * Inv_LW;
                  pParam->FEFET1dvt1 = model->FEFET1dvt1
                                    + model->FEFET1ldvt1 * Inv_L
                                    + model->FEFET1wdvt1 * Inv_W
                                    + model->FEFET1pdvt1 * Inv_LW;
                  pParam->FEFET1dvt2 = model->FEFET1dvt2
                                    + model->FEFET1ldvt2 * Inv_L
                                    + model->FEFET1wdvt2 * Inv_W
                                    + model->FEFET1pdvt2 * Inv_LW;
                  pParam->FEFET1dvt0w = model->FEFET1dvt0w
                                    + model->FEFET1ldvt0w * Inv_L
                                    + model->FEFET1wdvt0w * Inv_W
                                    + model->FEFET1pdvt0w * Inv_LW;
                  pParam->FEFET1dvt1w = model->FEFET1dvt1w
                                    + model->FEFET1ldvt1w * Inv_L
                                    + model->FEFET1wdvt1w * Inv_W
                                    + model->FEFET1pdvt1w * Inv_LW;
                  pParam->FEFET1dvt2w = model->FEFET1dvt2w
                                    + model->FEFET1ldvt2w * Inv_L
                                    + model->FEFET1wdvt2w * Inv_W
                                    + model->FEFET1pdvt2w * Inv_LW;
                  pParam->FEFET1drout = model->FEFET1drout
                                     + model->FEFET1ldrout * Inv_L
                                     + model->FEFET1wdrout * Inv_W
                                     + model->FEFET1pdrout * Inv_LW;
                  pParam->FEFET1dsub = model->FEFET1dsub
                                    + model->FEFET1ldsub * Inv_L
                                    + model->FEFET1wdsub * Inv_W
                                    + model->FEFET1pdsub * Inv_LW;
                  pParam->FEFET1vth0 = model->FEFET1vth0
                                    + model->FEFET1lvth0 * Inv_L
                                    + model->FEFET1wvth0 * Inv_W
                                    + model->FEFET1pvth0 * Inv_LW;
                  pParam->FEFET1ua = model->FEFET1ua
                                  + model->FEFET1lua * Inv_L
                                  + model->FEFET1wua * Inv_W
                                  + model->FEFET1pua * Inv_LW;
                  pParam->FEFET1ua1 = model->FEFET1ua1
                                   + model->FEFET1lua1 * Inv_L
                                   + model->FEFET1wua1 * Inv_W
                                   + model->FEFET1pua1 * Inv_LW;
                  pParam->FEFET1ub = model->FEFET1ub
                                  + model->FEFET1lub * Inv_L
                                  + model->FEFET1wub * Inv_W
                                  + model->FEFET1pub * Inv_LW;
                  pParam->FEFET1ub1 = model->FEFET1ub1
                                   + model->FEFET1lub1 * Inv_L
                                   + model->FEFET1wub1 * Inv_W
                                   + model->FEFET1pub1 * Inv_LW;
                  pParam->FEFET1uc = model->FEFET1uc
                                  + model->FEFET1luc * Inv_L
                                  + model->FEFET1wuc * Inv_W
                                  + model->FEFET1puc * Inv_LW;
                  pParam->FEFET1uc1 = model->FEFET1uc1
                                   + model->FEFET1luc1 * Inv_L
                                   + model->FEFET1wuc1 * Inv_W
                                   + model->FEFET1puc1 * Inv_LW;
                  pParam->FEFET1ud = model->FEFET1ud
                                  + model->FEFET1lud * Inv_L
                                  + model->FEFET1wud * Inv_W
                                  + model->FEFET1pud * Inv_LW;
                  pParam->FEFET1ud1 = model->FEFET1ud1
                                  + model->FEFET1lud1 * Inv_L
                                  + model->FEFET1wud1 * Inv_W
                                  + model->FEFET1pud1 * Inv_LW;
                  pParam->FEFET1up = model->FEFET1up
                                  + model->FEFET1lup * Inv_L
                                  + model->FEFET1wup * Inv_W
                                  + model->FEFET1pup * Inv_LW;
                  pParam->FEFET1lp = model->FEFET1lp
                                  + model->FEFET1llp * Inv_L
                                  + model->FEFET1wlp * Inv_W
                                  + model->FEFET1plp * Inv_LW;
                  pParam->FEFET1eu = model->FEFET1eu
                                  + model->FEFET1leu * Inv_L
                                  + model->FEFET1weu * Inv_W
                                  + model->FEFET1peu * Inv_LW;
                  pParam->FEFET1u0 = model->FEFET1u0
                                  + model->FEFET1lu0 * Inv_L
                                  + model->FEFET1wu0 * Inv_W
                                  + model->FEFET1pu0 * Inv_LW;
                  pParam->FEFET1ute = model->FEFET1ute
                                   + model->FEFET1lute * Inv_L
                                   + model->FEFET1wute * Inv_W
                                   + model->FEFET1pute * Inv_LW;
                  pParam->FEFET1voff = model->FEFET1voff
                                    + model->FEFET1lvoff * Inv_L
                                    + model->FEFET1wvoff * Inv_W
                                    + model->FEFET1pvoff * Inv_LW;
                  pParam->FEFET1tvoff = model->FEFET1tvoff
                                    + model->FEFET1ltvoff * Inv_L
                                    + model->FEFET1wtvoff * Inv_W
                                    + model->FEFET1ptvoff * Inv_LW;
                  pParam->FEFET1minv = model->FEFET1minv
                                    + model->FEFET1lminv * Inv_L
                                    + model->FEFET1wminv * Inv_W
                                    + model->FEFET1pminv * Inv_LW;
                  pParam->FEFET1fprout = model->FEFET1fprout
                                     + model->FEFET1lfprout * Inv_L
                                     + model->FEFET1wfprout * Inv_W
                                     + model->FEFET1pfprout * Inv_LW;
                  pParam->FEFET1pdits = model->FEFET1pdits
                                     + model->FEFET1lpdits * Inv_L
                                     + model->FEFET1wpdits * Inv_W
                                     + model->FEFET1ppdits * Inv_LW;
                  pParam->FEFET1pditsd = model->FEFET1pditsd
                                      + model->FEFET1lpditsd * Inv_L
                                      + model->FEFET1wpditsd * Inv_W
                                      + model->FEFET1ppditsd * Inv_LW;
                  pParam->FEFET1delta = model->FEFET1delta
                                     + model->FEFET1ldelta * Inv_L
                                     + model->FEFET1wdelta * Inv_W
                                     + model->FEFET1pdelta * Inv_LW;
                  pParam->FEFET1rdsw = model->FEFET1rdsw
                                    + model->FEFET1lrdsw * Inv_L
                                    + model->FEFET1wrdsw * Inv_W
                                    + model->FEFET1prdsw * Inv_LW;
                  pParam->FEFET1rdw = model->FEFET1rdw
                                    + model->FEFET1lrdw * Inv_L
                                    + model->FEFET1wrdw * Inv_W
                                    + model->FEFET1prdw * Inv_LW;
                  pParam->FEFET1rsw = model->FEFET1rsw
                                    + model->FEFET1lrsw * Inv_L
                                    + model->FEFET1wrsw * Inv_W
                                    + model->FEFET1prsw * Inv_LW;
                  pParam->FEFET1prwg = model->FEFET1prwg
                                    + model->FEFET1lprwg * Inv_L
                                    + model->FEFET1wprwg * Inv_W
                                    + model->FEFET1pprwg * Inv_LW;
                  pParam->FEFET1prwb = model->FEFET1prwb
                                    + model->FEFET1lprwb * Inv_L
                                    + model->FEFET1wprwb * Inv_W
                                    + model->FEFET1pprwb * Inv_LW;
                  pParam->FEFET1prt = model->FEFET1prt
                                    + model->FEFET1lprt * Inv_L
                                    + model->FEFET1wprt * Inv_W
                                    + model->FEFET1pprt * Inv_LW;
                  pParam->FEFET1eta0 = model->FEFET1eta0
                                    + model->FEFET1leta0 * Inv_L
                                    + model->FEFET1weta0 * Inv_W
                                    + model->FEFET1peta0 * Inv_LW;
                  pParam->FEFET1etab = model->FEFET1etab
                                    + model->FEFET1letab * Inv_L
                                    + model->FEFET1wetab * Inv_W
                                    + model->FEFET1petab * Inv_LW;
                  pParam->FEFET1pclm = model->FEFET1pclm
                                    + model->FEFET1lpclm * Inv_L
                                    + model->FEFET1wpclm * Inv_W
                                    + model->FEFET1ppclm * Inv_LW;
                  pParam->FEFET1pdibl1 = model->FEFET1pdibl1
                                      + model->FEFET1lpdibl1 * Inv_L
                                      + model->FEFET1wpdibl1 * Inv_W
                                      + model->FEFET1ppdibl1 * Inv_LW;
                  pParam->FEFET1pdibl2 = model->FEFET1pdibl2
                                      + model->FEFET1lpdibl2 * Inv_L
                                      + model->FEFET1wpdibl2 * Inv_W
                                      + model->FEFET1ppdibl2 * Inv_LW;
                  pParam->FEFET1pdiblb = model->FEFET1pdiblb
                                      + model->FEFET1lpdiblb * Inv_L
                                      + model->FEFET1wpdiblb * Inv_W
                                      + model->FEFET1ppdiblb * Inv_LW;
                  pParam->FEFET1pscbe1 = model->FEFET1pscbe1
                                      + model->FEFET1lpscbe1 * Inv_L
                                      + model->FEFET1wpscbe1 * Inv_W
                                      + model->FEFET1ppscbe1 * Inv_LW;
                  pParam->FEFET1pscbe2 = model->FEFET1pscbe2
                                      + model->FEFET1lpscbe2 * Inv_L
                                      + model->FEFET1wpscbe2 * Inv_W
                                      + model->FEFET1ppscbe2 * Inv_LW;
                  pParam->FEFET1pvag = model->FEFET1pvag
                                    + model->FEFET1lpvag * Inv_L
                                    + model->FEFET1wpvag * Inv_W
                                    + model->FEFET1ppvag * Inv_LW;
                  pParam->FEFET1wr = model->FEFET1wr
                                  + model->FEFET1lwr * Inv_L
                                  + model->FEFET1wwr * Inv_W
                                  + model->FEFET1pwr * Inv_LW;
                  pParam->FEFET1dwg = model->FEFET1dwg
                                   + model->FEFET1ldwg * Inv_L
                                   + model->FEFET1wdwg * Inv_W
                                   + model->FEFET1pdwg * Inv_LW;
                  pParam->FEFET1dwb = model->FEFET1dwb
                                   + model->FEFET1ldwb * Inv_L
                                   + model->FEFET1wdwb * Inv_W
                                   + model->FEFET1pdwb * Inv_LW;
                  pParam->FEFET1b0 = model->FEFET1b0
                                  + model->FEFET1lb0 * Inv_L
                                  + model->FEFET1wb0 * Inv_W
                                  + model->FEFET1pb0 * Inv_LW;
                  pParam->FEFET1b1 = model->FEFET1b1
                                  + model->FEFET1lb1 * Inv_L
                                  + model->FEFET1wb1 * Inv_W
                                  + model->FEFET1pb1 * Inv_LW;
                  pParam->FEFET1alpha0 = model->FEFET1alpha0
                                      + model->FEFET1lalpha0 * Inv_L
                                      + model->FEFET1walpha0 * Inv_W
                                      + model->FEFET1palpha0 * Inv_LW;
                  pParam->FEFET1alpha1 = model->FEFET1alpha1
                                      + model->FEFET1lalpha1 * Inv_L
                                      + model->FEFET1walpha1 * Inv_W
                                      + model->FEFET1palpha1 * Inv_LW;
                  pParam->FEFET1beta0 = model->FEFET1beta0
                                     + model->FEFET1lbeta0 * Inv_L
                                     + model->FEFET1wbeta0 * Inv_W
                                     + model->FEFET1pbeta0 * Inv_LW;
                  pParam->FEFET1agidl = model->FEFET1agidl
                                     + model->FEFET1lagidl * Inv_L
                                     + model->FEFET1wagidl * Inv_W
                                     + model->FEFET1pagidl * Inv_LW;
                  pParam->FEFET1bgidl = model->FEFET1bgidl
                                     + model->FEFET1lbgidl * Inv_L
                                     + model->FEFET1wbgidl * Inv_W
                                     + model->FEFET1pbgidl * Inv_LW;
                  pParam->FEFET1cgidl = model->FEFET1cgidl
                                     + model->FEFET1lcgidl * Inv_L
                                     + model->FEFET1wcgidl * Inv_W
                                     + model->FEFET1pcgidl * Inv_LW;
                  pParam->FEFET1egidl = model->FEFET1egidl
                                     + model->FEFET1legidl * Inv_L
                                     + model->FEFET1wegidl * Inv_W
                                     + model->FEFET1pegidl * Inv_LW;
                  pParam->FEFET1aigc = model->FEFET1aigc
                                     + model->FEFET1laigc * Inv_L
                                     + model->FEFET1waigc * Inv_W
                                     + model->FEFET1paigc * Inv_LW;
                  pParam->FEFET1bigc = model->FEFET1bigc
                                     + model->FEFET1lbigc * Inv_L
                                     + model->FEFET1wbigc * Inv_W
                                     + model->FEFET1pbigc * Inv_LW;
                  pParam->FEFET1cigc = model->FEFET1cigc
                                     + model->FEFET1lcigc * Inv_L
                                     + model->FEFET1wcigc * Inv_W
                                     + model->FEFET1pcigc * Inv_LW;
                  pParam->FEFET1aigsd = model->FEFET1aigsd
                                     + model->FEFET1laigsd * Inv_L
                                     + model->FEFET1waigsd * Inv_W
                                     + model->FEFET1paigsd * Inv_LW;
                  pParam->FEFET1bigsd = model->FEFET1bigsd
                                     + model->FEFET1lbigsd * Inv_L
                                     + model->FEFET1wbigsd * Inv_W
                                     + model->FEFET1pbigsd * Inv_LW;
                  pParam->FEFET1cigsd = model->FEFET1cigsd
                                     + model->FEFET1lcigsd * Inv_L
                                     + model->FEFET1wcigsd * Inv_W
                                     + model->FEFET1pcigsd * Inv_LW;
                  pParam->FEFET1aigbacc = model->FEFET1aigbacc
                                       + model->FEFET1laigbacc * Inv_L
                                       + model->FEFET1waigbacc * Inv_W
                                       + model->FEFET1paigbacc * Inv_LW;
                  pParam->FEFET1bigbacc = model->FEFET1bigbacc
                                       + model->FEFET1lbigbacc * Inv_L
                                       + model->FEFET1wbigbacc * Inv_W
                                       + model->FEFET1pbigbacc * Inv_LW;
                  pParam->FEFET1cigbacc = model->FEFET1cigbacc
                                       + model->FEFET1lcigbacc * Inv_L
                                       + model->FEFET1wcigbacc * Inv_W
                                       + model->FEFET1pcigbacc * Inv_LW;
                  pParam->FEFET1aigbinv = model->FEFET1aigbinv
                                       + model->FEFET1laigbinv * Inv_L
                                       + model->FEFET1waigbinv * Inv_W
                                       + model->FEFET1paigbinv * Inv_LW;
                  pParam->FEFET1bigbinv = model->FEFET1bigbinv
                                       + model->FEFET1lbigbinv * Inv_L
                                       + model->FEFET1wbigbinv * Inv_W
                                       + model->FEFET1pbigbinv * Inv_LW;
                  pParam->FEFET1cigbinv = model->FEFET1cigbinv
                                       + model->FEFET1lcigbinv * Inv_L
                                       + model->FEFET1wcigbinv * Inv_W
                                       + model->FEFET1pcigbinv * Inv_LW;
                  pParam->FEFET1nigc = model->FEFET1nigc
                                       + model->FEFET1lnigc * Inv_L
                                       + model->FEFET1wnigc * Inv_W
                                       + model->FEFET1pnigc * Inv_LW;
                  pParam->FEFET1nigbacc = model->FEFET1nigbacc
                                       + model->FEFET1lnigbacc * Inv_L
                                       + model->FEFET1wnigbacc * Inv_W
                                       + model->FEFET1pnigbacc * Inv_LW;
                  pParam->FEFET1nigbinv = model->FEFET1nigbinv
                                       + model->FEFET1lnigbinv * Inv_L
                                       + model->FEFET1wnigbinv * Inv_W
                                       + model->FEFET1pnigbinv * Inv_LW;
                  pParam->FEFET1ntox = model->FEFET1ntox
                                    + model->FEFET1lntox * Inv_L
                                    + model->FEFET1wntox * Inv_W
                                    + model->FEFET1pntox * Inv_LW;
                  pParam->FEFET1eigbinv = model->FEFET1eigbinv
                                       + model->FEFET1leigbinv * Inv_L
                                       + model->FEFET1weigbinv * Inv_W
                                       + model->FEFET1peigbinv * Inv_LW;
                  pParam->FEFET1pigcd = model->FEFET1pigcd
                                     + model->FEFET1lpigcd * Inv_L
                                     + model->FEFET1wpigcd * Inv_W
                                     + model->FEFET1ppigcd * Inv_LW;
                  pParam->FEFET1poxedge = model->FEFET1poxedge
                                       + model->FEFET1lpoxedge * Inv_L
                                       + model->FEFET1wpoxedge * Inv_W
                                       + model->FEFET1ppoxedge * Inv_LW;
                  pParam->FEFET1xrcrg1 = model->FEFET1xrcrg1
                                      + model->FEFET1lxrcrg1 * Inv_L
                                      + model->FEFET1wxrcrg1 * Inv_W
                                      + model->FEFET1pxrcrg1 * Inv_LW;
                  pParam->FEFET1xrcrg2 = model->FEFET1xrcrg2
                                      + model->FEFET1lxrcrg2 * Inv_L
                                      + model->FEFET1wxrcrg2 * Inv_W
                                      + model->FEFET1pxrcrg2 * Inv_LW;
                  pParam->FEFET1lambda = model->FEFET1lambda
                                      + model->FEFET1llambda * Inv_L
                                      + model->FEFET1wlambda * Inv_W
                                      + model->FEFET1plambda * Inv_LW;
                  pParam->FEFET1vtl = model->FEFET1vtl
                                      + model->FEFET1lvtl * Inv_L
                                      + model->FEFET1wvtl * Inv_W
                                      + model->FEFET1pvtl * Inv_LW;
                  pParam->FEFET1xn = model->FEFET1xn
                                      + model->FEFET1lxn * Inv_L
                                      + model->FEFET1wxn * Inv_W
                                      + model->FEFET1pxn * Inv_LW;
                  pParam->FEFET1vfbsdoff = model->FEFET1vfbsdoff
                                      + model->FEFET1lvfbsdoff * Inv_L
                                      + model->FEFET1wvfbsdoff * Inv_W
                                      + model->FEFET1pvfbsdoff * Inv_LW;
                  pParam->FEFET1tvfbsdoff = model->FEFET1tvfbsdoff
                                      + model->FEFET1ltvfbsdoff * Inv_L
                                      + model->FEFET1wtvfbsdoff * Inv_W
                                      + model->FEFET1ptvfbsdoff * Inv_LW;

                  pParam->FEFET1cgsl = model->FEFET1cgsl
                                    + model->FEFET1lcgsl * Inv_L
                                    + model->FEFET1wcgsl * Inv_W
                                    + model->FEFET1pcgsl * Inv_LW;
                  pParam->FEFET1cgdl = model->FEFET1cgdl
                                    + model->FEFET1lcgdl * Inv_L
                                    + model->FEFET1wcgdl * Inv_W
                                    + model->FEFET1pcgdl * Inv_LW;
                  pParam->FEFET1ckappas = model->FEFET1ckappas
                                       + model->FEFET1lckappas * Inv_L
                                       + model->FEFET1wckappas * Inv_W
                                        + model->FEFET1pckappas * Inv_LW;
                  pParam->FEFET1ckappad = model->FEFET1ckappad
                                       + model->FEFET1lckappad * Inv_L
                                       + model->FEFET1wckappad * Inv_W
                                       + model->FEFET1pckappad * Inv_LW;
                  pParam->FEFET1cf = model->FEFET1cf
                                  + model->FEFET1lcf * Inv_L
                                  + model->FEFET1wcf * Inv_W
                                  + model->FEFET1pcf * Inv_LW;
                  pParam->FEFET1clc = model->FEFET1clc
                                   + model->FEFET1lclc * Inv_L
                                   + model->FEFET1wclc * Inv_W
                                   + model->FEFET1pclc * Inv_LW;
                  pParam->FEFET1cle = model->FEFET1cle
                                   + model->FEFET1lcle * Inv_L
                                   + model->FEFET1wcle * Inv_W
                                   + model->FEFET1pcle * Inv_LW;
                  pParam->FEFET1vfbcv = model->FEFET1vfbcv
                                     + model->FEFET1lvfbcv * Inv_L
                                     + model->FEFET1wvfbcv * Inv_W
                                     + model->FEFET1pvfbcv * Inv_LW;
                  pParam->FEFET1acde = model->FEFET1acde
                                    + model->FEFET1lacde * Inv_L
                                    + model->FEFET1wacde * Inv_W
                                    + model->FEFET1pacde * Inv_LW;
                  pParam->FEFET1moin = model->FEFET1moin
                                    + model->FEFET1lmoin * Inv_L
                                    + model->FEFET1wmoin * Inv_W
                                    + model->FEFET1pmoin * Inv_LW;
                  pParam->FEFET1noff = model->FEFET1noff
                                    + model->FEFET1lnoff * Inv_L
                                    + model->FEFET1wnoff * Inv_W
                                    + model->FEFET1pnoff * Inv_LW;
                  pParam->FEFET1voffcv = model->FEFET1voffcv
                                      + model->FEFET1lvoffcv * Inv_L
                                      + model->FEFET1wvoffcv * Inv_W
                                      + model->FEFET1pvoffcv * Inv_LW;
                  pParam->FEFET1kvth0we = model->FEFET1kvth0we
                                      + model->FEFET1lkvth0we * Inv_L
                                      + model->FEFET1wkvth0we * Inv_W
                                      + model->FEFET1pkvth0we * Inv_LW;
                  pParam->FEFET1k2we = model->FEFET1k2we
                                      + model->FEFET1lk2we * Inv_L
                                      + model->FEFET1wk2we * Inv_W
                                      + model->FEFET1pk2we * Inv_LW;
                  pParam->FEFET1ku0we = model->FEFET1ku0we
                                      + model->FEFET1lku0we * Inv_L
                                      + model->FEFET1wku0we * Inv_W
                                      + model->FEFET1pku0we * Inv_LW;

                  pParam->FEFET1abulkCVfactor = 1.0 + pow((pParam->FEFET1clc
                                             / pParam->FEFET1leffCV),
                                             pParam->FEFET1cle);

                  T0 = (TRatio - 1.0);

                  PowWeffWr = pow(pParam->FEFET1weffCJ * 1.0e6, pParam->FEFET1wr) * here->FEFET1nf;

                  T1 = T2 = T3 = T4 = 0.0;
                  if(model->FEFET1tempMod == 0) {
                          pParam->FEFET1ua = pParam->FEFET1ua + pParam->FEFET1ua1 * T0;
                          pParam->FEFET1ub = pParam->FEFET1ub + pParam->FEFET1ub1 * T0;
                          pParam->FEFET1uc = pParam->FEFET1uc + pParam->FEFET1uc1 * T0;
                          pParam->FEFET1ud = pParam->FEFET1ud + pParam->FEFET1ud1 * T0;
                          pParam->FEFET1vsattemp = pParam->FEFET1vsat - pParam->FEFET1at * T0;
                          T10 = pParam->FEFET1prt * T0;
                     if(model->FEFET1rdsMod) {
                          /* External Rd(V) */
                          T1 = pParam->FEFET1rdw + T10;
                          T2 = model->FEFET1rdwmin + T10;
                          /* External Rs(V) */
                          T3 = pParam->FEFET1rsw + T10;
                          T4 = model->FEFET1rswmin + T10;
                     }
                          /* Internal Rds(V) in IV */
                          pParam->FEFET1rds0 = (pParam->FEFET1rdsw + T10)
                                            * here->FEFET1nf / PowWeffWr;
                          pParam->FEFET1rdswmin = (model->FEFET1rdswmin + T10)
                                               * here->FEFET1nf / PowWeffWr;
                  } else { /* tempMod = 1, 2 */
                          pParam->FEFET1ua = pParam->FEFET1ua * (1.0 + pParam->FEFET1ua1 * delTemp) ;
                          pParam->FEFET1ub = pParam->FEFET1ub * (1.0 + pParam->FEFET1ub1 * delTemp);
                          pParam->FEFET1uc = pParam->FEFET1uc * (1.0 + pParam->FEFET1uc1 * delTemp);
                          pParam->FEFET1ud = pParam->FEFET1ud * (1.0 + pParam->FEFET1ud1 * delTemp);
                          pParam->FEFET1vsattemp = pParam->FEFET1vsat * (1.0 - pParam->FEFET1at * delTemp);
                          T10 = 1.0 + pParam->FEFET1prt * delTemp;
                     if(model->FEFET1rdsMod) {
                          /* External Rd(V) */
                          T1 = pParam->FEFET1rdw * T10;
                          T2 = model->FEFET1rdwmin * T10;
                          /* External Rs(V) */
                          T3 = pParam->FEFET1rsw * T10;
                          T4 = model->FEFET1rswmin * T10;
                     }
                          /* Internal Rds(V) in IV */
                          pParam->FEFET1rds0 = pParam->FEFET1rdsw * T10 * here->FEFET1nf / PowWeffWr;
                          pParam->FEFET1rdswmin = model->FEFET1rdswmin * T10 * here->FEFET1nf / PowWeffWr;
                  }
                  if (T1 < 0.0)
                  {   T1 = 0.0;
                      printf("Warning: Rdw at current temperature is negative; set to 0.\n");
                  }
                  if (T2 < 0.0)
                  {   T2 = 0.0;
                      printf("Warning: Rdwmin at current temperature is negative; set to 0.\n");
                  }
                  pParam->FEFET1rd0 = T1 / PowWeffWr;
                  pParam->FEFET1rdwmin = T2 / PowWeffWr;
                  if (T3 < 0.0)
                  {   T3 = 0.0;
                      printf("Warning: Rsw at current temperature is negative; set to 0.\n");
                  }
                  if (T4 < 0.0)
                  {   T4 = 0.0;
                      printf("Warning: Rswmin at current temperature is negative; set to 0.\n");
                  }
                  pParam->FEFET1rs0 = T3 / PowWeffWr;
                  pParam->FEFET1rswmin = T4 / PowWeffWr;

                  if (pParam->FEFET1u0 > 1.0)
                      pParam->FEFET1u0 = pParam->FEFET1u0 / 1.0e4;

                  /* mobility channel length dependence */
                  T5 = 1.0 - pParam->FEFET1up * exp( - pParam->FEFET1leff / pParam->FEFET1lp);
                  pParam->FEFET1u0temp = pParam->FEFET1u0 * T5
                                      * pow(TRatio, pParam->FEFET1ute);
                  if (pParam->FEFET1eu < 0.0)
                  {   pParam->FEFET1eu = 0.0;
                      printf("Warning: eu has been negative; reset to 0.0.\n");
                  }

                  pParam->FEFET1vfbsdoff = pParam->FEFET1vfbsdoff * (1.0 + pParam->FEFET1tvfbsdoff * delTemp);
                  pParam->FEFET1voff = pParam->FEFET1voff * (1.0 + pParam->FEFET1tvoff * delTemp);

                /* Source End Velocity Limit  */
                        if((model->FEFET1vtlGiven) && (model->FEFET1vtl > 0.0) )
                      {
                     if(model->FEFET1lc < 0.0) pParam->FEFET1lc = 0.0;
                     else   pParam->FEFET1lc = model->FEFET1lc ;
                     T0 = pParam->FEFET1leff / (pParam->FEFET1xn * pParam->FEFET1leff + pParam->FEFET1lc);
                     pParam->FEFET1tfactor = (1.0 - T0) / (1.0 + T0 );
                       }

                  pParam->FEFET1cgdo = (model->FEFET1cgdo + pParam->FEFET1cf)
                                    * pParam->FEFET1weffCV;
                  pParam->FEFET1cgso = (model->FEFET1cgso + pParam->FEFET1cf)
                                    * pParam->FEFET1weffCV;
                  pParam->FEFET1cgbo = model->FEFET1cgbo * pParam->FEFET1leffCV * here->FEFET1nf;

                  if (!model->FEFET1ndepGiven && model->FEFET1gamma1Given)
                  {   T0 = pParam->FEFET1gamma1 * model->FEFET1coxe;
                      pParam->FEFET1ndep = 3.01248e22 * T0 * T0;
                  }

                  pParam->FEFET1phi = Vtm0 * log(pParam->FEFET1ndep / ni)
                                   + pParam->FEFET1phin + 0.4;

                  pParam->FEFET1sqrtPhi = sqrt(pParam->FEFET1phi);
                  pParam->FEFET1phis3 = pParam->FEFET1sqrtPhi * pParam->FEFET1phi;

                  pParam->FEFET1Xdep0 = sqrt(2.0 * EPSSI / (Charge_q
                                     * pParam->FEFET1ndep * 1.0e6))
                                     * pParam->FEFET1sqrtPhi;
                  pParam->FEFET1sqrtXdep0 = sqrt(pParam->FEFET1Xdep0);
                  pParam->FEFET1litl = sqrt(3.0 * pParam->FEFET1xj
                                    * model->FEFET1toxe);
                  pParam->FEFET1vbi = Vtm0 * log(pParam->FEFET1nsd
                                   * pParam->FEFET1ndep / (ni * ni));

                  if (pParam->FEFET1ngate > 0.0)
                  {   pParam->FEFET1vfbsd = Vtm0 * log(pParam->FEFET1ngate
                                         / pParam->FEFET1nsd);
                  }
                  else
                      pParam->FEFET1vfbsd = 0.0;

                  pParam->FEFET1cdep0 = sqrt(Charge_q * EPSSI
                                     * pParam->FEFET1ndep * 1.0e6 / 2.0
                                     / pParam->FEFET1phi);

                  pParam->FEFET1ToxRatio = exp(pParam->FEFET1ntox
                                        * log(model->FEFET1toxref / model->FEFET1toxe))
                                        / model->FEFET1toxe / model->FEFET1toxe;
                  pParam->FEFET1ToxRatioEdge = exp(pParam->FEFET1ntox
                                            * log(model->FEFET1toxref
                                            / (model->FEFET1toxe * pParam->FEFET1poxedge)))
                                            / model->FEFET1toxe / model->FEFET1toxe
                                            / pParam->FEFET1poxedge / pParam->FEFET1poxedge;
                  pParam->FEFET1Aechvb = (model->FEFET1type == NMOS) ? 4.97232e-7 : 3.42537e-7;
                  pParam->FEFET1Bechvb = (model->FEFET1type == NMOS) ? 7.45669e11 : 1.16645e12;
                  pParam->FEFET1AechvbEdge = pParam->FEFET1Aechvb * pParam->FEFET1weff
                                          * model->FEFET1dlcig * pParam->FEFET1ToxRatioEdge;
                  pParam->FEFET1BechvbEdge = -pParam->FEFET1Bechvb
                                          * model->FEFET1toxe * pParam->FEFET1poxedge;
                  pParam->FEFET1Aechvb *= pParam->FEFET1weff * pParam->FEFET1leff
                                       * pParam->FEFET1ToxRatio;
                  pParam->FEFET1Bechvb *= -model->FEFET1toxe;


                  pParam->FEFET1mstar = 0.5 + atan(pParam->FEFET1minv) / PI;
                  pParam->FEFET1voffcbn =  pParam->FEFET1voff + model->FEFET1voffl / pParam->FEFET1leff;

                  pParam->FEFET1ldeb = sqrt(EPSSI * Vtm0 / (Charge_q
                                    * pParam->FEFET1ndep * 1.0e6)) / 3.0;
                  pParam->FEFET1acde *= pow((pParam->FEFET1ndep / 2.0e16), -0.25);


                  if (model->FEFET1k1Given || model->FEFET1k2Given)
                  {   if (!model->FEFET1k1Given)
                      {
                          if ((!ckt->CKTcurJob) || (ckt->CKTcurJob->JOBtype < 9)) /* don't print in sensitivity */
                              fprintf(stdout, "Warning: k1 should be specified with k2.\n");
                          pParam->FEFET1k1 = 0.53;
                      }
                      if (!model->FEFET1k2Given)
                      {
                          if ((!ckt->CKTcurJob) || (ckt->CKTcurJob->JOBtype < 9)) /* don't print in sensitivity */
                              fprintf(stdout, "Warning: k2 should be specified with k1.\n");
                          pParam->FEFET1k2 = -0.0186;
                      }
                      if ((!ckt->CKTcurJob) || (ckt->CKTcurJob->JOBtype < 9)) { /* don't print in sensitivity */
                          if (model->FEFET1nsubGiven)
                              fprintf(stdout, "Warning: nsub is ignored because k1 or k2 is given.\n");
                          if (model->FEFET1xtGiven)
                              fprintf(stdout, "Warning: xt is ignored because k1 or k2 is given.\n");
                          if (model->FEFET1vbxGiven)
                              fprintf(stdout, "Warning: vbx is ignored because k1 or k2 is given.\n");
                          if (model->FEFET1gamma1Given)
                              fprintf(stdout, "Warning: gamma1 is ignored because k1 or k2 is given.\n");
                          if (model->FEFET1gamma2Given)
                              fprintf(stdout, "Warning: gamma2 is ignored because k1 or k2 is given.\n");
                      }
                  }
                  else
                  {   if (!model->FEFET1vbxGiven)
                          pParam->FEFET1vbx = pParam->FEFET1phi - 7.7348e-4
                                           * pParam->FEFET1ndep
                                           * pParam->FEFET1xt * pParam->FEFET1xt;
                      if (pParam->FEFET1vbx > 0.0)
                          pParam->FEFET1vbx = -pParam->FEFET1vbx;
                      if (pParam->FEFET1vbm > 0.0)
                          pParam->FEFET1vbm = -pParam->FEFET1vbm;

                      if (!model->FEFET1gamma1Given)
                          pParam->FEFET1gamma1 = 5.753e-12
                                              * sqrt(pParam->FEFET1ndep)
                                              / model->FEFET1coxe;
                      if (!model->FEFET1gamma2Given)
                          pParam->FEFET1gamma2 = 5.753e-12
                                              * sqrt(pParam->FEFET1nsub)
                                              / model->FEFET1coxe;

                      T0 = pParam->FEFET1gamma1 - pParam->FEFET1gamma2;
                      T1 = sqrt(pParam->FEFET1phi - pParam->FEFET1vbx)
                         - pParam->FEFET1sqrtPhi;
                      T2 = sqrt(pParam->FEFET1phi * (pParam->FEFET1phi
                         - pParam->FEFET1vbm)) - pParam->FEFET1phi;
                      pParam->FEFET1k2 = T0 * T1 / (2.0 * T2 + pParam->FEFET1vbm);
                      pParam->FEFET1k1 = pParam->FEFET1gamma2 - 2.0
                                      * pParam->FEFET1k2 * sqrt(pParam->FEFET1phi
                                      - pParam->FEFET1vbm);
                  }

                  if (!model->FEFET1vfbGiven)
                  {   if (model->FEFET1vth0Given)
                      {   pParam->FEFET1vfb = model->FEFET1type * pParam->FEFET1vth0
                                           - pParam->FEFET1phi - pParam->FEFET1k1
                                           * pParam->FEFET1sqrtPhi;
                      }
                      else
                      {   pParam->FEFET1vfb = -1.0;
                      }
                  }
                   if (!model->FEFET1vth0Given)
                  {   pParam->FEFET1vth0 = model->FEFET1type * (pParam->FEFET1vfb
                                        + pParam->FEFET1phi + pParam->FEFET1k1
                                        * pParam->FEFET1sqrtPhi);
                  }

                  pParam->FEFET1k1ox = pParam->FEFET1k1 * model->FEFET1toxe
                                    / model->FEFET1toxm;

                  tmp = sqrt(EPSSI / (model->FEFET1epsrox * EPS0)
                      * model->FEFET1toxe * pParam->FEFET1Xdep0);
                    T0 = pParam->FEFET1dsub * pParam->FEFET1leff / tmp;
                  if (T0 < EXP_THRESHOLD)
                    {   T1 = exp(T0);
                            T2 = T1 - 1.0;
                            T3 = T2 * T2;
                      T4 = T3 + 2.0 * T1 * MIN_EXP;
                      pParam->FEFET1theta0vb0 = T1 / T4;
                  }
                  else
                      pParam->FEFET1theta0vb0 = 1.0 / (MAX_EXP - 2.0);

                   T0 = pParam->FEFET1drout * pParam->FEFET1leff / tmp;
                  if (T0 < EXP_THRESHOLD)
                         {   T1 = exp(T0);
                            T2 = T1 - 1.0;
                      T3 = T2 * T2;
                      T4 = T3 + 2.0 * T1 * MIN_EXP;
                      T5 = T1 / T4;
                  }
                  else
                      T5 = 1.0 / (MAX_EXP - 2.0); /* 3.0 * MIN_EXP omitted */
                  pParam->FEFET1thetaRout = pParam->FEFET1pdibl1 * T5
                                         + pParam->FEFET1pdibl2;

                  tmp = sqrt(pParam->FEFET1Xdep0);
                  tmp1 = pParam->FEFET1vbi - pParam->FEFET1phi;
                  tmp2 = model->FEFET1factor1 * tmp;

                  T0 = pParam->FEFET1dvt1w * pParam->FEFET1weff
                     * pParam->FEFET1leff / tmp2;
                  if (T0 < EXP_THRESHOLD)
                  {   T1 = exp(T0);
                      T2 = T1 - 1.0;
                      T3 = T2 * T2;
                      T4 = T3 + 2.0 * T1 * MIN_EXP;
                      T8 = T1 / T4;
                  }
                  else
                      T8 = 1.0 / (MAX_EXP - 2.0);
                  T0 = pParam->FEFET1dvt0w * T8;
                  T8 = T0 * tmp1;

                  T0 = pParam->FEFET1dvt1 * pParam->FEFET1leff / tmp2;
                  if (T0 < EXP_THRESHOLD)
                  {   T1 = exp(T0);
                      T2 = T1 - 1.0;
                      T3 = T2 * T2;
                      T4 = T3 + 2.0 * T1 * MIN_EXP;
                      T9 = T1 / T4;
                  }
                  else
                      T9 = 1.0 / (MAX_EXP - 2.0);
                  T9 = pParam->FEFET1dvt0 * T9 * tmp1;

                  T4 = model->FEFET1toxe * pParam->FEFET1phi
                     / (pParam->FEFET1weff + pParam->FEFET1w0);

                  T0 = sqrt(1.0 + pParam->FEFET1lpe0 / pParam->FEFET1leff);
                  if((model->FEFET1tempMod == 1) || (model->FEFET1tempMod == 0))
                          T3 = (pParam->FEFET1kt1 + pParam->FEFET1kt1l / pParam->FEFET1leff)
                                     * (TRatio - 1.0);
                  if(model->FEFET1tempMod == 2)
                        T3 = - pParam->FEFET1kt1 * (TRatio - 1.0);

                  T5 = pParam->FEFET1k1ox * (T0 - 1.0) * pParam->FEFET1sqrtPhi
                     + T3;
                  pParam->FEFET1vfbzbfactor = - T8 - T9 + pParam->FEFET1k3 * T4 + T5
                                             - pParam->FEFET1phi - pParam->FEFET1k1 * pParam->FEFET1sqrtPhi;

                  /* stress effect */

                      wlod = model->FEFET1wlod;
                      if (model->FEFET1wlod < 0.0)
                  {   fprintf(stderr, "Warning: WLOD = %g is less than 0. 0.0 is used\n",model->FEFET1wlod);
                             wlod = 0.0;
                  }
                  T0 = pow(Lnew, model->FEFET1llodku0);
                  W_tmp = Wnew + wlod;
                  T1 = pow(W_tmp, model->FEFET1wlodku0);
                  tmp1 = model->FEFET1lku0 / T0 + model->FEFET1wku0 / T1
                         + model->FEFET1pku0 / (T0 * T1);
                  pParam->FEFET1ku0 = 1.0 + tmp1;

                  T0 = pow(Lnew, model->FEFET1llodvth);
                  T1 = pow(W_tmp, model->FEFET1wlodvth);
                  tmp1 = model->FEFET1lkvth0 / T0 + model->FEFET1wkvth0 / T1
                       + model->FEFET1pkvth0 / (T0 * T1);
                  pParam->FEFET1kvth0 = 1.0 + tmp1;
                  pParam->FEFET1kvth0 = sqrt(pParam->FEFET1kvth0*pParam->FEFET1kvth0 + DELTA);

                  T0 = (TRatio - 1.0);
                  pParam->FEFET1ku0temp = pParam->FEFET1ku0 * (1.0 + model->FEFET1tku0 *T0) + DELTA;

                  Inv_saref = 1.0/(model->FEFET1saref + 0.5*Ldrn);
                  Inv_sbref = 1.0/(model->FEFET1sbref + 0.5*Ldrn);
                  pParam->FEFET1inv_od_ref = Inv_saref + Inv_sbref;
                  pParam->FEFET1rho_ref = model->FEFET1ku0 / pParam->FEFET1ku0temp * pParam->FEFET1inv_od_ref;

              } /* End of SizeNotFound */

              /*  stress effect */
              if( (here->FEFET1sa > 0.0) && (here->FEFET1sb > 0.0) &&
                        ((here->FEFET1nf == 1.0) || ((here->FEFET1nf > 1.0) && (here->FEFET1sd > 0.0))) )
              {          Inv_sa = 0;
                        Inv_sb = 0;

                         kvsat = model->FEFET1kvsat;
                  if (model->FEFET1kvsat < -1.0 )
                  {   fprintf(stderr, "Warning: KVSAT = %g is too small; -1.0 is used.\n",model->FEFET1kvsat);
                             kvsat = -1.0;
                      }
                      if (model->FEFET1kvsat > 1.0)
                      {   fprintf(stderr, "Warning: KVSAT = %g is too big; 1.0 is used.\n",model->FEFET1kvsat);
                         kvsat = 1.0;
                      }

                        for(i = 0; i < here->FEFET1nf; i++){
                           T0 = 1.0 / here->FEFET1nf / (here->FEFET1sa + 0.5*Ldrn + i * (here->FEFET1sd +Ldrn));
                            T1 = 1.0 / here->FEFET1nf / (here->FEFET1sb + 0.5*Ldrn + i * (here->FEFET1sd +Ldrn));
                           Inv_sa += T0;
                            Inv_sb += T1;
                  }
                  Inv_ODeff = Inv_sa + Inv_sb;
                  rho = model->FEFET1ku0 / pParam->FEFET1ku0temp * Inv_ODeff;
                  T0 = (1.0 + rho)/(1.0 + pParam->FEFET1rho_ref);
                  here->FEFET1u0temp = pParam->FEFET1u0temp * T0;

                  T1 = (1.0 + kvsat * rho)/(1.0 + kvsat * pParam->FEFET1rho_ref);
                  here->FEFET1vsattemp = pParam->FEFET1vsattemp * T1;

                  OD_offset = Inv_ODeff - pParam->FEFET1inv_od_ref;
                  dvth0_lod = model->FEFET1kvth0 / pParam->FEFET1kvth0 * OD_offset;
                  dk2_lod = model->FEFET1stk2 / pow(pParam->FEFET1kvth0, model->FEFET1lodk2) *
                                   OD_offset;
                  deta0_lod = model->FEFET1steta0 / pow(pParam->FEFET1kvth0, model->FEFET1lodeta0) *
                                     OD_offset;
                  here->FEFET1vth0 = pParam->FEFET1vth0 + dvth0_lod;

                  here->FEFET1eta0 = pParam->FEFET1eta0 + deta0_lod;
                  here->FEFET1k2 = pParam->FEFET1k2 + dk2_lod;
               } else {
                      here->FEFET1u0temp = pParam->FEFET1u0temp;
                      here->FEFET1vth0 = pParam->FEFET1vth0;
                      here->FEFET1vsattemp = pParam->FEFET1vsattemp;
                      here->FEFET1eta0 = pParam->FEFET1eta0;
                      here->FEFET1k2 = pParam->FEFET1k2;
              }

              /*  Well Proximity Effect  */
              if (model->FEFET1wpemod)
              { if( (!here->FEFET1scaGiven) && (!here->FEFET1scbGiven) && (!here->FEFET1sccGiven) )
                {   if((here->FEFET1scGiven) && (here->FEFET1sc > 0.0) )
                          {   T1 = here->FEFET1sc + Wdrn;
                        T2 = 1.0 / model->FEFET1scref;
                        here->FEFET1sca = model->FEFET1scref * model->FEFET1scref
                                        / (here->FEFET1sc * T1);
                        here->FEFET1scb = ( (0.1 * here->FEFET1sc + 0.01 * model->FEFET1scref)
                                        * exp(-10.0 * here->FEFET1sc * T2)
                                        - (0.1 * T1 + 0.01 * model->FEFET1scref)
                                        * exp(-10.0 * T1 * T2) ) / Wdrn;
                        here->FEFET1scc = ( (0.05 * here->FEFET1sc + 0.0025 * model->FEFET1scref)
                                        * exp(-20.0 * here->FEFET1sc * T2)
                                        - (0.05 * T1 + 0.0025 * model->FEFET1scref)
                                        * exp(-20.0 * T1 * T2) ) / Wdrn;
                    } else {
                      //fprintf(stderr, "Warning: No WPE as none of SCA, SCB, SCC, SC is given and/or SC not positive.\n");
                    }
                }
                sceff = here->FEFET1sca + model->FEFET1web * here->FEFET1scb
                      + model->FEFET1wec * here->FEFET1scc;
                here->FEFET1vth0 += pParam->FEFET1kvth0we * sceff;
                here->FEFET1k2 +=  pParam->FEFET1k2we * sceff;
                  T3 =  1.0 + pParam->FEFET1ku0we * sceff;
                if (T3 <= 0.0)
                {
                        fprintf(stderr, "Warning: ku0we = %g is negatively too high. Negative mobility! \n", T3);
                        T3 = 0.0;
                }
                here->FEFET1u0temp *= T3;
              }

            /* adding delvto  */
            here->FEFET1vth0 += here->FEFET1delvto;
            here->FEFET1vfb = pParam->FEFET1vfb + model->FEFET1type * here->FEFET1delvto;

            /* Instance variables calculation  */
            T3 = model->FEFET1type * here->FEFET1vth0
               - here->FEFET1vfb - pParam->FEFET1phi;
            T4 = T3 + T3;
            T5 = 2.5 * T3;
            here->FEFET1vtfbphi1 = (model->FEFET1type == NMOS) ? T4 : T5;
            if (here->FEFET1vtfbphi1 < 0.0)
                here->FEFET1vtfbphi1 = 0.0;

            here->FEFET1vtfbphi2 = 4.0 * T3;
            if (here->FEFET1vtfbphi2 < 0.0)
                here->FEFET1vtfbphi2 = 0.0;

            if (here->FEFET1k2 < 0.0)
            {   T0 = 0.5 * pParam->FEFET1k1 / here->FEFET1k2;
                here->FEFET1vbsc = 0.9 * (pParam->FEFET1phi - T0 * T0);
                if (here->FEFET1vbsc > -3.0)
                    here->FEFET1vbsc = -3.0;
                else if (here->FEFET1vbsc < -30.0)
                    here->FEFET1vbsc = -30.0;
            }
            else
                here->FEFET1vbsc = -30.0;
            if (here->FEFET1vbsc > pParam->FEFET1vbm)
                here->FEFET1vbsc = pParam->FEFET1vbm;
            here->FEFET1k2ox = here->FEFET1k2 * model->FEFET1toxe
                              / model->FEFET1toxm;

            here->FEFET1vfbzb = pParam->FEFET1vfbzbfactor
                                +  model->FEFET1type * here->FEFET1vth0 ;

              here->FEFET1cgso = pParam->FEFET1cgso;
              here->FEFET1cgdo = pParam->FEFET1cgdo;

              lnl = log(pParam->FEFET1leff * 1.0e6);
              lnw = log(pParam->FEFET1weff * 1.0e6);
              lnnf = log(here->FEFET1nf);

              bodymode = 5;
              if( ( !model->FEFET1rbps0Given) ||
                  ( !model->FEFET1rbpd0Given) )
                bodymode = 1;
              else
                if( (!model->FEFET1rbsbx0Given && !model->FEFET1rbsby0Given) ||
                      (!model->FEFET1rbdbx0Given && !model->FEFET1rbdby0Given) )
                  bodymode = 3;

              if(here->FEFET1rbodyMod == 2)
                {
                  if (bodymode == 5)
                    {
                      rbsbx =  exp( log(model->FEFET1rbsbx0) + model->FEFET1rbsdbxl * lnl +
                                    model->FEFET1rbsdbxw * lnw + model->FEFET1rbsdbxnf * lnnf );
                      rbsby =  exp( log(model->FEFET1rbsby0) + model->FEFET1rbsdbyl * lnl +
                                    model->FEFET1rbsdbyw * lnw + model->FEFET1rbsdbynf * lnnf );
                      here->FEFET1rbsb = rbsbx * rbsby / (rbsbx + rbsby);


                      rbdbx =  exp( log(model->FEFET1rbdbx0) + model->FEFET1rbsdbxl * lnl +
                                    model->FEFET1rbsdbxw * lnw + model->FEFET1rbsdbxnf * lnnf );
                      rbdby =  exp( log(model->FEFET1rbdby0) + model->FEFET1rbsdbyl * lnl +
                                    model->FEFET1rbsdbyw * lnw + model->FEFET1rbsdbynf * lnnf );
                      here->FEFET1rbdb = rbdbx * rbdby / (rbdbx + rbdby);
                    }

                  if ((bodymode == 3)|| (bodymode == 5))
                    {
                      here->FEFET1rbps = exp( log(model->FEFET1rbps0) + model->FEFET1rbpsl * lnl +
                                             model->FEFET1rbpsw * lnw + model->FEFET1rbpsnf * lnnf );
                      here->FEFET1rbpd = exp( log(model->FEFET1rbpd0) + model->FEFET1rbpdl * lnl +
                                             model->FEFET1rbpdw * lnw + model->FEFET1rbpdnf * lnnf );
                    }

                  rbpbx =  exp( log(model->FEFET1rbpbx0) + model->FEFET1rbpbxl * lnl +
                                model->FEFET1rbpbxw * lnw + model->FEFET1rbpbxnf * lnnf );
                  rbpby =  exp( log(model->FEFET1rbpby0) + model->FEFET1rbpbyl * lnl +
                                model->FEFET1rbpbyw * lnw + model->FEFET1rbpbynf * lnnf );
                  here->FEFET1rbpb = rbpbx*rbpby/(rbpbx + rbpby);
                }


              if ((here->FEFET1rbodyMod == 1 ) || ((here->FEFET1rbodyMod == 2 ) && (bodymode == 5)) )
              {   if (here->FEFET1rbdb < 1.0e-3)
                      here->FEFET1grbdb = 1.0e3; /* in mho */
                  else
                      here->FEFET1grbdb = model->FEFET1gbmin + 1.0 / here->FEFET1rbdb;
                  if (here->FEFET1rbpb < 1.0e-3)
                      here->FEFET1grbpb = 1.0e3;
                  else
                      here->FEFET1grbpb = model->FEFET1gbmin + 1.0 / here->FEFET1rbpb;
                  if (here->FEFET1rbps < 1.0e-3)
                      here->FEFET1grbps = 1.0e3;
                  else
                      here->FEFET1grbps = model->FEFET1gbmin + 1.0 / here->FEFET1rbps;
                  if (here->FEFET1rbsb < 1.0e-3)
                      here->FEFET1grbsb = 1.0e3;
                  else
                      here->FEFET1grbsb = model->FEFET1gbmin + 1.0 / here->FEFET1rbsb;
                  if (here->FEFET1rbpd < 1.0e-3)
                      here->FEFET1grbpd = 1.0e3;
                  else
                      here->FEFET1grbpd = model->FEFET1gbmin + 1.0 / here->FEFET1rbpd;

              }

              if((here->FEFET1rbodyMod == 2) && (bodymode == 3))
              {
                      here->FEFET1grbdb = here->FEFET1grbsb = model->FEFET1gbmin;
                  if (here->FEFET1rbpb < 1.0e-3)
                      here->FEFET1grbpb = 1.0e3;
                  else
                      here->FEFET1grbpb = model->FEFET1gbmin + 1.0 / here->FEFET1rbpb;
                  if (here->FEFET1rbps < 1.0e-3)
                      here->FEFET1grbps = 1.0e3;
                  else
                      here->FEFET1grbps = model->FEFET1gbmin + 1.0 / here->FEFET1rbps;
                  if (here->FEFET1rbpd < 1.0e-3)
                      here->FEFET1grbpd = 1.0e3;
                  else
                      here->FEFET1grbpd = model->FEFET1gbmin + 1.0 / here->FEFET1rbpd;
              }

              if((here->FEFET1rbodyMod == 2) && (bodymode == 1))
              {
                      here->FEFET1grbdb = here->FEFET1grbsb = model->FEFET1gbmin;
                      here->FEFET1grbps = here->FEFET1grbpd = 1.0e3;
                  if (here->FEFET1rbpb < 1.0e-3)
                      here->FEFET1grbpb = 1.0e3;
                  else
                      here->FEFET1grbpb = model->FEFET1gbmin + 1.0 / here->FEFET1rbpb;
              }


              /*
               * Process geomertry dependent parasitics
               */

              here->FEFET1grgeltd = model->FEFET1rshg * (here->FEFET1xgw
                      + pParam->FEFET1weffCJ / 3.0 / here->FEFET1ngcon) /
                      (here->FEFET1ngcon * here->FEFET1nf *
                      (Lnew - model->FEFET1xgl));
              if (here->FEFET1grgeltd > 0.0)
                  here->FEFET1grgeltd = 1.0 / here->FEFET1grgeltd;
              else
              {   here->FEFET1grgeltd = 1.0e3; /* mho */
                  if (here->FEFET1rgateMod != 0)
                  printf("Warning: The gate conductance reset to 1.0e3 mho.\n");
              }

              DMCGeff = model->FEFET1dmcg - model->FEFET1dmcgt;
              DMCIeff = model->FEFET1dmci;
              DMDGeff = model->FEFET1dmdg - model->FEFET1dmcgt;

              if (here->FEFET1sourcePerimeterGiven)
              {   if (model->FEFET1perMod == 0)
                      here->FEFET1Pseff = here->FEFET1sourcePerimeter;
                  else
                      here->FEFET1Pseff = here->FEFET1sourcePerimeter
                                       - pParam->FEFET1weffCJ * here->FEFET1nf;
              }
              else
                  FEFET1PAeffGeo(here->FEFET1nf, here->FEFET1geoMod, here->FEFET1min,
                                    pParam->FEFET1weffCJ, DMCGeff, DMCIeff, DMDGeff,
                                    &(here->FEFET1Pseff), &dumPd, &dumAs, &dumAd);

              if (here->FEFET1drainPerimeterGiven)
              {   if (model->FEFET1perMod == 0)
                      here->FEFET1Pdeff = here->FEFET1drainPerimeter;
                  else
                      here->FEFET1Pdeff = here->FEFET1drainPerimeter
                                       - pParam->FEFET1weffCJ * here->FEFET1nf;
              }
              else
                  FEFET1PAeffGeo(here->FEFET1nf, here->FEFET1geoMod, here->FEFET1min,
                                    pParam->FEFET1weffCJ, DMCGeff, DMCIeff, DMDGeff,
                                    &dumPs, &(here->FEFET1Pdeff), &dumAs, &dumAd);

              if (here->FEFET1sourceAreaGiven)
                  here->FEFET1Aseff = here->FEFET1sourceArea;
              else
                  FEFET1PAeffGeo(here->FEFET1nf, here->FEFET1geoMod, here->FEFET1min,
                                    pParam->FEFET1weffCJ, DMCGeff, DMCIeff, DMDGeff,
                                    &dumPs, &dumPd, &(here->FEFET1Aseff), &dumAd);

              if (here->FEFET1drainAreaGiven)
                  here->FEFET1Adeff = here->FEFET1drainArea;
              else
                  FEFET1PAeffGeo(here->FEFET1nf, here->FEFET1geoMod, here->FEFET1min,
                                    pParam->FEFET1weffCJ, DMCGeff, DMCIeff, DMDGeff,
                                    &dumPs, &dumPd, &dumAs, &(here->FEFET1Adeff));

              /* Processing S/D resistance and conductance below */
              if(here->FEFET1sNodePrime != here->FEFET1sNode)
              {
                 here->FEFET1sourceConductance = 0.0;
                 if(here->FEFET1sourceSquaresGiven)
                 {
                    here->FEFET1sourceConductance = model->FEFET1sheetResistance
                                               * here->FEFET1sourceSquares;
                 } else if (here->FEFET1rgeoMod > 0)
                 {
                    FEFET1RdseffGeo(here->FEFET1nf, here->FEFET1geoMod,
                      here->FEFET1rgeoMod, here->FEFET1min,
                      pParam->FEFET1weffCJ, model->FEFET1sheetResistance,
                  DMCGeff, DMCIeff, DMDGeff, 1, &(here->FEFET1sourceConductance));
                 } else
                 {
                    here->FEFET1sourceConductance = 0.0;
                 }

                 if (here->FEFET1sourceConductance > 0.0)
                     here->FEFET1sourceConductance = 1.0
                                            / here->FEFET1sourceConductance;
                 else
                 {
                     here->FEFET1sourceConductance = 1.0e3; /* mho */
                     printf ("Warning: Source conductance reset to 1.0e3 mho.\n");
                 }
              } else
              {
                  here->FEFET1sourceConductance = 0.0;
              }

              if(here->FEFET1dNodePrime != here->FEFET1dNode)
              {
                 here->FEFET1drainConductance = 0.0;
                 if(here->FEFET1drainSquaresGiven)
                 {
                    here->FEFET1drainConductance = model->FEFET1sheetResistance
                                              * here->FEFET1drainSquares;
                 } else if (here->FEFET1rgeoMod > 0)
                 {
                    FEFET1RdseffGeo(here->FEFET1nf, here->FEFET1geoMod,
                      here->FEFET1rgeoMod, here->FEFET1min,
                      pParam->FEFET1weffCJ, model->FEFET1sheetResistance,
                  DMCGeff, DMCIeff, DMDGeff, 0, &(here->FEFET1drainConductance));
                 } else
                 {
                    here->FEFET1drainConductance = 0.0;
                 }

                 if (here->FEFET1drainConductance > 0.0)
                     here->FEFET1drainConductance = 1.0
                                           / here->FEFET1drainConductance;
                 else
                 {
                     here->FEFET1drainConductance = 1.0e3; /* mho */
                     printf ("Warning: Drain conductance reset to 1.0e3 mho.\n");
                  }
              } else
              {
                  here->FEFET1drainConductance = 0.0;
              }

               /* End of Rsd processing */


              Nvtms = model->FEFET1vtm * model->FEFET1SjctEmissionCoeff;
              if ((here->FEFET1Aseff <= 0.0) && (here->FEFET1Pseff <= 0.0))
              {   SourceSatCurrent = 1.0e-14;
              }
              else
              {   SourceSatCurrent = here->FEFET1Aseff * model->FEFET1SjctTempSatCurDensity
                                   + here->FEFET1Pseff * model->FEFET1SjctSidewallTempSatCurDensity
                                   + pParam->FEFET1weffCJ * here->FEFET1nf
                                   * model->FEFET1SjctGateSidewallTempSatCurDensity;
              }
              if (SourceSatCurrent > 0.0)
              {   switch(model->FEFET1dioMod)
                  {   case 0:
                          if ((model->FEFET1bvs / Nvtms) > EXP_THRESHOLD)
                              here->FEFET1XExpBVS = model->FEFET1xjbvs * MIN_EXP;
                          else
                              here->FEFET1XExpBVS = model->FEFET1xjbvs * exp(-model->FEFET1bvs / Nvtms);
                          break;
                      case 1:
                          FEFET1DioIjthVjmEval(Nvtms, model->FEFET1ijthsfwd, SourceSatCurrent,
                                              0.0, &(here->FEFET1vjsmFwd));
                          here->FEFET1IVjsmFwd = SourceSatCurrent * exp(here->FEFET1vjsmFwd / Nvtms);
                          break;
                      case 2:
                          if ((model->FEFET1bvs / Nvtms) > EXP_THRESHOLD)
                          {   here->FEFET1XExpBVS = model->FEFET1xjbvs * MIN_EXP;
                              tmp = MIN_EXP;
                          }
                          else
                          {   here->FEFET1XExpBVS = exp(-model->FEFET1bvs / Nvtms);
                              tmp = here->FEFET1XExpBVS;
                              here->FEFET1XExpBVS *= model->FEFET1xjbvs;
                          }

                          FEFET1DioIjthVjmEval(Nvtms, model->FEFET1ijthsfwd, SourceSatCurrent,
                                                     here->FEFET1XExpBVS, &(here->FEFET1vjsmFwd));
                          T0 = exp(here->FEFET1vjsmFwd / Nvtms);
                          here->FEFET1IVjsmFwd = SourceSatCurrent * (T0 - here->FEFET1XExpBVS / T0
                                                + here->FEFET1XExpBVS - 1.0);
                          here->FEFET1SslpFwd = SourceSatCurrent
                                               * (T0 + here->FEFET1XExpBVS / T0) / Nvtms;

                          T2 = model->FEFET1ijthsrev / SourceSatCurrent;
                          if (T2 < 1.0)
                          {   T2 = 10.0;
                              fprintf(stderr, "Warning: ijthsrev too small and set to 10 times IsbSat.\n");
                          }
                          here->FEFET1vjsmRev = -model->FEFET1bvs
                                             - Nvtms * log((T2 - 1.0) / model->FEFET1xjbvs);
                          T1 = model->FEFET1xjbvs * exp(-(model->FEFET1bvs
                             + here->FEFET1vjsmRev) / Nvtms);
                          here->FEFET1IVjsmRev = SourceSatCurrent * (1.0 + T1);
                          here->FEFET1SslpRev = -SourceSatCurrent * T1 / Nvtms;
                          break;
                  default:
                          printf("Specified dioMod = %d not matched\n", model->FEFET1dioMod);
                  }
              }

              Nvtmd = model->FEFET1vtm * model->FEFET1DjctEmissionCoeff;
              if ((here->FEFET1Adeff <= 0.0) && (here->FEFET1Pdeff <= 0.0))
              {   DrainSatCurrent = 1.0e-14;
              }
              else
              {   DrainSatCurrent = here->FEFET1Adeff * model->FEFET1DjctTempSatCurDensity
                                  + here->FEFET1Pdeff * model->FEFET1DjctSidewallTempSatCurDensity
                                  + pParam->FEFET1weffCJ * here->FEFET1nf
                                  * model->FEFET1DjctGateSidewallTempSatCurDensity;
              }
              if (DrainSatCurrent > 0.0)
              {   switch(model->FEFET1dioMod)
                  {   case 0:
                          if ((model->FEFET1bvd / Nvtmd) > EXP_THRESHOLD)
                          here->FEFET1XExpBVD = model->FEFET1xjbvd * MIN_EXP;
                          else
                          here->FEFET1XExpBVD = model->FEFET1xjbvd * exp(-model->FEFET1bvd / Nvtmd);
                          break;
                      case 1:
                          FEFET1DioIjthVjmEval(Nvtmd, model->FEFET1ijthdfwd, DrainSatCurrent,
                                              0.0, &(here->FEFET1vjdmFwd));
                          here->FEFET1IVjdmFwd = DrainSatCurrent * exp(here->FEFET1vjdmFwd / Nvtmd);
                          break;
                      case 2:
                          if ((model->FEFET1bvd / Nvtmd) > EXP_THRESHOLD)
                          {   here->FEFET1XExpBVD = model->FEFET1xjbvd * MIN_EXP;
                              tmp = MIN_EXP;
                          }
                          else
                          {   here->FEFET1XExpBVD = exp(-model->FEFET1bvd / Nvtmd);
                              tmp = here->FEFET1XExpBVD;
                              here->FEFET1XExpBVD *= model->FEFET1xjbvd;
                          }

                          FEFET1DioIjthVjmEval(Nvtmd, model->FEFET1ijthdfwd, DrainSatCurrent,
                                              here->FEFET1XExpBVD, &(here->FEFET1vjdmFwd));
                          T0 = exp(here->FEFET1vjdmFwd / Nvtmd);
                          here->FEFET1IVjdmFwd = DrainSatCurrent * (T0 - here->FEFET1XExpBVD / T0
                                              + here->FEFET1XExpBVD - 1.0);
                          here->FEFET1DslpFwd = DrainSatCurrent
                                               * (T0 + here->FEFET1XExpBVD / T0) / Nvtmd;

                          T2 = model->FEFET1ijthdrev / DrainSatCurrent;
                          if (T2 < 1.0)
                          {   T2 = 10.0;
                              fprintf(stderr, "Warning: ijthdrev too small and set to 10 times IdbSat.\n");
                          }
                          here->FEFET1vjdmRev = -model->FEFET1bvd
                                             - Nvtmd * log((T2 - 1.0) / model->FEFET1xjbvd); /* bugfix */
                          T1 = model->FEFET1xjbvd * exp(-(model->FEFET1bvd
                             + here->FEFET1vjdmRev) / Nvtmd);
                          here->FEFET1IVjdmRev = DrainSatCurrent * (1.0 + T1);
                          here->FEFET1DslpRev = -DrainSatCurrent * T1 / Nvtmd;
                          break;
                  default:
                          printf("Specified dioMod = %d not matched\n", model->FEFET1dioMod);
                  }
              }

                /* GEDL current reverse bias */
                T0 = (TRatio - 1.0);
                model->FEFET1njtstemp = model->FEFET1njts * (1.0 + model->FEFET1tnjts * T0);
                model->FEFET1njtsswtemp = model->FEFET1njtssw * (1.0 + model->FEFET1tnjtssw * T0);
                model->FEFET1njtsswgtemp = model->FEFET1njtsswg * (1.0 + model->FEFET1tnjtsswg * T0);
                T7 = Eg0 / model->FEFET1vtm * T0;
                T9 = model->FEFET1xtss * T7;
                DEXP(T9, T1);
                T9 = model->FEFET1xtsd * T7;
                DEXP(T9, T2);
                T9 = model->FEFET1xtssws * T7;
                DEXP(T9, T3);
                T9 = model->FEFET1xtsswd * T7;
                DEXP(T9, T4);
                T9 = model->FEFET1xtsswgs * T7;
                DEXP(T9, T5);
                T9 = model->FEFET1xtsswgd * T7;
                DEXP(T9, T6);

                T10 = pParam->FEFET1weffCJ * here->FEFET1nf;
                here->FEFET1SjctTempRevSatCur = T1 * here->FEFET1Aseff * model->FEFET1jtss;
                here->FEFET1DjctTempRevSatCur = T2 * here->FEFET1Adeff * model->FEFET1jtsd;
                here->FEFET1SswTempRevSatCur = T3 * here->FEFET1Pseff * model->FEFET1jtssws;
                here->FEFET1DswTempRevSatCur = T4 * here->FEFET1Pdeff * model->FEFET1jtsswd;
                here->FEFET1SswgTempRevSatCur = T5 * T10 * model->FEFET1jtsswgs;
                here->FEFET1DswgTempRevSatCur = T6 * T10 * model->FEFET1jtsswgd;


              if (FEFET1checkModel(model, here, ckt))
              {
                  SPfrontEnd->IFerrorf (ERR_FATAL, "Fatal error(s) detected during FEFET1.5.0 parameter checking for %s in model %s", model->FEFET1modName, here->FEFET1name);
                  return(E_BADPARM);
              }
         } /* End instance */
    }
    return(OK);
}
