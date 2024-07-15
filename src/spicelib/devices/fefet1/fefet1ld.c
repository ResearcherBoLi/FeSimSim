/**********
Copyright 2024 Xidian University
Author: 2024 Bo Li
Modified: 2024/01/27  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "fefet1def.h"
#include "ngspice/trandefs.h"
#include "ngspice/const.h"
#include "ngspice/sperror.h"
#include "ngspice/devdefs.h"
#include "ngspice/suffix.h"
/*
#define MAX_EXP 2.688117142e+43
#define MIN_EXP 3.720075976e-44
#define EXP_THRESHOLD 100.0
*/
#define MAX_EXP 5.834617425e14
#define MIN_EXP 1.713908431e-15
#define EXP_THRESHOLD 34.0
#define EPSSI 1.03594e-10
#define Charge_q 1.60219e-19
#define DELTA_1 0.02
#define DELTA_2 0.02
#define DELTA_3 0.02
#define DELTA_4 0.02
#define MM  3  /* smooth coeff */
#define DEXP(A,B,C) {                                                         \
        if (A > EXP_THRESHOLD) {                                              \
            B = MAX_EXP*(1.0+(A)-EXP_THRESHOLD);                              \
            C = MAX_EXP;                                                      \
        } else if (A < -EXP_THRESHOLD)  {                                     \
            B = MIN_EXP;                                                      \
            C = 0;                                                            \
        } else   {                                                            \
            B = exp(A);                                                       \
            C = B;                                                            \
        }                                                                     \
    }

#ifdef USE_OMP
int FEFET1LoadOMP(FEFET1instance *here, CKTcircuit *ckt);
void FEFET1LoadRhsMat(GENmodel *inModel, CKTcircuit *ckt);
#endif

int FEFET1polyDepletion(double phi, double ngate,double coxe, double Vgs, double *Vgs_eff, double *dVgs_eff_dVg);

int
FEFET1load(GENmodel *inModel, CKTcircuit *ckt)
{
    #ifdef USE_OMP
        int idx;
        FEFET1model *model = (FEFET1model*)inModel;
        int error = 0;
        FEFET1instance **InstArray;
        InstArray = model->FEFET1InstanceArray;

    #pragma omp parallel for
        for (idx = 0; idx < model->FEFET1InstCount; idx++) {
            FEFET1instance *here = InstArray[idx];
            int local_error = FEFET1LoadOMP(here, ckt);
            if (local_error)
                error = local_error;
        }

        FEFET1LoadRhsMat(inModel, ckt);

        return error;
}


int FEFET1LoadOMP(FEFET1instance *here, CKTcircuit *ckt) {
    FEFET1model *model = FEFET1modPtr(here);
    #else
    FEFET1model *model = (FEFET1model*)inModel;
    FEFET1instance *here;
    #endif

    double ceqgstot, dgstot_dvd, dgstot_dvg, dgstot_dvs, dgstot_dvb;
    double ceqgdtot, dgdtot_dvd, dgdtot_dvg, dgdtot_dvs, dgdtot_dvb;
    double gstot, gstotd, gstotg, gstots, gstotb, gspr, Rs, Rd;
    double gdtot, gdtotd, gdtotg, gdtots, gdtotb, gdpr;
    double vgs_eff, vgd_eff, dvgs_eff_dvg, dvgd_eff_dvg;
    double dRs_dvg, dRd_dvg, dRs_dvb, dRd_dvb;
    double dT0_dvg, dT1_dvb, dT3_dvg, dT3_dvb;
    double vses, vdes, vdedo, delvses, delvded, delvdes;
    double Isestot=0.0, cseshat=0.0, Idedtot=0.0, cdedhat=0.0;
    #ifndef NEWCONV
    double tol0, tol1, tol2, tol3, tol4, tol5, tol6;
    #endif

    double geltd, gcrg, gcrgg, gcrgd, gcrgs, gcrgb, ceqgcrg;
    double vges, vgms, vgedo, vgmdo, vged, vgmd, delvged, delvgmd;
    double delvges, delvgms, vgmb;
    double gcgmgmb=0.0, gcgmdb=0.0, gcgmsb=0.0, gcdgmb, gcsgmb;
    double gcgmbb=0.0, gcbgmb, qgmb, qgmid=0.0, ceqqgmid;

    double vbd, vbs, vds, vgb, vgd, vgs, vgdo;
    #ifndef PREDICTOR
    double xfact;
    #endif
    double vdbs, vdbd, vsbs, vsbdo, vsbd;
    double delvdbs, delvdbd, delvsbs;
    double delvbd_jct, delvbs_jct, vbs_jct, vbd_jct;

    double SourceSatCurrent, DrainSatCurrent;
    double ag0, qgb, von, cbhat=0.0, VgstNVt, ExpVgst;
    double ceqqb, ceqqd, ceqqg, ceqqjd=0.0, ceqqjs=0.0, ceq, geq;
    double cdrain, cdhat=0.0, ceqdrn, ceqbd, ceqbs, ceqjd, ceqjs, gjbd, gjbs;
    double czbd, czbdsw, czbdswg, czbs, czbssw, czbsswg, evbd, evbs, arg, sarg;
    double delvbd, delvbs, delvds, delvgd, delvgs;
    double Vfbeff, dVfbeff_dVg, dVfbeff_dVb, V3, V4;
    double gcbdb, gcbgb, gcbsb, gcddb, gcdgb, gcdsb, gcgdb, gcggb, gcgsb, gcsdb;
    double gcgbb, gcdbb, gcsbb, gcbbb;
    double gcdbdb, gcsbsb;
    double gcsgb, gcssb, MJD, MJSWD, MJSWGD, MJS, MJSWS, MJSWGS;
    double qgate=0.0, qbulk=0.0, qdrn=0.0, qsrc, cqgate, cqbody, cqdrn;
    double Vdb, Vds, Vgs, Vbs, Gmbs, FwdSum, RevSum;
    double Igidl, Ggidld, Ggidlg, Ggidlb;
    double Voxacc=0.0, dVoxacc_dVg=0.0, dVoxacc_dVb=0.0;
    double Voxdepinv=0.0, dVoxdepinv_dVg=0.0, dVoxdepinv_dVd=0.0, dVoxdepinv_dVb=0.0;
    double VxNVt=0.0, ExpVxNVt, Vaux=0.0, dVaux_dVg=0.0, dVaux_dVd=0.0, dVaux_dVb=0.0;
    double Igc, dIgc_dVg, dIgc_dVd, dIgc_dVb;
    double Igcs, dIgcs_dVg, dIgcs_dVd, dIgcs_dVb;
    double Igcd, dIgcd_dVg, dIgcd_dVd, dIgcd_dVb;
    double Igs, dIgs_dVg, dIgs_dVs, Igd, dIgd_dVg, dIgd_dVd;
    double Igbacc, dIgbacc_dVg, dIgbacc_dVb;
    double Igbinv, dIgbinv_dVg, dIgbinv_dVd, dIgbinv_dVb;
    double Pigcd, dPigcd_dVg, dPigcd_dVd, dPigcd_dVb;
    double Istoteq, gIstotg, gIstotd, gIstots, gIstotb;
    double Idtoteq, gIdtotg, gIdtotd, gIdtots, gIdtotb;
    double Ibtoteq, gIbtotg, gIbtotd, gIbtots, gIbtotb;
    double Igtoteq, gIgtotg, gIgtotd, gIgtots, gIgtotb;
    double Igstot=0.0, cgshat=0.0, Igdtot=0.0, cgdhat=0.0, Igbtot=0.0, cgbhat=0.0;
    double Vgs_eff, Vfb=0.0, Vth_NarrowW;
    double Phis, dPhis_dVb, sqrtPhis, dsqrtPhis_dVb, Vth, dVth_dVb, dVth_dVd;
    double Vgst, dVgst_dVg, dVgst_dVb, dVgs_eff_dVg, Nvtms, Nvtmd;
    double Vtm, Vtm0;
    double n, dn_dVb, dn_dVd, voffcv, noff, dnoff_dVd, dnoff_dVb;
    double V0, CoxWLcen, QovCox, LINK;
    double DeltaPhi, dDeltaPhi_dVg, VgDP, dVgDP_dVg;
    double Cox, Tox, Tcen, dTcen_dVg, dTcen_dVd, dTcen_dVb;
    double Ccen, Coxeff, dCoxeff_dVd, dCoxeff_dVg, dCoxeff_dVb;
    double Denomi, dDenomi_dVg, dDenomi_dVd, dDenomi_dVb;
    double ueff, dueff_dVg, dueff_dVd, dueff_dVb; 
    double Esat, Vdsat;
    double EsatL, dEsatL_dVg, dEsatL_dVd, dEsatL_dVb;
    double dVdsat_dVg, dVdsat_dVb, dVdsat_dVd, Vasat, dAlphaz_dVg, dAlphaz_dVb; 
    double dVasat_dVg, dVasat_dVb, dVasat_dVd, Va, dVa_dVd, dVa_dVg, dVa_dVb; 
    double Vbseff, dVbseff_dVb, VbseffCV, dVbseffCV_dVb; 
    double Arg1, One_Third_CoxWL, Two_Third_CoxWL, Alphaz, CoxWL; 
    double T0=0.0, dT0_dVg, dT0_dVd, dT0_dVb;
    double T1, dT1_dVg, dT1_dVd, dT1_dVb;
    double T2, dT2_dVg, dT2_dVd, dT2_dVb;
    double T3, dT3_dVg, dT3_dVd, dT3_dVb;
    double T4, dT4_dVd, dT4_dVb;
    double T5, dT5_dVg, dT5_dVd, dT5_dVb;
    double T6, dT6_dVg, dT6_dVd, dT6_dVb;
    double T7, dT7_dVg, dT7_dVd, dT7_dVb;
    double T8, dT8_dVg, dT8_dVd, dT8_dVb;
    double T9, dT9_dVg, dT9_dVd, dT9_dVb;
    double T10, dT10_dVg, dT10_dVb, dT10_dVd; 
    double T11, T12, T13, T14;
    double tmp, Abulk, dAbulk_dVb, Abulk0, dAbulk0_dVb;
    double Cclm, dCclm_dVg, dCclm_dVd, dCclm_dVb;
    double FP, dFP_dVg, PvagTerm, dPvagTerm_dVg, dPvagTerm_dVd, dPvagTerm_dVb;
    double VADITS, dVADITS_dVg, dVADITS_dVd;
    double Lpe_Vb, dDITS_Sft_dVb, dDITS_Sft_dVd;
    double VACLM, dVACLM_dVg, dVACLM_dVd, dVACLM_dVb;
    double VADIBL, dVADIBL_dVg, dVADIBL_dVd, dVADIBL_dVb;
    double Xdep, dXdep_dVb, lt1, dlt1_dVb, ltw, dltw_dVb, Delt_vth, dDelt_vth_dVb;
    double Theta0, dTheta0_dVb;
    double TempRatio, tmp1, tmp2, tmp3, tmp4;
    double DIBL_Sft, dDIBL_Sft_dVd, Lambda, dLambda_dVg;
    double Idtot, Ibtot, a1, ScalingFactor;

    double Vgsteff, dVgsteff_dVg, dVgsteff_dVd, dVgsteff_dVb; 
    double Vdseff, dVdseff_dVg, dVdseff_dVd, dVdseff_dVb; 
    double VdseffCV, dVdseffCV_dVg, dVdseffCV_dVd, dVdseffCV_dVb; 
    double diffVds, dAbulk_dVg;
    double beta, dbeta_dVg, dbeta_dVd, dbeta_dVb;
    double gche, dgche_dVg, dgche_dVd, dgche_dVb;
    double fgche1, dfgche1_dVg, dfgche1_dVd, dfgche1_dVb;
    double fgche2, dfgche2_dVg, dfgche2_dVd, dfgche2_dVb;
    double Idl, dIdl_dVg, dIdl_dVd, dIdl_dVb;
    double Idsa, dIdsa_dVg, dIdsa_dVd, dIdsa_dVb;
    double Ids, Gm, Gds, Gmb, devbs_dvb, devbd_dvb;
    double Isub, Gbd, Gbg, Gbb;
    double VASCBE, dVASCBE_dVg, dVASCBE_dVd, dVASCBE_dVb;
    double CoxeffWovL;
    double Rds, dRds_dVg, dRds_dVb, WVCox, WVCoxRds;
    double Vgst2Vtm, VdsatCV;
    double Leff, Weff, dWeff_dVg, dWeff_dVb;
    double AbulkCV, dAbulkCV_dVb;
    double qcheq, qdef, gqdef=0.0, cqdef=0.0, cqcheq=0.0;
    double gcqdb=0.0, gcqsb=0.0, gcqgb=0.0, gcqbb=0.0;
    double dxpart, sxpart, ggtg, ggtd, ggts, ggtb;
    double ddxpart_dVd, ddxpart_dVg, ddxpart_dVb, ddxpart_dVs;
    double dsxpart_dVd, dsxpart_dVg, dsxpart_dVb, dsxpart_dVs;
    double gbspsp, gbbdp, gbbsp, gbspg, gbspb, gbspdp; 
    double gbdpdp, gbdpg, gbdpb, gbdpsp; 
    double qgdo, qgso, cgdo, cgso;
    double Cgg, Cgd, Cgb, Cdg, Cdd, Cds;
    double Csg, Csd, Css, Csb, Cbg, Cbd, Cbb;
    double Cgg1, Cgb1, Cgd1, Cbg1, Cbb1, Cbd1, Qac0, Qsub0;
    double dQac0_dVg, dQac0_dVb, dQsub0_dVg, dQsub0_dVd, dQsub0_dVb;
    double ggidld, ggidlg, ggidlb, ggislg, ggislb, ggisls;
    double Igisl, Ggislg, Ggislb, Ggisls;
    double Nvtmrs, Nvtmrssw, Nvtmrsswg;

    double vs, Fsevl, dvs_dVg, dvs_dVd, dvs_dVb, dFsevl_dVg, dFsevl_dVd, dFsevl_dVb;
    double vgdx, vgsx;
    struct fefet1SizeDependParam *pParam;
    int ByPass, ChargeComputationNeeded, error, Check, Check1, Check2;

    double m;

    ScalingFactor = 1.0e-9;
    ChargeComputationNeeded =  
                    ((ckt->CKTmode & (MODEAC | MODETRAN | MODEINITSMSIG)) ||
                    ((ckt->CKTmode & MODETRANOP) && (ckt->CKTmode & MODEUIC)))
                    ? 1 : 0;
    ChargeComputationNeeded = 1;

    #ifndef USE_OMP
    for (; model != NULL; model = FEFET1nextModel(model))
    {    for (here = FEFET1instances(model); here != NULL; 
            here = FEFET1nextInstance(here))
        {
    #endif

            Check = Check1 = Check2 = 1;
            ByPass = 0;
        pParam = here->pParam;

            if ((ckt->CKTmode & MODEINITSMSIG))
        {   vds = *(ckt->CKTstate0 + here->FEFET1vds);
                vgs = *(ckt->CKTstate0 + here->FEFET1vgs);
                vbs = *(ckt->CKTstate0 + here->FEFET1vbs);
                vges = *(ckt->CKTstate0 + here->FEFET1vges);
                vgms = *(ckt->CKTstate0 + here->FEFET1vgms);
                vdbs = *(ckt->CKTstate0 + here->FEFET1vdbs);
                vsbs = *(ckt->CKTstate0 + here->FEFET1vsbs);
                vses = *(ckt->CKTstate0 + here->FEFET1vses);
                vdes = *(ckt->CKTstate0 + here->FEFET1vdes);

                qdef = *(ckt->CKTstate0 + here->FEFET1qdef);
            }
        else if ((ckt->CKTmode & MODEINITTRAN))
        {   vds = *(ckt->CKTstate1 + here->FEFET1vds);
                vgs = *(ckt->CKTstate1 + here->FEFET1vgs);
                vbs = *(ckt->CKTstate1 + here->FEFET1vbs);
                vges = *(ckt->CKTstate1 + here->FEFET1vges);
                vgms = *(ckt->CKTstate1 + here->FEFET1vgms);
                vdbs = *(ckt->CKTstate1 + here->FEFET1vdbs);
                vsbs = *(ckt->CKTstate1 + here->FEFET1vsbs);
                vses = *(ckt->CKTstate1 + here->FEFET1vses);
                vdes = *(ckt->CKTstate1 + here->FEFET1vdes);

                qdef = *(ckt->CKTstate1 + here->FEFET1qdef);
            }
        else if ((ckt->CKTmode & MODEINITJCT) && !here->FEFET1off)
        {   vds = model->FEFET1type * here->FEFET1icVDS;
                vgs = vges = vgms = model->FEFET1type * here->FEFET1icVGS;
                vbs = vdbs = vsbs = model->FEFET1type * here->FEFET1icVBS;
            if (vds > 0.0)
            {   vdes = vds + 0.01;
            vses = -0.01;
            }
            else if (vds < 0.0)
                {   vdes = vds - 0.01;
                    vses = 0.01;
                }
            else
                vdes = vses = 0.0;

                qdef = 0.0;

                if ((vds == 0.0) && (vgs == 0.0) && (vbs == 0.0) &&
                    ((ckt->CKTmode & (MODETRAN | MODEAC|MODEDCOP |
                    MODEDCTRANCURVE)) || (!(ckt->CKTmode & MODEUIC))))
            {   vds = 0.1;
            vdes = 0.11;
            vses = -0.01;
                    vgs = vges = vgms = model->FEFET1type 
                                        * here->FEFET1vth0 + 0.1;
                    vbs = vdbs = vsbs = 0.0;
                }
            }
        else if ((ckt->CKTmode & (MODEINITJCT | MODEINITFIX)) && 
                    (here->FEFET1off)) 
            {   vds = vgs = vbs = vges = vgms = 0.0;
                vdbs = vsbs = vdes = vses = qdef = 0.0;
        }
            else
        {
    #ifndef PREDICTOR
                if ((ckt->CKTmode & MODEINITPRED))
            {   xfact = ckt->CKTdelta / ckt->CKTdeltaOld[1];
                    *(ckt->CKTstate0 + here->FEFET1vds) = 
                            *(ckt->CKTstate1 + here->FEFET1vds);
                    vds = (1.0 + xfact)* (*(ckt->CKTstate1 + here->FEFET1vds))
                            - (xfact * (*(ckt->CKTstate2 + here->FEFET1vds)));
                    *(ckt->CKTstate0 + here->FEFET1vgs) = 
                            *(ckt->CKTstate1 + here->FEFET1vgs);
                    vgs = (1.0 + xfact)* (*(ckt->CKTstate1 + here->FEFET1vgs))
                            - (xfact * (*(ckt->CKTstate2 + here->FEFET1vgs)));
                    *(ckt->CKTstate0 + here->FEFET1vges) =
                            *(ckt->CKTstate1 + here->FEFET1vges);
                    vges = (1.0 + xfact)* (*(ckt->CKTstate1 + here->FEFET1vges))
                            - (xfact * (*(ckt->CKTstate2 + here->FEFET1vges)));
                    *(ckt->CKTstate0 + here->FEFET1vgms) =
                            *(ckt->CKTstate1 + here->FEFET1vgms);
                    vgms = (1.0 + xfact)* (*(ckt->CKTstate1 + here->FEFET1vgms))
                            - (xfact * (*(ckt->CKTstate2 + here->FEFET1vgms)));
                    *(ckt->CKTstate0 + here->FEFET1vbs) = 
                            *(ckt->CKTstate1 + here->FEFET1vbs);
                    vbs = (1.0 + xfact)* (*(ckt->CKTstate1 + here->FEFET1vbs))
                            - (xfact * (*(ckt->CKTstate2 + here->FEFET1vbs)));
                    *(ckt->CKTstate0 + here->FEFET1vbd) = 
                            *(ckt->CKTstate0 + here->FEFET1vbs)
                            - *(ckt->CKTstate0 + here->FEFET1vds);
                    *(ckt->CKTstate0 + here->FEFET1vdbs) =
                            *(ckt->CKTstate1 + here->FEFET1vdbs);
                    vdbs = (1.0 + xfact)* (*(ckt->CKTstate1 + here->FEFET1vdbs))
                            - (xfact * (*(ckt->CKTstate2 + here->FEFET1vdbs)));
                    *(ckt->CKTstate0 + here->FEFET1vdbd) =
                            *(ckt->CKTstate0 + here->FEFET1vdbs)
                            - *(ckt->CKTstate0 + here->FEFET1vds);
                    *(ckt->CKTstate0 + here->FEFET1vsbs) =
                            *(ckt->CKTstate1 + here->FEFET1vsbs);
                    vsbs = (1.0 + xfact)* (*(ckt->CKTstate1 + here->FEFET1vsbs))
                            - (xfact * (*(ckt->CKTstate2 + here->FEFET1vsbs)));
                    *(ckt->CKTstate0 + here->FEFET1vses) =
                            *(ckt->CKTstate1 + here->FEFET1vses);
                    vses = (1.0 + xfact)* (*(ckt->CKTstate1 + here->FEFET1vses))
                            - (xfact * (*(ckt->CKTstate2 + here->FEFET1vses)));
                    *(ckt->CKTstate0 + here->FEFET1vdes) =
                            *(ckt->CKTstate1 + here->FEFET1vdes);
                    vdes = (1.0 + xfact)* (*(ckt->CKTstate1 + here->FEFET1vdes))
                            - (xfact * (*(ckt->CKTstate2 + here->FEFET1vdes)));

                    *(ckt->CKTstate0 + here->FEFET1qdef) =
                            *(ckt->CKTstate1 + here->FEFET1qdef);
                    qdef = (1.0 + xfact)* (*(ckt->CKTstate1 + here->FEFET1qdef))
                            -(xfact * (*(ckt->CKTstate2 + here->FEFET1qdef)));
                }
            else
            {
    #endif /* PREDICTOR */
                    vds = model->FEFET1type
                        * (*(ckt->CKTrhsOld + here->FEFET1dNodePrime)
                        - *(ckt->CKTrhsOld + here->FEFET1sNodePrime));
                    vgs = model->FEFET1type
                        * (*(ckt->CKTrhsOld + here->FEFET1gNodePrime) 
                        - *(ckt->CKTrhsOld + here->FEFET1sNodePrime));
                    vbs = model->FEFET1type
                        * (*(ckt->CKTrhsOld + here->FEFET1bNodePrime)
                        - *(ckt->CKTrhsOld + here->FEFET1sNodePrime));
                    vges = model->FEFET1type
                            * (*(ckt->CKTrhsOld + here->FEFET1gNodeExt)
                            - *(ckt->CKTrhsOld + here->FEFET1sNodePrime));
                    vgms = model->FEFET1type
                            * (*(ckt->CKTrhsOld + here->FEFET1gNodeMid)
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
                    qdef = model->FEFET1type
                            * (*(ckt->CKTrhsOld + here->FEFET1qNode));
    #ifndef PREDICTOR
                }
    #endif /* PREDICTOR */

                vgdo = *(ckt->CKTstate0 + here->FEFET1vgs)
                        - *(ckt->CKTstate0 + here->FEFET1vds);
            vgedo = *(ckt->CKTstate0 + here->FEFET1vges)
                        - *(ckt->CKTstate0 + here->FEFET1vds);
            vgmdo = *(ckt->CKTstate0 + here->FEFET1vgms)
                        - *(ckt->CKTstate0 + here->FEFET1vds);

                vbd = vbs - vds;
                vdbd = vdbs - vds;
                vgd = vgs - vds;
            vged = vges - vds;
            vgmd = vgms - vds;

                delvbd = vbd - *(ckt->CKTstate0 + here->FEFET1vbd);
                delvdbd = vdbd - *(ckt->CKTstate0 + here->FEFET1vdbd);
                delvgd = vgd - vgdo;
            delvged = vged - vgedo;
            delvgmd = vgmd - vgmdo;

                delvds = vds - *(ckt->CKTstate0 + here->FEFET1vds);
                delvgs = vgs - *(ckt->CKTstate0 + here->FEFET1vgs);
            delvges = vges - *(ckt->CKTstate0 + here->FEFET1vges);
            delvgms = vgms - *(ckt->CKTstate0 + here->FEFET1vgms);
                delvbs = vbs - *(ckt->CKTstate0 + here->FEFET1vbs);
                delvdbs = vdbs - *(ckt->CKTstate0 + here->FEFET1vdbs);
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
                    Ibtot = here->FEFET1cbs + here->FEFET1cbd 
                - here->FEFET1Igidl - here->FEFET1Igisl - here->FEFET1csub;
                    cbhat = Ibtot + here->FEFET1gbd * delvbd_jct
                            + here->FEFET1gbs * delvbs_jct - (here->FEFET1gbbs + here->FEFET1ggidlb)
                            * delvbs - (here->FEFET1gbgs + here->FEFET1ggidlg) * delvgs
                - (here->FEFET1gbds + here->FEFET1ggidld - here->FEFET1ggisls) * delvds 
                - here->FEFET1ggislg * delvgd - here->FEFET1ggislb* delvbd;

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
                    Ibtot = here->FEFET1cbs + here->FEFET1cbd 
                - here->FEFET1Igidl - here->FEFET1Igisl - here->FEFET1csub;
                    cbhat = Ibtot + here->FEFET1gbs * delvbs_jct + here->FEFET1gbd 
                            * delvbd_jct - (here->FEFET1gbbs + here->FEFET1ggislb) * delvbd
                            - (here->FEFET1gbgs + here->FEFET1ggislg) * delvgd
                + (here->FEFET1gbds + here->FEFET1ggisld - here->FEFET1ggidls) * delvds
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


    #ifndef NOBYPASS
                /* Following should be one IF statement, but some C compilers 
                    * can't handle that all at once, so we split it into several
                    * successive IF's */

                if ((!(ckt->CKTmode & MODEINITPRED)) && (ckt->CKTbypass))
                if ((fabs(delvds) < (ckt->CKTreltol * MAX(fabs(vds),
                    fabs(*(ckt->CKTstate0 + here->FEFET1vds))) + ckt->CKTvoltTol)))
                if ((fabs(delvgs) < (ckt->CKTreltol * MAX(fabs(vgs),
                    fabs(*(ckt->CKTstate0 + here->FEFET1vgs))) + ckt->CKTvoltTol)))
                if ((fabs(delvbs) < (ckt->CKTreltol * MAX(fabs(vbs),
                    fabs(*(ckt->CKTstate0 + here->FEFET1vbs))) + ckt->CKTvoltTol)))
                if ((fabs(delvbd) < (ckt->CKTreltol * MAX(fabs(vbd),
                    fabs(*(ckt->CKTstate0 + here->FEFET1vbd))) + ckt->CKTvoltTol)))
            if ((here->FEFET1rgateMod == 0) || (here->FEFET1rgateMod == 1) 
                || (fabs(delvges) < (ckt->CKTreltol * MAX(fabs(vges),
            fabs(*(ckt->CKTstate0 + here->FEFET1vges))) + ckt->CKTvoltTol)))
            if ((here->FEFET1rgateMod != 3) || (fabs(delvgms) < (ckt->CKTreltol
                    * MAX(fabs(vgms), fabs(*(ckt->CKTstate0 + here->FEFET1vgms)))
                    + ckt->CKTvoltTol)))
                if ((!here->FEFET1rbodyMod) || (fabs(delvdbs) < (ckt->CKTreltol
                    * MAX(fabs(vdbs), fabs(*(ckt->CKTstate0 + here->FEFET1vdbs)))
                    + ckt->CKTvoltTol)))
                if ((!here->FEFET1rbodyMod) || (fabs(delvdbd) < (ckt->CKTreltol
                    * MAX(fabs(vdbd), fabs(*(ckt->CKTstate0 + here->FEFET1vdbd)))
                    + ckt->CKTvoltTol)))
                if ((!here->FEFET1rbodyMod) || (fabs(delvsbs) < (ckt->CKTreltol
                    * MAX(fabs(vsbs), fabs(*(ckt->CKTstate0 + here->FEFET1vsbs)))
                    + ckt->CKTvoltTol)))
                if ((!model->FEFET1rdsMod) || (fabs(delvses) < (ckt->CKTreltol
                    * MAX(fabs(vses), fabs(*(ckt->CKTstate0 + here->FEFET1vses)))
                    + ckt->CKTvoltTol)))
                if ((!model->FEFET1rdsMod) || (fabs(delvdes) < (ckt->CKTreltol
                    * MAX(fabs(vdes), fabs(*(ckt->CKTstate0 + here->FEFET1vdes)))
                    + ckt->CKTvoltTol)))
                if ((fabs(cdhat - Idtot) < ckt->CKTreltol
                    * MAX(fabs(cdhat), fabs(Idtot)) + ckt->CKTabstol))
                if ((fabs(cbhat - Ibtot) < ckt->CKTreltol
                    * MAX(fabs(cbhat), fabs(Ibtot)) + ckt->CKTabstol))
                if ((!model->FEFET1igcMod) || ((fabs(cgshat - Igstot) < ckt->CKTreltol
                    * MAX(fabs(cgshat), fabs(Igstot)) + ckt->CKTabstol)))
                if ((!model->FEFET1igcMod) || ((fabs(cgdhat - Igdtot) < ckt->CKTreltol
                    * MAX(fabs(cgdhat), fabs(Igdtot)) + ckt->CKTabstol)))
                if ((!model->FEFET1igbMod) || ((fabs(cgbhat - Igbtot) < ckt->CKTreltol
                    * MAX(fabs(cgbhat), fabs(Igbtot)) + ckt->CKTabstol)))
                if ((!model->FEFET1rdsMod) || ((fabs(cseshat - Isestot) < ckt->CKTreltol
                    * MAX(fabs(cseshat), fabs(Isestot)) + ckt->CKTabstol)))
                if ((!model->FEFET1rdsMod) || ((fabs(cdedhat - Idedtot) < ckt->CKTreltol
                    * MAX(fabs(cdedhat), fabs(Idedtot)) + ckt->CKTabstol)))
                {   vds = *(ckt->CKTstate0 + here->FEFET1vds);
                    vgs = *(ckt->CKTstate0 + here->FEFET1vgs);
                    vbs = *(ckt->CKTstate0 + here->FEFET1vbs);
            vges = *(ckt->CKTstate0 + here->FEFET1vges);
            vgms = *(ckt->CKTstate0 + here->FEFET1vgms);

                    vbd = *(ckt->CKTstate0 + here->FEFET1vbd);
                    vdbs = *(ckt->CKTstate0 + here->FEFET1vdbs);
                    vdbd = *(ckt->CKTstate0 + here->FEFET1vdbd);
                    vsbs = *(ckt->CKTstate0 + here->FEFET1vsbs);
                    vses = *(ckt->CKTstate0 + here->FEFET1vses);
                    vdes = *(ckt->CKTstate0 + here->FEFET1vdes);

                    vgd = vgs - vds;
                    vgb = vgs - vbs;
            vged = vges - vds;
            vgmd = vgms - vds;
            vgmb = vgms - vbs;

                    vbs_jct = (!here->FEFET1rbodyMod) ? vbs : vsbs;
                    vbd_jct = (!here->FEFET1rbodyMod) ? vbd : vdbd;

    /*** qdef should not be kept fixed even if vgs, vds & vbs has converged 
    ****               qdef = *(ckt->CKTstate0 + here->FEFET1qdef);  
    ***/
                    cdrain = here->FEFET1cd;

                    if ((ckt->CKTmode & (MODETRAN | MODEAC)) || 
                        ((ckt->CKTmode & MODETRANOP) && 
                        (ckt->CKTmode & MODEUIC)))
            {   ByPass = 1;

                        qgate = here->FEFET1qgate;
                        qbulk = here->FEFET1qbulk;
                        qdrn = here->FEFET1qdrn;
                cgdo = here->FEFET1cgdo;
                qgdo = here->FEFET1qgdo;
                        cgso = here->FEFET1cgso;
                        qgso = here->FEFET1qgso;

                goto line755;
                        }
                else
                goto line850;
                }
    #endif /*NOBYPASS*/

                von = here->FEFET1von;
                if (*(ckt->CKTstate0 + here->FEFET1vds) >= 0.0)
            {   vgs = DEVfetlim(vgs, *(ckt->CKTstate0 + here->FEFET1vgs), von);
                    vds = vgs - vgd;
                    vds = DEVlimvds(vds, *(ckt->CKTstate0 + here->FEFET1vds));
                    vgd = vgs - vds;
                    if (here->FEFET1rgateMod == 3)
                    {   vges = DEVfetlim(vges, *(ckt->CKTstate0 + here->FEFET1vges), von);
                        vgms = DEVfetlim(vgms, *(ckt->CKTstate0 + here->FEFET1vgms), von);
                        vged = vges - vds;
                        vgmd = vgms - vds;
                    }
                    else if ((here->FEFET1rgateMod == 1) || (here->FEFET1rgateMod == 2))
                    {   vges = DEVfetlim(vges, *(ckt->CKTstate0 + here->FEFET1vges), von);
                        vged = vges - vds;
                    }

                    if (model->FEFET1rdsMod)
            {   vdes = DEVlimvds(vdes, *(ckt->CKTstate0 + here->FEFET1vdes));
                vses = -DEVlimvds(-vses, -(*(ckt->CKTstate0 + here->FEFET1vses)));
            }

                }
            else
            {   vgd = DEVfetlim(vgd, vgdo, von);
                    vds = vgs - vgd;
                    vds = -DEVlimvds(-vds, -(*(ckt->CKTstate0 + here->FEFET1vds)));
                    vgs = vgd + vds;

            if (here->FEFET1rgateMod == 3)
                    {   vged = DEVfetlim(vged, vgedo, von);
                        vges = vged + vds;
                        vgmd = DEVfetlim(vgmd, vgmdo, von);
                        vgms = vgmd + vds;
                    }
                    if ((here->FEFET1rgateMod == 1) || (here->FEFET1rgateMod == 2))
                    {   vged = DEVfetlim(vged, vgedo, von);
                        vges = vged + vds;
                    }

                    if (model->FEFET1rdsMod)
                    {   vdes = -DEVlimvds(-vdes, -(*(ckt->CKTstate0 + here->FEFET1vdes)));
                        vses = DEVlimvds(vses, *(ckt->CKTstate0 + here->FEFET1vses));
                    }
                }

                if (vds >= 0.0)
            {   vbs = DEVpnjlim(vbs, *(ckt->CKTstate0 + here->FEFET1vbs),
                                    CONSTvt0, model->FEFET1vcrit, &Check);
                    vbd = vbs - vds;
                    if (here->FEFET1rbodyMod)
                    {   vdbs = DEVpnjlim(vdbs, *(ckt->CKTstate0 + here->FEFET1vdbs),
                                            CONSTvt0, model->FEFET1vcrit, &Check1);
                        vdbd = vdbs - vds;
                        vsbs = DEVpnjlim(vsbs, *(ckt->CKTstate0 + here->FEFET1vsbs),
                                            CONSTvt0, model->FEFET1vcrit, &Check2);
                        if ((Check1 == 0) && (Check2 == 0))
                            Check = 0;
                        else 
                            Check = 1;
                    }
                }
            else
                {   vbd = DEVpnjlim(vbd, *(ckt->CKTstate0 + here->FEFET1vbd),
                                    CONSTvt0, model->FEFET1vcrit, &Check); 
                    vbs = vbd + vds;
                    if (here->FEFET1rbodyMod)
                    {   vdbd = DEVpnjlim(vdbd, *(ckt->CKTstate0 + here->FEFET1vdbd),
                                            CONSTvt0, model->FEFET1vcrit, &Check1);
                        vdbs = vdbd + vds;
                        vsbdo = *(ckt->CKTstate0 + here->FEFET1vsbs)
                                - *(ckt->CKTstate0 + here->FEFET1vds);
                        vsbd = vsbs - vds;
                        vsbd = DEVpnjlim(vsbd, vsbdo, CONSTvt0, model->FEFET1vcrit, &Check2);
                        vsbs = vsbd + vds;
                        if ((Check1 == 0) && (Check2 == 0))
                            Check = 0;
                        else
                            Check = 1;
                    }
                }
            }

            /* Calculate DC currents and their derivatives */
            vbd = vbs - vds;
            vgd = vgs - vds;
            vgb = vgs - vbs;
        vged = vges - vds;
        vgmd = vgms - vds;
        vgmb = vgms - vbs;
            vdbd = vdbs - vds;

            vbs_jct = (!here->FEFET1rbodyMod) ? vbs : vsbs;
            vbd_jct = (!here->FEFET1rbodyMod) ? vbd : vdbd;

            /* Source/drain junction diode DC model begins */
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

        if (SourceSatCurrent <= 0.0)
        {   here->FEFET1gbs = ckt->CKTgmin;
                here->FEFET1cbs = here->FEFET1gbs * vbs_jct;
            }
            else
        {   switch(model->FEFET1dioMod)
                {   case 0:
                        evbs = exp(vbs_jct / Nvtms);
                        T1 = model->FEFET1xjbvs * exp(-(model->FEFET1bvs + vbs_jct) / Nvtms);
                /* WDLiu: Magic T1 in this form; different from FEFET1 beta. */
                here->FEFET1gbs = SourceSatCurrent * (evbs + T1) / Nvtms + ckt->CKTgmin;
                here->FEFET1cbs = SourceSatCurrent * (evbs + here->FEFET1XExpBVS
                        - T1 - 1.0) + ckt->CKTgmin * vbs_jct;
                break;
                    case 1:
                T2 = vbs_jct / Nvtms;
                    if (T2 < -EXP_THRESHOLD)
                {   here->FEFET1gbs = ckt->CKTgmin;
                            here->FEFET1cbs = SourceSatCurrent * (MIN_EXP - 1.0)
                                            + ckt->CKTgmin * vbs_jct;
                        }
                else if (vbs_jct <= here->FEFET1vjsmFwd)
                {   evbs = exp(T2);
                here->FEFET1gbs = SourceSatCurrent * evbs / Nvtms + ckt->CKTgmin;
                            here->FEFET1cbs = SourceSatCurrent * (evbs - 1.0)
                                            + ckt->CKTgmin * vbs_jct;
                    }
                else
                {   T0 = here->FEFET1IVjsmFwd / Nvtms;
                            here->FEFET1gbs = T0 + ckt->CKTgmin;
                            here->FEFET1cbs = here->FEFET1IVjsmFwd - SourceSatCurrent + T0 
                        * (vbs_jct - here->FEFET1vjsmFwd) + ckt->CKTgmin * vbs_jct;
                }	
                        break;
                    case 2:
                        if (vbs_jct < here->FEFET1vjsmRev)
                        {   T0 = vbs_jct / Nvtms;
                            if (T0 < -EXP_THRESHOLD)
                            {    evbs = MIN_EXP;
                    devbs_dvb = 0.0;
                }
                            else
                {    evbs = exp(T0);
                                devbs_dvb = evbs / Nvtms;
                }

                T1 = evbs - 1.0;
                T2 = here->FEFET1IVjsmRev + here->FEFET1SslpRev
                    * (vbs_jct - here->FEFET1vjsmRev);
                here->FEFET1gbs = devbs_dvb * T2 + T1 * here->FEFET1SslpRev + ckt->CKTgmin;
                            here->FEFET1cbs = T1 * T2 + ckt->CKTgmin * vbs_jct;
                        }         
                        else if (vbs_jct <= here->FEFET1vjsmFwd)
                        {   T0 = vbs_jct / Nvtms;
                            if (T0 < -EXP_THRESHOLD)
                            {    evbs = MIN_EXP;
                                devbs_dvb = 0.0;
                            }
                            else
                            {    evbs = exp(T0);
                                devbs_dvb = evbs / Nvtms;
                            }

                T1 = (model->FEFET1bvs + vbs_jct) / Nvtms;
                            if (T1 > EXP_THRESHOLD)
                            {   T2 = MIN_EXP;
                    T3 = 0.0;
                }
                            else
                {   T2 = exp(-T1);
                    T3 = -T2 /Nvtms;
                }
                            here->FEFET1gbs = SourceSatCurrent * (devbs_dvb - model->FEFET1xjbvs * T3)
                        + ckt->CKTgmin;
                here->FEFET1cbs = SourceSatCurrent * (evbs + here->FEFET1XExpBVS - 1.0
                            - model->FEFET1xjbvs * T2) + ckt->CKTgmin * vbs_jct;
                        }
                else
                {   here->FEFET1gbs = here->FEFET1SslpFwd + ckt->CKTgmin;
                            here->FEFET1cbs = here->FEFET1IVjsmFwd + here->FEFET1SslpFwd * (vbs_jct
                        - here->FEFET1vjsmFwd) + ckt->CKTgmin * vbs_jct;
                }
                        break;
                    default: break;
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

        if (DrainSatCurrent <= 0.0)
        {   here->FEFET1gbd = ckt->CKTgmin;
                here->FEFET1cbd = here->FEFET1gbd * vbd_jct;
            }
            else
            {   switch(model->FEFET1dioMod)
                {   case 0:
                        evbd = exp(vbd_jct / Nvtmd);
                        T1 = model->FEFET1xjbvd * exp(-(model->FEFET1bvd + vbd_jct) / Nvtmd);
                        /* WDLiu: Magic T1 in this form; different from FEFET1 beta. */
                        here->FEFET1gbd = DrainSatCurrent * (evbd + T1) / Nvtmd + ckt->CKTgmin;
                        here->FEFET1cbd = DrainSatCurrent * (evbd + here->FEFET1XExpBVD
                                        - T1 - 1.0) + ckt->CKTgmin * vbd_jct;
                        break;
                    case 1:
                T2 = vbd_jct / Nvtmd;
                        if (T2 < -EXP_THRESHOLD)
                        {   here->FEFET1gbd = ckt->CKTgmin;
                            here->FEFET1cbd = DrainSatCurrent * (MIN_EXP - 1.0)
                                            + ckt->CKTgmin * vbd_jct;
                        }
                        else if (vbd_jct <= here->FEFET1vjdmFwd)
                        {   evbd = exp(T2);
                            here->FEFET1gbd = DrainSatCurrent * evbd / Nvtmd + ckt->CKTgmin;
                            here->FEFET1cbd = DrainSatCurrent * (evbd - 1.0)
                                            + ckt->CKTgmin * vbd_jct;
                        }
                        else
                        {   T0 = here->FEFET1IVjdmFwd / Nvtmd;
                            here->FEFET1gbd = T0 + ckt->CKTgmin;
                            here->FEFET1cbd = here->FEFET1IVjdmFwd - DrainSatCurrent + T0
                                            * (vbd_jct - here->FEFET1vjdmFwd) + ckt->CKTgmin * vbd_jct;
                        }
                        break;
                    case 2:
                        if (vbd_jct < here->FEFET1vjdmRev)
                        {   T0 = vbd_jct / Nvtmd;
                            if (T0 < -EXP_THRESHOLD)
                            {    evbd = MIN_EXP;
                                devbd_dvb = 0.0;
                            }
                            else
                            {    evbd = exp(T0);
                                devbd_dvb = evbd / Nvtmd;
                            }

                            T1 = evbd - 1.0;
                            T2 = here->FEFET1IVjdmRev + here->FEFET1DslpRev
                                * (vbd_jct - here->FEFET1vjdmRev);
                            here->FEFET1gbd = devbd_dvb * T2 + T1 * here->FEFET1DslpRev + ckt->CKTgmin;
                            here->FEFET1cbd = T1 * T2 + ckt->CKTgmin * vbd_jct;
                        }
                        else if (vbd_jct <= here->FEFET1vjdmFwd)
                        {   T0 = vbd_jct / Nvtmd;
                            if (T0 < -EXP_THRESHOLD)
                            {    evbd = MIN_EXP;
                                devbd_dvb = 0.0;
                            }
                            else
                            {    evbd = exp(T0);
                                devbd_dvb = evbd / Nvtmd;
                            }

                            T1 = (model->FEFET1bvd + vbd_jct) / Nvtmd;
                            if (T1 > EXP_THRESHOLD)
                            {   T2 = MIN_EXP;
                                T3 = 0.0;
                            }
                            else
                            {   T2 = exp(-T1);
                                T3 = -T2 /Nvtmd;
                            }     
                            here->FEFET1gbd = DrainSatCurrent * (devbd_dvb - model->FEFET1xjbvd * T3)
                                            + ckt->CKTgmin;
                            here->FEFET1cbd = DrainSatCurrent * (evbd + here->FEFET1XExpBVD - 1.0
                                            - model->FEFET1xjbvd * T2) + ckt->CKTgmin * vbd_jct;
                        }
                        else
                        {   here->FEFET1gbd = here->FEFET1DslpFwd + ckt->CKTgmin;
                            here->FEFET1cbd = here->FEFET1IVjdmFwd + here->FEFET1DslpFwd * (vbd_jct
                                            - here->FEFET1vjdmFwd) + ckt->CKTgmin * vbd_jct;
                        }
                        break;
                    default: break;
                }
            } 

        /* trap-assisted tunneling and recombination current for reverse bias  */
            Nvtmrssw = model->FEFET1vtm0 * model->FEFET1njtsswtemp;
            Nvtmrsswg = model->FEFET1vtm0 * model->FEFET1njtsswgtemp;
            Nvtmrs = model->FEFET1vtm0 * model->FEFET1njtstemp;

            if ((model->FEFET1vtss - vbs_jct) < (model->FEFET1vtss * 1e-3))
            { T9 = 1.0e3; 
            T0 = - vbs_jct / Nvtmrs * T9;
            DEXP(T0, T1, T10);
            dT1_dVb = T10 / Nvtmrs * T9; 
            } else {
            T9 = 1.0 / (model->FEFET1vtss - vbs_jct);
            T0 = -vbs_jct / Nvtmrs * model->FEFET1vtss * T9;
            dT0_dVb = model->FEFET1vtss / Nvtmrs * (T9 + vbs_jct * T9 * T9) ;
            DEXP(T0, T1, T10);
            dT1_dVb = T10 * dT0_dVb;
            }

        if ((model->FEFET1vtsd - vbd_jct) < (model->FEFET1vtsd * 1e-3) )
            { T9 = 1.0e3;
            T0 = -vbd_jct / Nvtmrs * T9;
            DEXP(T0, T2, T10);
            dT2_dVb = T10 / Nvtmrs * T9; 
            } else {
            T9 = 1.0 / (model->FEFET1vtsd - vbd_jct);
            T0 = -vbd_jct / Nvtmrs * model->FEFET1vtsd * T9;
            dT0_dVb = model->FEFET1vtsd / Nvtmrs * (T9 + vbd_jct * T9 * T9) ;
            DEXP(T0, T2, T10);
            dT2_dVb = T10 * dT0_dVb;
            }

            if ((model->FEFET1vtssws - vbs_jct) < (model->FEFET1vtssws * 1e-3) )
            { T9 = 1.0e3; 
            T0 = -vbs_jct / Nvtmrssw * T9;
            DEXP(T0, T3, T10);
            dT3_dVb = T10 / Nvtmrssw * T9; 
            } else {
            T9 = 1.0 / (model->FEFET1vtssws - vbs_jct);
            T0 = -vbs_jct / Nvtmrssw * model->FEFET1vtssws * T9;
            dT0_dVb = model->FEFET1vtssws / Nvtmrssw * (T9 + vbs_jct * T9 * T9) ;
            DEXP(T0, T3, T10);
            dT3_dVb = T10 * dT0_dVb;
            }

            if ((model->FEFET1vtsswd - vbd_jct) < (model->FEFET1vtsswd * 1e-3) )
            { T9 = 1.0e3; 
            T0 = -vbd_jct / Nvtmrssw * T9;
            DEXP(T0, T4, T10);
            dT4_dVb = T10 / Nvtmrssw * T9; 
            } else {
            T9 = 1.0 / (model->FEFET1vtsswd - vbd_jct);
            T0 = -vbd_jct / Nvtmrssw * model->FEFET1vtsswd * T9;
            dT0_dVb = model->FEFET1vtsswd / Nvtmrssw * (T9 + vbd_jct * T9 * T9) ;
            DEXP(T0, T4, T10);
            dT4_dVb = T10 * dT0_dVb;
            }

            if ((model->FEFET1vtsswgs - vbs_jct) < (model->FEFET1vtsswgs * 1e-3) )
            { T9 = 1.0e3; 
            T0 = -vbs_jct / Nvtmrsswg * T9;
            DEXP(T0, T5, T10);
            dT5_dVb = T10 / Nvtmrsswg * T9; 
            } else {
            T9 = 1.0 / (model->FEFET1vtsswgs - vbs_jct);
            T0 = -vbs_jct / Nvtmrsswg * model->FEFET1vtsswgs * T9;
            dT0_dVb = model->FEFET1vtsswgs / Nvtmrsswg * (T9 + vbs_jct * T9 * T9) ;
            DEXP(T0, T5, T10);
            dT5_dVb = T10 * dT0_dVb;
            }

            if ((model->FEFET1vtsswgd - vbd_jct) < (model->FEFET1vtsswgd * 1e-3) )
            { T9 = 1.0e3; 
            T0 = -vbd_jct / Nvtmrsswg * T9;
            DEXP(T0, T6, T10);
            dT6_dVb = T10 / Nvtmrsswg * T9; 
            } else {
            T9 = 1.0 / (model->FEFET1vtsswgd - vbd_jct);
            T0 = -vbd_jct / Nvtmrsswg * model->FEFET1vtsswgd * T9;
            dT0_dVb = model->FEFET1vtsswgd / Nvtmrsswg * (T9 + vbd_jct * T9 * T9) ;
            DEXP(T0, T6, T10);
            dT6_dVb = T10 * dT0_dVb;
            }

        here->FEFET1gbs += here->FEFET1SjctTempRevSatCur * dT1_dVb
                    + here->FEFET1SswTempRevSatCur * dT3_dVb
                    + here->FEFET1SswgTempRevSatCur * dT5_dVb; 
        here->FEFET1cbs -= here->FEFET1SjctTempRevSatCur * (T1 - 1.0)
                    + here->FEFET1SswTempRevSatCur * (T3 - 1.0)
                    + here->FEFET1SswgTempRevSatCur * (T5 - 1.0); 
        here->FEFET1gbd += here->FEFET1DjctTempRevSatCur * dT2_dVb
                    + here->FEFET1DswTempRevSatCur * dT4_dVb
                    + here->FEFET1DswgTempRevSatCur * dT6_dVb; 
        here->FEFET1cbd -= here->FEFET1DjctTempRevSatCur * (T2 - 1.0) 
                    + here->FEFET1DswTempRevSatCur * (T4 - 1.0)
                    + here->FEFET1DswgTempRevSatCur * (T6 - 1.0); 

            /* End of diode DC model */

            if (vds >= 0.0)
        {   here->FEFET1mode = 1;
                Vds = vds;
                Vgs = vgs;
                Vbs = vbs;
            Vdb = vds - vbs;  /* WDLiu: for GIDL */
            }
        else
        {   here->FEFET1mode = -1;
                Vds = -vds;
                Vgs = vgd;
                Vbs = vbd;
            Vdb = -vbs;
            }

        T0 = Vbs - here->FEFET1vbsc - 0.001;
        T1 = sqrt(T0 * T0 - 0.004 * here->FEFET1vbsc);
        if (T0 >= 0.0)
        {   Vbseff = here->FEFET1vbsc + 0.5 * (T0 + T1);
                dVbseff_dVb = 0.5 * (1.0 + T0 / T1);
        }
        else
        {   T2 = -0.002 / (T1 - T0);
            Vbseff = here->FEFET1vbsc * (1.0 + T2);
            dVbseff_dVb = T2 * here->FEFET1vbsc / T1;
        }

        /* JX: Correction to forward body bias  */
        T9 = 0.95 * pParam->FEFET1phi;
        T0 = T9 - Vbseff - 0.001;
        T1 = sqrt(T0 * T0 + 0.004 * T9);
        Vbseff = T9 - 0.5 * (T0 + T1);
            dVbseff_dVb *= 0.5 * (1.0 + T0 / T1);

            Phis = pParam->FEFET1phi - Vbseff;
            dPhis_dVb = -1.0;
            sqrtPhis = sqrt(Phis);
            dsqrtPhis_dVb = -0.5 / sqrtPhis; 

            Xdep = pParam->FEFET1Xdep0 * sqrtPhis / pParam->FEFET1sqrtPhi;
            dXdep_dVb = (pParam->FEFET1Xdep0 / pParam->FEFET1sqrtPhi)
                * dsqrtPhis_dVb;

            Leff = pParam->FEFET1leff;
            Vtm = model->FEFET1vtm;
            Vtm0 = model->FEFET1vtm0;

            /* Vth Calculation */
            T3 = sqrt(Xdep);
            V0 = pParam->FEFET1vbi - pParam->FEFET1phi;

            T0 = pParam->FEFET1dvt2 * Vbseff;
            if (T0 >= - 0.5)
        {   T1 = 1.0 + T0;
            T2 = pParam->FEFET1dvt2;
        }
        else
        {   T4 = 1.0 / (3.0 + 8.0 * T0);
            T1 = (1.0 + 3.0 * T0) * T4; 
            T2 = pParam->FEFET1dvt2 * T4 * T4;
        }
            lt1 = model->FEFET1factor1 * T3 * T1;
            dlt1_dVb = model->FEFET1factor1 * (0.5 / T3 * T1 * dXdep_dVb + T3 * T2);

            T0 = pParam->FEFET1dvt2w * Vbseff;
            if (T0 >= - 0.5)
        {   T1 = 1.0 + T0;
            T2 = pParam->FEFET1dvt2w;
        }
        else
        {   T4 = 1.0 / (3.0 + 8.0 * T0);
            T1 = (1.0 + 3.0 * T0) * T4; 
            T2 = pParam->FEFET1dvt2w * T4 * T4;
        }
            ltw = model->FEFET1factor1 * T3 * T1;
            dltw_dVb = model->FEFET1factor1 * (0.5 / T3 * T1 * dXdep_dVb + T3 * T2);

            T0 = pParam->FEFET1dvt1 * Leff / lt1;
            if (T0 < EXP_THRESHOLD)
            {   T1 = exp(T0);
                T2 = T1 - 1.0;
                T3 = T2 * T2;
                T4 = T3 + 2.0 * T1 * MIN_EXP;
                Theta0 = T1 / T4;
                dT1_dVb = -T0 * T1 * dlt1_dVb / lt1;
                dTheta0_dVb = dT1_dVb * (T4 - 2.0 * T1 * (T2 + MIN_EXP)) / T4 / T4;
            }
            else
            {   Theta0 = 1.0 / (MAX_EXP - 2.0); /* 3.0 * MIN_EXP omitted */
                dTheta0_dVb = 0.0;
            }
            here->FEFET1thetavth = pParam->FEFET1dvt0 * Theta0;
            Delt_vth = here->FEFET1thetavth * V0;
            dDelt_vth_dVb = pParam->FEFET1dvt0 * dTheta0_dVb * V0;

            T0 = pParam->FEFET1dvt1w * pParam->FEFET1weff * Leff / ltw;
            if (T0 < EXP_THRESHOLD)
            {   T1 = exp(T0);
                T2 = T1 - 1.0;
                T3 = T2 * T2;
                T4 = T3 + 2.0 * T1 * MIN_EXP;
                T5 = T1 / T4;
                dT1_dVb = -T0 * T1 * dltw_dVb / ltw; 
                dT5_dVb = dT1_dVb * (T4 - 2.0 * T1 * (T2 + MIN_EXP)) / T4 / T4;
            }
            else
            {   T5 = 1.0 / (MAX_EXP - 2.0); /* 3.0 * MIN_EXP omitted */
                dT5_dVb = 0.0;
            }
            T0 = pParam->FEFET1dvt0w * T5;
            T2 = T0 * V0;
            dT2_dVb = pParam->FEFET1dvt0w * dT5_dVb * V0;

            TempRatio =  ckt->CKTtemp / model->FEFET1tnom - 1.0;
            T0 = sqrt(1.0 + pParam->FEFET1lpe0 / Leff);
            T1 = pParam->FEFET1k1ox * (T0 - 1.0) * pParam->FEFET1sqrtPhi
                + (pParam->FEFET1kt1 + pParam->FEFET1kt1l / Leff
                + pParam->FEFET1kt2 * Vbseff) * TempRatio;
            Vth_NarrowW = model->FEFET1toxe * pParam->FEFET1phi
                    / (pParam->FEFET1weff + pParam->FEFET1w0);

        T3 = here->FEFET1eta0 + pParam->FEFET1etab * Vbseff;
        if (T3 < 1.0e-4)
        {   T9 = 1.0 / (3.0 - 2.0e4 * T3);
            T3 = (2.0e-4 - T3) * T9;
            T4 = T9 * T9;
        }
        else
        {   T4 = 1.0;
        }
        dDIBL_Sft_dVd = T3 * pParam->FEFET1theta0vb0;
            DIBL_Sft = dDIBL_Sft_dVd * Vds;

        Lpe_Vb = sqrt(1.0 + pParam->FEFET1lpeb / Leff);

            Vth = model->FEFET1type * here->FEFET1vth0 + (pParam->FEFET1k1ox * sqrtPhis
            - pParam->FEFET1k1 * pParam->FEFET1sqrtPhi) * Lpe_Vb
                - here->FEFET1k2ox * Vbseff - Delt_vth - T2 + (pParam->FEFET1k3
                + pParam->FEFET1k3b * Vbseff) * Vth_NarrowW + T1 - DIBL_Sft;

            dVth_dVb = Lpe_Vb * pParam->FEFET1k1ox * dsqrtPhis_dVb - here->FEFET1k2ox
                    - dDelt_vth_dVb - dT2_dVb + pParam->FEFET1k3b * Vth_NarrowW
                    - pParam->FEFET1etab * Vds * pParam->FEFET1theta0vb0 * T4
                    + pParam->FEFET1kt2 * TempRatio;
            dVth_dVd = -dDIBL_Sft_dVd;


            /* Calculate n */
            tmp1 = EPSSI / Xdep;
        here->FEFET1nstar = model->FEFET1vtm / Charge_q * (model->FEFET1coxe
                + tmp1 + pParam->FEFET1cit);  
            tmp2 = pParam->FEFET1nfactor * tmp1;
            tmp3 = pParam->FEFET1cdsc + pParam->FEFET1cdscb * Vbseff
                + pParam->FEFET1cdscd * Vds;
        tmp4 = (tmp2 + tmp3 * Theta0 + pParam->FEFET1cit) / model->FEFET1coxe;
        if (tmp4 >= -0.5)
        {   n = 1.0 + tmp4;
            dn_dVb = (-tmp2 / Xdep * dXdep_dVb + tmp3 * dTheta0_dVb
                        + pParam->FEFET1cdscb * Theta0) / model->FEFET1coxe;
                dn_dVd = pParam->FEFET1cdscd * Theta0 / model->FEFET1coxe;
        }
        else
        {   T0 = 1.0 / (3.0 + 8.0 * tmp4);
            n = (1.0 + 3.0 * tmp4) * T0;
            T0 *= T0;
            dn_dVb = (-tmp2 / Xdep * dXdep_dVb + tmp3 * dTheta0_dVb
                        + pParam->FEFET1cdscb * Theta0) / model->FEFET1coxe * T0;
                dn_dVd = pParam->FEFET1cdscd * Theta0 / model->FEFET1coxe * T0;
        }


            /* Vth correction for Pocket implant */
        if (pParam->FEFET1dvtp0 > 0.0)
            {   T0 = -pParam->FEFET1dvtp1 * Vds;
                if (T0 < -EXP_THRESHOLD)
                {   T2 = MIN_EXP;
                    dT2_dVd = 0.0;
                }
                else
                {   T2 = exp(T0);
                    dT2_dVd = -pParam->FEFET1dvtp1 * T2;
                }

                T3 = Leff + pParam->FEFET1dvtp0 * (1.0 + T2);
                dT3_dVd = pParam->FEFET1dvtp0 * dT2_dVd;
                if (model->FEFET1tempMod < 2)
                {
                    T4 = Vtm * log(Leff / T3);
                    dT4_dVd = -Vtm * dT3_dVd / T3;
                }
                else
                {
                    T4 = model->FEFET1vtm0 * log(Leff / T3);
                    dT4_dVd = -model->FEFET1vtm0 * dT3_dVd / T3;
                }
                dDITS_Sft_dVd = dn_dVd * T4 + n * dT4_dVd;
                dDITS_Sft_dVb = T4 * dn_dVb;

                Vth -= n * T4;
                dVth_dVd -= dDITS_Sft_dVd;
                dVth_dVb -= dDITS_Sft_dVb;
        }
            here->FEFET1von = Vth;


            /* Poly Gate Si Depletion Effect */
        T0 = here->FEFET1vfb + pParam->FEFET1phi;

            FEFET1polyDepletion(T0, pParam->FEFET1ngate, model->FEFET1coxe, vgs, &vgs_eff, &dvgs_eff_dvg);

            FEFET1polyDepletion(T0, pParam->FEFET1ngate, model->FEFET1coxe, vgd, &vgd_eff, &dvgd_eff_dvg);
            
            if(here->FEFET1mode>0) {
                Vgs_eff = vgs_eff;
                dVgs_eff_dVg = dvgs_eff_dvg;
            } else {
                Vgs_eff = vgd_eff;
                dVgs_eff_dVg = dvgd_eff_dvg;
            }
            here->FEFET1vgs_eff = vgs_eff;
            here->FEFET1vgd_eff = vgd_eff;
            here->FEFET1dvgs_eff_dvg = dvgs_eff_dvg;
            here->FEFET1dvgd_eff_dvg = dvgd_eff_dvg;


            Vgst = Vgs_eff - Vth;

        /* Calculate Vgsteff */
        T0 = n * Vtm;
        T1 = pParam->FEFET1mstar * Vgst;
        T2 = T1 / T0;
        if (T2 > EXP_THRESHOLD)
        {   T10 = T1;
            dT10_dVg = pParam->FEFET1mstar * dVgs_eff_dVg;
                dT10_dVd = -dVth_dVd * pParam->FEFET1mstar;
                dT10_dVb = -dVth_dVb * pParam->FEFET1mstar;
        }
        else if (T2 < -EXP_THRESHOLD)
        {   T10 = Vtm * log(1.0 + MIN_EXP);
                dT10_dVg = 0.0;
                dT10_dVd = T10 * dn_dVd;
                dT10_dVb = T10 * dn_dVb;
            T10 *= n;
        }
        else
        {   ExpVgst = exp(T2);
            T3 = Vtm * log(1.0 + ExpVgst);
                T10 = n * T3;
                dT10_dVg = pParam->FEFET1mstar * ExpVgst / (1.0 + ExpVgst);
                dT10_dVb = T3 * dn_dVb - dT10_dVg * (dVth_dVb + Vgst * dn_dVb / n);
                dT10_dVd = T3 * dn_dVd - dT10_dVg * (dVth_dVd + Vgst * dn_dVd / n);
            dT10_dVg *= dVgs_eff_dVg;
        }

        T1 = pParam->FEFET1voffcbn - (1.0 - pParam->FEFET1mstar) * Vgst;
        T2 = T1 / T0;
            if (T2 < -EXP_THRESHOLD)
            {   T3 = model->FEFET1coxe * MIN_EXP / pParam->FEFET1cdep0;
            T9 = pParam->FEFET1mstar + T3 * n;
                dT9_dVg = 0.0;
                dT9_dVd = dn_dVd * T3;
                dT9_dVb = dn_dVb * T3;
            }
            else if (T2 > EXP_THRESHOLD)
            {   T3 = model->FEFET1coxe * MAX_EXP / pParam->FEFET1cdep0;
                T9 = pParam->FEFET1mstar + T3 * n;
                dT9_dVg = 0.0;
                dT9_dVd = dn_dVd * T3;
                dT9_dVb = dn_dVb * T3;
            }
            else
            {   ExpVgst = exp(T2);
            T3 = model->FEFET1coxe / pParam->FEFET1cdep0;
            T4 = T3 * ExpVgst;
            T5 = T1 * T4 / T0;
                T9 = pParam->FEFET1mstar + n * T4;
                dT9_dVg = T3 * (pParam->FEFET1mstar - 1.0) * ExpVgst / Vtm;
                dT9_dVb = T4 * dn_dVb - dT9_dVg * dVth_dVb - T5 * dn_dVb;
                dT9_dVd = T4 * dn_dVd - dT9_dVg * dVth_dVd - T5 * dn_dVd;
                dT9_dVg *= dVgs_eff_dVg;
            }

            here->FEFET1Vgsteff = Vgsteff = T10 / T9;
        T11 = T9 * T9;
            dVgsteff_dVg = (T9 * dT10_dVg - T10 * dT9_dVg) / T11;
            dVgsteff_dVd = (T9 * dT10_dVd - T10 * dT9_dVd) / T11;
            dVgsteff_dVb = (T9 * dT10_dVb - T10 * dT9_dVb) / T11;

            /* Calculate Effective Channel Geometry */
            T9 = sqrtPhis - pParam->FEFET1sqrtPhi;
            Weff = pParam->FEFET1weff - 2.0 * (pParam->FEFET1dwg * Vgsteff 
                + pParam->FEFET1dwb * T9); 
            dWeff_dVg = -2.0 * pParam->FEFET1dwg;
            dWeff_dVb = -2.0 * pParam->FEFET1dwb * dsqrtPhis_dVb;

            if (Weff < 2.0e-8) /* to avoid the discontinuity problem due to Weff*/
        {   T0 = 1.0 / (6.0e-8 - 2.0 * Weff);
            Weff = 2.0e-8 * (4.0e-8 - Weff) * T0;
            T0 *= T0 * 4.0e-16;
                dWeff_dVg *= T0;
            dWeff_dVb *= T0;
            }

        if (model->FEFET1rdsMod == 1)
            Rds = dRds_dVg = dRds_dVb = 0.0;
            else
            {   T0 = 1.0 + pParam->FEFET1prwg * Vgsteff;
            dT0_dVg = -pParam->FEFET1prwg / T0 / T0;
            T1 = pParam->FEFET1prwb * T9;
            dT1_dVb = pParam->FEFET1prwb * dsqrtPhis_dVb;

            T2 = 1.0 / T0 + T1;
            T3 = T2 + sqrt(T2 * T2 + 0.01); /* 0.01 = 4.0 * 0.05 * 0.05 */
            dT3_dVg = 1.0 + T2 / (T3 - T2);
            dT3_dVb = dT3_dVg * dT1_dVb;
            dT3_dVg *= dT0_dVg;

            T4 = pParam->FEFET1rds0 * 0.5;
            Rds = pParam->FEFET1rdswmin + T3 * T4;
                dRds_dVg = T4 * dT3_dVg;
                dRds_dVb = T4 * dT3_dVb;

            if (Rds > 0.0)
            here->FEFET1grdsw = 1.0 / Rds; 
            else
                    here->FEFET1grdsw = 0.0;
            }
        
            /* Calculate Abulk */
        T9 = 0.5 * pParam->FEFET1k1ox * Lpe_Vb / sqrtPhis;
            T1 = T9 + here->FEFET1k2ox - pParam->FEFET1k3b * Vth_NarrowW;
            dT1_dVb = -T9 / sqrtPhis * dsqrtPhis_dVb;

            T9 = sqrt(pParam->FEFET1xj * Xdep);
            tmp1 = Leff + 2.0 * T9;
            T5 = Leff / tmp1; 
            tmp2 = pParam->FEFET1a0 * T5;
            tmp3 = pParam->FEFET1weff + pParam->FEFET1b1; 
            tmp4 = pParam->FEFET1b0 / tmp3;
            T2 = tmp2 + tmp4;
            dT2_dVb = -T9 / tmp1 / Xdep * dXdep_dVb;
            T6 = T5 * T5;
            T7 = T5 * T6;

            Abulk0 = 1.0 + T1 * T2; 
            dAbulk0_dVb = T1 * tmp2 * dT2_dVb + T2 * dT1_dVb;

            T8 = pParam->FEFET1ags * pParam->FEFET1a0 * T7;
            dAbulk_dVg = -T1 * T8;
            Abulk = Abulk0 + dAbulk_dVg * Vgsteff; 
            dAbulk_dVb = dAbulk0_dVb - T8 * Vgsteff * (dT1_dVb
                + 3.0 * T1 * dT2_dVb);

            if (Abulk0 < 0.1) /* added to avoid the problems caused by Abulk0 */
        {   T9 = 1.0 / (3.0 - 20.0 * Abulk0);
            Abulk0 = (0.2 - Abulk0) * T9;
            dAbulk0_dVb *= T9 * T9;
        }

            if (Abulk < 0.1)
        {   T9 = 1.0 / (3.0 - 20.0 * Abulk);
            Abulk = (0.2 - Abulk) * T9;
                T10 = T9 * T9;
            dAbulk_dVb *= T10;
                dAbulk_dVg *= T10;
        }
        here->FEFET1Abulk = Abulk;

            T2 = pParam->FEFET1keta * Vbseff;
        if (T2 >= -0.9)
        {   T0 = 1.0 / (1.0 + T2);
                dT0_dVb = -pParam->FEFET1keta * T0 * T0;
        }
        else
        {   T1 = 1.0 / (0.8 + T2);
            T0 = (17.0 + 20.0 * T2) * T1;
                dT0_dVb = -pParam->FEFET1keta * T1 * T1;
        }
        dAbulk_dVg *= T0;
        dAbulk_dVb = dAbulk_dVb * T0 + Abulk * dT0_dVb;
        dAbulk0_dVb = dAbulk0_dVb * T0 + Abulk0 * dT0_dVb;
        Abulk *= T0;
        Abulk0 *= T0;

            /* Mobility calculation */
            if (model->FEFET1mobMod == 0)
            {   T0 = Vgsteff + Vth + Vth;
                T2 = pParam->FEFET1ua + pParam->FEFET1uc * Vbseff;
                T3 = T0 / model->FEFET1toxe;
                T6 = pParam->FEFET1ud / T3 / T3 * Vth * Vth;
                T5 = T3 * (T2 + pParam->FEFET1ub * T3) + T6;
                T7 = - 2.0 * T6 / T0;
                dDenomi_dVg = (T2 + 2.0 * pParam->FEFET1ub * T3) / model->FEFET1toxe + T7;
                dDenomi_dVd = dDenomi_dVg * 2.0 * dVth_dVd;
                dDenomi_dVb = dDenomi_dVg * 2.0 * dVth_dVb + pParam->FEFET1uc * T3;
            }
            else if (model->FEFET1mobMod == 1)
            {   T0 = Vgsteff + Vth + Vth;
                T2 = 1.0 + pParam->FEFET1uc * Vbseff;
                T3 = T0 / model->FEFET1toxe;
                T4 = T3 * (pParam->FEFET1ua + pParam->FEFET1ub * T3);
                T6 = pParam->FEFET1ud / T3 / T3 * Vth * Vth;
                T5 = T4 * T2 + T6;
                T7 = - 2.0 * T6 / T0;
                dDenomi_dVg = (pParam->FEFET1ua + 2.0 * pParam->FEFET1ub * T3) * T2
                            / model->FEFET1toxe + T7;
                dDenomi_dVd = dDenomi_dVg * 2.0 * dVth_dVd;
                dDenomi_dVb = dDenomi_dVg * 2.0 * dVth_dVb + pParam->FEFET1uc * T4;
            }
        else
        {   T0 = (Vgsteff + here->FEFET1vtfbphi1) / model->FEFET1toxe;
            T1 = exp(pParam->FEFET1eu * log(T0));
            dT1_dVg = T1 * pParam->FEFET1eu / T0 / model->FEFET1toxe;
            T2 = pParam->FEFET1ua + pParam->FEFET1uc * Vbseff;
                T3 = T0 / model->FEFET1toxe;
                T6 = pParam->FEFET1ud / T3 / T3 * Vth * Vth;
            T5 = T1 * T2 + T6;
                T7 = - 2.0 * T6 / T0;
            dDenomi_dVg = T2 * dT1_dVg + T7;
                dDenomi_dVd = 0.0;
                dDenomi_dVb = T1 * pParam->FEFET1uc;
        }

        if (T5 >= -0.8)
        {   Denomi = 1.0 + T5;
        }
        else
        {   T9 = 1.0 / (7.0 + 10.0 * T5);
            Denomi = (0.6 + T5) * T9;
            T9 *= T9;
                dDenomi_dVg *= T9;
                dDenomi_dVd *= T9;
                dDenomi_dVb *= T9;
        }

            here->FEFET1ueff = ueff = here->FEFET1u0temp / Denomi;
        T9 = -ueff / Denomi;
            dueff_dVg = T9 * dDenomi_dVg;
            dueff_dVd = T9 * dDenomi_dVd;
            dueff_dVb = T9 * dDenomi_dVb;

            /* Saturation Drain Voltage  Vdsat */
            WVCox = Weff * here->FEFET1vsattemp * model->FEFET1coxe;
            WVCoxRds = WVCox * Rds; 

            Esat = 2.0 * here->FEFET1vsattemp / ueff;
            here->FEFET1EsatL = EsatL = Esat * Leff;
            T0 = -EsatL /ueff;
            dEsatL_dVg = T0 * dueff_dVg;
            dEsatL_dVd = T0 * dueff_dVd;
            dEsatL_dVb = T0 * dueff_dVb;
    
        /* Sqrt() */
            a1 = pParam->FEFET1a1;
        if (a1 == 0.0)
        {   Lambda = pParam->FEFET1a2;
            dLambda_dVg = 0.0;
        }
        else if (a1 > 0.0)
        {   T0 = 1.0 - pParam->FEFET1a2;
            T1 = T0 - pParam->FEFET1a1 * Vgsteff - 0.0001;
            T2 = sqrt(T1 * T1 + 0.0004 * T0);
            Lambda = pParam->FEFET1a2 + T0 - 0.5 * (T1 + T2);
            dLambda_dVg = 0.5 * pParam->FEFET1a1 * (1.0 + T1 / T2);
        }
        else
        {   T1 = pParam->FEFET1a2 + pParam->FEFET1a1 * Vgsteff - 0.0001;
            T2 = sqrt(T1 * T1 + 0.0004 * pParam->FEFET1a2);
            Lambda = 0.5 * (T1 + T2);
            dLambda_dVg = 0.5 * pParam->FEFET1a1 * (1.0 + T1 / T2);
        }

            Vgst2Vtm = Vgsteff + 2.0 * Vtm;
            if (Rds > 0)
            {   tmp2 = dRds_dVg / Rds + dWeff_dVg / Weff;
                tmp3 = dRds_dVb / Rds + dWeff_dVb / Weff;
            }
            else
            {   tmp2 = dWeff_dVg / Weff;
                tmp3 = dWeff_dVb / Weff;
        }
            if ((Rds == 0.0) && (Lambda == 1.0))
            {   T0 = 1.0 / (Abulk * EsatL + Vgst2Vtm);
                tmp1 = 0.0;
            T1 = T0 * T0;
            T2 = Vgst2Vtm * T0;
                T3 = EsatL * Vgst2Vtm;
                Vdsat = T3 * T0;
                            
                dT0_dVg = -(Abulk * dEsatL_dVg + EsatL * dAbulk_dVg + 1.0) * T1;
                dT0_dVd = -(Abulk * dEsatL_dVd) * T1; 
                dT0_dVb = -(Abulk * dEsatL_dVb + dAbulk_dVb * EsatL) * T1;   

                dVdsat_dVg = T3 * dT0_dVg + T2 * dEsatL_dVg + EsatL * T0;
                dVdsat_dVd = T3 * dT0_dVd + T2 * dEsatL_dVd;
                dVdsat_dVb = T3 * dT0_dVb + T2 * dEsatL_dVb;   
            }
            else
            {   tmp1 = dLambda_dVg / (Lambda * Lambda);
                T9 = Abulk * WVCoxRds;
            T8 = Abulk * T9;
            T7 = Vgst2Vtm * T9;
                T6 = Vgst2Vtm * WVCoxRds;
                T0 = 2.0 * Abulk * (T9 - 1.0 + 1.0 / Lambda); 
                dT0_dVg = 2.0 * (T8 * tmp2 - Abulk * tmp1
                + (2.0 * T9 + 1.0 / Lambda - 1.0) * dAbulk_dVg);
                
                dT0_dVb = 2.0 * (T8 * (2.0 / Abulk * dAbulk_dVb + tmp3)
                + (1.0 / Lambda - 1.0) * dAbulk_dVb);
            dT0_dVd = 0.0; 
                T1 = Vgst2Vtm * (2.0 / Lambda - 1.0) + Abulk * EsatL + 3.0 * T7;
                
                dT1_dVg = (2.0 / Lambda - 1.0) - 2.0 * Vgst2Vtm * tmp1
                + Abulk * dEsatL_dVg + EsatL * dAbulk_dVg + 3.0 * (T9
                + T7 * tmp2 + T6 * dAbulk_dVg);
                dT1_dVb = Abulk * dEsatL_dVb + EsatL * dAbulk_dVb
                    + 3.0 * (T6 * dAbulk_dVb + T7 * tmp3);
                dT1_dVd = Abulk * dEsatL_dVd;

                T2 = Vgst2Vtm * (EsatL + 2.0 * T6);
                dT2_dVg = EsatL + Vgst2Vtm * dEsatL_dVg
                + T6 * (4.0 + 2.0 * Vgst2Vtm * tmp2);
                dT2_dVb = Vgst2Vtm * (dEsatL_dVb + 2.0 * T6 * tmp3);
                dT2_dVd = Vgst2Vtm * dEsatL_dVd;

                T3 = sqrt(T1 * T1 - 2.0 * T0 * T2);
                Vdsat = (T1 - T3) / T0;

                dT3_dVg = (T1 * dT1_dVg - 2.0 * (T0 * dT2_dVg + T2 * dT0_dVg))
                    / T3;
                dT3_dVd = (T1 * dT1_dVd - 2.0 * (T0 * dT2_dVd + T2 * dT0_dVd))
                / T3;
                dT3_dVb = (T1 * dT1_dVb - 2.0 * (T0 * dT2_dVb + T2 * dT0_dVb))
                / T3;

                dVdsat_dVg = (dT1_dVg - (T1 * dT1_dVg - dT0_dVg * T2
                - T0 * dT2_dVg) / T3 - Vdsat * dT0_dVg) / T0;
                dVdsat_dVb = (dT1_dVb - (T1 * dT1_dVb - dT0_dVb * T2
                - T0 * dT2_dVb) / T3 - Vdsat * dT0_dVb) / T0;
                dVdsat_dVd = (dT1_dVd - (T1 * dT1_dVd - T0 * dT2_dVd) / T3) / T0;
            }
            here->FEFET1vdsat = Vdsat;

            /* Calculate Vdseff */
            T1 = Vdsat - Vds - pParam->FEFET1delta;
            dT1_dVg = dVdsat_dVg;
            dT1_dVd = dVdsat_dVd - 1.0;
            dT1_dVb = dVdsat_dVb;

            T2 = sqrt(T1 * T1 + 4.0 * pParam->FEFET1delta * Vdsat);
        T0 = T1 / T2;
        T9 = 2.0 * pParam->FEFET1delta;
        T3 = T9 / T2;
            dT2_dVg = T0 * dT1_dVg + T3 * dVdsat_dVg;
            dT2_dVd = T0 * dT1_dVd + T3 * dVdsat_dVd;
            dT2_dVb = T0 * dT1_dVb + T3 * dVdsat_dVb;

        if (T1 >= 0.0)
        {   Vdseff = Vdsat - 0.5 * (T1 + T2);
            dVdseff_dVg = dVdsat_dVg - 0.5 * (dT1_dVg + dT2_dVg);
                dVdseff_dVd = dVdsat_dVd - 0.5 * (dT1_dVd + dT2_dVd);
                dVdseff_dVb = dVdsat_dVb - 0.5 * (dT1_dVb + dT2_dVb);
        }
        else
        {   T4 = T9 / (T2 - T1);
            T5 = 1.0 - T4;
            T6 = Vdsat * T4 / (T2 - T1);
            Vdseff = Vdsat * T5;
                dVdseff_dVg = dVdsat_dVg * T5 + T6 * (dT2_dVg - dT1_dVg);
                dVdseff_dVd = dVdsat_dVd * T5 + T6 * (dT2_dVd - dT1_dVd);
                dVdseff_dVb = dVdsat_dVb * T5 + T6 * (dT2_dVb - dT1_dVb);
        }

            if (Vds == 0.0)
            {  Vdseff = 0.0;
                dVdseff_dVg = 0.0;
                dVdseff_dVb = 0.0; 
            }

            if (Vdseff > Vds)
                Vdseff = Vds;
            diffVds = Vds - Vdseff;
            here->FEFET1Vdseff = Vdseff;
            
            /* Velocity Overshoot */
            if((model->FEFET1lambdaGiven) && (model->FEFET1lambda > 0.0) )
            {  
            T1 =  Leff * ueff;
            T2 = pParam->FEFET1lambda / T1;
            T3 = -T2 / T1 * Leff;
            dT2_dVd = T3 * dueff_dVd;
            dT2_dVg = T3 * dueff_dVg;
            dT2_dVb = T3 * dueff_dVb;
            T5 = 1.0 / (Esat * pParam->FEFET1litl);
            T4 = -T5 / EsatL;
            dT5_dVg = dEsatL_dVg * T4;
            dT5_dVd = dEsatL_dVd * T4; 
            dT5_dVb = dEsatL_dVb * T4; 
            T6 = 1.0 + diffVds  * T5;
            dT6_dVg = dT5_dVg * diffVds - dVdseff_dVg * T5;
            dT6_dVd = dT5_dVd * diffVds + (1.0 - dVdseff_dVd) * T5;
            dT6_dVb = dT5_dVb * diffVds - dVdseff_dVb * T5;
            T7 = 2.0 / (T6 * T6 + 1.0);
            T8 = 1.0 - T7;
            T9 = T6 * T7 * T7;
            dT8_dVg = T9 * dT6_dVg;
            dT8_dVd = T9 * dT6_dVd;
            dT8_dVb = T9 * dT6_dVb;
            T10 = 1.0 + T2 * T8;
            dT10_dVg = dT2_dVg * T8 + T2 * dT8_dVg;
            dT10_dVd = dT2_dVd * T8 + T2 * dT8_dVd;
            dT10_dVb = dT2_dVb * T8 + T2 * dT8_dVb;
            if(T10 == 1.0)
                    dT10_dVg = dT10_dVd = dT10_dVb = 0.0;

            dEsatL_dVg *= T10;
            dEsatL_dVg += EsatL * dT10_dVg;
            dEsatL_dVd *= T10;
            dEsatL_dVd += EsatL * dT10_dVd;
            dEsatL_dVb *= T10;
            dEsatL_dVb += EsatL * dT10_dVb;
            EsatL *= T10;
            here->FEFET1EsatL = EsatL;
            }

            /* Calculate Vasat */
            tmp4 = 1.0 - 0.5 * Abulk * Vdsat / Vgst2Vtm;
            T9 = WVCoxRds * Vgsteff;
        T8 = T9 / Vgst2Vtm;
            T0 = EsatL + Vdsat + 2.0 * T9 * tmp4;
            
            T7 = 2.0 * WVCoxRds * tmp4;
            dT0_dVg = dEsatL_dVg + dVdsat_dVg + T7 * (1.0 + tmp2 * Vgsteff)
            - T8 * (Abulk * dVdsat_dVg - Abulk * Vdsat / Vgst2Vtm
            + Vdsat * dAbulk_dVg);   
            
            dT0_dVb = dEsatL_dVb + dVdsat_dVb + T7 * tmp3 * Vgsteff
            - T8 * (dAbulk_dVb * Vdsat + Abulk * dVdsat_dVb);
            dT0_dVd = dEsatL_dVd + dVdsat_dVd - T8 * Abulk * dVdsat_dVd;

            T9 = WVCoxRds * Abulk; 
            T1 = 2.0 / Lambda - 1.0 + T9; 
            dT1_dVg = -2.0 * tmp1 +  WVCoxRds * (Abulk * tmp2 + dAbulk_dVg);
            dT1_dVb = dAbulk_dVb * WVCoxRds + T9 * tmp3;

            Vasat = T0 / T1;
            dVasat_dVg = (dT0_dVg - Vasat * dT1_dVg) / T1;
            dVasat_dVb = (dT0_dVb - Vasat * dT1_dVb) / T1;
            dVasat_dVd = dT0_dVd / T1;

        /* Calculate Idl first */
            tmp1 = here->FEFET1vtfbphi2;
            tmp2 = 2.0e8 * model->FEFET1toxp;
            dT0_dVg = 1.0 / tmp2;
            T0 = (Vgsteff + tmp1) * dT0_dVg;

            tmp3 = exp(0.7 * log(T0));
            T1 = 1.0 + tmp3;
            T2 = 0.7 * tmp3 / T0;
            Tcen = 1.9e-9 / T1;
            dTcen_dVg = -Tcen * T2 * dT0_dVg / T1;

            Coxeff = EPSSI * model->FEFET1coxp
                    / (EPSSI + model->FEFET1coxp * Tcen);
            dCoxeff_dVg = -Coxeff * Coxeff * dTcen_dVg / EPSSI;

            CoxeffWovL = Coxeff * Weff / Leff;
            beta = ueff * CoxeffWovL;
            T3 = ueff / Leff;
            dbeta_dVg = CoxeffWovL * dueff_dVg + T3
                        * (Weff * dCoxeff_dVg + Coxeff * dWeff_dVg);
            dbeta_dVd = CoxeffWovL * dueff_dVd;
            dbeta_dVb = CoxeffWovL * dueff_dVb + T3 * Coxeff * dWeff_dVb;

            here->FEFET1AbovVgst2Vtm = Abulk / Vgst2Vtm;
            T0 = 1.0 - 0.5 * Vdseff * here->FEFET1AbovVgst2Vtm;
            dT0_dVg = -0.5 * (Abulk * dVdseff_dVg
                    - Abulk * Vdseff / Vgst2Vtm + Vdseff * dAbulk_dVg) / Vgst2Vtm;
            dT0_dVd = -0.5 * Abulk * dVdseff_dVd / Vgst2Vtm;
            dT0_dVb = -0.5 * (Abulk * dVdseff_dVb + dAbulk_dVb * Vdseff)
                    / Vgst2Vtm;

            fgche1 = Vgsteff * T0;
            dfgche1_dVg = Vgsteff * dT0_dVg + T0;
            dfgche1_dVd = Vgsteff * dT0_dVd;
            dfgche1_dVb = Vgsteff * dT0_dVb;

            T9 = Vdseff / EsatL;
            fgche2 = 1.0 + T9;
            dfgche2_dVg = (dVdseff_dVg - T9 * dEsatL_dVg) / EsatL;
            dfgche2_dVd = (dVdseff_dVd - T9 * dEsatL_dVd) / EsatL;
            dfgche2_dVb = (dVdseff_dVb - T9 * dEsatL_dVb) / EsatL;

            gche = beta * fgche1 / fgche2;
            dgche_dVg = (beta * dfgche1_dVg + fgche1 * dbeta_dVg
                        - gche * dfgche2_dVg) / fgche2;
            dgche_dVd = (beta * dfgche1_dVd + fgche1 * dbeta_dVd
                        - gche * dfgche2_dVd) / fgche2;
            dgche_dVb = (beta * dfgche1_dVb + fgche1 * dbeta_dVb
                        - gche * dfgche2_dVb) / fgche2;

            T0 = 1.0 + gche * Rds;
            Idl = gche / T0;
            T1 = (1.0 - Idl * Rds) / T0;
            T2 = Idl * Idl;
            dIdl_dVg = T1 * dgche_dVg - T2 * dRds_dVg;
            dIdl_dVd = T1 * dgche_dVd;
            dIdl_dVb = T1 * dgche_dVb - T2 * dRds_dVb;

            /* Calculate degradation factor due to pocket implant */

        if (pParam->FEFET1fprout <= 0.0)
        {   FP = 1.0;
            dFP_dVg = 0.0;
        }
        else
        {   T9 = pParam->FEFET1fprout * sqrt(Leff) / Vgst2Vtm;
                FP = 1.0 / (1.0 + T9);
                dFP_dVg = FP * FP * T9 / Vgst2Vtm;
        }

            /* Calculate VACLM */
            T8 = pParam->FEFET1pvag / EsatL;
            T9 = T8 * Vgsteff;
            if (T9 > -0.9)
            {   PvagTerm = 1.0 + T9;
                dPvagTerm_dVg = T8 * (1.0 - Vgsteff * dEsatL_dVg / EsatL);
                dPvagTerm_dVb = -T9 * dEsatL_dVb / EsatL;
                dPvagTerm_dVd = -T9 * dEsatL_dVd / EsatL;
            }
            else
            {   T4 = 1.0 / (17.0 + 20.0 * T9);
                PvagTerm = (0.8 + T9) * T4;
                T4 *= T4;
                dPvagTerm_dVg = T8 * (1.0 - Vgsteff * dEsatL_dVg / EsatL) * T4;
                T9 *= T4 / EsatL;
                dPvagTerm_dVb = -T9 * dEsatL_dVb;
                dPvagTerm_dVd = -T9 * dEsatL_dVd;
            }

            if ((pParam->FEFET1pclm > MIN_EXP) && (diffVds > 1.0e-10))
        {   T0 = 1.0 + Rds * Idl;
                dT0_dVg = dRds_dVg * Idl + Rds * dIdl_dVg;
                dT0_dVd = Rds * dIdl_dVd;
                dT0_dVb = dRds_dVb * Idl + Rds * dIdl_dVb;

                T2 = Vdsat / Esat;
            T1 = Leff + T2;
                dT1_dVg = (dVdsat_dVg - T2 * dEsatL_dVg / Leff) / Esat;
                dT1_dVd = (dVdsat_dVd - T2 * dEsatL_dVd / Leff) / Esat;
                dT1_dVb = (dVdsat_dVb - T2 * dEsatL_dVb / Leff) / Esat;

                Cclm = FP * PvagTerm * T0 * T1 / (pParam->FEFET1pclm * pParam->FEFET1litl);
                dCclm_dVg = Cclm * (dFP_dVg / FP + dPvagTerm_dVg / PvagTerm
                            + dT0_dVg / T0 + dT1_dVg / T1);
                dCclm_dVb = Cclm * (dPvagTerm_dVb / PvagTerm + dT0_dVb / T0
                            + dT1_dVb / T1);
                dCclm_dVd = Cclm * (dPvagTerm_dVd / PvagTerm + dT0_dVd / T0
                            + dT1_dVd / T1);
                VACLM = Cclm * diffVds;

                dVACLM_dVg = dCclm_dVg * diffVds - dVdseff_dVg * Cclm;
                dVACLM_dVb = dCclm_dVb * diffVds - dVdseff_dVb * Cclm;
                dVACLM_dVd = dCclm_dVd * diffVds + (1.0 - dVdseff_dVd) * Cclm;
            }
            else
            {   VACLM = Cclm = MAX_EXP;
                dVACLM_dVd = dVACLM_dVg = dVACLM_dVb = 0.0;
                dCclm_dVd = dCclm_dVg = dCclm_dVb = 0.0;
            }

            /* Calculate VADIBL */
            if (pParam->FEFET1thetaRout > MIN_EXP)
        {   T8 = Abulk * Vdsat;
            T0 = Vgst2Vtm * T8;
                dT0_dVg = Vgst2Vtm * Abulk * dVdsat_dVg + T8
                + Vgst2Vtm * Vdsat * dAbulk_dVg;
                dT0_dVb = Vgst2Vtm * (dAbulk_dVb * Vdsat + Abulk * dVdsat_dVb);
                dT0_dVd = Vgst2Vtm * Abulk * dVdsat_dVd;

                T1 = Vgst2Vtm + T8;
                dT1_dVg = 1.0 + Abulk * dVdsat_dVg + Vdsat * dAbulk_dVg;
                dT1_dVb = Abulk * dVdsat_dVb + dAbulk_dVb * Vdsat;
                dT1_dVd = Abulk * dVdsat_dVd;

            T9 = T1 * T1;
            T2 = pParam->FEFET1thetaRout;
                VADIBL = (Vgst2Vtm - T0 / T1) / T2;
                dVADIBL_dVg = (1.0 - dT0_dVg / T1 + T0 * dT1_dVg / T9) / T2;
                dVADIBL_dVb = (-dT0_dVb / T1 + T0 * dT1_dVb / T9) / T2;
                dVADIBL_dVd = (-dT0_dVd / T1 + T0 * dT1_dVd / T9) / T2;

            T7 = pParam->FEFET1pdiblb * Vbseff;
            if (T7 >= -0.9)
            {   T3 = 1.0 / (1.0 + T7);
                    VADIBL *= T3;
                    dVADIBL_dVg *= T3;
                    dVADIBL_dVb = (dVADIBL_dVb - VADIBL * pParam->FEFET1pdiblb)
                    * T3;
                    dVADIBL_dVd *= T3;
            }
            else
            {   T4 = 1.0 / (0.8 + T7);
            T3 = (17.0 + 20.0 * T7) * T4;
                    dVADIBL_dVg *= T3;
                    dVADIBL_dVb = dVADIBL_dVb * T3
                    - VADIBL * pParam->FEFET1pdiblb * T4 * T4;
                    dVADIBL_dVd *= T3;
                    VADIBL *= T3;
            }

                dVADIBL_dVg = dVADIBL_dVg * PvagTerm + VADIBL * dPvagTerm_dVg;
                dVADIBL_dVb = dVADIBL_dVb * PvagTerm + VADIBL * dPvagTerm_dVb;
                dVADIBL_dVd = dVADIBL_dVd * PvagTerm + VADIBL * dPvagTerm_dVd;
                VADIBL *= PvagTerm;
            }
        else
        {   VADIBL = MAX_EXP;
                dVADIBL_dVd = dVADIBL_dVg = dVADIBL_dVb = 0.0;
            }

            /* Calculate Va */
            Va = Vasat + VACLM;
            dVa_dVg = dVasat_dVg + dVACLM_dVg;
            dVa_dVb = dVasat_dVb + dVACLM_dVb;
            dVa_dVd = dVasat_dVd + dVACLM_dVd;

            /* Calculate VADITS */
            T0 = pParam->FEFET1pditsd * Vds;
            if (T0 > EXP_THRESHOLD)
            {   T1 = MAX_EXP;
                dT1_dVd = 0;
            }
        else
            {   T1 = exp(T0);
                dT1_dVd = T1 * pParam->FEFET1pditsd;
            }

            if (pParam->FEFET1pdits > MIN_EXP)
            {   T2 = 1.0 + model->FEFET1pditsl * Leff;
                VADITS = (1.0 + T2 * T1) / pParam->FEFET1pdits;
                dVADITS_dVg = VADITS * dFP_dVg;
                dVADITS_dVd = FP * T2 * dT1_dVd / pParam->FEFET1pdits;
            VADITS *= FP;
            }
        else
            {   VADITS = MAX_EXP;
                dVADITS_dVg = dVADITS_dVd = 0;
            }

            /* Calculate VASCBE */
        if (pParam->FEFET1pscbe2 > 0.0)
        {   if (diffVds > pParam->FEFET1pscbe1 * pParam->FEFET1litl
            / EXP_THRESHOLD)
            {   T0 =  pParam->FEFET1pscbe1 * pParam->FEFET1litl / diffVds;
                VASCBE = Leff * exp(T0) / pParam->FEFET1pscbe2;
                    T1 = T0 * VASCBE / diffVds;
                    dVASCBE_dVg = T1 * dVdseff_dVg;
                    dVASCBE_dVd = -T1 * (1.0 - dVdseff_dVd);
                    dVASCBE_dVb = T1 * dVdseff_dVb;
                }
            else
            {   VASCBE = MAX_EXP * Leff/pParam->FEFET1pscbe2;
                    dVASCBE_dVg = dVASCBE_dVd = dVASCBE_dVb = 0.0;
                }
        }
        else
        {   VASCBE = MAX_EXP;
                dVASCBE_dVg = dVASCBE_dVd = dVASCBE_dVb = 0.0;
        }

        /* Add DIBL to Ids */
            T9 = diffVds / VADIBL;
            T0 = 1.0 + T9;
            Idsa = Idl * T0;
            dIdsa_dVg = T0 * dIdl_dVg - Idl * (dVdseff_dVg + T9 * dVADIBL_dVg) / VADIBL;
            dIdsa_dVd = T0 * dIdl_dVd + Idl
                        * (1.0 - dVdseff_dVd - T9 * dVADIBL_dVd) / VADIBL;
            dIdsa_dVb = T0 * dIdl_dVb - Idl * (dVdseff_dVb + T9 * dVADIBL_dVb) / VADIBL;

            /* Add DITS to Ids */
            T9 = diffVds / VADITS;
            T0 = 1.0 + T9;
            dIdsa_dVg = T0 * dIdsa_dVg - Idsa * (dVdseff_dVg + T9 * dVADITS_dVg) / VADITS;
            dIdsa_dVd = T0 * dIdsa_dVd + Idsa 
                * (1.0 - dVdseff_dVd - T9 * dVADITS_dVd) / VADITS;
            dIdsa_dVb = T0 * dIdsa_dVb - Idsa * dVdseff_dVb / VADITS;
            Idsa *= T0;

            /* Add CLM to Ids */
            T0 = log(Va / Vasat);
            dT0_dVg = dVa_dVg / Va - dVasat_dVg / Vasat;
            dT0_dVb = dVa_dVb / Va - dVasat_dVb / Vasat;
            dT0_dVd = dVa_dVd / Va - dVasat_dVd / Vasat;
            T1 = T0 / Cclm;
            T9 = 1.0 + T1;
            dT9_dVg = (dT0_dVg - T1 * dCclm_dVg) / Cclm;
            dT9_dVb = (dT0_dVb - T1 * dCclm_dVb) / Cclm;
            dT9_dVd = (dT0_dVd - T1 * dCclm_dVd) / Cclm;

            dIdsa_dVg = dIdsa_dVg * T9 + Idsa * dT9_dVg;
            dIdsa_dVb = dIdsa_dVb * T9 + Idsa * dT9_dVb;
            dIdsa_dVd = dIdsa_dVd * T9 + Idsa * dT9_dVd;
            Idsa *= T9;

            /* Substrate current begins */
            tmp = pParam->FEFET1alpha0 + pParam->FEFET1alpha1 * Leff;
            if ((tmp <= 0.0) || (pParam->FEFET1beta0 <= 0.0))
            {   Isub = Gbd = Gbb = Gbg = 0.0;
            }
            else
            {   T2 = tmp / Leff;
                if (diffVds > pParam->FEFET1beta0 / EXP_THRESHOLD)
                {   T0 = -pParam->FEFET1beta0 / diffVds;
                    T1 = T2 * diffVds * exp(T0);
                    T3 = T1 / diffVds * (T0 - 1.0);
                    dT1_dVg = T3 * dVdseff_dVg;
                    dT1_dVd = T3 * (dVdseff_dVd - 1.0);
                    dT1_dVb = T3 * dVdseff_dVb;
                }
                else
                {   T3 = T2 * MIN_EXP;
                    T1 = T3 * diffVds;
                    dT1_dVg = -T3 * dVdseff_dVg;
                    dT1_dVd = T3 * (1.0 - dVdseff_dVd);
                    dT1_dVb = -T3 * dVdseff_dVb;
                }
                T4 = Idsa * Vdseff;
                Isub = T1 * T4;
                Gbg = T1 * (dIdsa_dVg * Vdseff + Idsa * dVdseff_dVg)
                    + T4 * dT1_dVg;
                Gbd = T1 * (dIdsa_dVd * Vdseff + Idsa * dVdseff_dVd)
                    + T4 * dT1_dVd;
                Gbb = T1 * (dIdsa_dVb * Vdseff + Idsa * dVdseff_dVb)
                    + T4 * dT1_dVb;

                Gbd += Gbg * dVgsteff_dVd;
                Gbb += Gbg * dVgsteff_dVb;
                Gbg *= dVgsteff_dVg;
                Gbb *= dVbseff_dVb;
            }
            here->FEFET1csub = Isub;
            here->FEFET1gbbs = Gbb;
            here->FEFET1gbgs = Gbg;
            here->FEFET1gbds = Gbd;

            /* Add SCBE to Ids */
            T9 = diffVds / VASCBE;
            T0 = 1.0 + T9;
            Ids = Idsa * T0;

            Gm = T0 * dIdsa_dVg - Idsa 
            * (dVdseff_dVg + T9 * dVASCBE_dVg) / VASCBE;
            Gds = T0 * dIdsa_dVd + Idsa 
            * (1.0 - dVdseff_dVd - T9 * dVASCBE_dVd) / VASCBE;
            Gmb = T0 * dIdsa_dVb - Idsa
            * (dVdseff_dVb + T9 * dVASCBE_dVb) / VASCBE;


        tmp1 = Gds + Gm * dVgsteff_dVd;
        tmp2 = Gmb + Gm * dVgsteff_dVb;
        tmp3 = Gm;

            Gm = (Ids * dVdseff_dVg + Vdseff * tmp3) * dVgsteff_dVg;
            Gds = Ids * (dVdseff_dVd + dVdseff_dVg * dVgsteff_dVd)
            + Vdseff * tmp1;
            Gmb = (Ids * (dVdseff_dVb + dVdseff_dVg * dVgsteff_dVb)
            + Vdseff * tmp2) * dVbseff_dVb;

            cdrain = Ids * Vdseff;
            
            /* Source End Velocity Limit  */
            if((model->FEFET1vtlGiven) && (model->FEFET1vtl > 0.0) ) {
            T12 = 1.0 / Leff / CoxeffWovL;
            T11 = T12 / Vgsteff;
            T10 = -T11 / Vgsteff;
            vs = cdrain * T11; /* vs */
            dvs_dVg = Gm * T11 + cdrain * T10 * dVgsteff_dVg;
            dvs_dVd = Gds * T11 + cdrain * T10 * dVgsteff_dVd;
            dvs_dVb = Gmb * T11 + cdrain * T10 * dVgsteff_dVb;
            T0 = 2 * MM;
            T1 = vs / (pParam->FEFET1vtl * pParam->FEFET1tfactor);
            if(T1 > 0.0)  
            {	T2 = 1.0 + exp(T0 * log(T1));
                T3 = (T2 - 1.0) * T0 / vs; 
                Fsevl = 1.0 / exp(log(T2)/ T0);
                dT2_dVg = T3 * dvs_dVg;
                dT2_dVd = T3 * dvs_dVd;
                dT2_dVb = T3 * dvs_dVb;
                T4 = -1.0 / T0 * Fsevl / T2;
                dFsevl_dVg = T4 * dT2_dVg;
                dFsevl_dVd = T4 * dT2_dVd;
                dFsevl_dVb = T4 * dT2_dVb;
            } else {
                Fsevl = 1.0;
                dFsevl_dVg = 0.0;
                dFsevl_dVd = 0.0;
                dFsevl_dVb = 0.0;
            }
            Gm *=Fsevl;
            Gm += cdrain * dFsevl_dVg;
            Gmb *=Fsevl;
            Gmb += cdrain * dFsevl_dVb;
            Gds *=Fsevl;
            Gds += cdrain * dFsevl_dVd;

            cdrain *= Fsevl; 
            } 

            here->FEFET1gds = Gds;
            here->FEFET1gm = Gm;
            here->FEFET1gmbs = Gmb;
            here->FEFET1IdovVds = Ids;
            if( here->FEFET1IdovVds <= 1.0e-9) here->FEFET1IdovVds = 1.0e-9;

            /* Calculate Rg */
            if ((here->FEFET1rgateMod > 1) ||
                (here->FEFET1trnqsMod != 0) || (here->FEFET1acnqsMod != 0))
        {   T9 = pParam->FEFET1xrcrg2 * model->FEFET1vtm;
                T0 = T9 * beta;
                dT0_dVd = (dbeta_dVd + dbeta_dVg * dVgsteff_dVd) * T9;
                dT0_dVb = (dbeta_dVb + dbeta_dVg * dVgsteff_dVb) * T9;
                dT0_dVg = dbeta_dVg * T9;

            here->FEFET1gcrg = pParam->FEFET1xrcrg1 * ( T0 + Ids);
            here->FEFET1gcrgd = pParam->FEFET1xrcrg1 * (dT0_dVd + tmp1);
                here->FEFET1gcrgb = pParam->FEFET1xrcrg1 * (dT0_dVb + tmp2)
                    * dVbseff_dVb;	
                here->FEFET1gcrgg = pParam->FEFET1xrcrg1 * (dT0_dVg + tmp3)
                    * dVgsteff_dVg;

            if (here->FEFET1nf != 1.0)
            {   here->FEFET1gcrg *= here->FEFET1nf; 
            here->FEFET1gcrgg *= here->FEFET1nf;
            here->FEFET1gcrgd *= here->FEFET1nf;
            here->FEFET1gcrgb *= here->FEFET1nf;
            }

                if (here->FEFET1rgateMod == 2)
                {   T10 = here->FEFET1grgeltd * here->FEFET1grgeltd;
            T11 = here->FEFET1grgeltd + here->FEFET1gcrg;
            here->FEFET1gcrg = here->FEFET1grgeltd * here->FEFET1gcrg / T11;
                    T12 = T10 / T11 / T11;
                    here->FEFET1gcrgg *= T12;
                    here->FEFET1gcrgd *= T12;
                    here->FEFET1gcrgb *= T12;
                }
                here->FEFET1gcrgs = -(here->FEFET1gcrgg + here->FEFET1gcrgd
                    + here->FEFET1gcrgb);
        }


        /* Calculate bias-dependent external S/D resistance */
            if (model->FEFET1rdsMod)
        {   /* Rs(V) */
                T0 = vgs - pParam->FEFET1vfbsd;
                T1 = sqrt(T0 * T0 + 1.0e-4);
                vgs_eff = 0.5 * (T0 + T1);
                dvgs_eff_dvg = vgs_eff / T1;

                T0 = 1.0 + pParam->FEFET1prwg * vgs_eff;
                dT0_dvg = -pParam->FEFET1prwg / T0 / T0 * dvgs_eff_dvg;
                T1 = -pParam->FEFET1prwb * vbs;
                dT1_dvb = -pParam->FEFET1prwb;

                T2 = 1.0 / T0 + T1;
                T3 = T2 + sqrt(T2 * T2 + 0.01);
                dT3_dvg = T3 / (T3 - T2);
                dT3_dvb = dT3_dvg * dT1_dvb;
                dT3_dvg *= dT0_dvg;

                T4 = pParam->FEFET1rs0 * 0.5;
                Rs = pParam->FEFET1rswmin + T3 * T4;
                dRs_dvg = T4 * dT3_dvg;
                dRs_dvb = T4 * dT3_dvb;

                T0 = 1.0 + here->FEFET1sourceConductance * Rs;
                here->FEFET1gstot = here->FEFET1sourceConductance / T0;
                T0 = -here->FEFET1gstot * here->FEFET1gstot;
                dgstot_dvd = 0.0; /* place holder */
                dgstot_dvg = T0 * dRs_dvg;
                dgstot_dvb = T0 * dRs_dvb;
                dgstot_dvs = -(dgstot_dvg + dgstot_dvb + dgstot_dvd);

            /* Rd(V) */
                T0 = vgd - pParam->FEFET1vfbsd;
                T1 = sqrt(T0 * T0 + 1.0e-4);
                vgd_eff = 0.5 * (T0 + T1);
                dvgd_eff_dvg = vgd_eff / T1;

                T0 = 1.0 + pParam->FEFET1prwg * vgd_eff;
                dT0_dvg = -pParam->FEFET1prwg / T0 / T0 * dvgd_eff_dvg;
                T1 = -pParam->FEFET1prwb * vbd;
                dT1_dvb = -pParam->FEFET1prwb;

                T2 = 1.0 / T0 + T1;
                T3 = T2 + sqrt(T2 * T2 + 0.01);
                dT3_dvg = T3 / (T3 - T2);
                dT3_dvb = dT3_dvg * dT1_dvb;
                dT3_dvg *= dT0_dvg;

                T4 = pParam->FEFET1rd0 * 0.5;
                Rd = pParam->FEFET1rdwmin + T3 * T4;
                dRd_dvg = T4 * dT3_dvg;
                dRd_dvb = T4 * dT3_dvb;

                T0 = 1.0 + here->FEFET1drainConductance * Rd;
                here->FEFET1gdtot = here->FEFET1drainConductance / T0;
                T0 = -here->FEFET1gdtot * here->FEFET1gdtot;
                dgdtot_dvs = 0.0;
                dgdtot_dvg = T0 * dRd_dvg;
                dgdtot_dvb = T0 * dRd_dvb;
                dgdtot_dvd = -(dgdtot_dvg + dgdtot_dvb + dgdtot_dvs);

                here->FEFET1gstotd = vses * dgstot_dvd;
                here->FEFET1gstotg = vses * dgstot_dvg;
                here->FEFET1gstots = vses * dgstot_dvs;
                here->FEFET1gstotb = vses * dgstot_dvb;

                T2 = vdes - vds;
                here->FEFET1gdtotd = T2 * dgdtot_dvd;
                here->FEFET1gdtotg = T2 * dgdtot_dvg;
                here->FEFET1gdtots = T2 * dgdtot_dvs;
                here->FEFET1gdtotb = T2 * dgdtot_dvb;
        }
        else /* WDLiu: for bypass */
        {   here->FEFET1gstot = here->FEFET1gstotd = here->FEFET1gstotg = 0.0;
            here->FEFET1gstots = here->FEFET1gstotb = 0.0;
            here->FEFET1gdtot = here->FEFET1gdtotd = here->FEFET1gdtotg = 0.0;
                here->FEFET1gdtots = here->FEFET1gdtotb = 0.0;
        }

            /* Calculate GIDL current */
            vgs_eff = here->FEFET1vgs_eff;
            dvgs_eff_dvg = here->FEFET1dvgs_eff_dvg;
            T0 = 3.0 * model->FEFET1toxe;

            T1 = (vds - vgs_eff - pParam->FEFET1egidl ) / T0;
            if ((pParam->FEFET1agidl <= 0.0) || (pParam->FEFET1bgidl <= 0.0)
                || (T1 <= 0.0) || (pParam->FEFET1cgidl <= 0.0) || (vbd > 0.0))
                Igidl = Ggidld = Ggidlg = Ggidlb = 0.0;
            else {
                dT1_dVd = 1.0 / T0;
                dT1_dVg = -dvgs_eff_dvg * dT1_dVd;
                T2 = pParam->FEFET1bgidl / T1;
                if (T2 < 100.0)
                {   Igidl = pParam->FEFET1agidl * pParam->FEFET1weffCJ * T1 * exp(-T2);
                    T3 = Igidl * (1.0 + T2) / T1;
                    Ggidld = T3 * dT1_dVd;
                    Ggidlg = T3 * dT1_dVg;
                }
                else
                {   Igidl = pParam->FEFET1agidl * pParam->FEFET1weffCJ * 3.720075976e-44;
                    Ggidld = Igidl * dT1_dVd;
                    Ggidlg = Igidl * dT1_dVg;
                    Igidl *= T1;
                }
            
                T4 = vbd * vbd;
                T5 = -vbd * T4;
                T6 = pParam->FEFET1cgidl + T5;
                T7 = T5 / T6;
                T8 = 3.0 * pParam->FEFET1cgidl * T4 / T6 / T6;
                Ggidld = Ggidld * T7 + Igidl * T8;
                Ggidlg = Ggidlg * T7;
                Ggidlb = -Igidl * T8;
                Igidl *= T7;
            }
            here->FEFET1Igidl = Igidl;
            here->FEFET1ggidld = Ggidld;
            here->FEFET1ggidlg = Ggidlg;
            here->FEFET1ggidlb = Ggidlb;

            /* Calculate GISL current  */
            vgd_eff = here->FEFET1vgd_eff;
            dvgd_eff_dvg = here->FEFET1dvgd_eff_dvg;

            T1 = (-vds - vgd_eff - pParam->FEFET1egidl ) / T0;

            if ((pParam->FEFET1agidl <= 0.0) || (pParam->FEFET1bgidl <= 0.0)
                || (T1 <= 0.0) || (pParam->FEFET1cgidl <= 0.0) || (vbs > 0.0))
                Igisl = Ggisls = Ggislg = Ggislb = 0.0;
            else {
                dT1_dVd = 1.0 / T0;
                dT1_dVg = -dvgd_eff_dvg * dT1_dVd;
                T2 = pParam->FEFET1bgidl / T1;
                if (T2 < 100.0) 
                {   Igisl = pParam->FEFET1agidl * pParam->FEFET1weffCJ * T1 * exp(-T2);
                    T3 = Igisl * (1.0 + T2) / T1;
                    Ggisls = T3 * dT1_dVd;
                    Ggislg = T3 * dT1_dVg;
                }
                else 
                {   Igisl = pParam->FEFET1agidl * pParam->FEFET1weffCJ * 3.720075976e-44;
                    Ggisls = Igisl * dT1_dVd;
                    Ggislg = Igisl * dT1_dVg;
                    Igisl *= T1;
                }
            
                T4 = vbs * vbs;
                T5 = -vbs * T4;
                T6 = pParam->FEFET1cgidl + T5;
                T7 = T5 / T6;
                T8 = 3.0 * pParam->FEFET1cgidl * T4 / T6 / T6;
                Ggisls = Ggisls * T7 + Igisl * T8;
                Ggislg = Ggislg * T7;
                Ggislb = -Igisl * T8;
                Igisl *= T7;
            }
            here->FEFET1Igisl = Igisl;
            here->FEFET1ggisls = Ggisls;
            here->FEFET1ggislg = Ggislg;
            here->FEFET1ggislb = Ggislb;


            /* Calculate gate tunneling current */
            if ((model->FEFET1igcMod != 0) || (model->FEFET1igbMod != 0))
            {   Vfb = here->FEFET1vfbzb;
                V3 = Vfb - Vgs_eff + Vbseff - DELTA_3;
                if (Vfb <= 0.0)
                    T0 = sqrt(V3 * V3 - 4.0 * DELTA_3 * Vfb);
                else
                    T0 = sqrt(V3 * V3 + 4.0 * DELTA_3 * Vfb);
                T1 = 0.5 * (1.0 + V3 / T0);
                Vfbeff = Vfb - 0.5 * (V3 + T0);
                dVfbeff_dVg = T1 * dVgs_eff_dVg;
                dVfbeff_dVb = -T1; /* WDLiu: -No surprise? No. -Good! */

                Voxacc = Vfb - Vfbeff;
                dVoxacc_dVg = -dVfbeff_dVg;
                dVoxacc_dVb = -dVfbeff_dVb;
                if (Voxacc < 0.0) /* WDLiu: Avoiding numerical instability. */
                    Voxacc = dVoxacc_dVg = dVoxacc_dVb = 0.0;

                T0 = 0.5 * pParam->FEFET1k1ox;
                T3 = Vgs_eff - Vfbeff - Vbseff - Vgsteff;
                if (pParam->FEFET1k1ox == 0.0)
                    Voxdepinv = dVoxdepinv_dVg = dVoxdepinv_dVd
                    = dVoxdepinv_dVb = 0.0;
                else if (T3 < 0.0)
                {   Voxdepinv = -T3;
                    dVoxdepinv_dVg = -dVgs_eff_dVg + dVfbeff_dVg
                                    + dVgsteff_dVg;
                    dVoxdepinv_dVd = dVgsteff_dVd;
                    dVoxdepinv_dVb = dVfbeff_dVb + 1.0 + dVgsteff_dVb;
            }
                else
                {   T1 = sqrt(T0 * T0 + T3);
                    T2 = T0 / T1;
                    Voxdepinv = pParam->FEFET1k1ox * (T1 - T0);
                    dVoxdepinv_dVg = T2 * (dVgs_eff_dVg - dVfbeff_dVg
                    - dVgsteff_dVg);
                    dVoxdepinv_dVd = -T2 * dVgsteff_dVd;
                    dVoxdepinv_dVb = -T2 * (dVfbeff_dVb + 1.0 + dVgsteff_dVb);
                }

                Voxdepinv += Vgsteff;
                dVoxdepinv_dVg += dVgsteff_dVg;
                dVoxdepinv_dVd += dVgsteff_dVd;
                dVoxdepinv_dVb += dVgsteff_dVb;
            }

            if(model->FEFET1tempMod < 2)
                tmp = Vtm;
            else /* model->FEFET1tempMod = 2 */
                tmp = Vtm0;
            if (model->FEFET1igcMod)
            {   T0 = tmp * pParam->FEFET1nigc;
            if(model->FEFET1igcMod == 1) {
                    VxNVt = (Vgs_eff - model->FEFET1type * here->FEFET1vth0) / T0;
                    if (VxNVt > EXP_THRESHOLD)
                    {   Vaux = Vgs_eff - model->FEFET1type * here->FEFET1vth0;
                        dVaux_dVg = dVgs_eff_dVg;
                dVaux_dVd = 0.0;
                        dVaux_dVb = 0.0;
                    }
            } else if (model->FEFET1igcMod == 2) {
                    VxNVt = (Vgs_eff - here->FEFET1von) / T0;
                    if (VxNVt > EXP_THRESHOLD)
                    {   Vaux = Vgs_eff - here->FEFET1von;
                        dVaux_dVg = dVgs_eff_dVg;
                        dVaux_dVd = -dVth_dVd;
                        dVaux_dVb = -dVth_dVb;
                    }
                } 
                if (VxNVt < -EXP_THRESHOLD)
                {   Vaux = T0 * log(1.0 + MIN_EXP);
                    dVaux_dVg = dVaux_dVd = dVaux_dVb = 0.0;
                }
                else if ((VxNVt >= -EXP_THRESHOLD) && (VxNVt <= EXP_THRESHOLD))
                {   ExpVxNVt = exp(VxNVt);
                    Vaux = T0 * log(1.0 + ExpVxNVt);
                    dVaux_dVg = ExpVxNVt / (1.0 + ExpVxNVt);
            if(model->FEFET1igcMod == 1) {
                dVaux_dVd = 0.0;
                        dVaux_dVb = 0.0;
                    } else if (model->FEFET1igcMod == 2) {
                            dVaux_dVd = -dVgs_eff_dVg * dVth_dVd;
                            dVaux_dVb = -dVgs_eff_dVg * dVth_dVb;
            }
            dVaux_dVg *= dVgs_eff_dVg;
                }

                T2 = Vgs_eff * Vaux;
                dT2_dVg = dVgs_eff_dVg * Vaux + Vgs_eff * dVaux_dVg;
                dT2_dVd = Vgs_eff * dVaux_dVd;
                dT2_dVb = Vgs_eff * dVaux_dVb;

                T11 = pParam->FEFET1Aechvb;
                T12 = pParam->FEFET1Bechvb;
                T3 = pParam->FEFET1aigc * pParam->FEFET1cigc
                    - pParam->FEFET1bigc;
                T4 = pParam->FEFET1bigc * pParam->FEFET1cigc;
                T5 = T12 * (pParam->FEFET1aigc + T3 * Voxdepinv
                    - T4 * Voxdepinv * Voxdepinv);

                if (T5 > EXP_THRESHOLD)
                {   T6 = MAX_EXP;
                    dT6_dVg = dT6_dVd = dT6_dVb = 0.0;
                }
                else if (T5 < -EXP_THRESHOLD)
                {   T6 = MIN_EXP;
                    dT6_dVg = dT6_dVd = dT6_dVb = 0.0;
                }
                else
                {   T6 = exp(T5);
                    dT6_dVg = T6 * T12 * (T3 - 2.0 * T4 * Voxdepinv);
                    dT6_dVd = dT6_dVg * dVoxdepinv_dVd;
                    dT6_dVb = dT6_dVg * dVoxdepinv_dVb;
                    dT6_dVg *= dVoxdepinv_dVg;
                }

                Igc = T11 * T2 * T6;
                dIgc_dVg = T11 * (T2 * dT6_dVg + T6 * dT2_dVg);
                dIgc_dVd = T11 * (T2 * dT6_dVd + T6 * dT2_dVd);
                dIgc_dVb = T11 * (T2 * dT6_dVb + T6 * dT2_dVb);

                if (model->FEFET1pigcdGiven)
                {   Pigcd = pParam->FEFET1pigcd;
                    dPigcd_dVg = dPigcd_dVd = dPigcd_dVb = 0.0;
                }
                else
                {   T11 = pParam->FEFET1Bechvb * model->FEFET1toxe;
                    T12 = Vgsteff + 1.0e-20;
                    T13 = T11 / T12 / T12;
                    T14 = -T13 / T12;
                    Pigcd = T13 * (1.0 - 0.5 * Vdseff / T12); 
                    dPigcd_dVg = T14 * (2.0 + 0.5 * (dVdseff_dVg
                                - 3.0 * Vdseff / T12));
                    dPigcd_dVd = 0.5 * T14 * dVdseff_dVd;
                    dPigcd_dVb = 0.5 * T14 * dVdseff_dVb;
                }

                T7 = -Pigcd * Vdseff; /* bugfix */
                dT7_dVg = -Vdseff * dPigcd_dVg - Pigcd * dVdseff_dVg;
                dT7_dVd = -Vdseff * dPigcd_dVd - Pigcd * dVdseff_dVd + dT7_dVg * dVgsteff_dVd;
                dT7_dVb = -Vdseff * dPigcd_dVb - Pigcd * dVdseff_dVb + dT7_dVg * dVgsteff_dVb;
                dT7_dVg *= dVgsteff_dVg;
                dT7_dVb *= dVbseff_dVb;
                T8 = T7 * T7 + 2.0e-4;
                dT8_dVg = 2.0 * T7;
                dT8_dVd = dT8_dVg * dT7_dVd;
                dT8_dVb = dT8_dVg * dT7_dVb;
                dT8_dVg *= dT7_dVg;

                if (T7 > EXP_THRESHOLD)
                {   T9 = MAX_EXP;
                    dT9_dVg = dT9_dVd = dT9_dVb = 0.0;
                }
                else if (T7 < -EXP_THRESHOLD)
                {   T9 = MIN_EXP;
                    dT9_dVg = dT9_dVd = dT9_dVb = 0.0;
                }
                else
                {   T9 = exp(T7);
                    dT9_dVg = T9 * dT7_dVg;
                    dT9_dVd = T9 * dT7_dVd;
                    dT9_dVb = T9 * dT7_dVb;
                }

                T0 = T8 * T8;
                T1 = T9 - 1.0 + 1.0e-4;
                T10 = (T1 - T7) / T8;
                dT10_dVg = (dT9_dVg - dT7_dVg - T10 * dT8_dVg) / T8;
                dT10_dVd = (dT9_dVd - dT7_dVd - T10 * dT8_dVd) / T8;
                dT10_dVb = (dT9_dVb - dT7_dVb - T10 * dT8_dVb) / T8;

                Igcs = Igc * T10;
                dIgcs_dVg = dIgc_dVg * T10 + Igc * dT10_dVg;
                dIgcs_dVd = dIgc_dVd * T10 + Igc * dT10_dVd;
                dIgcs_dVb = dIgc_dVb * T10 + Igc * dT10_dVb;

                T1 = T9 - 1.0 - 1.0e-4;
                T10 = (T7 * T9 - T1) / T8;
                dT10_dVg = (dT7_dVg * T9 + (T7 - 1.0) * dT9_dVg
                        - T10 * dT8_dVg) / T8;
                dT10_dVd = (dT7_dVd * T9 + (T7 - 1.0) * dT9_dVd
                        - T10 * dT8_dVd) / T8;
                dT10_dVb = (dT7_dVb * T9 + (T7 - 1.0) * dT9_dVb
                        - T10 * dT8_dVb) / T8;
                Igcd = Igc * T10;
                dIgcd_dVg = dIgc_dVg * T10 + Igc * dT10_dVg;
                dIgcd_dVd = dIgc_dVd * T10 + Igc * dT10_dVd;
                dIgcd_dVb = dIgc_dVb * T10 + Igc * dT10_dVb;

                here->FEFET1Igcs = Igcs;
                here->FEFET1gIgcsg = dIgcs_dVg;
                here->FEFET1gIgcsd = dIgcs_dVd;
                here->FEFET1gIgcsb =  dIgcs_dVb * dVbseff_dVb;
                here->FEFET1Igcd = Igcd;
                here->FEFET1gIgcdg = dIgcd_dVg;
                here->FEFET1gIgcdd = dIgcd_dVd;
                here->FEFET1gIgcdb = dIgcd_dVb * dVbseff_dVb;

                T0 = vgs - (pParam->FEFET1vfbsd + pParam->FEFET1vfbsdoff);
                vgs_eff = sqrt(T0 * T0 + 1.0e-4);
                dvgs_eff_dvg = T0 / vgs_eff;

                T2 = vgs * vgs_eff;
                dT2_dVg = vgs * dvgs_eff_dvg + vgs_eff;
                T11 = pParam->FEFET1AechvbEdge;
                T12 = pParam->FEFET1BechvbEdge;
                T3 = pParam->FEFET1aigsd * pParam->FEFET1cigsd
                    - pParam->FEFET1bigsd;
                T4 = pParam->FEFET1bigsd * pParam->FEFET1cigsd;
                T5 = T12 * (pParam->FEFET1aigsd + T3 * vgs_eff
                    - T4 * vgs_eff * vgs_eff);
                if (T5 > EXP_THRESHOLD)
                {   T6 = MAX_EXP;
                    dT6_dVg = 0.0;
                }
                else if (T5 < -EXP_THRESHOLD)
                {   T6 = MIN_EXP;
                    dT6_dVg = 0.0;
                }
                else
                {   T6 = exp(T5);
                    dT6_dVg = T6 * T12 * (T3 - 2.0 * T4 * vgs_eff)
                * dvgs_eff_dvg;
                }
                Igs = T11 * T2 * T6;
                dIgs_dVg = T11 * (T2 * dT6_dVg + T6 * dT2_dVg);
                dIgs_dVs = -dIgs_dVg;


                T0 = vgd - (pParam->FEFET1vfbsd + pParam->FEFET1vfbsdoff);
                vgd_eff = sqrt(T0 * T0 + 1.0e-4);
                dvgd_eff_dvg = T0 / vgd_eff;

                T2 = vgd * vgd_eff;
                dT2_dVg = vgd * dvgd_eff_dvg + vgd_eff;
                T5 = T12 * (pParam->FEFET1aigsd + T3 * vgd_eff
                    - T4 * vgd_eff * vgd_eff);
                if (T5 > EXP_THRESHOLD)
                {   T6 = MAX_EXP;
                    dT6_dVg = 0.0;
                }
                else if (T5 < -EXP_THRESHOLD)
                {   T6 = MIN_EXP;
                    dT6_dVg = 0.0;
                }
                else
                {   T6 = exp(T5);
                    dT6_dVg = T6 * T12 * (T3 - 2.0 * T4 * vgd_eff)
                            * dvgd_eff_dvg;
                }
                Igd = T11 * T2 * T6;
                dIgd_dVg = T11 * (T2 * dT6_dVg + T6 * dT2_dVg);
                dIgd_dVd = -dIgd_dVg;

                here->FEFET1Igs = Igs;
                here->FEFET1gIgsg = dIgs_dVg;
                here->FEFET1gIgss = dIgs_dVs;
                here->FEFET1Igd = Igd;
                here->FEFET1gIgdg = dIgd_dVg;
                here->FEFET1gIgdd = dIgd_dVd;
            }
            else
            {   here->FEFET1Igcs = here->FEFET1gIgcsg = here->FEFET1gIgcsd
                    = here->FEFET1gIgcsb = 0.0;
                here->FEFET1Igcd = here->FEFET1gIgcdg = here->FEFET1gIgcdd
                            = here->FEFET1gIgcdb = 0.0;
                here->FEFET1Igs = here->FEFET1gIgsg = here->FEFET1gIgss = 0.0;
                here->FEFET1Igd = here->FEFET1gIgdg = here->FEFET1gIgdd = 0.0;
            }

            if (model->FEFET1igbMod)
            {   T0 = tmp * pParam->FEFET1nigbacc;
            T1 = -Vgs_eff + Vbseff + Vfb;
                VxNVt = T1 / T0;
                if (VxNVt > EXP_THRESHOLD)
                {   Vaux = T1;
                    dVaux_dVg = -dVgs_eff_dVg;
                    dVaux_dVb = 1.0;
                }
                else if (VxNVt < -EXP_THRESHOLD)
                {   Vaux = T0 * log(1.0 + MIN_EXP);
                    dVaux_dVg = dVaux_dVb = 0.0;
                }
                else
                {   ExpVxNVt = exp(VxNVt);
                    Vaux = T0 * log(1.0 + ExpVxNVt);
                    dVaux_dVb = ExpVxNVt / (1.0 + ExpVxNVt); 
                    dVaux_dVg = -dVaux_dVb * dVgs_eff_dVg;
                }

            T2 = (Vgs_eff - Vbseff) * Vaux;
                dT2_dVg = dVgs_eff_dVg * Vaux + (Vgs_eff - Vbseff) * dVaux_dVg;
                dT2_dVb = -Vaux + (Vgs_eff - Vbseff) * dVaux_dVb;

                T11 = 4.97232e-7 * pParam->FEFET1weff
            * pParam->FEFET1leff * pParam->FEFET1ToxRatio;
                T12 = -7.45669e11 * model->FEFET1toxe;
            T3 = pParam->FEFET1aigbacc * pParam->FEFET1cigbacc
                    - pParam->FEFET1bigbacc;
                T4 = pParam->FEFET1bigbacc * pParam->FEFET1cigbacc;
            T5 = T12 * (pParam->FEFET1aigbacc + T3 * Voxacc
            - T4 * Voxacc * Voxacc);

                if (T5 > EXP_THRESHOLD)
                {   T6 = MAX_EXP;
                    dT6_dVg = dT6_dVb = 0.0;
                }
                else if (T5 < -EXP_THRESHOLD)
                {   T6 = MIN_EXP;
                    dT6_dVg = dT6_dVb = 0.0;
                }
                else
                {   T6 = exp(T5);
                    dT6_dVg = T6 * T12 * (T3 - 2.0 * T4 * Voxacc);
                    dT6_dVb = dT6_dVg * dVoxacc_dVb;
                    dT6_dVg *= dVoxacc_dVg;
                }

                Igbacc = T11 * T2 * T6;
                dIgbacc_dVg = T11 * (T2 * dT6_dVg + T6 * dT2_dVg);
                dIgbacc_dVb = T11 * (T2 * dT6_dVb + T6 * dT2_dVb);


                T0 = tmp * pParam->FEFET1nigbinv;
                T1 = Voxdepinv - pParam->FEFET1eigbinv;
                VxNVt = T1 / T0;
                if (VxNVt > EXP_THRESHOLD)
                {   Vaux = T1;
                    dVaux_dVg = dVoxdepinv_dVg;
                    dVaux_dVd = dVoxdepinv_dVd;
                    dVaux_dVb = dVoxdepinv_dVb;
                }
                else if (VxNVt < -EXP_THRESHOLD)
                {   Vaux = T0 * log(1.0 + MIN_EXP);
                    dVaux_dVg = dVaux_dVd = dVaux_dVb = 0.0;
                }
                else
                {   ExpVxNVt = exp(VxNVt);
                    Vaux = T0 * log(1.0 + ExpVxNVt);
            dVaux_dVg = ExpVxNVt / (1.0 + ExpVxNVt);
                    dVaux_dVd = dVaux_dVg * dVoxdepinv_dVd;
                    dVaux_dVb = dVaux_dVg * dVoxdepinv_dVb;
                    dVaux_dVg *= dVoxdepinv_dVg;
                }

                T2 = (Vgs_eff - Vbseff) * Vaux;
                dT2_dVg = dVgs_eff_dVg * Vaux + (Vgs_eff - Vbseff) * dVaux_dVg;
                dT2_dVd = (Vgs_eff - Vbseff) * dVaux_dVd;
                dT2_dVb = -Vaux + (Vgs_eff - Vbseff) * dVaux_dVb;

                T11 *= 0.75610;
                T12 *= 1.31724;
                T3 = pParam->FEFET1aigbinv * pParam->FEFET1cigbinv
                    - pParam->FEFET1bigbinv;
                T4 = pParam->FEFET1bigbinv * pParam->FEFET1cigbinv;
                T5 = T12 * (pParam->FEFET1aigbinv + T3 * Voxdepinv
                    - T4 * Voxdepinv * Voxdepinv);

                if (T5 > EXP_THRESHOLD)
                {   T6 = MAX_EXP;
                    dT6_dVg = dT6_dVd = dT6_dVb = 0.0;
                }
                else if (T5 < -EXP_THRESHOLD)
                {   T6 = MIN_EXP;
                    dT6_dVg = dT6_dVd = dT6_dVb = 0.0;
                }
                else
                {   T6 = exp(T5);
                    dT6_dVg = T6 * T12 * (T3 - 2.0 * T4 * Voxdepinv);
                    dT6_dVd = dT6_dVg * dVoxdepinv_dVd;
                    dT6_dVb = dT6_dVg * dVoxdepinv_dVb;
                    dT6_dVg *= dVoxdepinv_dVg;
                }

                Igbinv = T11 * T2 * T6;
                dIgbinv_dVg = T11 * (T2 * dT6_dVg + T6 * dT2_dVg);
                dIgbinv_dVd = T11 * (T2 * dT6_dVd + T6 * dT2_dVd);
                dIgbinv_dVb = T11 * (T2 * dT6_dVb + T6 * dT2_dVb);

                here->FEFET1Igb = Igbinv + Igbacc;
                here->FEFET1gIgbg = dIgbinv_dVg + dIgbacc_dVg;
                here->FEFET1gIgbd = dIgbinv_dVd;
                here->FEFET1gIgbb = (dIgbinv_dVb + dIgbacc_dVb) * dVbseff_dVb;
            }
            else
            {  here->FEFET1Igb = here->FEFET1gIgbg = here->FEFET1gIgbd
                    = here->FEFET1gIgbs = here->FEFET1gIgbb = 0.0;   
            } /* End of Gate current */

        if (here->FEFET1nf != 1.0)
            {   cdrain *= here->FEFET1nf;
                here->FEFET1gds *= here->FEFET1nf;
                here->FEFET1gm *= here->FEFET1nf;
                here->FEFET1gmbs *= here->FEFET1nf;
            here->FEFET1IdovVds *= here->FEFET1nf;
                    
                here->FEFET1gbbs *= here->FEFET1nf;
                here->FEFET1gbgs *= here->FEFET1nf;
                here->FEFET1gbds *= here->FEFET1nf;
                here->FEFET1csub *= here->FEFET1nf;

                here->FEFET1Igidl *= here->FEFET1nf;
                here->FEFET1ggidld *= here->FEFET1nf;
                here->FEFET1ggidlg *= here->FEFET1nf;
            here->FEFET1ggidlb *= here->FEFET1nf;
        
                here->FEFET1Igisl *= here->FEFET1nf;
                here->FEFET1ggisls *= here->FEFET1nf;
                here->FEFET1ggislg *= here->FEFET1nf;
                here->FEFET1ggislb *= here->FEFET1nf;

                here->FEFET1Igcs *= here->FEFET1nf;
                here->FEFET1gIgcsg *= here->FEFET1nf;
                here->FEFET1gIgcsd *= here->FEFET1nf;
                here->FEFET1gIgcsb *= here->FEFET1nf;
                here->FEFET1Igcd *= here->FEFET1nf;
                here->FEFET1gIgcdg *= here->FEFET1nf;
                here->FEFET1gIgcdd *= here->FEFET1nf;
                here->FEFET1gIgcdb *= here->FEFET1nf;

                here->FEFET1Igs *= here->FEFET1nf;
                here->FEFET1gIgsg *= here->FEFET1nf;
                here->FEFET1gIgss *= here->FEFET1nf;
                here->FEFET1Igd *= here->FEFET1nf;
                here->FEFET1gIgdg *= here->FEFET1nf;
                here->FEFET1gIgdd *= here->FEFET1nf;

                here->FEFET1Igb *= here->FEFET1nf;
                here->FEFET1gIgbg *= here->FEFET1nf;
                here->FEFET1gIgbd *= here->FEFET1nf;
                here->FEFET1gIgbb *= here->FEFET1nf;
        }

            here->FEFET1ggidls = -(here->FEFET1ggidld + here->FEFET1ggidlg
                    + here->FEFET1ggidlb);
            here->FEFET1ggisld = -(here->FEFET1ggisls + here->FEFET1ggislg
                    + here->FEFET1ggislb);
            here->FEFET1gIgbs = -(here->FEFET1gIgbg + here->FEFET1gIgbd
                            + here->FEFET1gIgbb);
            here->FEFET1gIgcss = -(here->FEFET1gIgcsg + here->FEFET1gIgcsd
                                + here->FEFET1gIgcsb);
            here->FEFET1gIgcds = -(here->FEFET1gIgcdg + here->FEFET1gIgcdd
                                + here->FEFET1gIgcdb);
        here->FEFET1cd = cdrain;


            if (model->FEFET1tnoiMod == 0)
            {   Abulk = Abulk0 * pParam->FEFET1abulkCVfactor;
                Vdsat = Vgsteff / Abulk;
                T0 = Vdsat - Vds - DELTA_4;
                T1 = sqrt(T0 * T0 + 4.0 * DELTA_4 * Vdsat);
                if (T0 >= 0.0)
                    Vdseff = Vdsat - 0.5 * (T0 + T1);
                else
                {   T3 = (DELTA_4 + DELTA_4) / (T1 - T0);
                    T4 = 1.0 - T3;
                    T5 = Vdsat * T3 / (T1 - T0);
                    Vdseff = Vdsat * T4;
                }
                if (Vds == 0.0)
                    Vdseff = 0.0;

                T0 = Abulk * Vdseff;
                T1 = 12.0 * (Vgsteff - 0.5 * T0 + 1.0e-20);
                T2 = Vdseff / T1;
                T3 = T0 * T2;
                here->FEFET1qinv = Coxeff * pParam->FEFET1weffCV * here->FEFET1nf
                                * pParam->FEFET1leffCV
                                * (Vgsteff - 0.5 * T0 + Abulk * T3);
            }

        /*
            *  FEFET1 C-V begins
        */

            if ((model->FEFET1xpart < 0) || (!ChargeComputationNeeded))
        {   qgate  = qdrn = qsrc = qbulk = 0.0;
                here->FEFET1cggb = here->FEFET1cgsb = here->FEFET1cgdb = 0.0;
                here->FEFET1cdgb = here->FEFET1cdsb = here->FEFET1cddb = 0.0;
                here->FEFET1cbgb = here->FEFET1cbsb = here->FEFET1cbdb = 0.0;
                here->FEFET1csgb = here->FEFET1cssb = here->FEFET1csdb = 0.0;
                here->FEFET1cgbb = here->FEFET1csbb = here->FEFET1cdbb = here->FEFET1cbbb = 0.0;
                here->FEFET1cqdb = here->FEFET1cqsb = here->FEFET1cqgb 
                                = here->FEFET1cqbb = 0.0;
                here->FEFET1gtau = 0.0;
                goto finished;
            }
        else if (model->FEFET1capMod == 0)
        {
                if (Vbseff < 0.0)
            {   Vbseff = Vbs;
                    dVbseff_dVb = 1.0;
                }
            else
            {   Vbseff = pParam->FEFET1phi - Phis;
                    dVbseff_dVb = -dPhis_dVb;
                }

                Vfb = pParam->FEFET1vfbcv;
                Vth = Vfb + pParam->FEFET1phi + pParam->FEFET1k1ox * sqrtPhis; 
                Vgst = Vgs_eff - Vth;
                dVth_dVb = pParam->FEFET1k1ox * dsqrtPhis_dVb; 
                dVgst_dVb = -dVth_dVb;
                dVgst_dVg = dVgs_eff_dVg; 

                CoxWL = model->FEFET1coxe * pParam->FEFET1weffCV
                        * pParam->FEFET1leffCV * here->FEFET1nf;
                Arg1 = Vgs_eff - Vbseff - Vfb;

                if (Arg1 <= 0.0)
            {   qgate = CoxWL * Arg1;
                    qbulk = -qgate;
                    qdrn = 0.0;

                    here->FEFET1cggb = CoxWL * dVgs_eff_dVg;
                    here->FEFET1cgdb = 0.0;
                    here->FEFET1cgsb = CoxWL * (dVbseff_dVb - dVgs_eff_dVg);

                    here->FEFET1cdgb = 0.0;
                    here->FEFET1cddb = 0.0;
                    here->FEFET1cdsb = 0.0;

                    here->FEFET1cbgb = -CoxWL * dVgs_eff_dVg;
                    here->FEFET1cbdb = 0.0;
                    here->FEFET1cbsb = -here->FEFET1cgsb;
                } /* Arg1 <= 0.0, end of accumulation */
            else if (Vgst <= 0.0)
            {   T1 = 0.5 * pParam->FEFET1k1ox;
                T2 = sqrt(T1 * T1 + Arg1);
                qgate = CoxWL * pParam->FEFET1k1ox * (T2 - T1);
                    qbulk = -qgate;
                    qdrn = 0.0;

                T0 = CoxWL * T1 / T2;
                here->FEFET1cggb = T0 * dVgs_eff_dVg;
                here->FEFET1cgdb = 0.0;
                    here->FEFET1cgsb = T0 * (dVbseff_dVb - dVgs_eff_dVg);
    
                    here->FEFET1cdgb = 0.0;
                    here->FEFET1cddb = 0.0;
                    here->FEFET1cdsb = 0.0;

                    here->FEFET1cbgb = -here->FEFET1cggb;
                    here->FEFET1cbdb = 0.0;
                    here->FEFET1cbsb = -here->FEFET1cgsb;
                } /* Vgst <= 0.0, end of depletion */
            else
            {   One_Third_CoxWL = CoxWL / 3.0;
                    Two_Third_CoxWL = 2.0 * One_Third_CoxWL;

                    AbulkCV = Abulk0 * pParam->FEFET1abulkCVfactor;
                    dAbulkCV_dVb = pParam->FEFET1abulkCVfactor * dAbulk0_dVb;
                Vdsat = Vgst / AbulkCV;
                dVdsat_dVg = dVgs_eff_dVg / AbulkCV;
                dVdsat_dVb = - (Vdsat * dAbulkCV_dVb + dVth_dVb)/ AbulkCV; 

                    if (model->FEFET1xpart > 0.5)
            {   /* 0/100 Charge partition model */
                if (Vdsat <= Vds)
                {   /* saturation region */
                        T1 = Vdsat / 3.0;
                        qgate = CoxWL * (Vgs_eff - Vfb
                        - pParam->FEFET1phi - T1);
                        T2 = -Two_Third_CoxWL * Vgst;
                        qbulk = -(qgate + T2);
                        qdrn = 0.0;

                        here->FEFET1cggb = One_Third_CoxWL * (3.0
                        - dVdsat_dVg) * dVgs_eff_dVg;
                        T2 = -One_Third_CoxWL * dVdsat_dVb;
                        here->FEFET1cgsb = -(here->FEFET1cggb + T2);
                            here->FEFET1cgdb = 0.0;
        
                            here->FEFET1cdgb = 0.0;
                            here->FEFET1cddb = 0.0;
                            here->FEFET1cdsb = 0.0;

                        here->FEFET1cbgb = -(here->FEFET1cggb
                        - Two_Third_CoxWL * dVgs_eff_dVg);
                        T3 = -(T2 + Two_Third_CoxWL * dVth_dVb);
                        here->FEFET1cbsb = -(here->FEFET1cbgb + T3);
                            here->FEFET1cbdb = 0.0;
                    }
                else
                {   /* linear region */
                Alphaz = Vgst / Vdsat;
                        T1 = 2.0 * Vdsat - Vds;
                        T2 = Vds / (3.0 * T1);
                        T3 = T2 * Vds;
                        T9 = 0.25 * CoxWL;
                        T4 = T9 * Alphaz;
                        T7 = 2.0 * Vds - T1 - 3.0 * T3;
                        T8 = T3 - T1 - 2.0 * Vds;
                        qgate = CoxWL * (Vgs_eff - Vfb 
                        - pParam->FEFET1phi - 0.5 * (Vds - T3));
                        T10 = T4 * T8;
                        qdrn = T4 * T7;
                        qbulk = -(qgate + qdrn + T10);
    
                            T5 = T3 / T1;
                            here->FEFET1cggb = CoxWL * (1.0 - T5 * dVdsat_dVg)
                        * dVgs_eff_dVg;
                            T11 = -CoxWL * T5 * dVdsat_dVb;
                            here->FEFET1cgdb = CoxWL * (T2 - 0.5 + 0.5 * T5);
                            here->FEFET1cgsb = -(here->FEFET1cggb + T11
                                            + here->FEFET1cgdb);
                            T6 = 1.0 / Vdsat;
                            dAlphaz_dVg = T6 * (1.0 - Alphaz * dVdsat_dVg);
                            dAlphaz_dVb = -T6 * (dVth_dVb + Alphaz * dVdsat_dVb);
                            T7 = T9 * T7;
                            T8 = T9 * T8;
                            T9 = 2.0 * T4 * (1.0 - 3.0 * T5);
                            here->FEFET1cdgb = (T7 * dAlphaz_dVg - T9
                        * dVdsat_dVg) * dVgs_eff_dVg;
                            T12 = T7 * dAlphaz_dVb - T9 * dVdsat_dVb;
                            here->FEFET1cddb = T4 * (3.0 - 6.0 * T2 - 3.0 * T5);
                            here->FEFET1cdsb = -(here->FEFET1cdgb + T12
                                            + here->FEFET1cddb);

                            T9 = 2.0 * T4 * (1.0 + T5);
                            T10 = (T8 * dAlphaz_dVg - T9 * dVdsat_dVg)
                    * dVgs_eff_dVg;
                            T11 = T8 * dAlphaz_dVb - T9 * dVdsat_dVb;
                            T12 = T4 * (2.0 * T2 + T5 - 1.0); 
                            T0 = -(T10 + T11 + T12);

                            here->FEFET1cbgb = -(here->FEFET1cggb
                        + here->FEFET1cdgb + T10);
                            here->FEFET1cbdb = -(here->FEFET1cgdb 
                        + here->FEFET1cddb + T12);
                            here->FEFET1cbsb = -(here->FEFET1cgsb
                        + here->FEFET1cdsb + T0);
                        }
                    }
            else if (model->FEFET1xpart < 0.5)
            {   /* 40/60 Charge partition model */
                if (Vds >= Vdsat)
                {   /* saturation region */
                        T1 = Vdsat / 3.0;
                        qgate = CoxWL * (Vgs_eff - Vfb
                        - pParam->FEFET1phi - T1);
                        T2 = -Two_Third_CoxWL * Vgst;
                        qbulk = -(qgate + T2);
                        qdrn = 0.4 * T2;

                        here->FEFET1cggb = One_Third_CoxWL * (3.0 
                        - dVdsat_dVg) * dVgs_eff_dVg;
                        T2 = -One_Third_CoxWL * dVdsat_dVb;
                        here->FEFET1cgsb = -(here->FEFET1cggb + T2);
                        here->FEFET1cgdb = 0.0;
        
                T3 = 0.4 * Two_Third_CoxWL;
                            here->FEFET1cdgb = -T3 * dVgs_eff_dVg;
                            here->FEFET1cddb = 0.0;
                T4 = T3 * dVth_dVb;
                            here->FEFET1cdsb = -(T4 + here->FEFET1cdgb);

                        here->FEFET1cbgb = -(here->FEFET1cggb 
                        - Two_Third_CoxWL * dVgs_eff_dVg);
                        T3 = -(T2 + Two_Third_CoxWL * dVth_dVb);
                        here->FEFET1cbsb = -(here->FEFET1cbgb + T3);
                            here->FEFET1cbdb = 0.0;
                    }
                else
                {   /* linear region  */
                Alphaz = Vgst / Vdsat;
                T1 = 2.0 * Vdsat - Vds;
                T2 = Vds / (3.0 * T1);
                T3 = T2 * Vds;
                T9 = 0.25 * CoxWL;
                T4 = T9 * Alphaz;
                qgate = CoxWL * (Vgs_eff - Vfb - pParam->FEFET1phi
                    - 0.5 * (Vds - T3));

                T5 = T3 / T1;
                            here->FEFET1cggb = CoxWL * (1.0 - T5 * dVdsat_dVg)
                        * dVgs_eff_dVg;
                            tmp = -CoxWL * T5 * dVdsat_dVb;
                            here->FEFET1cgdb = CoxWL * (T2 - 0.5 + 0.5 * T5);
                            here->FEFET1cgsb = -(here->FEFET1cggb 
                        + here->FEFET1cgdb + tmp);

                T6 = 1.0 / Vdsat;
                            dAlphaz_dVg = T6 * (1.0 - Alphaz * dVdsat_dVg);
                            dAlphaz_dVb = -T6 * (dVth_dVb + Alphaz * dVdsat_dVb);

                T6 = 8.0 * Vdsat * Vdsat - 6.0 * Vdsat * Vds
                    + 1.2 * Vds * Vds;
                T8 = T2 / T1;
                T7 = Vds - T1 - T8 * T6;
                qdrn = T4 * T7;
                T7 *= T9;
                tmp = T8 / T1;
                tmp1 = T4 * (2.0 - 4.0 * tmp * T6
                    + T8 * (16.0 * Vdsat - 6.0 * Vds));

                            here->FEFET1cdgb = (T7 * dAlphaz_dVg - tmp1
                        * dVdsat_dVg) * dVgs_eff_dVg;
                            T10 = T7 * dAlphaz_dVb - tmp1 * dVdsat_dVb;
                            here->FEFET1cddb = T4 * (2.0 - (1.0 / (3.0 * T1
                        * T1) + 2.0 * tmp) * T6 + T8
                        * (6.0 * Vdsat - 2.4 * Vds));
                            here->FEFET1cdsb = -(here->FEFET1cdgb 
                        + T10 + here->FEFET1cddb);

                T7 = 2.0 * (T1 + T3);
                qbulk = -(qgate - T4 * T7);
                T7 *= T9;
                T0 = 4.0 * T4 * (1.0 - T5);
                T12 = (-T7 * dAlphaz_dVg - here->FEFET1cdgb
                    - T0 * dVdsat_dVg) * dVgs_eff_dVg;
                T11 = -T7 * dAlphaz_dVb - T10 - T0 * dVdsat_dVb;
                T10 = -4.0 * T4 * (T2 - 0.5 + 0.5 * T5) 
                    - here->FEFET1cddb;
                            tmp = -(T10 + T11 + T12);

                            here->FEFET1cbgb = -(here->FEFET1cggb 
                        + here->FEFET1cdgb + T12);
                            here->FEFET1cbdb = -(here->FEFET1cgdb
                        + here->FEFET1cddb + T10);  
                            here->FEFET1cbsb = -(here->FEFET1cgsb
                        + here->FEFET1cdsb + tmp);
                        }
                    }
            else
            {   /* 50/50 partitioning */
                if (Vds >= Vdsat)
                {   /* saturation region */
                        T1 = Vdsat / 3.0;
                        qgate = CoxWL * (Vgs_eff - Vfb
                        - pParam->FEFET1phi - T1);
                        T2 = -Two_Third_CoxWL * Vgst;
                        qbulk = -(qgate + T2);
                        qdrn = 0.5 * T2;

                        here->FEFET1cggb = One_Third_CoxWL * (3.0
                        - dVdsat_dVg) * dVgs_eff_dVg;
                        T2 = -One_Third_CoxWL * dVdsat_dVb;
                        here->FEFET1cgsb = -(here->FEFET1cggb + T2);
                        here->FEFET1cgdb = 0.0;
        
                            here->FEFET1cdgb = -One_Third_CoxWL * dVgs_eff_dVg;
                            here->FEFET1cddb = 0.0;
                T4 = One_Third_CoxWL * dVth_dVb;
                            here->FEFET1cdsb = -(T4 + here->FEFET1cdgb);

                        here->FEFET1cbgb = -(here->FEFET1cggb 
                        - Two_Third_CoxWL * dVgs_eff_dVg);
                        T3 = -(T2 + Two_Third_CoxWL * dVth_dVb);
                        here->FEFET1cbsb = -(here->FEFET1cbgb + T3);
                            here->FEFET1cbdb = 0.0;
                    }
                else
                {   /* linear region */
                Alphaz = Vgst / Vdsat;
                T1 = 2.0 * Vdsat - Vds;
                T2 = Vds / (3.0 * T1);
                T3 = T2 * Vds;
                T9 = 0.25 * CoxWL;
                T4 = T9 * Alphaz;
                qgate = CoxWL * (Vgs_eff - Vfb - pParam->FEFET1phi
                    - 0.5 * (Vds - T3));

                T5 = T3 / T1;
                            here->FEFET1cggb = CoxWL * (1.0 - T5 * dVdsat_dVg)
                        * dVgs_eff_dVg;
                            tmp = -CoxWL * T5 * dVdsat_dVb;
                            here->FEFET1cgdb = CoxWL * (T2 - 0.5 + 0.5 * T5);
                            here->FEFET1cgsb = -(here->FEFET1cggb 
                        + here->FEFET1cgdb + tmp);

                T6 = 1.0 / Vdsat;
                            dAlphaz_dVg = T6 * (1.0 - Alphaz * dVdsat_dVg);
                            dAlphaz_dVb = -T6 * (dVth_dVb + Alphaz * dVdsat_dVb);

                T7 = T1 + T3;
                qdrn = -T4 * T7;
                qbulk = - (qgate + qdrn + qdrn);
                T7 *= T9;
                T0 = T4 * (2.0 * T5 - 2.0);

                            here->FEFET1cdgb = (T0 * dVdsat_dVg - T7
                        * dAlphaz_dVg) * dVgs_eff_dVg;
                T12 = T0 * dVdsat_dVb - T7 * dAlphaz_dVb;
                            here->FEFET1cddb = T4 * (1.0 - 2.0 * T2 - T5);
                            here->FEFET1cdsb = -(here->FEFET1cdgb + T12
                                            + here->FEFET1cddb);

                            here->FEFET1cbgb = -(here->FEFET1cggb
                        + 2.0 * here->FEFET1cdgb);
                            here->FEFET1cbdb = -(here->FEFET1cgdb
                        + 2.0 * here->FEFET1cddb);
                            here->FEFET1cbsb = -(here->FEFET1cgsb
                        + 2.0 * here->FEFET1cdsb);
                        } /* end of linear region */
                } /* end of 50/50 partition */
            } /* end of inversion */
            } /* end of capMod=0 */ 
        else
        {   if (Vbseff < 0.0)
            {   VbseffCV = Vbseff;
                    dVbseffCV_dVb = 1.0;
                }
            else
            {   VbseffCV = pParam->FEFET1phi - Phis;
                    dVbseffCV_dVb = -dPhis_dVb;
                }

                CoxWL = model->FEFET1coxe * pParam->FEFET1weffCV
                * pParam->FEFET1leffCV * here->FEFET1nf;

                /* Seperate VgsteffCV with noff and voffcv */
                noff = n * pParam->FEFET1noff;
                dnoff_dVd = pParam->FEFET1noff * dn_dVd;
                dnoff_dVb = pParam->FEFET1noff * dn_dVb;
                T0 = Vtm * noff;
                voffcv = pParam->FEFET1voffcv;
                VgstNVt = (Vgst - voffcv) / T0;

                if (VgstNVt > EXP_THRESHOLD)
                {   Vgsteff = Vgst - voffcv;
                    dVgsteff_dVg = dVgs_eff_dVg;
                    dVgsteff_dVd = -dVth_dVd;
                    dVgsteff_dVb = -dVth_dVb;
                }
                else if (VgstNVt < -EXP_THRESHOLD)
                {   Vgsteff = T0 * log(1.0 + MIN_EXP);
                    dVgsteff_dVg = 0.0;
                    dVgsteff_dVd = Vgsteff / noff;
                    dVgsteff_dVb = dVgsteff_dVd * dnoff_dVb;
                    dVgsteff_dVd *= dnoff_dVd;
                }
                else
                {   ExpVgst = exp(VgstNVt);
                    Vgsteff = T0 * log(1.0 + ExpVgst);
                    dVgsteff_dVg = ExpVgst / (1.0 + ExpVgst);
                    dVgsteff_dVd = -dVgsteff_dVg * (dVth_dVd + (Vgst - voffcv)
                                / noff * dnoff_dVd) + Vgsteff / noff * dnoff_dVd;
                    dVgsteff_dVb = -dVgsteff_dVg * (dVth_dVb + (Vgst - voffcv)
                                / noff * dnoff_dVb) + Vgsteff / noff * dnoff_dVb;
                    dVgsteff_dVg *= dVgs_eff_dVg;
                } /* End of VgsteffCV */


            if (model->FEFET1capMod == 1)
            {   Vfb = here->FEFET1vfbzb;
                    V3 = Vfb - Vgs_eff + VbseffCV - DELTA_3;
            if (Vfb <= 0.0)
                T0 = sqrt(V3 * V3 - 4.0 * DELTA_3 * Vfb);
            else
                T0 = sqrt(V3 * V3 + 4.0 * DELTA_3 * Vfb);

            T1 = 0.5 * (1.0 + V3 / T0);
            Vfbeff = Vfb - 0.5 * (V3 + T0);
            dVfbeff_dVg = T1 * dVgs_eff_dVg;
            dVfbeff_dVb = -T1 * dVbseffCV_dVb;
            Qac0 = CoxWL * (Vfbeff - Vfb);
            dQac0_dVg = CoxWL * dVfbeff_dVg;
            dQac0_dVb = CoxWL * dVfbeff_dVb;

                    T0 = 0.5 * pParam->FEFET1k1ox;
            T3 = Vgs_eff - Vfbeff - VbseffCV - Vgsteff;
                    if (pParam->FEFET1k1ox == 0.0)
                    {   T1 = 0.0;
                        T2 = 0.0;
                    }
            else if (T3 < 0.0)
            {   T1 = T0 + T3 / pParam->FEFET1k1ox;
                        T2 = CoxWL;
            }
            else
            {   T1 = sqrt(T0 * T0 + T3);
                        T2 = CoxWL * T0 / T1;
            }

            Qsub0 = CoxWL * pParam->FEFET1k1ox * (T1 - T0);

                    dQsub0_dVg = T2 * (dVgs_eff_dVg - dVfbeff_dVg - dVgsteff_dVg);
                    dQsub0_dVd = -T2 * dVgsteff_dVd;
                    dQsub0_dVb = -T2 * (dVfbeff_dVb + dVbseffCV_dVb 
                                + dVgsteff_dVb);

                    AbulkCV = Abulk0 * pParam->FEFET1abulkCVfactor;
                    dAbulkCV_dVb = pParam->FEFET1abulkCVfactor * dAbulk0_dVb;
                VdsatCV = Vgsteff / AbulkCV;

            T0 = VdsatCV - Vds - DELTA_4;
            dT0_dVg = 1.0 / AbulkCV;
            dT0_dVb = -VdsatCV * dAbulkCV_dVb / AbulkCV; 
            T1 = sqrt(T0 * T0 + 4.0 * DELTA_4 * VdsatCV);
                    dT1_dVg = (T0 + DELTA_4 + DELTA_4) / T1;
                    dT1_dVd = -T0 / T1;
                    dT1_dVb = dT1_dVg * dT0_dVb;
            dT1_dVg *= dT0_dVg;
            if (T0 >= 0.0)
            {   VdseffCV = VdsatCV - 0.5 * (T0 + T1);
                        dVdseffCV_dVg = 0.5 * (dT0_dVg - dT1_dVg);
                        dVdseffCV_dVd = 0.5 * (1.0 - dT1_dVd);
                        dVdseffCV_dVb = 0.5 * (dT0_dVb - dT1_dVb);
            }
                    else
                    {   T3 = (DELTA_4 + DELTA_4) / (T1 - T0);
                T4 = 1.0 - T3;
                T5 = VdsatCV * T3 / (T1 - T0);
                VdseffCV = VdsatCV * T4;
                        dVdseffCV_dVg = dT0_dVg * T4 + T5 * (dT1_dVg - dT0_dVg);
                        dVdseffCV_dVd = T5 * (dT1_dVd + 1.0);
                        dVdseffCV_dVb = dT0_dVb * (1.0 - T5) + T5 * dT1_dVb;
                    }

                    if (Vds == 0.0)
                    {  VdseffCV = 0.0;
                        dVdseffCV_dVg = 0.0;
                        dVdseffCV_dVb = 0.0;
                    }

                T0 = AbulkCV * VdseffCV;
                    T1 = 12.0 * (Vgsteff - 0.5 * T0 + 1.0e-20);
            T2 = VdseffCV / T1;
            T3 = T0 * T2;

                    T4 = (1.0 - 12.0 * T2 * T2 * AbulkCV);
                    T5 = (6.0 * T0 * (4.0 * Vgsteff - T0) / (T1 * T1) - 0.5);
                    T6 = 12.0 * T2 * T2 * Vgsteff;

                    qgate = CoxWL * (Vgsteff - 0.5 * VdseffCV + T3);
                    Cgg1 = CoxWL * (T4 + T5 * dVdseffCV_dVg);
                    Cgd1 = CoxWL * T5 * dVdseffCV_dVd + Cgg1 * dVgsteff_dVd;
                    Cgb1 = CoxWL * (T5 * dVdseffCV_dVb + T6 * dAbulkCV_dVb)
                + Cgg1 * dVgsteff_dVb;
            Cgg1 *= dVgsteff_dVg;

            T7 = 1.0 - AbulkCV;
                    qbulk = CoxWL * T7 * (0.5 * VdseffCV - T3);
            T4 = -T7 * (T4 - 1.0);
            T5 = -T7 * T5;
            T6 = -(T7 * T6 + (0.5 * VdseffCV - T3));
                    Cbg1 = CoxWL * (T4 + T5 * dVdseffCV_dVg);
                    Cbd1 = CoxWL * T5 * dVdseffCV_dVd + Cbg1 * dVgsteff_dVd;
                    Cbb1 = CoxWL * (T5 * dVdseffCV_dVb + T6 * dAbulkCV_dVb)
                + Cbg1 * dVgsteff_dVb;
            Cbg1 *= dVgsteff_dVg;

                    if (model->FEFET1xpart > 0.5)
            {   /* 0/100 Charge petition model */
                T1 = T1 + T1;
                        qsrc = -CoxWL * (0.5 * Vgsteff + 0.25 * T0
                - T0 * T0 / T1);
                T7 = (4.0 * Vgsteff - T0) / (T1 * T1);
                T4 = -(0.5 + 24.0 * T0 * T0 / (T1 * T1));
                T5 = -(0.25 * AbulkCV - 12.0 * AbulkCV * T0 * T7);
                        T6 = -(0.25 * VdseffCV - 12.0 * T0 * VdseffCV * T7);
                        Csg = CoxWL * (T4 + T5 * dVdseffCV_dVg);
                        Csd = CoxWL * T5 * dVdseffCV_dVd + Csg * dVgsteff_dVd;
                        Csb = CoxWL * (T5 * dVdseffCV_dVb + T6 * dAbulkCV_dVb)
                + Csg * dVgsteff_dVb;
                Csg *= dVgsteff_dVg;
                    }
            else if (model->FEFET1xpart < 0.5)
            {   /* 40/60 Charge petition model */
                T1 = T1 / 12.0;
                T2 = 0.5 * CoxWL / (T1 * T1);
                T3 = Vgsteff * (2.0 * T0 * T0 / 3.0 + Vgsteff
                * (Vgsteff - 4.0 * T0 / 3.0))
                - 2.0 * T0 * T0 * T0 / 15.0;
                qsrc = -T2 * T3;
                T7 = 4.0 / 3.0 * Vgsteff * (Vgsteff - T0)
                + 0.4 * T0 * T0;
                T4 = -2.0 * qsrc / T1 - T2 * (Vgsteff * (3.0
                * Vgsteff - 8.0 * T0 / 3.0)
                + 2.0 * T0 * T0 / 3.0);
                T5 = (qsrc / T1 + T2 * T7) * AbulkCV;
                T6 = (qsrc / T1 * VdseffCV + T2 * T7 * VdseffCV);
                        Csg = (T4 + T5 * dVdseffCV_dVg);
                        Csd = T5 * dVdseffCV_dVd + Csg * dVgsteff_dVd;
                        Csb = (T5 * dVdseffCV_dVb + T6 * dAbulkCV_dVb)
                + Csg * dVgsteff_dVb;
                Csg *= dVgsteff_dVg;
                    }
            else
            {   /* 50/50 Charge petition model */
                        qsrc = -0.5 * (qgate + qbulk);
                        Csg = -0.5 * (Cgg1 + Cbg1);
                        Csb = -0.5 * (Cgb1 + Cbb1); 
                        Csd = -0.5 * (Cgd1 + Cbd1); 
                    }

            qgate += Qac0 + Qsub0;
            qbulk -= (Qac0 + Qsub0);
                    qdrn = -(qgate + qbulk + qsrc);

            Cgg = dQac0_dVg + dQsub0_dVg + Cgg1;
            Cgd = dQsub0_dVd + Cgd1;
            Cgb = dQac0_dVb + dQsub0_dVb + Cgb1;

            Cbg = Cbg1 - dQac0_dVg - dQsub0_dVg;
            Cbd = Cbd1 - dQsub0_dVd;
            Cbb = Cbb1 - dQac0_dVb - dQsub0_dVb;

            Cgb *= dVbseff_dVb;
            Cbb *= dVbseff_dVb;
            Csb *= dVbseff_dVb;

                    here->FEFET1cggb = Cgg;
                here->FEFET1cgsb = -(Cgg + Cgd + Cgb);
                here->FEFET1cgdb = Cgd;
                    here->FEFET1cdgb = -(Cgg + Cbg + Csg);
                here->FEFET1cdsb = (Cgg + Cgd + Cgb + Cbg + Cbd + Cbb
                        + Csg + Csd + Csb);
                here->FEFET1cddb = -(Cgd + Cbd + Csd);
                    here->FEFET1cbgb = Cbg;
                here->FEFET1cbsb = -(Cbg + Cbd + Cbb);
                here->FEFET1cbdb = Cbd;
            } 

                /* Charge-Thickness capMod (CTM) begins */
            else if (model->FEFET1capMod == 2)
            {   V3 = here->FEFET1vfbzb - Vgs_eff + VbseffCV - DELTA_3;
            if (here->FEFET1vfbzb <= 0.0)
                T0 = sqrt(V3 * V3 - 4.0 * DELTA_3 * here->FEFET1vfbzb);
            else
                T0 = sqrt(V3 * V3 + 4.0 * DELTA_3 * here->FEFET1vfbzb);

            T1 = 0.5 * (1.0 + V3 / T0);
            Vfbeff = here->FEFET1vfbzb - 0.5 * (V3 + T0);
            dVfbeff_dVg = T1 * dVgs_eff_dVg;
            dVfbeff_dVb = -T1 * dVbseffCV_dVb;

                    Cox = model->FEFET1coxp;
                    Tox = 1.0e8 * model->FEFET1toxp;
                    T0 = (Vgs_eff - VbseffCV - here->FEFET1vfbzb) / Tox;
                    dT0_dVg = dVgs_eff_dVg / Tox;
                    dT0_dVb = -dVbseffCV_dVb / Tox;

                    tmp = T0 * pParam->FEFET1acde;
                    if ((-EXP_THRESHOLD < tmp) && (tmp < EXP_THRESHOLD))
                    {   Tcen = pParam->FEFET1ldeb * exp(tmp);
                        dTcen_dVg = pParam->FEFET1acde * Tcen;
                        dTcen_dVb = dTcen_dVg * dT0_dVb;
                        dTcen_dVg *= dT0_dVg;
                    }
                    else if (tmp <= -EXP_THRESHOLD)
                    {   Tcen = pParam->FEFET1ldeb * MIN_EXP;
                        dTcen_dVg = dTcen_dVb = 0.0;
                    }
                    else
                    {   Tcen = pParam->FEFET1ldeb * MAX_EXP;
                        dTcen_dVg = dTcen_dVb = 0.0;
                    }

                    LINK = 1.0e-3 * model->FEFET1toxp;
                    V3 = pParam->FEFET1ldeb - Tcen - LINK;
                    V4 = sqrt(V3 * V3 + 4.0 * LINK * pParam->FEFET1ldeb);
                    Tcen = pParam->FEFET1ldeb - 0.5 * (V3 + V4);
                    T1 = 0.5 * (1.0 + V3 / V4);
                    dTcen_dVg *= T1;
                    dTcen_dVb *= T1;

                    Ccen = EPSSI / Tcen;
                    T2 = Cox / (Cox + Ccen);
                    Coxeff = T2 * Ccen;
                    T3 = -Ccen / Tcen;
                    dCoxeff_dVg = T2 * T2 * T3;
                    dCoxeff_dVb = dCoxeff_dVg * dTcen_dVb;
                    dCoxeff_dVg *= dTcen_dVg;
                    CoxWLcen = CoxWL * Coxeff / model->FEFET1coxe;

                    Qac0 = CoxWLcen * (Vfbeff - here->FEFET1vfbzb);
                    QovCox = Qac0 / Coxeff;
                    dQac0_dVg = CoxWLcen * dVfbeff_dVg
                                + QovCox * dCoxeff_dVg;
                    dQac0_dVb = CoxWLcen * dVfbeff_dVb 
                    + QovCox * dCoxeff_dVb;

                    T0 = 0.5 * pParam->FEFET1k1ox;
                    T3 = Vgs_eff - Vfbeff - VbseffCV - Vgsteff;
                    if (pParam->FEFET1k1ox == 0.0)
                    {   T1 = 0.0;
                        T2 = 0.0;
                    }
                    else if (T3 < 0.0)
                    {   T1 = T0 + T3 / pParam->FEFET1k1ox;
                        T2 = CoxWLcen;
                    }
                    else
                    {   T1 = sqrt(T0 * T0 + T3);
                        T2 = CoxWLcen * T0 / T1;
                    }

                    Qsub0 = CoxWLcen * pParam->FEFET1k1ox * (T1 - T0);
                    QovCox = Qsub0 / Coxeff;
                    dQsub0_dVg = T2 * (dVgs_eff_dVg - dVfbeff_dVg - dVgsteff_dVg)
                                + QovCox * dCoxeff_dVg;
                    dQsub0_dVd = -T2 * dVgsteff_dVd;
                    dQsub0_dVb = -T2 * (dVfbeff_dVb + dVbseffCV_dVb + dVgsteff_dVb)
                                + QovCox * dCoxeff_dVb;

            /* Gate-bias dependent delta Phis begins */
            if (pParam->FEFET1k1ox <= 0.0)
            {   Denomi = 0.25 * pParam->FEFET1moin * Vtm;
                        T0 = 0.5 * pParam->FEFET1sqrtPhi;
            }
            else
            {   Denomi = pParam->FEFET1moin * Vtm 
                    * pParam->FEFET1k1ox * pParam->FEFET1k1ox;
                        T0 = pParam->FEFET1k1ox * pParam->FEFET1sqrtPhi;
            }
                    T1 = 2.0 * T0 + Vgsteff;

            DeltaPhi = Vtm * log(1.0 + T1 * Vgsteff / Denomi);
            dDeltaPhi_dVg = 2.0 * Vtm * (T1 -T0) / (Denomi + T1 * Vgsteff);
            /* End of delta Phis */

            /* VgDP = Vgsteff - DeltaPhi */
            T0 = Vgsteff - DeltaPhi - 0.001;
            dT0_dVg = 1.0 - dDeltaPhi_dVg;
            T1 = sqrt(T0 * T0 + Vgsteff * 0.004);
            VgDP = 0.5 * (T0 + T1);
            dVgDP_dVg = 0.5 * (dT0_dVg + (T0 * dT0_dVg + 0.002) / T1);                  
                    
                    Tox += Tox; /* WDLiu: Tcen reevaluated below due to different Vgsteff */
                    T0 = (Vgsteff + here->FEFET1vtfbphi2) / Tox;
                    tmp = exp(0.7 * log(T0));
                    T1 = 1.0 + tmp;
                    T2 = 0.7 * tmp / (T0 * Tox);
                    Tcen = 1.9e-9 / T1;
                    dTcen_dVg = -Tcen * T2 / T1;
                    dTcen_dVd = dTcen_dVg * dVgsteff_dVd;
                    dTcen_dVb = dTcen_dVg * dVgsteff_dVb;
                    dTcen_dVg *= dVgsteff_dVg;

            Ccen = EPSSI / Tcen;
            T0 = Cox / (Cox + Ccen);
            Coxeff = T0 * Ccen;
            T1 = -Ccen / Tcen;
            dCoxeff_dVg = T0 * T0 * T1;
            dCoxeff_dVd = dCoxeff_dVg * dTcen_dVd;
            dCoxeff_dVb = dCoxeff_dVg * dTcen_dVb;
            dCoxeff_dVg *= dTcen_dVg;
            CoxWLcen = CoxWL * Coxeff / model->FEFET1coxe;

                    AbulkCV = Abulk0 * pParam->FEFET1abulkCVfactor;
                    dAbulkCV_dVb = pParam->FEFET1abulkCVfactor * dAbulk0_dVb;
                    VdsatCV = VgDP / AbulkCV;

                    T0 = VdsatCV - Vds - DELTA_4;
                    dT0_dVg = dVgDP_dVg / AbulkCV;
                    dT0_dVb = -VdsatCV * dAbulkCV_dVb / AbulkCV;
                    T1 = sqrt(T0 * T0 + 4.0 * DELTA_4 * VdsatCV);
                    dT1_dVg = (T0 + DELTA_4 + DELTA_4) / T1;
                    dT1_dVd = -T0 / T1;
                    dT1_dVb = dT1_dVg * dT0_dVb;
                    dT1_dVg *= dT0_dVg;
                    if (T0 >= 0.0)
                    {   VdseffCV = VdsatCV - 0.5 * (T0 + T1);
                        dVdseffCV_dVg = 0.5 * (dT0_dVg - dT1_dVg);
                        dVdseffCV_dVd = 0.5 * (1.0 - dT1_dVd);
                        dVdseffCV_dVb = 0.5 * (dT0_dVb - dT1_dVb);
                    }
                    else
                    {   T3 = (DELTA_4 + DELTA_4) / (T1 - T0);
                        T4 = 1.0 - T3;
                        T5 = VdsatCV * T3 / (T1 - T0);
                        VdseffCV = VdsatCV * T4;
                        dVdseffCV_dVg = dT0_dVg * T4 + T5 * (dT1_dVg - dT0_dVg);
                        dVdseffCV_dVd = T5 * (dT1_dVd + 1.0);
                        dVdseffCV_dVb = dT0_dVb * (1.0 - T5) + T5 * dT1_dVb;
                    }

                    if (Vds == 0.0)
                    {  VdseffCV = 0.0;
                        dVdseffCV_dVg = 0.0;
                        dVdseffCV_dVb = 0.0;
                    }

                    T0 = AbulkCV * VdseffCV;
            T1 = VgDP;
                    T2 = 12.0 * (T1 - 0.5 * T0 + 1.0e-20);
                    T3 = T0 / T2;
                    T4 = 1.0 - 12.0 * T3 * T3;
                    T5 = AbulkCV * (6.0 * T0 * (4.0 * T1 - T0) / (T2 * T2) - 0.5);
            T6 = T5 * VdseffCV / AbulkCV;

                    qgate = CoxWLcen * (T1 - T0 * (0.5 - T3));
            QovCox = qgate / Coxeff;
            Cgg1 = CoxWLcen * (T4 * dVgDP_dVg 
                + T5 * dVdseffCV_dVg);
            Cgd1 = CoxWLcen * T5 * dVdseffCV_dVd + Cgg1 
                * dVgsteff_dVd + QovCox * dCoxeff_dVd;
            Cgb1 = CoxWLcen * (T5 * dVdseffCV_dVb + T6 * dAbulkCV_dVb) 
                + Cgg1 * dVgsteff_dVb + QovCox * dCoxeff_dVb;
            Cgg1 = Cgg1 * dVgsteff_dVg + QovCox * dCoxeff_dVg;


                    T7 = 1.0 - AbulkCV;
                    T8 = T2 * T2;
                    T9 = 12.0 * T7 * T0 * T0 / (T8 * AbulkCV);
                    T10 = T9 * dVgDP_dVg;
                    T11 = -T7 * T5 / AbulkCV;
                    T12 = -(T9 * T1 / AbulkCV + VdseffCV * (0.5 - T0 / T2));

            qbulk = CoxWLcen * T7 * (0.5 * VdseffCV - T0 * VdseffCV / T2);
            QovCox = qbulk / Coxeff;
            Cbg1 = CoxWLcen * (T10 + T11 * dVdseffCV_dVg);
            Cbd1 = CoxWLcen * T11 * dVdseffCV_dVd + Cbg1
                * dVgsteff_dVd + QovCox * dCoxeff_dVd; 
            Cbb1 = CoxWLcen * (T11 * dVdseffCV_dVb + T12 * dAbulkCV_dVb)
                + Cbg1 * dVgsteff_dVb + QovCox * dCoxeff_dVb;
            Cbg1 = Cbg1 * dVgsteff_dVg + QovCox * dCoxeff_dVg;

                    if (model->FEFET1xpart > 0.5)
            {   /* 0/100 partition */
                qsrc = -CoxWLcen * (T1 / 2.0 + T0 / 4.0 
                - 0.5 * T0 * T0 / T2);
                QovCox = qsrc / Coxeff;
                T2 += T2;
                T3 = T2 * T2;
                T7 = -(0.25 - 12.0 * T0 * (4.0 * T1 - T0) / T3);
                T4 = -(0.5 + 24.0 * T0 * T0 / T3) * dVgDP_dVg;
                T5 = T7 * AbulkCV;
                T6 = T7 * VdseffCV;

                Csg = CoxWLcen * (T4 + T5 * dVdseffCV_dVg);
                Csd = CoxWLcen * T5 * dVdseffCV_dVd + Csg * dVgsteff_dVd
                + QovCox * dCoxeff_dVd;
                Csb = CoxWLcen * (T5 * dVdseffCV_dVb + T6 * dAbulkCV_dVb)
                + Csg * dVgsteff_dVb + QovCox * dCoxeff_dVb;
                Csg = Csg * dVgsteff_dVg + QovCox * dCoxeff_dVg;
                    }
            else if (model->FEFET1xpart < 0.5)
            {   /* 40/60 partition */
                T2 = T2 / 12.0;
                T3 = 0.5 * CoxWLcen / (T2 * T2);
                        T4 = T1 * (2.0 * T0 * T0 / 3.0 + T1 * (T1 - 4.0 
                            * T0 / 3.0)) - 2.0 * T0 * T0 * T0 / 15.0;
                qsrc = -T3 * T4;
                QovCox = qsrc / Coxeff;
                T8 = 4.0 / 3.0 * T1 * (T1 - T0) + 0.4 * T0 * T0;
                T5 = -2.0 * qsrc / T2 - T3 * (T1 * (3.0 * T1 - 8.0 
                * T0 / 3.0) + 2.0 * T0 * T0 / 3.0);
                T6 = AbulkCV * (qsrc / T2 + T3 * T8);
                T7 = T6 * VdseffCV / AbulkCV; 

                Csg = T5 * dVgDP_dVg + T6 * dVdseffCV_dVg; 
                Csd = Csg * dVgsteff_dVd + T6 * dVdseffCV_dVd 
                + QovCox * dCoxeff_dVd;
                Csb = Csg * dVgsteff_dVb + T6 * dVdseffCV_dVb 
                    + T7 * dAbulkCV_dVb + QovCox * dCoxeff_dVb; 
                Csg = Csg * dVgsteff_dVg + QovCox * dCoxeff_dVg;
                    }
            else
            {   /* 50/50 partition */
                        qsrc = -0.5 * qgate;
                        Csg = -0.5 * Cgg1;
                        Csd = -0.5 * Cgd1; 
                        Csb = -0.5 * Cgb1; 
                    }

            qgate += Qac0 + Qsub0 - qbulk;
            qbulk -= (Qac0 + Qsub0);
                    qdrn = -(qgate + qbulk + qsrc);

            Cbg = Cbg1 - dQac0_dVg - dQsub0_dVg;
            Cbd = Cbd1 - dQsub0_dVd;
            Cbb = Cbb1 - dQac0_dVb - dQsub0_dVb;

                    Cgg = Cgg1 - Cbg;
                    Cgd = Cgd1 - Cbd;
                    Cgb = Cgb1 - Cbb;

            Cgb *= dVbseff_dVb;
            Cbb *= dVbseff_dVb;
            Csb *= dVbseff_dVb;

                    here->FEFET1cggb = Cgg;
                here->FEFET1cgsb = -(Cgg + Cgd + Cgb);
                here->FEFET1cgdb = Cgd;
                    here->FEFET1cdgb = -(Cgg + Cbg + Csg);
                here->FEFET1cdsb = (Cgg + Cgd + Cgb + Cbg + Cbd + Cbb
                        + Csg + Csd + Csb);
                here->FEFET1cddb = -(Cgd + Cbd + Csd);
                    here->FEFET1cbgb = Cbg;
                here->FEFET1cbsb = -(Cbg + Cbd + Cbb);
                here->FEFET1cbdb = Cbd;
            }  /* End of CTM */
            }

            here->FEFET1csgb = - here->FEFET1cggb - here->FEFET1cdgb - here->FEFET1cbgb;
            here->FEFET1csdb = - here->FEFET1cgdb - here->FEFET1cddb - here->FEFET1cbdb;
            here->FEFET1cssb = - here->FEFET1cgsb - here->FEFET1cdsb - here->FEFET1cbsb;
            here->FEFET1cgbb = - here->FEFET1cgdb - here->FEFET1cggb - here->FEFET1cgsb;
            here->FEFET1cdbb = - here->FEFET1cddb - here->FEFET1cdgb - here->FEFET1cdsb;
            here->FEFET1cbbb = - here->FEFET1cbgb - here->FEFET1cbdb - here->FEFET1cbsb;
            here->FEFET1csbb = - here->FEFET1cgbb - here->FEFET1cdbb - here->FEFET1cbbb;
            here->FEFET1qgate = qgate;
            here->FEFET1qbulk = qbulk;
            here->FEFET1qdrn = qdrn;
            here->FEFET1qsrc = -(qgate + qbulk + qdrn);

            /* NQS begins */
            if ((here->FEFET1trnqsMod) || (here->FEFET1acnqsMod))
            {   here->FEFET1qchqs = qcheq = -(qbulk + qgate);
                here->FEFET1cqgb = -(here->FEFET1cggb + here->FEFET1cbgb);
                here->FEFET1cqdb = -(here->FEFET1cgdb + here->FEFET1cbdb);
                here->FEFET1cqsb = -(here->FEFET1cgsb + here->FEFET1cbsb);
                here->FEFET1cqbb = -(here->FEFET1cqgb + here->FEFET1cqdb
                                + here->FEFET1cqsb);

                CoxWL = model->FEFET1coxe * pParam->FEFET1weffCV * here->FEFET1nf
                        * pParam->FEFET1leffCV;
                T1 = here->FEFET1gcrg / CoxWL; /* 1 / tau */
                here->FEFET1gtau = T1 * ScalingFactor;

            if (here->FEFET1acnqsMod)
                    here->FEFET1taunet = 1.0 / T1;

                *(ckt->CKTstate0 + here->FEFET1qcheq) = qcheq;
                if (ckt->CKTmode & MODEINITTRAN)
                    *(ckt->CKTstate1 + here->FEFET1qcheq) =
                                    *(ckt->CKTstate0 + here->FEFET1qcheq);
            if (here->FEFET1trnqsMod)
                {   error = NIintegrate(ckt, &geq, &ceq, 0.0, here->FEFET1qcheq);
                    if (error)
                        return(error);
            }
            }


    finished: 

        /* Calculate junction C-V */
            if (ChargeComputationNeeded)
        {   czbd = model->FEFET1DunitAreaTempJctCap * here->FEFET1Adeff; /* bug fix */
                czbs = model->FEFET1SunitAreaTempJctCap * here->FEFET1Aseff;
                czbdsw = model->FEFET1DunitLengthSidewallTempJctCap * here->FEFET1Pdeff;
                czbdswg = model->FEFET1DunitLengthGateSidewallTempJctCap
                        * pParam->FEFET1weffCJ * here->FEFET1nf;
                czbssw = model->FEFET1SunitLengthSidewallTempJctCap * here->FEFET1Pseff;
                czbsswg = model->FEFET1SunitLengthGateSidewallTempJctCap
                        * pParam->FEFET1weffCJ * here->FEFET1nf;

                MJS = model->FEFET1SbulkJctBotGradingCoeff;
                MJSWS = model->FEFET1SbulkJctSideGradingCoeff;
            MJSWGS = model->FEFET1SbulkJctGateSideGradingCoeff;

                MJD = model->FEFET1DbulkJctBotGradingCoeff;
                MJSWD = model->FEFET1DbulkJctSideGradingCoeff;
                MJSWGD = model->FEFET1DbulkJctGateSideGradingCoeff;

                /* Source Bulk Junction */
            if (vbs_jct == 0.0)
            {   *(ckt->CKTstate0 + here->FEFET1qbs) = 0.0;
                    here->FEFET1capbs = czbs + czbssw + czbsswg;
            }
            else if (vbs_jct < 0.0)
            {   if (czbs > 0.0)
            {   arg = 1.0 - vbs_jct / model->FEFET1PhiBS;
                if (MJS == 0.5)
                            sarg = 1.0 / sqrt(arg);
                else
                            sarg = exp(-MJS * log(arg));
                        *(ckt->CKTstate0 + here->FEFET1qbs) = model->FEFET1PhiBS * czbs 
                            * (1.0 - arg * sarg) / (1.0 - MJS);
                here->FEFET1capbs = czbs * sarg;
            }
            else
            {   *(ckt->CKTstate0 + here->FEFET1qbs) = 0.0;
                here->FEFET1capbs = 0.0;
            }
            if (czbssw > 0.0)
            {   arg = 1.0 - vbs_jct / model->FEFET1PhiBSWS;
                if (MJSWS == 0.5)
                            sarg = 1.0 / sqrt(arg);
                else
                            sarg = exp(-MJSWS * log(arg));
                        *(ckt->CKTstate0 + here->FEFET1qbs) += model->FEFET1PhiBSWS * czbssw
                        * (1.0 - arg * sarg) / (1.0 - MJSWS);
                        here->FEFET1capbs += czbssw * sarg;
            }
            if (czbsswg > 0.0)
            {   arg = 1.0 - vbs_jct / model->FEFET1PhiBSWGS;
                if (MJSWGS == 0.5)
                            sarg = 1.0 / sqrt(arg);
                else
                            sarg = exp(-MJSWGS * log(arg));
                        *(ckt->CKTstate0 + here->FEFET1qbs) += model->FEFET1PhiBSWGS * czbsswg
                        * (1.0 - arg * sarg) / (1.0 - MJSWGS);
                        here->FEFET1capbs += czbsswg * sarg;
            }

                }
            else
            {   T0 = czbs + czbssw + czbsswg;
                    T1 = vbs_jct * (czbs * MJS / model->FEFET1PhiBS + czbssw * MJSWS 
                        / model->FEFET1PhiBSWS + czbsswg * MJSWGS / model->FEFET1PhiBSWGS);    
                    *(ckt->CKTstate0 + here->FEFET1qbs) = vbs_jct * (T0 + 0.5 * T1);
                    here->FEFET1capbs = T0 + T1;
                }

                /* Drain Bulk Junction */
            if (vbd_jct == 0.0)
            {   *(ckt->CKTstate0 + here->FEFET1qbd) = 0.0;
                    here->FEFET1capbd = czbd + czbdsw + czbdswg;
            }
            else if (vbd_jct < 0.0)
            {   if (czbd > 0.0)
            {   arg = 1.0 - vbd_jct / model->FEFET1PhiBD;
                if (MJD == 0.5)
                            sarg = 1.0 / sqrt(arg);
                else
                            sarg = exp(-MJD * log(arg));
                        *(ckt->CKTstate0 + here->FEFET1qbd) = model->FEFET1PhiBD* czbd 
                            * (1.0 - arg * sarg) / (1.0 - MJD);
                        here->FEFET1capbd = czbd * sarg;
            }
            else
            {   *(ckt->CKTstate0 + here->FEFET1qbd) = 0.0;
                        here->FEFET1capbd = 0.0;
            }
            if (czbdsw > 0.0)
            {   arg = 1.0 - vbd_jct / model->FEFET1PhiBSWD;
                if (MJSWD == 0.5)
                            sarg = 1.0 / sqrt(arg);
                else
                            sarg = exp(-MJSWD * log(arg));
                        *(ckt->CKTstate0 + here->FEFET1qbd) += model->FEFET1PhiBSWD * czbdsw 
                            * (1.0 - arg * sarg) / (1.0 - MJSWD);
                        here->FEFET1capbd += czbdsw * sarg;
            }
            if (czbdswg > 0.0)
            {   arg = 1.0 - vbd_jct / model->FEFET1PhiBSWGD;
                if (MJSWGD == 0.5)
                            sarg = 1.0 / sqrt(arg);
                else
                            sarg = exp(-MJSWGD * log(arg));
                        *(ckt->CKTstate0 + here->FEFET1qbd) += model->FEFET1PhiBSWGD * czbdswg
                        * (1.0 - arg * sarg) / (1.0 - MJSWGD);
                        here->FEFET1capbd += czbdswg * sarg;
            }
                }
            else
            {   T0 = czbd + czbdsw + czbdswg;
                    T1 = vbd_jct * (czbd * MJD / model->FEFET1PhiBD + czbdsw * MJSWD
                        / model->FEFET1PhiBSWD + czbdswg * MJSWGD / model->FEFET1PhiBSWGD);
                    *(ckt->CKTstate0 + here->FEFET1qbd) = vbd_jct * (T0 + 0.5 * T1);
                    here->FEFET1capbd = T0 + T1; 
                }
            }


            /*
            *  check convergence
            */

            if ((here->FEFET1off == 0) || (!(ckt->CKTmode & MODEINITFIX)))
        {   if (Check == 1)
            {   ckt->CKTnoncon++;
    #ifndef NEWCONV
                } 
            else
                {   if (here->FEFET1mode >= 0)
                    {   Idtot = here->FEFET1cd + here->FEFET1csub
                    + here->FEFET1Igidl - here->FEFET1cbd;
                    }
                    else
                    {   Idtot = here->FEFET1cd + here->FEFET1cbd - here->FEFET1Igidl; /* bugfix */
                    }
                    tol0 = ckt->CKTreltol * MAX(fabs(cdhat), fabs(Idtot))
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
                    }
            else if ((fabs(cgshat - Igstot) >= tol3) || (fabs(cgdhat - Igdtot) >= tol4)
                || (fabs(cgbhat - Igbtot) >= tol5))
                    {   ckt->CKTnoncon++;
                    }
                    else
                    {   Ibtot = here->FEFET1cbs + here->FEFET1cbd
                    - here->FEFET1Igidl - here->FEFET1Igisl - here->FEFET1csub;
                        tol6 = ckt->CKTreltol * MAX(fabs(cbhat), fabs(Ibtot))
                            + ckt->CKTabstol;
                        if (fabs(cbhat - Ibtot) > tol6)
                {   ckt->CKTnoncon++;
                        }
                    }
    #endif /* NEWCONV */
                }
            }
            *(ckt->CKTstate0 + here->FEFET1vds) = vds;
            *(ckt->CKTstate0 + here->FEFET1vgs) = vgs;
            *(ckt->CKTstate0 + here->FEFET1vbs) = vbs;
            *(ckt->CKTstate0 + here->FEFET1vbd) = vbd;
        *(ckt->CKTstate0 + here->FEFET1vges) = vges;
        *(ckt->CKTstate0 + here->FEFET1vgms) = vgms;
            *(ckt->CKTstate0 + here->FEFET1vdbs) = vdbs;
            *(ckt->CKTstate0 + here->FEFET1vdbd) = vdbd;
            *(ckt->CKTstate0 + here->FEFET1vsbs) = vsbs;
            *(ckt->CKTstate0 + here->FEFET1vses) = vses;
            *(ckt->CKTstate0 + here->FEFET1vdes) = vdes;
            *(ckt->CKTstate0 + here->FEFET1qdef) = qdef;


            if (!ChargeComputationNeeded)
                goto line850; 

        if (here->FEFET1rgateMod == 3) 
        {   
                vgdx = vgmd; 
                vgsx = vgms;
        }  
        else  /* For rgateMod == 0, 1 and 2 */
        {
                vgdx = vgd;
                vgsx = vgs;
        }
        if (model->FEFET1capMod == 0) 
        {  
                cgdo = pParam->FEFET1cgdo; 
                qgdo = pParam->FEFET1cgdo * vgdx;
                cgso = pParam->FEFET1cgso;
                qgso = pParam->FEFET1cgso * vgsx;
        }
        else /* For both capMod == 1 and 2 */
        {   T0 = vgdx + DELTA_1;
            T1 = sqrt(T0 * T0 + 4.0 * DELTA_1);
            T2 = 0.5 * (T0 - T1);

            T3 = pParam->FEFET1weffCV * pParam->FEFET1cgdl;
            T4 = sqrt(1.0 - 4.0 * T2 / pParam->FEFET1ckappad); 
            cgdo = pParam->FEFET1cgdo + T3 - T3 * (1.0 - 1.0 / T4)
            * (0.5 - 0.5 * T0 / T1);
            qgdo = (pParam->FEFET1cgdo + T3) * vgdx - T3 * (T2
            + 0.5 * pParam->FEFET1ckappad * (T4 - 1.0));

            T0 = vgsx + DELTA_1;
            T1 = sqrt(T0 * T0 + 4.0 * DELTA_1);
            T2 = 0.5 * (T0 - T1);
            T3 = pParam->FEFET1weffCV * pParam->FEFET1cgsl;
            T4 = sqrt(1.0 - 4.0 * T2 / pParam->FEFET1ckappas);
            cgso = pParam->FEFET1cgso + T3 - T3 * (1.0 - 1.0 / T4)
            * (0.5 - 0.5 * T0 / T1);
            qgso = (pParam->FEFET1cgso + T3) * vgsx - T3 * (T2
            + 0.5 * pParam->FEFET1ckappas * (T4 - 1.0));
        }

        if (here->FEFET1nf != 1.0)
        {   cgdo *= here->FEFET1nf;
            cgso *= here->FEFET1nf;
                qgdo *= here->FEFET1nf;
                qgso *= here->FEFET1nf;
        }	
            here->FEFET1cgdo = cgdo;
            here->FEFET1qgdo = qgdo;
            here->FEFET1cgso = cgso;
            here->FEFET1qgso = qgso;

    #ifndef NOBYPASS
    line755:
    #endif
            ag0 = ckt->CKTag[0];
            if (here->FEFET1mode > 0)
            {   if (here->FEFET1trnqsMod == 0)
                {   qdrn -= qgdo;
            if (here->FEFET1rgateMod == 3)
            {   gcgmgmb = (cgdo + cgso + pParam->FEFET1cgbo) * ag0;
                gcgmdb = -cgdo * ag0;
                        gcgmsb = -cgso * ag0;
                        gcgmbb = -pParam->FEFET1cgbo * ag0;

                        gcdgmb = gcgmdb;
                        gcsgmb = gcgmsb;
                        gcbgmb = gcgmbb;

                gcggb = here->FEFET1cggb * ag0;
                        gcgdb = here->FEFET1cgdb * ag0;
                        gcgsb = here->FEFET1cgsb * ag0;   
                        gcgbb = -(gcggb + gcgdb + gcgsb);

                gcdgb = here->FEFET1cdgb * ag0;
                gcsgb = -(here->FEFET1cggb + here->FEFET1cbgb
                                + here->FEFET1cdgb) * ag0;
                        gcbgb = here->FEFET1cbgb * ag0;

                        qgmb = pParam->FEFET1cgbo * vgmb;
                        qgmid = qgdo + qgso + qgmb;
                        qbulk -= qgmb;
                        qsrc = -(qgate + qgmid + qbulk + qdrn);
            }
            else
            {   gcggb = (here->FEFET1cggb + cgdo + cgso
                                + pParam->FEFET1cgbo ) * ag0;
                        gcgdb = (here->FEFET1cgdb - cgdo) * ag0;
                        gcgsb = (here->FEFET1cgsb - cgso) * ag0;
                        gcgbb = -(gcggb + gcgdb + gcgsb);

                gcdgb = (here->FEFET1cdgb - cgdo) * ag0;
                        gcsgb = -(here->FEFET1cggb + here->FEFET1cbgb
                                + here->FEFET1cdgb + cgso) * ag0;
                        gcbgb = (here->FEFET1cbgb - pParam->FEFET1cgbo) * ag0;

                gcdgmb = gcsgmb = gcbgmb = 0.0;

                        qgb = pParam->FEFET1cgbo * vgb;
                        qgate += qgdo + qgso + qgb;
                        qbulk -= qgb;
                        qsrc = -(qgate + qbulk + qdrn);
            }
                    gcddb = (here->FEFET1cddb + here->FEFET1capbd + cgdo) * ag0;
                    gcdsb = here->FEFET1cdsb * ag0;

                    gcsdb = -(here->FEFET1cgdb + here->FEFET1cbdb
                            + here->FEFET1cddb) * ag0;
                    gcssb = (here->FEFET1capbs + cgso - (here->FEFET1cgsb
                            + here->FEFET1cbsb + here->FEFET1cdsb)) * ag0;

                    if (!here->FEFET1rbodyMod)
                    {   gcdbb = -(gcdgb + gcddb + gcdsb + gcdgmb);
                        gcsbb = -(gcsgb + gcsdb + gcssb + gcsgmb);
                        gcbdb = (here->FEFET1cbdb - here->FEFET1capbd) * ag0;
                        gcbsb = (here->FEFET1cbsb - here->FEFET1capbs) * ag0;
                        gcdbdb = 0.0; gcsbsb = 0.0;
                    }
                    else
                    {   gcdbb  = -(here->FEFET1cddb + here->FEFET1cdgb 
                                + here->FEFET1cdsb) * ag0;
                        gcsbb = -(gcsgb + gcsdb + gcssb + gcsgmb)
                    + here->FEFET1capbs * ag0;
                        gcbdb = here->FEFET1cbdb * ag0;
                        gcbsb = here->FEFET1cbsb * ag0;

                        gcdbdb = -here->FEFET1capbd * ag0;
                        gcsbsb = -here->FEFET1capbs * ag0;
                    }
            gcbbb = -(gcbdb + gcbgb + gcbsb + gcbgmb);

                    ggtg = ggtd = ggtb = ggts = 0.0;
            sxpart = 0.6;
                    dxpart = 0.4;
            ddxpart_dVd = ddxpart_dVg = ddxpart_dVb = ddxpart_dVs = 0.0;
            dsxpart_dVd = dsxpart_dVg = dsxpart_dVb = dsxpart_dVs = 0.0;
                }
                else
                {   qcheq = here->FEFET1qchqs;
                    CoxWL = model->FEFET1coxe * pParam->FEFET1weffCV * here->FEFET1nf
                            * pParam->FEFET1leffCV;
                    T0 = qdef * ScalingFactor / CoxWL;

                    ggtg = here->FEFET1gtg = T0 * here->FEFET1gcrgg;
                    ggtd = here->FEFET1gtd = T0 * here->FEFET1gcrgd;
                    ggts = here->FEFET1gts = T0 * here->FEFET1gcrgs;
                    ggtb = here->FEFET1gtb = T0 * here->FEFET1gcrgb;
            gqdef = ScalingFactor * ag0;

                    gcqgb = here->FEFET1cqgb * ag0;
                    gcqdb = here->FEFET1cqdb * ag0;
                    gcqsb = here->FEFET1cqsb * ag0;
                    gcqbb = here->FEFET1cqbb * ag0;

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
                    {   dxpart = qdrn / qcheq;
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

                        ddxpart_dVb = -(ddxpart_dVd + ddxpart_dVg + ddxpart_dVs);
                    }
                    sxpart = 1.0 - dxpart;
                    dsxpart_dVd = -ddxpart_dVd;
                    dsxpart_dVg = -ddxpart_dVg;
                    dsxpart_dVs = -ddxpart_dVs;
                    dsxpart_dVb = -(dsxpart_dVd + dsxpart_dVg + dsxpart_dVs);

                    if (here->FEFET1rgateMod == 3)
                    {   gcgmgmb = (cgdo + cgso + pParam->FEFET1cgbo) * ag0;
                        gcgmdb = -cgdo * ag0;
                        gcgmsb = -cgso * ag0;
                        gcgmbb = -pParam->FEFET1cgbo * ag0;

                        gcdgmb = gcgmdb;
                        gcsgmb = gcgmsb;
                        gcbgmb = gcgmbb;

                        gcdgb = gcsgb = gcbgb = 0.0;
                gcggb = gcgdb = gcgsb = gcgbb = 0.0;

                        qgmb = pParam->FEFET1cgbo * vgmb;
                        qgmid = qgdo + qgso + qgmb;
                qgate = 0.0;
                        qbulk = -qgmb;
                qdrn = -qgdo;
                        qsrc = -(qgmid + qbulk + qdrn);
                    }
                    else
                    {   gcggb = (cgdo + cgso + pParam->FEFET1cgbo ) * ag0;
                        gcgdb = -cgdo * ag0;
                        gcgsb = -cgso * ag0;
                gcgbb = -pParam->FEFET1cgbo * ag0;

                        gcdgb = gcgdb;
                        gcsgb = gcgsb;
                        gcbgb = gcgbb;
                        gcdgmb = gcsgmb = gcbgmb = 0.0;

                        qgb = pParam->FEFET1cgbo * vgb;
                        qgate = qgdo + qgso + qgb;
                        qbulk = -qgb;
                qdrn = -qgdo;
                        qsrc = -(qgate + qbulk + qdrn);
                    }

                    gcddb = (here->FEFET1capbd + cgdo) * ag0;
                    gcdsb = gcsdb = 0.0;
                    gcssb = (here->FEFET1capbs + cgso) * ag0;

                    if (!here->FEFET1rbodyMod)
                    {   gcdbb = -(gcdgb + gcddb + gcdgmb);
                        gcsbb = -(gcsgb + gcssb + gcsgmb);
                        gcbdb = -here->FEFET1capbd * ag0;
                        gcbsb = -here->FEFET1capbs * ag0;
                        gcdbdb = 0.0; gcsbsb = 0.0;
                    }
                    else
                    {   gcdbb = gcsbb = gcbdb = gcbsb = 0.0;
                        gcdbdb = -here->FEFET1capbd * ag0;
                        gcsbsb = -here->FEFET1capbs * ag0;
                    }
                    gcbbb = -(gcbdb + gcbgb + gcbsb + gcbgmb);
                }
            }
            else
            {   if (here->FEFET1trnqsMod == 0)
                {   qsrc = qdrn - qgso;
            if (here->FEFET1rgateMod == 3)
            {   gcgmgmb = (cgdo + cgso + pParam->FEFET1cgbo) * ag0;
                gcgmdb = -cgdo * ag0;
                    gcgmsb = -cgso * ag0;
                    gcgmbb = -pParam->FEFET1cgbo * ag0;

                        gcdgmb = gcgmdb;
                        gcsgmb = gcgmsb;
                        gcbgmb = gcgmbb;

                        gcggb = here->FEFET1cggb * ag0;
                        gcgdb = here->FEFET1cgsb * ag0;
                        gcgsb = here->FEFET1cgdb * ag0;
                        gcgbb = -(gcggb + gcgdb + gcgsb);

                        gcdgb = -(here->FEFET1cggb + here->FEFET1cbgb
                                + here->FEFET1cdgb) * ag0;
                        gcsgb = here->FEFET1cdgb * ag0;
                        gcbgb = here->FEFET1cbgb * ag0;

                        qgmb = pParam->FEFET1cgbo * vgmb;
                        qgmid = qgdo + qgso + qgmb;
                        qbulk -= qgmb;
                        qdrn = -(qgate + qgmid + qbulk + qsrc);
            }
            else
            {   gcggb = (here->FEFET1cggb + cgdo + cgso
                                + pParam->FEFET1cgbo ) * ag0;
                        gcgdb = (here->FEFET1cgsb - cgdo) * ag0;
                        gcgsb = (here->FEFET1cgdb - cgso) * ag0;
                        gcgbb = -(gcggb + gcgdb + gcgsb);

                        gcdgb = -(here->FEFET1cggb + here->FEFET1cbgb
                                + here->FEFET1cdgb + cgdo) * ag0;
                        gcsgb = (here->FEFET1cdgb - cgso) * ag0;
                        gcbgb = (here->FEFET1cbgb - pParam->FEFET1cgbo) * ag0;

                        gcdgmb = gcsgmb = gcbgmb = 0.0;

                        qgb = pParam->FEFET1cgbo * vgb;
                        qgate += qgdo + qgso + qgb;
                        qbulk -= qgb;
                        qdrn = -(qgate + qbulk + qsrc);
            }
                    gcddb = (here->FEFET1capbd + cgdo - (here->FEFET1cgsb
                            + here->FEFET1cbsb + here->FEFET1cdsb)) * ag0;
                    gcdsb = -(here->FEFET1cgdb + here->FEFET1cbdb
                            + here->FEFET1cddb) * ag0;

                    gcsdb = here->FEFET1cdsb * ag0;
                    gcssb = (here->FEFET1cddb + here->FEFET1capbs + cgso) * ag0;

            if (!here->FEFET1rbodyMod)
            {   gcdbb = -(gcdgb + gcddb + gcdsb + gcdgmb);
                        gcsbb = -(gcsgb + gcsdb + gcssb + gcsgmb);
                        gcbdb = (here->FEFET1cbsb - here->FEFET1capbd) * ag0;
                        gcbsb = (here->FEFET1cbdb - here->FEFET1capbs) * ag0;
                        gcdbdb = 0.0; gcsbsb = 0.0;
                    }
                    else
                    {   gcdbb = -(gcdgb + gcddb + gcdsb + gcdgmb)
                    + here->FEFET1capbd * ag0;
                        gcsbb = -(here->FEFET1cddb + here->FEFET1cdgb
                                + here->FEFET1cdsb) * ag0;
                        gcbdb = here->FEFET1cbsb * ag0;
                        gcbsb = here->FEFET1cbdb * ag0;
                        gcdbdb = -here->FEFET1capbd * ag0;
                gcsbsb = -here->FEFET1capbs * ag0;
                    }
            gcbbb = -(gcbgb + gcbdb + gcbsb + gcbgmb);

                    ggtg = ggtd = ggtb = ggts = 0.0;
            sxpart = 0.4;
                    dxpart = 0.6;
            ddxpart_dVd = ddxpart_dVg = ddxpart_dVb = ddxpart_dVs = 0.0;
            dsxpart_dVd = dsxpart_dVg = dsxpart_dVb = dsxpart_dVs = 0.0;
                }
                else
                {   qcheq = here->FEFET1qchqs;
                    CoxWL = model->FEFET1coxe * pParam->FEFET1weffCV * here->FEFET1nf
                            * pParam->FEFET1leffCV;
                    T0 = qdef * ScalingFactor / CoxWL;
                    ggtg = here->FEFET1gtg = T0 * here->FEFET1gcrgg;
                    ggts = here->FEFET1gts = T0 * here->FEFET1gcrgd;
                    ggtd = here->FEFET1gtd = T0 * here->FEFET1gcrgs;
                    ggtb = here->FEFET1gtb = T0 * here->FEFET1gcrgb;
            gqdef = ScalingFactor * ag0;

                    gcqgb = here->FEFET1cqgb * ag0;
                    gcqdb = here->FEFET1cqsb * ag0;
                    gcqsb = here->FEFET1cqdb * ag0;
                    gcqbb = here->FEFET1cqbb * ag0;

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
                    {   sxpart = qdrn / qcheq;
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

                        dsxpart_dVb = -(dsxpart_dVd + dsxpart_dVg + dsxpart_dVs);
                    }
                    dxpart = 1.0 - sxpart;
                    ddxpart_dVd = -dsxpart_dVd;
                    ddxpart_dVg = -dsxpart_dVg;
                    ddxpart_dVs = -dsxpart_dVs;
                    ddxpart_dVb = -(ddxpart_dVd + ddxpart_dVg + ddxpart_dVs);

                    if (here->FEFET1rgateMod == 3)
                    {   gcgmgmb = (cgdo + cgso + pParam->FEFET1cgbo) * ag0;
                        gcgmdb = -cgdo * ag0;
                        gcgmsb = -cgso * ag0;
                        gcgmbb = -pParam->FEFET1cgbo * ag0;

                        gcdgmb = gcgmdb;
                        gcsgmb = gcgmsb;
                        gcbgmb = gcgmbb;

                        gcdgb = gcsgb = gcbgb = 0.0;
                        gcggb = gcgdb = gcgsb = gcgbb = 0.0;

                        qgmb = pParam->FEFET1cgbo * vgmb;
                        qgmid = qgdo + qgso + qgmb;
                        qgate = 0.0;
                        qbulk = -qgmb;
                        qdrn = -qgdo;
                        qsrc = -qgso;
                    }
                    else
                    {   gcggb = (cgdo + cgso + pParam->FEFET1cgbo ) * ag0;
                        gcgdb = -cgdo * ag0;
                        gcgsb = -cgso * ag0;
                        gcgbb = -pParam->FEFET1cgbo * ag0;

                        gcdgb = gcgdb;
                        gcsgb = gcgsb;
                        gcbgb = gcgbb;
                        gcdgmb = gcsgmb = gcbgmb = 0.0;

                        qgb = pParam->FEFET1cgbo * vgb;
                        qgate = qgdo + qgso + qgb;
                        qbulk = -qgb;
                        qdrn = -qgdo;
                        qsrc = -qgso;
                    }

                    gcddb = (here->FEFET1capbd + cgdo) * ag0;
                    gcdsb = gcsdb = 0.0;
                    gcssb = (here->FEFET1capbs + cgso) * ag0;
                    if (!here->FEFET1rbodyMod)
                    {   gcdbb = -(gcdgb + gcddb + gcdgmb);
                        gcsbb = -(gcsgb + gcssb + gcsgmb);
                        gcbdb = -here->FEFET1capbd * ag0;
                        gcbsb = -here->FEFET1capbs * ag0;
                        gcdbdb = 0.0; gcsbsb = 0.0;
                    }
                    else
                    {   gcdbb = gcsbb = gcbdb = gcbsb = 0.0;
                        gcdbdb = -here->FEFET1capbd * ag0;
                        gcsbsb = -here->FEFET1capbs * ag0;
                    }
                    gcbbb = -(gcbdb + gcbgb + gcbsb + gcbgmb);
                }
            }


            if (here->FEFET1trnqsMod)
            {   *(ckt->CKTstate0 + here->FEFET1qcdump) = qdef * ScalingFactor;
                if (ckt->CKTmode & MODEINITTRAN)
                    *(ckt->CKTstate1 + here->FEFET1qcdump) =
                                    *(ckt->CKTstate0 + here->FEFET1qcdump);
                error = NIintegrate(ckt, &geq, &ceq, 0.0, here->FEFET1qcdump);
                if (error)
                    return(error);
            }

            if (ByPass) goto line860;

            *(ckt->CKTstate0 + here->FEFET1qg) = qgate;
            *(ckt->CKTstate0 + here->FEFET1qd) = qdrn
                            - *(ckt->CKTstate0 + here->FEFET1qbd);
            *(ckt->CKTstate0 + here->FEFET1qs) = qsrc
                            - *(ckt->CKTstate0 + here->FEFET1qbs);
        if (here->FEFET1rgateMod == 3)
            *(ckt->CKTstate0 + here->FEFET1qgmid) = qgmid;

            if (!here->FEFET1rbodyMod)
            {   *(ckt->CKTstate0 + here->FEFET1qb) = qbulk
                                + *(ckt->CKTstate0 + here->FEFET1qbd)
                                + *(ckt->CKTstate0 + here->FEFET1qbs);
            }
            else
                *(ckt->CKTstate0 + here->FEFET1qb) = qbulk;


            /* Store small signal parameters */
            if (ckt->CKTmode & MODEINITSMSIG)
            {   goto line1000;
            }

            if (!ChargeComputationNeeded)
                goto line850;

            if (ckt->CKTmode & MODEINITTRAN)
            {   *(ckt->CKTstate1 + here->FEFET1qb) =
                        *(ckt->CKTstate0 + here->FEFET1qb);
                *(ckt->CKTstate1 + here->FEFET1qg) =
                        *(ckt->CKTstate0 + here->FEFET1qg);
                *(ckt->CKTstate1 + here->FEFET1qd) =
                        *(ckt->CKTstate0 + here->FEFET1qd);
                if (here->FEFET1rgateMod == 3)
                    *(ckt->CKTstate1 + here->FEFET1qgmid) =
                            *(ckt->CKTstate0 + here->FEFET1qgmid);
                if (here->FEFET1rbodyMod)
                {   *(ckt->CKTstate1 + here->FEFET1qbs) =
                                    *(ckt->CKTstate0 + here->FEFET1qbs);
                    *(ckt->CKTstate1 + here->FEFET1qbd) =
                                    *(ckt->CKTstate0 + here->FEFET1qbd);
                }
            }

            error = NIintegrate(ckt, &geq, &ceq, 0.0, here->FEFET1qb);
            if (error) 
                return(error);
            error = NIintegrate(ckt, &geq, &ceq, 0.0, here->FEFET1qg);
            if (error) 
                return(error);
            error = NIintegrate(ckt, &geq, &ceq, 0.0, here->FEFET1qd);
            if (error) 
                return(error);

            if (here->FEFET1rgateMod == 3)
            {   error = NIintegrate(ckt, &geq, &ceq, 0.0, here->FEFET1qgmid);
                if (error) return(error);
            }

            if (here->FEFET1rbodyMod)
            {   error = NIintegrate(ckt, &geq, &ceq, 0.0, here->FEFET1qbs);
                if (error) 
                    return(error);
                error = NIintegrate(ckt, &geq, &ceq, 0.0, here->FEFET1qbd);
                if (error) 
                    return(error);
            }

            goto line860;


    line850:
            /* Zero gcap and ceqcap if (!ChargeComputationNeeded) */
            ceqqg = ceqqb = ceqqd = 0.0;
            ceqqjd = ceqqjs = 0.0;
            cqcheq = cqdef = 0.0;

            gcdgb = gcddb = gcdsb = gcdbb = 0.0;
            gcsgb = gcsdb = gcssb = gcsbb = 0.0;
            gcggb = gcgdb = gcgsb = gcgbb = 0.0;
            gcbdb = gcbgb = gcbsb = gcbbb = 0.0;

        gcgmgmb = gcgmdb = gcgmsb = gcgmbb = 0.0;
        gcdgmb = gcsgmb = gcbgmb = ceqqgmid = 0.0;
            gcdbdb = gcsbsb = 0.0;

        gqdef = gcqgb = gcqdb = gcqsb = gcqbb = 0.0;
            ggtg = ggtd = ggtb = ggts = 0.0;
            sxpart = (1.0 - (dxpart = (here->FEFET1mode > 0) ? 0.4 : 0.6));
        ddxpart_dVd = ddxpart_dVg = ddxpart_dVb = ddxpart_dVs = 0.0;
        dsxpart_dVd = dsxpart_dVg = dsxpart_dVb = dsxpart_dVs = 0.0;

            if (here->FEFET1trnqsMod)
            {   CoxWL = model->FEFET1coxe * pParam->FEFET1weffCV * here->FEFET1nf
                        * pParam->FEFET1leffCV;
                T1 = here->FEFET1gcrg / CoxWL;
                here->FEFET1gtau = T1 * ScalingFactor;
            }
        else
                here->FEFET1gtau = 0.0;

            goto line900;

                
    line860:
            /* Calculate equivalent charge current */

            cqgate = *(ckt->CKTstate0 + here->FEFET1cqg);
            cqbody = *(ckt->CKTstate0 + here->FEFET1cqb);
            cqdrn = *(ckt->CKTstate0 + here->FEFET1cqd);

            ceqqg = cqgate - gcggb * vgb + gcgdb * vbd + gcgsb * vbs;
            ceqqd = cqdrn - gcdgb * vgb - gcdgmb * vgmb + (gcddb + gcdbdb)
                * vbd - gcdbdb * vbd_jct + gcdsb * vbs;
        ceqqb = cqbody - gcbgb * vgb - gcbgmb * vgmb
                    + gcbdb * vbd + gcbsb * vbs;


            if (here->FEFET1rgateMod == 3)
                ceqqgmid = *(ckt->CKTstate0 + here->FEFET1cqgmid)
                        + gcgmdb * vbd + gcgmsb * vbs - gcgmgmb * vgmb;
        else
            ceqqgmid = 0.0;	

            if (here->FEFET1rbodyMod)
            {   ceqqjs = *(ckt->CKTstate0 + here->FEFET1cqbs) + gcsbsb * vbs_jct;
                ceqqjd = *(ckt->CKTstate0 + here->FEFET1cqbd) + gcdbdb * vbd_jct; 
            }

            if (here->FEFET1trnqsMod)
            {   T0 = ggtg * vgb - ggtd * vbd - ggts * vbs;
                ceqqg += T0;
            T1 = qdef * here->FEFET1gtau;
                ceqqd -= dxpart * T0 + T1 * (ddxpart_dVg * vgb - ddxpart_dVd
                * vbd - ddxpart_dVs * vbs);
                cqdef = *(ckt->CKTstate0 + here->FEFET1cqcdump) - gqdef * qdef;
                cqcheq = *(ckt->CKTstate0 + here->FEFET1cqcheq)
                        - (gcqgb * vgb - gcqdb * vbd - gcqsb * vbs) + T0;
            }

            if (ckt->CKTmode & MODEINITTRAN)
            {   *(ckt->CKTstate1 + here->FEFET1cqb) =
                                *(ckt->CKTstate0 + here->FEFET1cqb);
                *(ckt->CKTstate1 + here->FEFET1cqg) =
                                *(ckt->CKTstate0 + here->FEFET1cqg);
                *(ckt->CKTstate1 + here->FEFET1cqd) =
                                *(ckt->CKTstate0 + here->FEFET1cqd);

                if (here->FEFET1rgateMod == 3)
                    *(ckt->CKTstate1 + here->FEFET1cqgmid) =
                                    *(ckt->CKTstate0 + here->FEFET1cqgmid);

                if (here->FEFET1rbodyMod)
                {   *(ckt->CKTstate1 + here->FEFET1cqbs) =
                                    *(ckt->CKTstate0 + here->FEFET1cqbs);
                    *(ckt->CKTstate1 + here->FEFET1cqbd) =
                                    *(ckt->CKTstate0 + here->FEFET1cqbd);
                }
            }


            /*
            *  Load current vector
            */

    line900:
            if (here->FEFET1mode >= 0)
        {   Gm = here->FEFET1gm;
                Gmbs = here->FEFET1gmbs;
                FwdSum = Gm + Gmbs;
                RevSum = 0.0;

                ceqdrn = model->FEFET1type * (cdrain - here->FEFET1gds * vds
                    - Gm * vgs - Gmbs * vbs);
                ceqbd = model->FEFET1type * (here->FEFET1csub + here->FEFET1Igidl
                        - (here->FEFET1gbds + here->FEFET1ggidld) * vds
                - (here->FEFET1gbgs + here->FEFET1ggidlg) * vgs
                - (here->FEFET1gbbs + here->FEFET1ggidlb) * vbs);
                ceqbs = model->FEFET1type * (here->FEFET1Igisl + here->FEFET1ggisls * vds 
                        - here->FEFET1ggislg * vgd - here->FEFET1ggislb * vbd);

                gbbdp = -(here->FEFET1gbds);
                gbbsp = here->FEFET1gbds + here->FEFET1gbgs + here->FEFET1gbbs; 
                
                gbdpg = here->FEFET1gbgs;
                gbdpdp = here->FEFET1gbds;
                gbdpb = here->FEFET1gbbs;
                gbdpsp = -(gbdpg + gbdpdp + gbdpb);

                gbspg = 0.0;
                gbspdp = 0.0;
                gbspb = 0.0;
                gbspsp = 0.0;

                if (model->FEFET1igcMod)
            {   gIstotg = here->FEFET1gIgsg + here->FEFET1gIgcsg;
            gIstotd = here->FEFET1gIgcsd;
                    gIstots = here->FEFET1gIgss + here->FEFET1gIgcss;
                    gIstotb = here->FEFET1gIgcsb;
                Istoteq = model->FEFET1type * (here->FEFET1Igs + here->FEFET1Igcs
                - gIstotg * vgs - here->FEFET1gIgcsd * vds
                - here->FEFET1gIgcsb * vbs);

                    gIdtotg = here->FEFET1gIgdg + here->FEFET1gIgcdg;
                    gIdtotd = here->FEFET1gIgdd + here->FEFET1gIgcdd;
                    gIdtots = here->FEFET1gIgcds;
                    gIdtotb = here->FEFET1gIgcdb;
                    Idtoteq = model->FEFET1type * (here->FEFET1Igd + here->FEFET1Igcd
                            - here->FEFET1gIgdg * vgd - here->FEFET1gIgcdg * vgs
                - here->FEFET1gIgcdd * vds - here->FEFET1gIgcdb * vbs);
            }
            else
            {   gIstotg = gIstotd = gIstots = gIstotb = Istoteq = 0.0;
            gIdtotg = gIdtotd = gIdtots = gIdtotb = Idtoteq = 0.0;
            }

                if (model->FEFET1igbMod)
                {   gIbtotg = here->FEFET1gIgbg;
                    gIbtotd = here->FEFET1gIgbd;
                    gIbtots = here->FEFET1gIgbs;
                    gIbtotb = here->FEFET1gIgbb;
                    Ibtoteq = model->FEFET1type * (here->FEFET1Igb
                            - here->FEFET1gIgbg * vgs - here->FEFET1gIgbd * vds
                            - here->FEFET1gIgbb * vbs);
                }
                else
                    gIbtotg = gIbtotd = gIbtots = gIbtotb = Ibtoteq = 0.0;

                if ((model->FEFET1igcMod != 0) || (model->FEFET1igbMod != 0))
                {   gIgtotg = gIstotg + gIdtotg + gIbtotg;
                    gIgtotd = gIstotd + gIdtotd + gIbtotd ;
                    gIgtots = gIstots + gIdtots + gIbtots;
                    gIgtotb = gIstotb + gIdtotb + gIbtotb;
                    Igtoteq = Istoteq + Idtoteq + Ibtoteq; 
            }
            else
                gIgtotg = gIgtotd = gIgtots = gIgtotb = Igtoteq = 0.0;


                if (here->FEFET1rgateMod == 2)
                    T0 = vges - vgs;
                else if (here->FEFET1rgateMod == 3)
                    T0 = vgms - vgs;
                if (here->FEFET1rgateMod > 1)
                {   gcrgd = here->FEFET1gcrgd * T0;
                    gcrgg = here->FEFET1gcrgg * T0;
                    gcrgs = here->FEFET1gcrgs * T0;
                    gcrgb = here->FEFET1gcrgb * T0;
                    ceqgcrg = -(gcrgd * vds + gcrgg * vgs
                            + gcrgb * vbs);
                    gcrgg -= here->FEFET1gcrg;
                    gcrg = here->FEFET1gcrg;
                }
                else
                ceqgcrg = gcrg = gcrgd = gcrgg = gcrgs = gcrgb = 0.0;
            }
        else
        {   Gm = -here->FEFET1gm;
                Gmbs = -here->FEFET1gmbs;
                FwdSum = 0.0;
                RevSum = -(Gm + Gmbs);

                ceqdrn = -model->FEFET1type * (cdrain + here->FEFET1gds * vds
                        + Gm * vgd + Gmbs * vbd);

                ceqbs = model->FEFET1type * (here->FEFET1csub + here->FEFET1Igisl 
                        + (here->FEFET1gbds + here->FEFET1ggisls) * vds
                - (here->FEFET1gbgs + here->FEFET1ggislg) * vgd
                        - (here->FEFET1gbbs + here->FEFET1ggislb) * vbd);
                ceqbd = model->FEFET1type * (here->FEFET1Igidl - here->FEFET1ggidld * vds 
                        - here->FEFET1ggidlg * vgs - here->FEFET1ggidlb * vbs);

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
                    Istoteq = model->FEFET1type * (here->FEFET1Igs + here->FEFET1Igcd
                            - here->FEFET1gIgsg * vgs - here->FEFET1gIgcdg * vgd
                + here->FEFET1gIgcdd * vds - here->FEFET1gIgcdb * vbd);

                    gIdtotg = here->FEFET1gIgdg + here->FEFET1gIgcsg;
                    gIdtotd = here->FEFET1gIgdd + here->FEFET1gIgcss;
                    gIdtots = here->FEFET1gIgcsd;
                    gIdtotb = here->FEFET1gIgcsb;
                    Idtoteq = model->FEFET1type * (here->FEFET1Igd + here->FEFET1Igcs
                            - (here->FEFET1gIgdg + here->FEFET1gIgcsg) * vgd
                            + here->FEFET1gIgcsd * vds - here->FEFET1gIgcsb * vbd);
                }
                else
                {   gIstotg = gIstotd = gIstots = gIstotb = Istoteq = 0.0;
                    gIdtotg = gIdtotd = gIdtots = gIdtotb = Idtoteq = 0.0;
                }

                if (model->FEFET1igbMod)
                {   gIbtotg = here->FEFET1gIgbg;
                    gIbtotd = here->FEFET1gIgbs;
                    gIbtots = here->FEFET1gIgbd;
                    gIbtotb = here->FEFET1gIgbb;
                    Ibtoteq = model->FEFET1type * (here->FEFET1Igb
                            - here->FEFET1gIgbg * vgd + here->FEFET1gIgbd * vds
                            - here->FEFET1gIgbb * vbd);
                }
                else
                    gIbtotg = gIbtotd = gIbtots = gIbtotb = Ibtoteq = 0.0;

                if ((model->FEFET1igcMod != 0) || (model->FEFET1igbMod != 0))
                {   gIgtotg = gIstotg + gIdtotg + gIbtotg;
                    gIgtotd = gIstotd + gIdtotd + gIbtotd ;
                    gIgtots = gIstots + gIdtots + gIbtots;
                    gIgtotb = gIstotb + gIdtotb + gIbtotb;
                    Igtoteq = Istoteq + Idtoteq + Ibtoteq;
                }
                else
                    gIgtotg = gIgtotd = gIgtots = gIgtotb = Igtoteq = 0.0;


                if (here->FEFET1rgateMod == 2)
                    T0 = vges - vgs;
                else if (here->FEFET1rgateMod == 3)
                    T0 = vgms - vgs;
                if (here->FEFET1rgateMod > 1)
                {   gcrgd = here->FEFET1gcrgs * T0;
                    gcrgg = here->FEFET1gcrgg * T0;
                    gcrgs = here->FEFET1gcrgd * T0;
                    gcrgb = here->FEFET1gcrgb * T0;
                    ceqgcrg = -(gcrgg * vgd - gcrgs * vds
                            + gcrgb * vbd);
                    gcrgg -= here->FEFET1gcrg;
                    gcrg = here->FEFET1gcrg;
                }
                else
                    ceqgcrg = gcrg = gcrgd = gcrgg = gcrgs = gcrgb = 0.0;
            }

            if (model->FEFET1rdsMod == 1)
            {   ceqgstot = model->FEFET1type * (here->FEFET1gstotd * vds
                        + here->FEFET1gstotg * vgs + here->FEFET1gstotb * vbs);
                /* WDLiu: ceqgstot flowing away from sNodePrime */
                gstot = here->FEFET1gstot;
                gstotd = here->FEFET1gstotd;
                gstotg = here->FEFET1gstotg;
                gstots = here->FEFET1gstots - gstot;
                gstotb = here->FEFET1gstotb;

                ceqgdtot = -model->FEFET1type * (here->FEFET1gdtotd * vds
                        + here->FEFET1gdtotg * vgs + here->FEFET1gdtotb * vbs);
                /* WDLiu: ceqgdtot defined as flowing into dNodePrime */
                gdtot = here->FEFET1gdtot;
                gdtotd = here->FEFET1gdtotd - gdtot;
                gdtotg = here->FEFET1gdtotg;
                gdtots = here->FEFET1gdtots;
                gdtotb = here->FEFET1gdtotb;
            }
            else
            {   gstot = gstotd = gstotg = gstots = gstotb = ceqgstot = 0.0;
                gdtot = gdtotd = gdtotg = gdtots = gdtotb = ceqgdtot = 0.0;
            }

        if (model->FEFET1type > 0)
            {   ceqjs = (here->FEFET1cbs - here->FEFET1gbs * vbs_jct);
                ceqjd = (here->FEFET1cbd - here->FEFET1gbd * vbd_jct);
            }
        else
            {   ceqjs = -(here->FEFET1cbs - here->FEFET1gbs * vbs_jct); 
                ceqjd = -(here->FEFET1cbd - here->FEFET1gbd * vbd_jct);
                ceqqg = -ceqqg;
                ceqqd = -ceqqd;
                ceqqb = -ceqqb;
            ceqgcrg = -ceqgcrg;

                if (here->FEFET1trnqsMod)
                {   cqdef = -cqdef;
                    cqcheq = -cqcheq;
            }

                if (here->FEFET1rbodyMod)
                {   ceqqjs = -ceqqjs;
                    ceqqjd = -ceqqjd;
                }

            if (here->FEFET1rgateMod == 3)
            ceqqgmid = -ceqqgmid;
        }


            /*
                *  Loading RHS
                */

            m = here->FEFET1m;

    #ifdef USE_OMP
        here->FEFET1rhsdPrime = m * (ceqjd - ceqbd + ceqgdtot
                                                        - ceqdrn - ceqqd + Idtoteq);
        here->FEFET1rhsgPrime = m * (ceqqg - ceqgcrg + Igtoteq);

        if (here->FEFET1rgateMod == 2)
            here->FEFET1rhsgExt = m * ceqgcrg;
        else if (here->FEFET1rgateMod == 3)
                here->FEFET1grhsMid = m * (ceqqgmid + ceqgcrg);

        if (!here->FEFET1rbodyMod)
        {   here->FEFET1rhsbPrime = m * (ceqbd + ceqbs - ceqjd
                                                            - ceqjs - ceqqb + Ibtoteq);
            here->FEFET1rhssPrime = m * (ceqdrn - ceqbs + ceqjs
                                + ceqqg + ceqqb + ceqqd + ceqqgmid - ceqgstot + Istoteq);
            }
            else
            {   here->FEFET1rhsdb = m * (ceqjd + ceqqjd);
                here->FEFET1rhsbPrime = m * (ceqbd + ceqbs - ceqqb + Ibtoteq);
                here->FEFET1rhssb = m * (ceqjs + ceqqjs);
                here->FEFET1rhssPrime = m * (ceqdrn - ceqbs + ceqjs + ceqqd
                    + ceqqg + ceqqb + ceqqjd + ceqqjs + ceqqgmid - ceqgstot + Istoteq);
            }

            if (model->FEFET1rdsMod)
            {   here->FEFET1rhsd = m * ceqgdtot;
                here->FEFET1rhss = m * ceqgstot;
            }

            if (here->FEFET1trnqsMod)
            here->FEFET1rhsq = m * (cqcheq - cqdef);
    #else
            (*(ckt->CKTrhs + here->FEFET1dNodePrime) += m * (ceqjd - ceqbd + ceqgdtot
                                                        - ceqdrn - ceqqd + Idtoteq));
            (*(ckt->CKTrhs + here->FEFET1gNodePrime) -= m * (ceqqg - ceqgcrg + Igtoteq));

            if (here->FEFET1rgateMod == 2)
                (*(ckt->CKTrhs + here->FEFET1gNodeExt) -= m * ceqgcrg);
            else if (here->FEFET1rgateMod == 3)
            (*(ckt->CKTrhs + here->FEFET1gNodeMid) -= m * (ceqqgmid + ceqgcrg));

            if (!here->FEFET1rbodyMod)
            {   (*(ckt->CKTrhs + here->FEFET1bNodePrime) += m * (ceqbd + ceqbs - ceqjd
                                                            - ceqjs - ceqqb + Ibtoteq));
                (*(ckt->CKTrhs + here->FEFET1sNodePrime) += m * (ceqdrn - ceqbs + ceqjs 
                                + ceqqg + ceqqb + ceqqd + ceqqgmid - ceqgstot + Istoteq));
            }
            else
            {   (*(ckt->CKTrhs + here->FEFET1dbNode) -= m * (ceqjd + ceqqjd));
                (*(ckt->CKTrhs + here->FEFET1bNodePrime) += m * (ceqbd + ceqbs - ceqqb + Ibtoteq));
                (*(ckt->CKTrhs + here->FEFET1sbNode) -= m * (ceqjs + ceqqjs));
                (*(ckt->CKTrhs + here->FEFET1sNodePrime) += m * (ceqdrn - ceqbs + ceqjs + ceqqd 
                    + ceqqg + ceqqb + ceqqjd + ceqqjs + ceqqgmid - ceqgstot + Istoteq));
            }

            if (model->FEFET1rdsMod)
        {   (*(ckt->CKTrhs + here->FEFET1dNode) -= m * ceqgdtot); 
            (*(ckt->CKTrhs + here->FEFET1sNode) += m * ceqgstot);
        }

            if (here->FEFET1trnqsMod)
                *(ckt->CKTrhs + here->FEFET1qNode) += m * (cqcheq - cqdef);
    #endif


            /*
                *  Loading matrix
                */

        if (!here->FEFET1rbodyMod)
            {   gjbd = here->FEFET1gbd;
                gjbs = here->FEFET1gbs;
            }
            else
                gjbd = gjbs = 0.0;

            if (!model->FEFET1rdsMod)
            {   gdpr = here->FEFET1drainConductance;
                gspr = here->FEFET1sourceConductance;
            }
            else
                gdpr = gspr = 0.0;

        geltd = here->FEFET1grgeltd;

            T1 = qdef * here->FEFET1gtau;

    #ifdef USE_OMP
        if (here->FEFET1rgateMod == 1)
        {   here->FEFET1_1 = m * geltd;
            here->FEFET1_2 = m * geltd;
            here->FEFET1_3 = m * geltd;
            here->FEFET1_4 = m * (gcggb + geltd - ggtg + gIgtotg);
            here->FEFET1_5 = m * (gcgdb - ggtd + gIgtotd);
            here->FEFET1_6 = m * (gcgsb - ggts + gIgtots);
            here->FEFET1_7 = m * (gcgbb - ggtb + gIgtotb);
        } /* WDLiu: gcrg already subtracted from all gcrgg below */
        else if (here->FEFET1rgateMod == 2)
        {   here->FEFET1_8 = m * gcrg;
            here->FEFET1_9 = m * gcrgg;
            here->FEFET1_10 = m * gcrgd;
            here->FEFET1_11 = m * gcrgs;
            here->FEFET1_12 = m * gcrgb;

            here->FEFET1_13 = m * gcrg;
            here->FEFET1_14 = m * (gcggb  - gcrgg - ggtg + gIgtotg);
            here->FEFET1_15 = m * (gcgdb - gcrgd - ggtd + gIgtotd);
            here->FEFET1_16 = m * (gcgsb - gcrgs - ggts + gIgtots);
            here->FEFET1_17 = m * (gcgbb - gcrgb - ggtb + gIgtotb);
        }
        else if (here->FEFET1rgateMod == 3)
        {   here->FEFET1_18 = m * geltd;
            here->FEFET1_19 = m * geltd;
            here->FEFET1_20 = m * geltd;
            here->FEFET1_21 = m * (geltd + gcrg + gcgmgmb);

            here->FEFET1_22 = m * (gcrgd + gcgmdb);
            here->FEFET1_23 = m * gcrgg;
            here->FEFET1_24 = m * (gcrgs + gcgmsb);
            here->FEFET1_25 = m * (gcrgb + gcgmbb);

            here->FEFET1_26 = m * gcdgmb;
            here->FEFET1_27 = m * gcrg;
            here->FEFET1_28 = m * gcsgmb;
            here->FEFET1_29 = m * gcbgmb;

            here->FEFET1_30 = m * (gcggb - gcrgg - ggtg + gIgtotg);
            here->FEFET1_31 = m * (gcgdb - gcrgd - ggtd + gIgtotd);
            here->FEFET1_32 = m * (gcgsb - gcrgs - ggts + gIgtots);
            here->FEFET1_33 = m * (gcgbb - gcrgb - ggtb + gIgtotb);
        }
        else
        {   here->FEFET1_34 = m * (gcggb - ggtg + gIgtotg);
            here->FEFET1_35 = m * (gcgdb - ggtd + gIgtotd);
            here->FEFET1_36 = m * (gcgsb - ggts + gIgtots);
            here->FEFET1_37 = m * (gcgbb - ggtb + gIgtotb);
        }

        if (model->FEFET1rdsMod)
        {   here->FEFET1_38 = m * gdtotg;
            here->FEFET1_39 = m * gdtots;
            here->FEFET1_40 = m * gdtotb;
            here->FEFET1_41 = m * gstotd;
            here->FEFET1_42 = m * gstotg;
            here->FEFET1_43 = m * gstotb;
        }

        here->FEFET1_44 = m * (gdpr + here->FEFET1gds + here->FEFET1gbd + T1 * ddxpart_dVd
                                    - gdtotd + RevSum + gcddb + gbdpdp + dxpart * ggtd - gIdtotd);
        here->FEFET1_45 = m * (gdpr + gdtot);
        here->FEFET1_46 = m * (Gm + gcdgb - gdtotg + gbdpg - gIdtotg
                                    + dxpart * ggtg + T1 * ddxpart_dVg);
        here->FEFET1_47 = m * (here->FEFET1gds + gdtots - dxpart * ggts + gIdtots
                                    - T1 * ddxpart_dVs + FwdSum - gcdsb - gbdpsp);
        here->FEFET1_48 = m * (gjbd + gdtotb - Gmbs - gcdbb - gbdpb + gIdtotb
                                    - T1 * ddxpart_dVb - dxpart * ggtb);

        here->FEFET1_49 = m * (gdpr - gdtotd);
        here->FEFET1_50 = m * (gdpr + gdtot);

        here->FEFET1_51 = m * (here->FEFET1gds + gstotd + RevSum - gcsdb - gbspdp
                                    - T1 * dsxpart_dVd - sxpart * ggtd + gIstotd);
        here->FEFET1_52 = m * (gcsgb - Gm - gstotg + gbspg + sxpart * ggtg
                                    + T1 * dsxpart_dVg - gIstotg);
        here->FEFET1_53 = m * (gspr + here->FEFET1gds + here->FEFET1gbs + T1 * dsxpart_dVs
                                    - gstots + FwdSum + gcssb + gbspsp + sxpart * ggts - gIstots);
        here->FEFET1_54 = m * (gspr + gstot);
        here->FEFET1_55 = m * (gjbs + gstotb + Gmbs - gcsbb - gbspb - sxpart * ggtb
                                    - T1 * dsxpart_dVb + gIstotb);

        here->FEFET1_56 = m * (gspr - gstots);
        here->FEFET1_57 = m * (gspr + gstot);

        here->FEFET1_58 = m * (gcbdb - gjbd + gbbdp - gIbtotd);
        here->FEFET1_59 = m * (gcbgb - here->FEFET1gbgs - gIbtotg);
        here->FEFET1_60 = m * (gcbsb - gjbs + gbbsp - gIbtots);
        here->FEFET1_61 = m * (gjbd + gjbs + gcbbb - here->FEFET1gbbs - gIbtotb);

        ggidld = here->FEFET1ggidld;
        ggidlg = here->FEFET1ggidlg;
        ggidlb = here->FEFET1ggidlb;
        ggislg = here->FEFET1ggislg;
        ggisls = here->FEFET1ggisls;
        ggislb = here->FEFET1ggislb;

        /* stamp gidl */
        here->FEFET1_62 = m * ggidld;
        here->FEFET1_63 = m * ggidlg;
        here->FEFET1_64 = m * (ggidlg + ggidld + ggidlb);
        here->FEFET1_65 = m * ggidlb;
        here->FEFET1_66 = m * ggidld;
        here->FEFET1_67 = m * ggidlg;
        here->FEFET1_68 = m * (ggidlg + ggidld + ggidlb);
        here->FEFET1_69 = m * ggidlb;
        /* stamp gisl */
        here->FEFET1_70 = m * (ggisls + ggislg + ggislb);
        here->FEFET1_71 = m * ggislg;
        here->FEFET1_72 = m * ggisls;
        here->FEFET1_73 = m * ggislb;
        here->FEFET1_74 = m * (ggislg + ggisls + ggislb);
        here->FEFET1_75 = m * ggislg;
        here->FEFET1_76 = m * ggisls;
        here->FEFET1_77 = m * ggislb;

        if (here->FEFET1rbodyMod)
        {   here->FEFET1_78 = m * (gcdbdb - here->FEFET1gbd);
            here->FEFET1_79 = m * (here->FEFET1gbs - gcsbsb);

            here->FEFET1_80 = m * (gcdbdb - here->FEFET1gbd);
            here->FEFET1_81 = m * (here->FEFET1gbd - gcdbdb
                            + here->FEFET1grbpd + here->FEFET1grbdb);
            here->FEFET1_82 = m * here->FEFET1grbpd;
            here->FEFET1_83 = m * here->FEFET1grbdb;

            here->FEFET1_84 = m * here->FEFET1grbpd;
            here->FEFET1_85 = m * here->FEFET1grbpb;
            here->FEFET1_86 = m * here->FEFET1grbps;
            here->FEFET1_87 = m * (here->FEFET1grbpd + here->FEFET1grbps
                            + here->FEFET1grbpb);
            /* WDLiu: (gcbbb - here->FEFET1gbbs) already added to BPbpPtr */

            here->FEFET1_88 = m * (gcsbsb - here->FEFET1gbs);
            here->FEFET1_89 = m * here->FEFET1grbps;
            here->FEFET1_90 = m * here->FEFET1grbsb;
            here->FEFET1_91 = m * (here->FEFET1gbs - gcsbsb
                            + here->FEFET1grbps + here->FEFET1grbsb);

            here->FEFET1_92 = m * here->FEFET1grbdb;
            here->FEFET1_93 = m * here->FEFET1grbpb;
            here->FEFET1_94 = m * here->FEFET1grbsb;
            here->FEFET1_95 = m * (here->FEFET1grbsb + here->FEFET1grbdb
                            + here->FEFET1grbpb);
        }

            if (here->FEFET1trnqsMod)
            {   here->FEFET1_96 = m * (gqdef + here->FEFET1gtau);
                here->FEFET1_97 = m * (ggtg - gcqgb);
                here->FEFET1_98 = m * (ggtd - gcqdb);
                here->FEFET1_99 = m * (ggts - gcqsb);
                here->FEFET1_100 = m * (ggtb - gcqbb);

                here->FEFET1_101 = m * dxpart * here->FEFET1gtau;
                here->FEFET1_102 = m * sxpart * here->FEFET1gtau;
                here->FEFET1_103 = m * here->FEFET1gtau;
            }
    #else
            if (here->FEFET1rgateMod == 1)
            {   (*(here->FEFET1GEgePtr) += m * geltd);
                (*(here->FEFET1GPgePtr) -= m * geltd);
                (*(here->FEFET1GEgpPtr) -= m * geltd);
            (*(here->FEFET1GPgpPtr) += m * (gcggb + geltd - ggtg + gIgtotg));
            (*(here->FEFET1GPdpPtr) += m * (gcgdb - ggtd + gIgtotd));
            (*(here->FEFET1GPspPtr) += m * (gcgsb - ggts + gIgtots));
            (*(here->FEFET1GPbpPtr) += m * (gcgbb - ggtb + gIgtotb));
            } /* WDLiu: gcrg already subtracted from all gcrgg below */
            else if (here->FEFET1rgateMod == 2)	
        {   (*(here->FEFET1GEgePtr) += m * gcrg);
                (*(here->FEFET1GEgpPtr) += m * gcrgg);
                (*(here->FEFET1GEdpPtr) += m * gcrgd);
                (*(here->FEFET1GEspPtr) += m * gcrgs);
            (*(here->FEFET1GEbpPtr) += m * gcrgb);	

                (*(here->FEFET1GPgePtr) -= m * gcrg);
            (*(here->FEFET1GPgpPtr) += m * (gcggb  - gcrgg - ggtg + gIgtotg));
            (*(here->FEFET1GPdpPtr) += m * (gcgdb - gcrgd - ggtd + gIgtotd));
            (*(here->FEFET1GPspPtr) += m * (gcgsb - gcrgs - ggts + gIgtots));
            (*(here->FEFET1GPbpPtr) += m * (gcgbb - gcrgb - ggtb + gIgtotb));
        }
        else if (here->FEFET1rgateMod == 3)
        {   (*(here->FEFET1GEgePtr) += m * geltd);
                (*(here->FEFET1GEgmPtr) -= m * geltd);
                (*(here->FEFET1GMgePtr) -= m * geltd);
                (*(here->FEFET1GMgmPtr) += m * (geltd + gcrg + gcgmgmb));

                (*(here->FEFET1GMdpPtr) += m * (gcrgd + gcgmdb));
                (*(here->FEFET1GMgpPtr) += m * gcrgg);
                (*(here->FEFET1GMspPtr) += m * (gcrgs + gcgmsb));
                (*(here->FEFET1GMbpPtr) += m * (gcrgb + gcgmbb));

                (*(here->FEFET1DPgmPtr) += m * gcdgmb);
                (*(here->FEFET1GPgmPtr) -= m * gcrg);
                (*(here->FEFET1SPgmPtr) += m * gcsgmb);
                (*(here->FEFET1BPgmPtr) += m * gcbgmb);

                (*(here->FEFET1GPgpPtr) += m * (gcggb - gcrgg - ggtg + gIgtotg));
                (*(here->FEFET1GPdpPtr) += m * (gcgdb - gcrgd - ggtd + gIgtotd));
                (*(here->FEFET1GPspPtr) += m * (gcgsb - gcrgs - ggts + gIgtots));
                (*(here->FEFET1GPbpPtr) += m * (gcgbb - gcrgb - ggtb + gIgtotb));
        }
        else
        {   (*(here->FEFET1GPgpPtr) += m * (gcggb - ggtg + gIgtotg));
            (*(here->FEFET1GPdpPtr) += m * (gcgdb - ggtd + gIgtotd));
                (*(here->FEFET1GPspPtr) += m * (gcgsb - ggts + gIgtots));
            (*(here->FEFET1GPbpPtr) += m * (gcgbb - ggtb + gIgtotb));
        }

        if (model->FEFET1rdsMod)
        {   (*(here->FEFET1DgpPtr) += m * gdtotg);
            (*(here->FEFET1DspPtr) += m * gdtots);
                (*(here->FEFET1DbpPtr) += m * gdtotb);
                (*(here->FEFET1SdpPtr) += m * gstotd);
                (*(here->FEFET1SgpPtr) += m * gstotg);
                (*(here->FEFET1SbpPtr) += m * gstotb);
        }

            (*(here->FEFET1DPdpPtr) += m * (gdpr + here->FEFET1gds + here->FEFET1gbd + T1 * ddxpart_dVd
                                    - gdtotd + RevSum + gcddb + gbdpdp + dxpart * ggtd - gIdtotd));
            (*(here->FEFET1DPdPtr) -= m * (gdpr + gdtot));
            (*(here->FEFET1DPgpPtr) += m * (Gm + gcdgb - gdtotg + gbdpg - gIdtotg
                    + dxpart * ggtg + T1 * ddxpart_dVg));
            (*(here->FEFET1DPspPtr) -= m * (here->FEFET1gds + gdtots - dxpart * ggts + gIdtots
                    - T1 * ddxpart_dVs + FwdSum - gcdsb - gbdpsp));
            (*(here->FEFET1DPbpPtr) -= m * (gjbd + gdtotb - Gmbs - gcdbb - gbdpb + gIdtotb
                    - T1 * ddxpart_dVb - dxpart * ggtb));

            (*(here->FEFET1DdpPtr) -= m * (gdpr - gdtotd));
            (*(here->FEFET1DdPtr) += m * (gdpr + gdtot));

            (*(here->FEFET1SPdpPtr) -= m * (here->FEFET1gds + gstotd + RevSum - gcsdb - gbspdp
                    - T1 * dsxpart_dVd - sxpart * ggtd + gIstotd));
            (*(here->FEFET1SPgpPtr) += m * (gcsgb - Gm - gstotg + gbspg + sxpart * ggtg
                    + T1 * dsxpart_dVg - gIstotg));
            (*(here->FEFET1SPspPtr) += m * (gspr + here->FEFET1gds + here->FEFET1gbs + T1 * dsxpart_dVs
                                    - gstots + FwdSum + gcssb + gbspsp + sxpart * ggts - gIstots));
            (*(here->FEFET1SPsPtr) -= m * (gspr + gstot));
            (*(here->FEFET1SPbpPtr) -= m * (gjbs + gstotb + Gmbs - gcsbb - gbspb - sxpart * ggtb
                    - T1 * dsxpart_dVb + gIstotb));

            (*(here->FEFET1SspPtr) -= m * (gspr - gstots));
            (*(here->FEFET1SsPtr) += m * (gspr + gstot));

            (*(here->FEFET1BPdpPtr) += m * (gcbdb - gjbd + gbbdp - gIbtotd));
            (*(here->FEFET1BPgpPtr) += m * (gcbgb - here->FEFET1gbgs - gIbtotg));
            (*(here->FEFET1BPspPtr) += m * (gcbsb - gjbs + gbbsp - gIbtots));
            (*(here->FEFET1BPbpPtr) += m * (gjbd + gjbs + gcbbb - here->FEFET1gbbs
                    - gIbtotb));

            ggidld = here->FEFET1ggidld;
            ggidlg = here->FEFET1ggidlg;
            ggidlb = here->FEFET1ggidlb;
            ggislg = here->FEFET1ggislg;
            ggisls = here->FEFET1ggisls;
            ggislb = here->FEFET1ggislb;

            /* stamp gidl */
            (*(here->FEFET1DPdpPtr) += m * ggidld);
            (*(here->FEFET1DPgpPtr) += m * ggidlg);
            (*(here->FEFET1DPspPtr) -= m * (ggidlg + ggidld + ggidlb));
            (*(here->FEFET1DPbpPtr) += m * ggidlb);
            (*(here->FEFET1BPdpPtr) -= m * ggidld);
            (*(here->FEFET1BPgpPtr) -= m * ggidlg);
            (*(here->FEFET1BPspPtr) += m * (ggidlg + ggidld + ggidlb));
            (*(here->FEFET1BPbpPtr) -= m * ggidlb);
                /* stamp gisl */
            (*(here->FEFET1SPdpPtr) -= m * (ggisls + ggislg + ggislb));
            (*(here->FEFET1SPgpPtr) += m * ggislg);
            (*(here->FEFET1SPspPtr) += m * ggisls);
            (*(here->FEFET1SPbpPtr) += m * ggislb);
            (*(here->FEFET1BPdpPtr) += m * (ggislg + ggisls + ggislb));
            (*(here->FEFET1BPgpPtr) -= m * ggislg);
            (*(here->FEFET1BPspPtr) -= m * ggisls);
            (*(here->FEFET1BPbpPtr) -= m * ggislb);


            if (here->FEFET1rbodyMod)
            {   (*(here->FEFET1DPdbPtr) += m * (gcdbdb - here->FEFET1gbd));
                (*(here->FEFET1SPsbPtr) -= m * (here->FEFET1gbs - gcsbsb));

                (*(here->FEFET1DBdpPtr) += m * (gcdbdb - here->FEFET1gbd));
                (*(here->FEFET1DBdbPtr) += m * (here->FEFET1gbd - gcdbdb 
                                        + here->FEFET1grbpd + here->FEFET1grbdb));
                (*(here->FEFET1DBbpPtr) -= m * here->FEFET1grbpd);
                (*(here->FEFET1DBbPtr) -= m * here->FEFET1grbdb);

                (*(here->FEFET1BPdbPtr) -= m * here->FEFET1grbpd);
                (*(here->FEFET1BPbPtr) -= m * here->FEFET1grbpb);
                (*(here->FEFET1BPsbPtr) -= m * here->FEFET1grbps);
                (*(here->FEFET1BPbpPtr) += m * (here->FEFET1grbpd + here->FEFET1grbps 
                                        + here->FEFET1grbpb));
            /* WDLiu: (gcbbb - here->FEFET1gbbs) already added to BPbpPtr */	

                (*(here->FEFET1SBspPtr) += m * (gcsbsb - here->FEFET1gbs));
                (*(here->FEFET1SBbpPtr) -= m * here->FEFET1grbps);
                (*(here->FEFET1SBbPtr) -= m * here->FEFET1grbsb);
                (*(here->FEFET1SBsbPtr) += m * (here->FEFET1gbs - gcsbsb 
                                        + here->FEFET1grbps + here->FEFET1grbsb));

                (*(here->FEFET1BdbPtr) -= m * here->FEFET1grbdb);
                (*(here->FEFET1BbpPtr) -= m * here->FEFET1grbpb);
                (*(here->FEFET1BsbPtr) -= m * here->FEFET1grbsb);
                (*(here->FEFET1BbPtr) += m * (here->FEFET1grbsb + here->FEFET1grbdb
                                        + here->FEFET1grbpb));
            }

            if (here->FEFET1trnqsMod)
            {   (*(here->FEFET1QqPtr) += m * (gqdef + here->FEFET1gtau));
                (*(here->FEFET1QgpPtr) += m * (ggtg - gcqgb));
                (*(here->FEFET1QdpPtr) += m * (ggtd - gcqdb));
                (*(here->FEFET1QspPtr) += m * (ggts - gcqsb));
                (*(here->FEFET1QbpPtr) += m * (ggtb - gcqbb));

                (*(here->FEFET1DPqPtr) += m * dxpart * here->FEFET1gtau);
                (*(here->FEFET1SPqPtr) += m * sxpart * here->FEFET1gtau);
                (*(here->FEFET1GPqPtr) -= m * here->FEFET1gtau);
            }
    #endif

    line1000:  ;

    #ifndef USE_OMP
        }  /* End of MOSFET Instance */
    }   /* End of Model Instance */
    #endif

    return(OK);
    }


    #ifdef USE_OMP
    void FEFET1LoadRhsMat(GENmodel *inModel, CKTcircuit *ckt)
    {
        int InstCount, idx;
        FEFET1instance **InstArray;
        FEFET1instance *here;
        FEFET1model *model = (FEFET1model*)inModel;

        InstArray = model->FEFET1InstanceArray;
        InstCount = model->FEFET1InstCount;

        for(idx = 0; idx < InstCount; idx++) {
        here = InstArray[idx];
        model = FEFET1modPtr(here);
            /* Update b for Ax = b */
            (*(ckt->CKTrhs + here->FEFET1dNodePrime) += here->FEFET1rhsdPrime);
            (*(ckt->CKTrhs + here->FEFET1gNodePrime) -= here->FEFET1rhsgPrime);

            if (here->FEFET1rgateMod == 2)
                (*(ckt->CKTrhs + here->FEFET1gNodeExt) -= here->FEFET1rhsgExt);
            else if (here->FEFET1rgateMod == 3)
                (*(ckt->CKTrhs + here->FEFET1gNodeMid) -= here->FEFET1grhsMid);

            if (!here->FEFET1rbodyMod)
            {   (*(ckt->CKTrhs + here->FEFET1bNodePrime) += here->FEFET1rhsbPrime);
                (*(ckt->CKTrhs + here->FEFET1sNodePrime) += here->FEFET1rhssPrime);
            }
            else
            {   (*(ckt->CKTrhs + here->FEFET1dbNode) -= here->FEFET1rhsdb);
                (*(ckt->CKTrhs + here->FEFET1bNodePrime) += here->FEFET1rhsbPrime);
                (*(ckt->CKTrhs + here->FEFET1sbNode) -= here->FEFET1rhssb);
                (*(ckt->CKTrhs + here->FEFET1sNodePrime) += here->FEFET1rhssPrime);
            }

            if (model->FEFET1rdsMod)
            {   (*(ckt->CKTrhs + here->FEFET1dNode) -= here->FEFET1rhsd);
                (*(ckt->CKTrhs + here->FEFET1sNode) += here->FEFET1rhss);
            }

            if (here->FEFET1trnqsMod)
                *(ckt->CKTrhs + here->FEFET1qNode) += here->FEFET1rhsq;


            /* Update A for Ax = b */
            if (here->FEFET1rgateMod == 1)
            {   (*(here->FEFET1GEgePtr) += here->FEFET1_1);
                (*(here->FEFET1GPgePtr) -= here->FEFET1_2);
                (*(here->FEFET1GEgpPtr) -= here->FEFET1_3);
                (*(here->FEFET1GPgpPtr) += here->FEFET1_4);
                (*(here->FEFET1GPdpPtr) += here->FEFET1_5);
                (*(here->FEFET1GPspPtr) += here->FEFET1_6);
                (*(here->FEFET1GPbpPtr) += here->FEFET1_7);
            }
            else if (here->FEFET1rgateMod == 2)
            {   (*(here->FEFET1GEgePtr) += here->FEFET1_8);
                (*(here->FEFET1GEgpPtr) += here->FEFET1_9);
                (*(here->FEFET1GEdpPtr) += here->FEFET1_10);
                (*(here->FEFET1GEspPtr) += here->FEFET1_11);
                (*(here->FEFET1GEbpPtr) += here->FEFET1_12);

                (*(here->FEFET1GPgePtr) -= here->FEFET1_13);
                (*(here->FEFET1GPgpPtr) += here->FEFET1_14);
                (*(here->FEFET1GPdpPtr) += here->FEFET1_15);
                (*(here->FEFET1GPspPtr) += here->FEFET1_16);
                (*(here->FEFET1GPbpPtr) += here->FEFET1_17);
            }
            else if (here->FEFET1rgateMod == 3)
            {   (*(here->FEFET1GEgePtr) += here->FEFET1_18);
                (*(here->FEFET1GEgmPtr) -= here->FEFET1_19);
                (*(here->FEFET1GMgePtr) -= here->FEFET1_20);
                (*(here->FEFET1GMgmPtr) += here->FEFET1_21);

                (*(here->FEFET1GMdpPtr) += here->FEFET1_22);
                (*(here->FEFET1GMgpPtr) += here->FEFET1_23);
                (*(here->FEFET1GMspPtr) += here->FEFET1_24);
                (*(here->FEFET1GMbpPtr) += here->FEFET1_25);

                (*(here->FEFET1DPgmPtr) += here->FEFET1_26);
                (*(here->FEFET1GPgmPtr) -= here->FEFET1_27);
                (*(here->FEFET1SPgmPtr) += here->FEFET1_28);
                (*(here->FEFET1BPgmPtr) += here->FEFET1_29);

                (*(here->FEFET1GPgpPtr) += here->FEFET1_30);
                (*(here->FEFET1GPdpPtr) += here->FEFET1_31);
                (*(here->FEFET1GPspPtr) += here->FEFET1_32);
                (*(here->FEFET1GPbpPtr) += here->FEFET1_33);
            }


                else
            {   (*(here->FEFET1GPgpPtr) += here->FEFET1_34);
                (*(here->FEFET1GPdpPtr) += here->FEFET1_35);
                (*(here->FEFET1GPspPtr) += here->FEFET1_36);
                (*(here->FEFET1GPbpPtr) += here->FEFET1_37);
            }


            if (model->FEFET1rdsMod)
            {   (*(here->FEFET1DgpPtr) += here->FEFET1_38);
                (*(here->FEFET1DspPtr) += here->FEFET1_39);
                (*(here->FEFET1DbpPtr) += here->FEFET1_40);
                (*(here->FEFET1SdpPtr) += here->FEFET1_41);
                (*(here->FEFET1SgpPtr) += here->FEFET1_42);
                (*(here->FEFET1SbpPtr) += here->FEFET1_43);
            }

            (*(here->FEFET1DPdpPtr) += here->FEFET1_44);
            (*(here->FEFET1DPdPtr) -= here->FEFET1_45);
            (*(here->FEFET1DPgpPtr) += here->FEFET1_46);
            (*(here->FEFET1DPspPtr) -= here->FEFET1_47);
            (*(here->FEFET1DPbpPtr) -= here->FEFET1_48);

            (*(here->FEFET1DdpPtr) -= here->FEFET1_49);
            (*(here->FEFET1DdPtr) += here->FEFET1_50);

            (*(here->FEFET1SPdpPtr) -= here->FEFET1_51);
            (*(here->FEFET1SPgpPtr) += here->FEFET1_52);
            (*(here->FEFET1SPspPtr) += here->FEFET1_53);
            (*(here->FEFET1SPsPtr) -= here->FEFET1_54);
            (*(here->FEFET1SPbpPtr) -= here->FEFET1_55);

            (*(here->FEFET1SspPtr) -= here->FEFET1_56);
            (*(here->FEFET1SsPtr) += here->FEFET1_57);

            (*(here->FEFET1BPdpPtr) += here->FEFET1_58);
            (*(here->FEFET1BPgpPtr) += here->FEFET1_59);
            (*(here->FEFET1BPspPtr) += here->FEFET1_60);
            (*(here->FEFET1BPbpPtr) += here->FEFET1_61);

            /* stamp gidl */
            (*(here->FEFET1DPdpPtr) += here->FEFET1_62);
            (*(here->FEFET1DPgpPtr) += here->FEFET1_63);
            (*(here->FEFET1DPspPtr) -= here->FEFET1_64);
            (*(here->FEFET1DPbpPtr) += here->FEFET1_65);
            (*(here->FEFET1BPdpPtr) -= here->FEFET1_66);
            (*(here->FEFET1BPgpPtr) -= here->FEFET1_67);
            (*(here->FEFET1BPspPtr) += here->FEFET1_68);
            (*(here->FEFET1BPbpPtr) -= here->FEFET1_69);
                /* stamp gisl */
            (*(here->FEFET1SPdpPtr) -= here->FEFET1_70);
            (*(here->FEFET1SPgpPtr) += here->FEFET1_71);
            (*(here->FEFET1SPspPtr) += here->FEFET1_72);
            (*(here->FEFET1SPbpPtr) += here->FEFET1_73);
            (*(here->FEFET1BPdpPtr) += here->FEFET1_74);
            (*(here->FEFET1BPgpPtr) -= here->FEFET1_75);
            (*(here->FEFET1BPspPtr) -= here->FEFET1_76);
            (*(here->FEFET1BPbpPtr) -= here->FEFET1_77);


            if (here->FEFET1rbodyMod)
            {   (*(here->FEFET1DPdbPtr) += here->FEFET1_78);
                (*(here->FEFET1SPsbPtr) -= here->FEFET1_79);

                (*(here->FEFET1DBdpPtr) += here->FEFET1_80);
                (*(here->FEFET1DBdbPtr) += here->FEFET1_81);
                (*(here->FEFET1DBbpPtr) -= here->FEFET1_82);
                (*(here->FEFET1DBbPtr) -= here->FEFET1_83);

                (*(here->FEFET1BPdbPtr) -= here->FEFET1_84);
                (*(here->FEFET1BPbPtr) -= here->FEFET1_85);
                (*(here->FEFET1BPsbPtr) -= here->FEFET1_86);
                (*(here->FEFET1BPbpPtr) += here->FEFET1_87);

                (*(here->FEFET1SBspPtr) += here->FEFET1_88);
                (*(here->FEFET1SBbpPtr) -= here->FEFET1_89);
                (*(here->FEFET1SBbPtr) -= here->FEFET1_90);
                (*(here->FEFET1SBsbPtr) += here->FEFET1_91);

                (*(here->FEFET1BdbPtr) -= here->FEFET1_92);
                (*(here->FEFET1BbpPtr) -= here->FEFET1_93);
                (*(here->FEFET1BsbPtr) -= here->FEFET1_94);
                (*(here->FEFET1BbPtr) += here->FEFET1_95);
            }

            if (here->FEFET1trnqsMod)
            {   (*(here->FEFET1QqPtr) += here->FEFET1_96);
                (*(here->FEFET1QgpPtr) += here->FEFET1_97);
                (*(here->FEFET1QdpPtr) += here->FEFET1_98);
                (*(here->FEFET1QspPtr) += here->FEFET1_99);
                (*(here->FEFET1QbpPtr) += here->FEFET1_100);

                (*(here->FEFET1DPqPtr) += here->FEFET1_101);
                (*(here->FEFET1SPqPtr) += here->FEFET1_102);
                (*(here->FEFET1GPqPtr) -= here->FEFET1_103);
            }
        }
    }

#endif


    /* function to compute poly depletion effect */
int FEFET1polyDepletion(
        double  phi,
        double  ngate,
        double  coxe,
        double  Vgs,
        double *Vgs_eff,
        double *dVgs_eff_dVg)
    {
    double T1, T2, T3, T4, T5, T6, T7, T8;

    /* Poly Gate Si Depletion Effect */
    if ((ngate > 1.0e18) && 
        (ngate < 1.0e25) && (Vgs > phi)) {
        T1 = 1.0e6 * CHARGE * EPSSI * ngate / (coxe * coxe);
        T8 = Vgs - phi;
        T4 = sqrt(1.0 + 2.0 * T8 / T1);
        T2 = 2.0 * T8 / (T4 + 1.0);
        T3 = 0.5 * T2 * T2 / T1; /* T3 = Vpoly */
        T7 = 1.12 - T3 - 0.05;
        T6 = sqrt(T7 * T7 + 0.224);
        T5 = 1.12 - 0.5 * (T7 + T6);
        *Vgs_eff = Vgs - T5;
        *dVgs_eff_dVg = 1.0 - (0.5 - 0.5 / T4) * (1.0 + T7 / T6); 
    }
    else {
        *Vgs_eff = Vgs;
        *dVgs_eff_dVg = 1.0;
    }
    return(0);
}
