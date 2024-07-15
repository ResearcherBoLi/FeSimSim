/**********
Copyright 2024 Xidian University.	All rights reserved.
 Author: Bo Li	at Hangzhou Institute of Technology	 
 Modified: 2024/06/15  Bo Li
 Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/ifsim.h"
#include "ngspice/cktdefs.h"
#include "ngspice/devdefs.h"
#include "fefet1def.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"

int
FEFET1mAsk(
CKTcircuit *ckt,
GENmodel *inst,
int which,
IFvalue *value)
{
    FEFET1model *model = (FEFET1model *)inst;

    NG_IGNORE(ckt);

    switch(which) 
    {   
            
        case FECAP1_MOD_FECAP:
            return(OK);
            
        case FECAP1_MOD_Alpha:
            value->rValue = model->FECAP1_Alpha;
            return(OK);

        case FECAP1_MOD_Beta: 
            value->rValue = model->FECAP1_Beta; 
            return(OK); 
        
        case FECAP1_MOD_Gamma: 
            value->rValue = model->FECAP1_Gamma; 
            return(OK); 

        case FECAP1_MOD_Ec: 
            value->rValue = model->FECAP1_Ec; 
            return(OK);

        case FECAP1_MOD_Ps: 
            value->rValue = model->FECAP1_Ps; 
            return(OK);

        case FECAP1_MOD_Rho: 
            value->rValue = model->FECAP1_Rho; 
            return(OK);

        case FECAP1_MOD_Thickness: 
            value->rValue = model->FECAP1_Thickness; 
            return(OK);          
        
        
        case FEFET1_MOD_MOBMOD :
            value->iValue = model->FEFET1mobMod; 
            return(OK);
        case FEFET1_MOD_PARAMCHK :
            value->iValue = model->FEFET1paramChk; 
            return(OK);
        case FEFET1_MOD_BINUNIT :
            value->iValue = model->FEFET1binUnit; 
            return(OK);
        case FEFET1_MOD_CAPMOD :
            value->iValue = model->FEFET1capMod; 
            return(OK);
        case FEFET1_MOD_DIOMOD :
            value->iValue = model->FEFET1dioMod;
            return(OK);
        case FEFET1_MOD_TRNQSMOD :
            value->iValue = model->FEFET1trnqsMod;
            return(OK);
        case FEFET1_MOD_ACNQSMOD :
            value->iValue = model->FEFET1acnqsMod;
            return(OK);
        case FEFET1_MOD_FNOIMOD :
            value->iValue = model->FEFET1fnoiMod; 
            return(OK);
        case FEFET1_MOD_TNOIMOD :
            value->iValue = model->FEFET1tnoiMod;
            return(OK);
        case FEFET1_MOD_RDSMOD :
            value->iValue = model->FEFET1rdsMod;
            return(OK);
        case FEFET1_MOD_RBODYMOD :
            value->iValue = model->FEFET1rbodyMod;
            return(OK);
        case FEFET1_MOD_RGATEMOD :
            value->iValue = model->FEFET1rgateMod;
            return(OK);
        case FEFET1_MOD_PERMOD :
            value->iValue = model->FEFET1perMod;
            return(OK);
        case FEFET1_MOD_GEOMOD :
            value->iValue = model->FEFET1geoMod;
            return(OK);
        case FEFET1_MOD_RGEOMOD :
            value->iValue = model->FEFET1rgeoMod;
            return(OK);
        case FEFET1_MOD_IGCMOD :
            value->iValue = model->FEFET1igcMod;
            return(OK);
        case FEFET1_MOD_IGBMOD :
            value->iValue = model->FEFET1igbMod;
            return(OK);
        case  FEFET1_MOD_TEMPMOD :
            value->iValue = model->FEFET1tempMod;
            return(OK);

        case  FEFET1_MOD_VERSION :
          value->sValue = model->FEFET1version;
            return(OK);
        case  FEFET1_MOD_TOXREF :
          value->rValue = model->FEFET1toxref;
          return(OK);
        case  FEFET1_MOD_TOXE :
          value->rValue = model->FEFET1toxe;
            return(OK);
        case  FEFET1_MOD_TOXP :
          value->rValue = model->FEFET1toxp;
            return(OK);
        case  FEFET1_MOD_TOXM :
          value->rValue = model->FEFET1toxm;
            return(OK);
        case  FEFET1_MOD_DTOX :
          value->rValue = model->FEFET1dtox;
            return(OK);
        case  FEFET1_MOD_EPSROX :
          value->rValue = model->FEFET1epsrox;
            return(OK);
        case  FEFET1_MOD_CDSC :
          value->rValue = model->FEFET1cdsc;
            return(OK);
        case  FEFET1_MOD_CDSCB :
          value->rValue = model->FEFET1cdscb;
            return(OK);

        case  FEFET1_MOD_CDSCD :
          value->rValue = model->FEFET1cdscd;
            return(OK);

        case  FEFET1_MOD_CIT :
          value->rValue = model->FEFET1cit;
            return(OK);
        case  FEFET1_MOD_NFACTOR :
          value->rValue = model->FEFET1nfactor;
            return(OK);
        case FEFET1_MOD_XJ:
            value->rValue = model->FEFET1xj;
            return(OK);
        case FEFET1_MOD_VSAT:
            value->rValue = model->FEFET1vsat;
            return(OK);
        case FEFET1_MOD_VTL:
            value->rValue = model->FEFET1vtl;
            return(OK);
        case FEFET1_MOD_XN:
            value->rValue = model->FEFET1xn;
            return(OK);
        case FEFET1_MOD_LC:
            value->rValue = model->FEFET1lc;
            return(OK);
        case FEFET1_MOD_LAMBDA:
            value->rValue = model->FEFET1lambda;
            return(OK);
        case FEFET1_MOD_AT:
            value->rValue = model->FEFET1at;
            return(OK);
        case FEFET1_MOD_A0:
            value->rValue = model->FEFET1a0;
            return(OK);

        case FEFET1_MOD_AGS:
            value->rValue = model->FEFET1ags;
            return(OK);

        case FEFET1_MOD_A1:
            value->rValue = model->FEFET1a1;
            return(OK);
        case FEFET1_MOD_A2:
            value->rValue = model->FEFET1a2;
            return(OK);
        case FEFET1_MOD_KETA:
            value->rValue = model->FEFET1keta;
            return(OK);   
        case FEFET1_MOD_NSUB:
            value->rValue = model->FEFET1nsub;
            return(OK);
        case FEFET1_MOD_NDEP:
            value->rValue = model->FEFET1ndep;
            return(OK);
        case FEFET1_MOD_NSD:
            value->rValue = model->FEFET1nsd;
            return(OK);
        case FEFET1_MOD_NGATE:
            value->rValue = model->FEFET1ngate;
            return(OK);
        case FEFET1_MOD_GAMMA1:
            value->rValue = model->FEFET1gamma1;
            return(OK);
        case FEFET1_MOD_GAMMA2:
            value->rValue = model->FEFET1gamma2;
            return(OK);
        case FEFET1_MOD_VBX:
            value->rValue = model->FEFET1vbx;
            return(OK);
        case FEFET1_MOD_VBM:
            value->rValue = model->FEFET1vbm;
            return(OK);
        case FEFET1_MOD_XT:
            value->rValue = model->FEFET1xt;
            return(OK);
        case  FEFET1_MOD_K1:
          value->rValue = model->FEFET1k1;
            return(OK);
        case  FEFET1_MOD_KT1:
          value->rValue = model->FEFET1kt1;
            return(OK);
        case  FEFET1_MOD_KT1L:
          value->rValue = model->FEFET1kt1l;
            return(OK);
        case  FEFET1_MOD_KT2 :
          value->rValue = model->FEFET1kt2;
            return(OK);
        case  FEFET1_MOD_K2 :
          value->rValue = model->FEFET1k2;
            return(OK);
        case  FEFET1_MOD_K3:
          value->rValue = model->FEFET1k3;
            return(OK);
        case  FEFET1_MOD_K3B:
          value->rValue = model->FEFET1k3b;
            return(OK);
        case  FEFET1_MOD_W0:
          value->rValue = model->FEFET1w0;
            return(OK);
        case  FEFET1_MOD_LPE0:
          value->rValue = model->FEFET1lpe0;
            return(OK);
        case  FEFET1_MOD_LPEB:
          value->rValue = model->FEFET1lpeb;
            return(OK);
        case  FEFET1_MOD_DVTP0:
          value->rValue = model->FEFET1dvtp0;
            return(OK);
        case  FEFET1_MOD_DVTP1:
          value->rValue = model->FEFET1dvtp1;
            return(OK);
        case  FEFET1_MOD_DVT0 :                
          value->rValue = model->FEFET1dvt0;
            return(OK);
        case  FEFET1_MOD_DVT1 :             
          value->rValue = model->FEFET1dvt1;
            return(OK);
        case  FEFET1_MOD_DVT2 :             
          value->rValue = model->FEFET1dvt2;
            return(OK);
        case  FEFET1_MOD_DVT0W :                
          value->rValue = model->FEFET1dvt0w;
            return(OK);
        case  FEFET1_MOD_DVT1W :             
          value->rValue = model->FEFET1dvt1w;
            return(OK);
        case  FEFET1_MOD_DVT2W :             
          value->rValue = model->FEFET1dvt2w;
            return(OK);
        case  FEFET1_MOD_DROUT :           
          value->rValue = model->FEFET1drout;
            return(OK);
        case  FEFET1_MOD_DSUB :           
          value->rValue = model->FEFET1dsub;
            return(OK);
        case FEFET1_MOD_VTH0:
            value->rValue = model->FEFET1vth0; 
            return(OK);
        case FEFET1_MOD_EU:
            value->rValue = model->FEFET1eu;
            return(OK);
        case FEFET1_MOD_UA:
            value->rValue = model->FEFET1ua; 
            return(OK);
        case FEFET1_MOD_UA1:
            value->rValue = model->FEFET1ua1; 
            return(OK);
        case FEFET1_MOD_UB:
            value->rValue = model->FEFET1ub;  
            return(OK);
        case FEFET1_MOD_UB1:
            value->rValue = model->FEFET1ub1;  
            return(OK);
        case FEFET1_MOD_UC:
            value->rValue = model->FEFET1uc; 
            return(OK);
        case FEFET1_MOD_UC1:
            value->rValue = model->FEFET1uc1; 
            return(OK);
        case FEFET1_MOD_UD:
            value->rValue = model->FEFET1ud; 
            return(OK);
        case FEFET1_MOD_UD1:
            value->rValue = model->FEFET1ud1; 
            return(OK);
        case FEFET1_MOD_UP:
            value->rValue = model->FEFET1up; 
            return(OK);
        case FEFET1_MOD_LP:
            value->rValue = model->FEFET1lp; 
            return(OK);
        case FEFET1_MOD_U0:
            value->rValue = model->FEFET1u0;
            return(OK);
        case FEFET1_MOD_UTE:
            value->rValue = model->FEFET1ute;
            return(OK);
        case FEFET1_MOD_VOFF:
            value->rValue = model->FEFET1voff;
            return(OK);
        case FEFET1_MOD_TVOFF:
            value->rValue = model->FEFET1tvoff;
            return(OK);
        case FEFET1_MOD_VFBSDOFF:
            value->rValue = model->FEFET1vfbsdoff;
            return(OK);
        case FEFET1_MOD_TVFBSDOFF:
            value->rValue = model->FEFET1tvfbsdoff;
            return(OK);
        case FEFET1_MOD_VOFFL:
            value->rValue = model->FEFET1voffl;
            return(OK);
        case FEFET1_MOD_MINV:
            value->rValue = model->FEFET1minv;
            return(OK);
        case FEFET1_MOD_FPROUT:
            value->rValue = model->FEFET1fprout;
            return(OK);
        case FEFET1_MOD_PDITS:
            value->rValue = model->FEFET1pdits;
            return(OK);
        case FEFET1_MOD_PDITSD:
            value->rValue = model->FEFET1pditsd;
            return(OK);
        case FEFET1_MOD_PDITSL:
            value->rValue = model->FEFET1pditsl;
            return(OK);
        case FEFET1_MOD_DELTA:
            value->rValue = model->FEFET1delta;
            return(OK);
        case FEFET1_MOD_RDSW:
            value->rValue = model->FEFET1rdsw; 
            return(OK);
        case FEFET1_MOD_RDSWMIN:
            value->rValue = model->FEFET1rdswmin;
            return(OK);
        case FEFET1_MOD_RDWMIN:
            value->rValue = model->FEFET1rdwmin;
            return(OK);
        case FEFET1_MOD_RSWMIN:
            value->rValue = model->FEFET1rswmin;
            return(OK);
        case FEFET1_MOD_RDW:
            value->rValue = model->FEFET1rdw;
            return(OK);
        case FEFET1_MOD_RSW:
            value->rValue = model->FEFET1rsw;
            return(OK);
        case FEFET1_MOD_PRWG:
            value->rValue = model->FEFET1prwg; 
            return(OK);             
        case FEFET1_MOD_PRWB:
            value->rValue = model->FEFET1prwb; 
            return(OK);             
        case FEFET1_MOD_PRT:
            value->rValue = model->FEFET1prt; 
            return(OK);              
        case FEFET1_MOD_ETA0:
            value->rValue = model->FEFET1eta0; 
            return(OK);               
        case FEFET1_MOD_ETAB:
            value->rValue = model->FEFET1etab; 
            return(OK);               
        case FEFET1_MOD_PCLM:
            value->rValue = model->FEFET1pclm; 
            return(OK);               
        case FEFET1_MOD_PDIBL1:
            value->rValue = model->FEFET1pdibl1; 
            return(OK);               
        case FEFET1_MOD_PDIBL2:
            value->rValue = model->FEFET1pdibl2; 
            return(OK);               
        case FEFET1_MOD_PDIBLB:
            value->rValue = model->FEFET1pdiblb; 
            return(OK);               
        case FEFET1_MOD_PSCBE1:
            value->rValue = model->FEFET1pscbe1; 
            return(OK);               
        case FEFET1_MOD_PSCBE2:
            value->rValue = model->FEFET1pscbe2; 
            return(OK);               
        case FEFET1_MOD_PVAG:
            value->rValue = model->FEFET1pvag; 
            return(OK);               
        case FEFET1_MOD_WR:
            value->rValue = model->FEFET1wr;
            return(OK);
        case FEFET1_MOD_DWG:
            value->rValue = model->FEFET1dwg;
            return(OK);
        case FEFET1_MOD_DWB:
            value->rValue = model->FEFET1dwb;
            return(OK);
        case FEFET1_MOD_B0:
            value->rValue = model->FEFET1b0;
            return(OK);
        case FEFET1_MOD_B1:
            value->rValue = model->FEFET1b1;
            return(OK);
        case FEFET1_MOD_ALPHA0:
            value->rValue = model->FEFET1alpha0;
            return(OK);
        case FEFET1_MOD_ALPHA1:
            value->rValue = model->FEFET1alpha1;
            return(OK);
        case FEFET1_MOD_BETA0:
            value->rValue = model->FEFET1beta0;
            return(OK);
        case FEFET1_MOD_AGIDL:
            value->rValue = model->FEFET1agidl;
            return(OK);
        case FEFET1_MOD_BGIDL:
            value->rValue = model->FEFET1bgidl;
            return(OK);
        case FEFET1_MOD_CGIDL:
            value->rValue = model->FEFET1cgidl;
            return(OK);
        case FEFET1_MOD_EGIDL:
            value->rValue = model->FEFET1egidl;
            return(OK);
        case FEFET1_MOD_AIGC:
            value->rValue = model->FEFET1aigc;
            return(OK);
        case FEFET1_MOD_BIGC:
            value->rValue = model->FEFET1bigc;
            return(OK);
        case FEFET1_MOD_CIGC:
            value->rValue = model->FEFET1cigc;
            return(OK);
        case FEFET1_MOD_AIGSD:
            value->rValue = model->FEFET1aigsd;
            return(OK);
        case FEFET1_MOD_BIGSD:
            value->rValue = model->FEFET1bigsd;
            return(OK);
        case FEFET1_MOD_CIGSD:
            value->rValue = model->FEFET1cigsd;
            return(OK);
        case FEFET1_MOD_AIGBACC:
            value->rValue = model->FEFET1aigbacc;
            return(OK);
        case FEFET1_MOD_BIGBACC:
            value->rValue = model->FEFET1bigbacc;
            return(OK);
        case FEFET1_MOD_CIGBACC:
            value->rValue = model->FEFET1cigbacc;
            return(OK);
        case FEFET1_MOD_AIGBINV:
            value->rValue = model->FEFET1aigbinv;
            return(OK);
        case FEFET1_MOD_BIGBINV:
            value->rValue = model->FEFET1bigbinv;
            return(OK);
        case FEFET1_MOD_CIGBINV:
            value->rValue = model->FEFET1cigbinv;
            return(OK);
        case FEFET1_MOD_NIGC:
            value->rValue = model->FEFET1nigc;
            return(OK);
        case FEFET1_MOD_NIGBACC:
            value->rValue = model->FEFET1nigbacc;
            return(OK);
        case FEFET1_MOD_NIGBINV:
            value->rValue = model->FEFET1nigbinv;
            return(OK);
        case FEFET1_MOD_NTOX:
            value->rValue = model->FEFET1ntox;
            return(OK);
        case FEFET1_MOD_EIGBINV:
            value->rValue = model->FEFET1eigbinv;
            return(OK);
        case FEFET1_MOD_PIGCD:
            value->rValue = model->FEFET1pigcd;
            return(OK);
        case FEFET1_MOD_POXEDGE:
            value->rValue = model->FEFET1poxedge;
            return(OK);
        case FEFET1_MOD_PHIN:
            value->rValue = model->FEFET1phin;
            return(OK);
        case FEFET1_MOD_XRCRG1:
            value->rValue = model->FEFET1xrcrg1;
            return(OK);
        case FEFET1_MOD_XRCRG2:
            value->rValue = model->FEFET1xrcrg2;
            return(OK);
        case FEFET1_MOD_TNOIA:
            value->rValue = model->FEFET1tnoia;
            return(OK);
        case FEFET1_MOD_TNOIB:
            value->rValue = model->FEFET1tnoib;
            return(OK);
        case FEFET1_MOD_RNOIA:
            value->rValue = model->FEFET1rnoia;
            return(OK);
        case FEFET1_MOD_RNOIB:
            value->rValue = model->FEFET1rnoib;
            return(OK);
        case FEFET1_MOD_NTNOI:
            value->rValue = model->FEFET1ntnoi;
            return(OK);
        case FEFET1_MOD_IJTHDFWD:
            value->rValue = model->FEFET1ijthdfwd;
            return(OK);
        case FEFET1_MOD_IJTHSFWD:
            value->rValue = model->FEFET1ijthsfwd;
            return(OK);
        case FEFET1_MOD_IJTHDREV:
            value->rValue = model->FEFET1ijthdrev;
            return(OK);
        case FEFET1_MOD_IJTHSREV:
            value->rValue = model->FEFET1ijthsrev;
            return(OK);
        case FEFET1_MOD_XJBVD:
            value->rValue = model->FEFET1xjbvd;
            return(OK);
        case FEFET1_MOD_XJBVS:
            value->rValue = model->FEFET1xjbvs;
            return(OK);
        case FEFET1_MOD_BVD:
            value->rValue = model->FEFET1bvd;
            return(OK);
        case FEFET1_MOD_BVS:
            value->rValue = model->FEFET1bvs;
            return(OK);
        case FEFET1_MOD_VFB:
            value->rValue = model->FEFET1vfb;
            return(OK);

        case FEFET1_MOD_JTSS:
            value->rValue = model->FEFET1jtss;
            return(OK);
        case FEFET1_MOD_JTSD:
            value->rValue = model->FEFET1jtsd;
            return(OK);
        case FEFET1_MOD_JTSSWS:
            value->rValue = model->FEFET1jtssws;
            return(OK);
        case FEFET1_MOD_JTSSWD:
            value->rValue = model->FEFET1jtsswd;
            return(OK);
        case FEFET1_MOD_JTSSWGS:
            value->rValue = model->FEFET1jtsswgs;
            return(OK);
        case FEFET1_MOD_JTSSWGD:
            value->rValue = model->FEFET1jtsswgd;
            return(OK);
        case FEFET1_MOD_NJTS:
            value->rValue = model->FEFET1njts;
            return(OK);
        case FEFET1_MOD_NJTSSW:
            value->rValue = model->FEFET1njtssw;
            return(OK);
        case FEFET1_MOD_NJTSSWG:
            value->rValue = model->FEFET1njtsswg;
            return(OK);
        case FEFET1_MOD_XTSS:
            value->rValue = model->FEFET1xtss;
            return(OK);
        case FEFET1_MOD_XTSD:
            value->rValue = model->FEFET1xtsd;
            return(OK);
        case FEFET1_MOD_XTSSWS:
            value->rValue = model->FEFET1xtssws;
            return(OK);
        case FEFET1_MOD_XTSSWD:
            value->rValue = model->FEFET1xtsswd;
            return(OK);
        case FEFET1_MOD_XTSSWGS:
            value->rValue = model->FEFET1xtsswgs;
            return(OK);
        case FEFET1_MOD_XTSSWGD:
            value->rValue = model->FEFET1xtsswgd;
            return(OK);
        case FEFET1_MOD_TNJTS:
            value->rValue = model->FEFET1tnjts;
            return(OK);
        case FEFET1_MOD_TNJTSSW:
            value->rValue = model->FEFET1tnjtssw;
            return(OK);
        case FEFET1_MOD_TNJTSSWG:
            value->rValue = model->FEFET1tnjtsswg;
            return(OK);
        case FEFET1_MOD_VTSS:
            value->rValue = model->FEFET1vtss;
            return(OK);
        case FEFET1_MOD_VTSD:
            value->rValue = model->FEFET1vtsd;
            return(OK);
        case FEFET1_MOD_VTSSWS:
            value->rValue = model->FEFET1vtssws;
            return(OK);
        case FEFET1_MOD_VTSSWD:
            value->rValue = model->FEFET1vtsswd;
            return(OK);
        case FEFET1_MOD_VTSSWGS:
            value->rValue = model->FEFET1vtsswgs;
            return(OK);
        case FEFET1_MOD_VTSSWGD:
            value->rValue = model->FEFET1vtsswgd;
            return(OK);

        case FEFET1_MOD_GBMIN:
            value->rValue = model->FEFET1gbmin;
            return(OK);
        case FEFET1_MOD_RBDB:
            value->rValue = model->FEFET1rbdb;
            return(OK);
        case FEFET1_MOD_RBPB:
            value->rValue = model->FEFET1rbpb;
            return(OK);
        case FEFET1_MOD_RBSB:
            value->rValue = model->FEFET1rbsb;
            return(OK);
        case FEFET1_MOD_RBPS:
            value->rValue = model->FEFET1rbps;
            return(OK);
        case FEFET1_MOD_RBPD:
            value->rValue = model->FEFET1rbpd;
            return(OK);

        case FEFET1_MOD_RBPS0:
            value->rValue = model->FEFET1rbps0;
            return(OK);
        case FEFET1_MOD_RBPSL:
            value->rValue = model->FEFET1rbpsl;
            return(OK);
        case FEFET1_MOD_RBPSW:
            value->rValue = model->FEFET1rbpsw;
            return(OK);
        case FEFET1_MOD_RBPSNF:
            value->rValue = model->FEFET1rbpsnf;
            return(OK);
        case FEFET1_MOD_RBPD0:
            value->rValue = model->FEFET1rbpd0;
            return(OK);
        case FEFET1_MOD_RBPDL:
            value->rValue = model->FEFET1rbpdl;
            return(OK);
        case FEFET1_MOD_RBPDW:
            value->rValue = model->FEFET1rbpdw;
            return(OK);
        case FEFET1_MOD_RBPDNF:
            value->rValue = model->FEFET1rbpdnf;
            return(OK);
        case FEFET1_MOD_RBPBX0:
            value->rValue = model->FEFET1rbpbx0;
            return(OK);
        case FEFET1_MOD_RBPBXL:
            value->rValue = model->FEFET1rbpbxl;
            return(OK);
        case FEFET1_MOD_RBPBXW:
            value->rValue = model->FEFET1rbpbxw;
            return(OK);
        case FEFET1_MOD_RBPBXNF:
            value->rValue = model->FEFET1rbpbxnf;
            return(OK);
        case FEFET1_MOD_RBPBY0:
            value->rValue = model->FEFET1rbpby0;
            return(OK);
        case FEFET1_MOD_RBPBYL:
            value->rValue = model->FEFET1rbpbyl;
            return(OK);
        case FEFET1_MOD_RBPBYW:
            value->rValue = model->FEFET1rbpbyw;
            return(OK);
        case FEFET1_MOD_RBPBYNF:
            value->rValue = model->FEFET1rbpbynf;
            return(OK);

        case FEFET1_MOD_RBSBX0:
            value->rValue = model->FEFET1rbsbx0;
            return(OK);
        case FEFET1_MOD_RBSBY0:
            value->rValue = model->FEFET1rbsby0;
            return(OK);
        case FEFET1_MOD_RBDBX0:
            value->rValue = model->FEFET1rbdbx0;
            return(OK);
        case FEFET1_MOD_RBDBY0:
            value->rValue = model->FEFET1rbdby0;
            return(OK);
        case FEFET1_MOD_RBSDBXL:
            value->rValue = model->FEFET1rbsdbxl;
            return(OK);
        case FEFET1_MOD_RBSDBXW:
            value->rValue = model->FEFET1rbsdbxw;
            return(OK);
        case FEFET1_MOD_RBSDBXNF:
            value->rValue = model->FEFET1rbsdbxnf;
            return(OK);
        case FEFET1_MOD_RBSDBYL:
            value->rValue = model->FEFET1rbsdbyl;
            return(OK);
        case FEFET1_MOD_RBSDBYW:
            value->rValue = model->FEFET1rbsdbyw;
            return(OK);
        case FEFET1_MOD_RBSDBYNF:
            value->rValue = model->FEFET1rbsdbynf;
            return(OK);


        case FEFET1_MOD_CGSL:
            value->rValue = model->FEFET1cgsl;
            return(OK);
        case FEFET1_MOD_CGDL:
            value->rValue = model->FEFET1cgdl;
            return(OK);
        case FEFET1_MOD_CKAPPAS:
            value->rValue = model->FEFET1ckappas;
            return(OK);
        case FEFET1_MOD_CKAPPAD:
            value->rValue = model->FEFET1ckappad;
            return(OK);
        case FEFET1_MOD_CF:
            value->rValue = model->FEFET1cf;
            return(OK);
        case FEFET1_MOD_CLC:
            value->rValue = model->FEFET1clc;
            return(OK);
        case FEFET1_MOD_CLE:
            value->rValue = model->FEFET1cle;
            return(OK);
        case FEFET1_MOD_DWC:
            value->rValue = model->FEFET1dwc;
            return(OK);
        case FEFET1_MOD_DLC:
            value->rValue = model->FEFET1dlc;
            return(OK);
        case FEFET1_MOD_XW:
            value->rValue = model->FEFET1xw;
            return(OK);
        case FEFET1_MOD_XL:
            value->rValue = model->FEFET1xl;
            return(OK);
        case FEFET1_MOD_DLCIG:
            value->rValue = model->FEFET1dlcig;
            return(OK);
        case FEFET1_MOD_DWJ:
            value->rValue = model->FEFET1dwj;
            return(OK);
        case FEFET1_MOD_VFBCV:
            value->rValue = model->FEFET1vfbcv; 
            return(OK);
        case FEFET1_MOD_ACDE:
            value->rValue = model->FEFET1acde;
            return(OK);
        case FEFET1_MOD_MOIN:
            value->rValue = model->FEFET1moin;
            return(OK);
        case FEFET1_MOD_NOFF:
            value->rValue = model->FEFET1noff;
            return(OK);
        case FEFET1_MOD_VOFFCV:
            value->rValue = model->FEFET1voffcv;
            return(OK);
        case FEFET1_MOD_DMCG:
            value->rValue = model->FEFET1dmcg;
            return(OK);
        case FEFET1_MOD_DMCI:
            value->rValue = model->FEFET1dmci;
            return(OK);
        case FEFET1_MOD_DMDG:
            value->rValue = model->FEFET1dmdg;
            return(OK);
        case FEFET1_MOD_DMCGT:
            value->rValue = model->FEFET1dmcgt;
            return(OK);
        case FEFET1_MOD_XGW:
            value->rValue = model->FEFET1xgw;
            return(OK);
        case FEFET1_MOD_XGL:
            value->rValue = model->FEFET1xgl;
            return(OK);
        case FEFET1_MOD_RSHG:
            value->rValue = model->FEFET1rshg;
            return(OK);
        case FEFET1_MOD_NGCON:
            value->rValue = model->FEFET1ngcon;
            return(OK);
        case FEFET1_MOD_TCJ:
            value->rValue = model->FEFET1tcj;
            return(OK);
        case FEFET1_MOD_TPB:
            value->rValue = model->FEFET1tpb;
            return(OK);
        case FEFET1_MOD_TCJSW:
            value->rValue = model->FEFET1tcjsw;
            return(OK);
        case FEFET1_MOD_TPBSW:
            value->rValue = model->FEFET1tpbsw;
            return(OK);
        case FEFET1_MOD_TCJSWG:
            value->rValue = model->FEFET1tcjswg;
            return(OK);
        case FEFET1_MOD_TPBSWG:
            value->rValue = model->FEFET1tpbswg;
            return(OK);

	/* Length dependence */
        case  FEFET1_MOD_LCDSC :
          value->rValue = model->FEFET1lcdsc;
            return(OK);
        case  FEFET1_MOD_LCDSCB :
          value->rValue = model->FEFET1lcdscb;
            return(OK);
        case  FEFET1_MOD_LCDSCD :
          value->rValue = model->FEFET1lcdscd;
            return(OK);
        case  FEFET1_MOD_LCIT :
          value->rValue = model->FEFET1lcit;
            return(OK);
        case  FEFET1_MOD_LNFACTOR :
          value->rValue = model->FEFET1lnfactor;
            return(OK);
        case FEFET1_MOD_LXJ:
            value->rValue = model->FEFET1lxj;
            return(OK);
        case FEFET1_MOD_LVSAT:
            value->rValue = model->FEFET1lvsat;
            return(OK);
        case FEFET1_MOD_LAT:
            value->rValue = model->FEFET1lat;
            return(OK);
        case FEFET1_MOD_LA0:
            value->rValue = model->FEFET1la0;
            return(OK);
        case FEFET1_MOD_LAGS:
            value->rValue = model->FEFET1lags;
            return(OK);
        case FEFET1_MOD_LA1:
            value->rValue = model->FEFET1la1;
            return(OK);
        case FEFET1_MOD_LA2:
            value->rValue = model->FEFET1la2;
            return(OK);
        case FEFET1_MOD_LKETA:
            value->rValue = model->FEFET1lketa;
            return(OK);   
        case FEFET1_MOD_LNSUB:
            value->rValue = model->FEFET1lnsub;
            return(OK);
        case FEFET1_MOD_LNDEP:
            value->rValue = model->FEFET1lndep;
            return(OK);
        case FEFET1_MOD_LNSD:
            value->rValue = model->FEFET1lnsd;
            return(OK);
        case FEFET1_MOD_LNGATE:
            value->rValue = model->FEFET1lngate;
            return(OK);
        case FEFET1_MOD_LGAMMA1:
            value->rValue = model->FEFET1lgamma1;
            return(OK);
        case FEFET1_MOD_LGAMMA2:
            value->rValue = model->FEFET1lgamma2;
            return(OK);
        case FEFET1_MOD_LVBX:
            value->rValue = model->FEFET1lvbx;
            return(OK);
        case FEFET1_MOD_LVBM:
            value->rValue = model->FEFET1lvbm;
            return(OK);
        case FEFET1_MOD_LXT:
            value->rValue = model->FEFET1lxt;
            return(OK);
        case  FEFET1_MOD_LK1:
          value->rValue = model->FEFET1lk1;
            return(OK);
        case  FEFET1_MOD_LKT1:
          value->rValue = model->FEFET1lkt1;
            return(OK);
        case  FEFET1_MOD_LKT1L:
          value->rValue = model->FEFET1lkt1l;
            return(OK);
        case  FEFET1_MOD_LKT2 :
          value->rValue = model->FEFET1lkt2;
            return(OK);
        case  FEFET1_MOD_LK2 :
          value->rValue = model->FEFET1lk2;
            return(OK);
        case  FEFET1_MOD_LK3:
          value->rValue = model->FEFET1lk3;
            return(OK);
        case  FEFET1_MOD_LK3B:
          value->rValue = model->FEFET1lk3b;
            return(OK);
        case  FEFET1_MOD_LW0:
          value->rValue = model->FEFET1lw0;
            return(OK);
        case  FEFET1_MOD_LLPE0:
          value->rValue = model->FEFET1llpe0;
            return(OK);
        case  FEFET1_MOD_LLPEB:
          value->rValue = model->FEFET1llpeb;
            return(OK);
        case  FEFET1_MOD_LDVTP0:
          value->rValue = model->FEFET1ldvtp0;
            return(OK);
        case  FEFET1_MOD_LDVTP1:
          value->rValue = model->FEFET1ldvtp1;
            return(OK);
        case  FEFET1_MOD_LDVT0:                
          value->rValue = model->FEFET1ldvt0;
            return(OK);
        case  FEFET1_MOD_LDVT1 :             
          value->rValue = model->FEFET1ldvt1;
            return(OK);
        case  FEFET1_MOD_LDVT2 :             
          value->rValue = model->FEFET1ldvt2;
            return(OK);
        case  FEFET1_MOD_LDVT0W :                
          value->rValue = model->FEFET1ldvt0w;
            return(OK);
        case  FEFET1_MOD_LDVT1W :             
          value->rValue = model->FEFET1ldvt1w;
            return(OK);
        case  FEFET1_MOD_LDVT2W :             
          value->rValue = model->FEFET1ldvt2w;
            return(OK);
        case  FEFET1_MOD_LDROUT :           
          value->rValue = model->FEFET1ldrout;
            return(OK);
        case  FEFET1_MOD_LDSUB :           
          value->rValue = model->FEFET1ldsub;
            return(OK);
        case FEFET1_MOD_LVTH0:
            value->rValue = model->FEFET1lvth0; 
            return(OK);
        case FEFET1_MOD_LUA:
            value->rValue = model->FEFET1lua; 
            return(OK);
        case FEFET1_MOD_LUA1:
            value->rValue = model->FEFET1lua1; 
            return(OK);
        case FEFET1_MOD_LUB:
            value->rValue = model->FEFET1lub;  
            return(OK);
        case FEFET1_MOD_LUB1:
            value->rValue = model->FEFET1lub1;  
            return(OK);
        case FEFET1_MOD_LUC:
            value->rValue = model->FEFET1luc; 
            return(OK);
        case FEFET1_MOD_LUC1:
            value->rValue = model->FEFET1luc1; 
            return(OK);
        case FEFET1_MOD_LUD:
            value->rValue = model->FEFET1lud; 
            return(OK);
        case FEFET1_MOD_LUD1:
            value->rValue = model->FEFET1lud1; 
            return(OK);
        case FEFET1_MOD_LUP:
            value->rValue = model->FEFET1lup; 
            return(OK);
        case FEFET1_MOD_LLP:
            value->rValue = model->FEFET1llp; 
            return(OK);
        case FEFET1_MOD_LU0:
            value->rValue = model->FEFET1lu0;
            return(OK);
        case FEFET1_MOD_LUTE:
            value->rValue = model->FEFET1lute;
            return(OK);
        case FEFET1_MOD_LVOFF:
            value->rValue = model->FEFET1lvoff;
            return(OK);
        case FEFET1_MOD_LTVOFF:
            value->rValue = model->FEFET1ltvoff;
            return(OK);
        case FEFET1_MOD_LMINV:
            value->rValue = model->FEFET1lminv;
            return(OK);
        case FEFET1_MOD_LFPROUT:
            value->rValue = model->FEFET1lfprout;
            return(OK);
        case FEFET1_MOD_LPDITS:
            value->rValue = model->FEFET1lpdits;
            return(OK);
        case FEFET1_MOD_LPDITSD:
            value->rValue = model->FEFET1lpditsd;
            return(OK);
        case FEFET1_MOD_LDELTA:
            value->rValue = model->FEFET1ldelta;
            return(OK);
        case FEFET1_MOD_LRDSW:
            value->rValue = model->FEFET1lrdsw; 
            return(OK);             
        case FEFET1_MOD_LRDW:
            value->rValue = model->FEFET1lrdw;
            return(OK);
        case FEFET1_MOD_LRSW:
            value->rValue = model->FEFET1lrsw;
            return(OK);
        case FEFET1_MOD_LPRWB:
            value->rValue = model->FEFET1lprwb; 
            return(OK);             
        case FEFET1_MOD_LPRWG:
            value->rValue = model->FEFET1lprwg; 
            return(OK);             
        case FEFET1_MOD_LPRT:
            value->rValue = model->FEFET1lprt; 
            return(OK);              
        case FEFET1_MOD_LETA0:
            value->rValue = model->FEFET1leta0; 
            return(OK);               
        case FEFET1_MOD_LETAB:
            value->rValue = model->FEFET1letab; 
            return(OK);               
        case FEFET1_MOD_LPCLM:
            value->rValue = model->FEFET1lpclm; 
            return(OK);               
        case FEFET1_MOD_LPDIBL1:
            value->rValue = model->FEFET1lpdibl1; 
            return(OK);               
        case FEFET1_MOD_LPDIBL2:
            value->rValue = model->FEFET1lpdibl2; 
            return(OK);               
        case FEFET1_MOD_LPDIBLB:
            value->rValue = model->FEFET1lpdiblb; 
            return(OK);               
        case FEFET1_MOD_LPSCBE1:
            value->rValue = model->FEFET1lpscbe1; 
            return(OK);               
        case FEFET1_MOD_LPSCBE2:
            value->rValue = model->FEFET1lpscbe2; 
            return(OK);               
        case FEFET1_MOD_LPVAG:
            value->rValue = model->FEFET1lpvag; 
            return(OK);               
        case FEFET1_MOD_LWR:
            value->rValue = model->FEFET1lwr;
            return(OK);
        case FEFET1_MOD_LDWG:
            value->rValue = model->FEFET1ldwg;
            return(OK);
        case FEFET1_MOD_LDWB:
            value->rValue = model->FEFET1ldwb;
            return(OK);
        case FEFET1_MOD_LB0:
            value->rValue = model->FEFET1lb0;
            return(OK);
        case FEFET1_MOD_LB1:
            value->rValue = model->FEFET1lb1;
            return(OK);
        case FEFET1_MOD_LALPHA0:
            value->rValue = model->FEFET1lalpha0;
            return(OK);
        case FEFET1_MOD_LALPHA1:
            value->rValue = model->FEFET1lalpha1;
            return(OK);
        case FEFET1_MOD_LBETA0:
            value->rValue = model->FEFET1lbeta0;
            return(OK);
        case FEFET1_MOD_LAGIDL:
            value->rValue = model->FEFET1lagidl;
            return(OK);
        case FEFET1_MOD_LBGIDL:
            value->rValue = model->FEFET1lbgidl;
            return(OK);
        case FEFET1_MOD_LCGIDL:
            value->rValue = model->FEFET1lcgidl;
            return(OK);
        case FEFET1_MOD_LEGIDL:
            value->rValue = model->FEFET1legidl;
            return(OK);
        case FEFET1_MOD_LAIGC:
            value->rValue = model->FEFET1laigc;
            return(OK);
        case FEFET1_MOD_LBIGC:
            value->rValue = model->FEFET1lbigc;
            return(OK);
        case FEFET1_MOD_LCIGC:
            value->rValue = model->FEFET1lcigc;
            return(OK);
        case FEFET1_MOD_LAIGSD:
            value->rValue = model->FEFET1laigsd;
            return(OK);
        case FEFET1_MOD_LBIGSD:
            value->rValue = model->FEFET1lbigsd;
            return(OK);
        case FEFET1_MOD_LCIGSD:
            value->rValue = model->FEFET1lcigsd;
            return(OK);
        case FEFET1_MOD_LAIGBACC:
            value->rValue = model->FEFET1laigbacc;
            return(OK);
        case FEFET1_MOD_LBIGBACC:
            value->rValue = model->FEFET1lbigbacc;
            return(OK);
        case FEFET1_MOD_LCIGBACC:
            value->rValue = model->FEFET1lcigbacc;
            return(OK);
        case FEFET1_MOD_LAIGBINV:
            value->rValue = model->FEFET1laigbinv;
            return(OK);
        case FEFET1_MOD_LBIGBINV:
            value->rValue = model->FEFET1lbigbinv;
            return(OK);
        case FEFET1_MOD_LCIGBINV:
            value->rValue = model->FEFET1lcigbinv;
            return(OK);
        case FEFET1_MOD_LNIGC:
            value->rValue = model->FEFET1lnigc;
            return(OK);
        case FEFET1_MOD_LNIGBACC:
            value->rValue = model->FEFET1lnigbacc;
            return(OK);
        case FEFET1_MOD_LNIGBINV:
            value->rValue = model->FEFET1lnigbinv;
            return(OK);
        case FEFET1_MOD_LNTOX:
            value->rValue = model->FEFET1lntox;
            return(OK);
        case FEFET1_MOD_LEIGBINV:
            value->rValue = model->FEFET1leigbinv;
            return(OK);
        case FEFET1_MOD_LPIGCD:
            value->rValue = model->FEFET1lpigcd;
            return(OK);
        case FEFET1_MOD_LPOXEDGE:
            value->rValue = model->FEFET1lpoxedge;
            return(OK);
        case FEFET1_MOD_LPHIN:
            value->rValue = model->FEFET1lphin;
            return(OK);
        case FEFET1_MOD_LXRCRG1:
            value->rValue = model->FEFET1lxrcrg1;
            return(OK);
        case FEFET1_MOD_LXRCRG2:
            value->rValue = model->FEFET1lxrcrg2;
            return(OK);
        case FEFET1_MOD_LEU:
            value->rValue = model->FEFET1leu;
            return(OK);
        case FEFET1_MOD_LVFB:
            value->rValue = model->FEFET1lvfb;
            return(OK);

        case FEFET1_MOD_LCGSL:
            value->rValue = model->FEFET1lcgsl;
            return(OK);
        case FEFET1_MOD_LCGDL:
            value->rValue = model->FEFET1lcgdl;
            return(OK);
        case FEFET1_MOD_LCKAPPAS:
            value->rValue = model->FEFET1lckappas;
            return(OK);
        case FEFET1_MOD_LCKAPPAD:
            value->rValue = model->FEFET1lckappad;
            return(OK);
        case FEFET1_MOD_LCF:
            value->rValue = model->FEFET1lcf;
            return(OK);
        case FEFET1_MOD_LCLC:
            value->rValue = model->FEFET1lclc;
            return(OK);
        case FEFET1_MOD_LCLE:
            value->rValue = model->FEFET1lcle;
            return(OK);
        case FEFET1_MOD_LVFBCV:
            value->rValue = model->FEFET1lvfbcv;
            return(OK);
        case FEFET1_MOD_LACDE:
            value->rValue = model->FEFET1lacde;
            return(OK);
        case FEFET1_MOD_LMOIN:
            value->rValue = model->FEFET1lmoin;
            return(OK);
        case FEFET1_MOD_LNOFF:
            value->rValue = model->FEFET1lnoff;
            return(OK);
        case FEFET1_MOD_LVOFFCV:
            value->rValue = model->FEFET1lvoffcv;
            return(OK);
        case FEFET1_MOD_LVFBSDOFF:
            value->rValue = model->FEFET1lvfbsdoff;
            return(OK);
        case FEFET1_MOD_LTVFBSDOFF:
            value->rValue = model->FEFET1ltvfbsdoff;
            return(OK);

	/* Width dependence */
        case  FEFET1_MOD_WCDSC :
          value->rValue = model->FEFET1wcdsc;
            return(OK);
        case  FEFET1_MOD_WCDSCB :
          value->rValue = model->FEFET1wcdscb;
            return(OK);
        case  FEFET1_MOD_WCDSCD :
          value->rValue = model->FEFET1wcdscd;
            return(OK);
        case  FEFET1_MOD_WCIT :
          value->rValue = model->FEFET1wcit;
            return(OK);
        case  FEFET1_MOD_WNFACTOR :
          value->rValue = model->FEFET1wnfactor;
            return(OK);
        case FEFET1_MOD_WXJ:
            value->rValue = model->FEFET1wxj;
            return(OK);
        case FEFET1_MOD_WVSAT:
            value->rValue = model->FEFET1wvsat;
            return(OK);
        case FEFET1_MOD_WAT:
            value->rValue = model->FEFET1wat;
            return(OK);
        case FEFET1_MOD_WA0:
            value->rValue = model->FEFET1wa0;
            return(OK);
        case FEFET1_MOD_WAGS:
            value->rValue = model->FEFET1wags;
            return(OK);
        case FEFET1_MOD_WA1:
            value->rValue = model->FEFET1wa1;
            return(OK);
        case FEFET1_MOD_WA2:
            value->rValue = model->FEFET1wa2;
            return(OK);
        case FEFET1_MOD_WKETA:
            value->rValue = model->FEFET1wketa;
            return(OK);   
        case FEFET1_MOD_WNSUB:
            value->rValue = model->FEFET1wnsub;
            return(OK);
        case FEFET1_MOD_WNDEP:
            value->rValue = model->FEFET1wndep;
            return(OK);
        case FEFET1_MOD_WNSD:
            value->rValue = model->FEFET1wnsd;
            return(OK);
        case FEFET1_MOD_WNGATE:
            value->rValue = model->FEFET1wngate;
            return(OK);
        case FEFET1_MOD_WGAMMA1:
            value->rValue = model->FEFET1wgamma1;
            return(OK);
        case FEFET1_MOD_WGAMMA2:
            value->rValue = model->FEFET1wgamma2;
            return(OK);
        case FEFET1_MOD_WVBX:
            value->rValue = model->FEFET1wvbx;
            return(OK);
        case FEFET1_MOD_WVBM:
            value->rValue = model->FEFET1wvbm;
            return(OK);
        case FEFET1_MOD_WXT:
            value->rValue = model->FEFET1wxt;
            return(OK);
        case  FEFET1_MOD_WK1:
          value->rValue = model->FEFET1wk1;
            return(OK);
        case  FEFET1_MOD_WKT1:
          value->rValue = model->FEFET1wkt1;
            return(OK);
        case  FEFET1_MOD_WKT1L:
          value->rValue = model->FEFET1wkt1l;
            return(OK);
        case  FEFET1_MOD_WKT2 :
          value->rValue = model->FEFET1wkt2;
            return(OK);
        case  FEFET1_MOD_WK2 :
          value->rValue = model->FEFET1wk2;
            return(OK);
        case  FEFET1_MOD_WK3:
          value->rValue = model->FEFET1wk3;
            return(OK);
        case  FEFET1_MOD_WK3B:
          value->rValue = model->FEFET1wk3b;
            return(OK);
        case  FEFET1_MOD_WW0:
          value->rValue = model->FEFET1ww0;
            return(OK);
        case  FEFET1_MOD_WLPE0:
          value->rValue = model->FEFET1wlpe0;
            return(OK);
        case  FEFET1_MOD_WDVTP0:
          value->rValue = model->FEFET1wdvtp0;
            return(OK);
        case  FEFET1_MOD_WDVTP1:
          value->rValue = model->FEFET1wdvtp1;
            return(OK);
        case  FEFET1_MOD_WLPEB:
          value->rValue = model->FEFET1wlpeb;
            return(OK);
        case  FEFET1_MOD_WDVT0:                
          value->rValue = model->FEFET1wdvt0;
            return(OK);
        case  FEFET1_MOD_WDVT1 :             
          value->rValue = model->FEFET1wdvt1;
            return(OK);
        case  FEFET1_MOD_WDVT2 :             
          value->rValue = model->FEFET1wdvt2;
            return(OK);
        case  FEFET1_MOD_WDVT0W :                
          value->rValue = model->FEFET1wdvt0w;
            return(OK);
        case  FEFET1_MOD_WDVT1W :             
          value->rValue = model->FEFET1wdvt1w;
            return(OK);
        case  FEFET1_MOD_WDVT2W :             
          value->rValue = model->FEFET1wdvt2w;
            return(OK);
        case  FEFET1_MOD_WDROUT :           
          value->rValue = model->FEFET1wdrout;
            return(OK);
        case  FEFET1_MOD_WDSUB :           
          value->rValue = model->FEFET1wdsub;
            return(OK);
        case FEFET1_MOD_WVTH0:
            value->rValue = model->FEFET1wvth0; 
            return(OK);
        case FEFET1_MOD_WUA:
            value->rValue = model->FEFET1wua; 
            return(OK);
        case FEFET1_MOD_WUA1:
            value->rValue = model->FEFET1wua1; 
            return(OK);
        case FEFET1_MOD_WUB:
            value->rValue = model->FEFET1wub;  
            return(OK);
        case FEFET1_MOD_WUB1:
            value->rValue = model->FEFET1wub1;  
            return(OK);
        case FEFET1_MOD_WUC:
            value->rValue = model->FEFET1wuc; 
            return(OK);
        case FEFET1_MOD_WUC1:
            value->rValue = model->FEFET1wuc1; 
            return(OK);
        case FEFET1_MOD_WUD:
            value->rValue = model->FEFET1wud; 
            return(OK);
        case FEFET1_MOD_WUD1:
            value->rValue = model->FEFET1wud1; 
            return(OK);
        case FEFET1_MOD_WUP:
            value->rValue = model->FEFET1wup; 
            return(OK);
        case FEFET1_MOD_WLP:
            value->rValue = model->FEFET1wlp; 
            return(OK);
        case FEFET1_MOD_WU0:
            value->rValue = model->FEFET1wu0;
            return(OK);
        case FEFET1_MOD_WUTE:
            value->rValue = model->FEFET1wute;
            return(OK);
        case FEFET1_MOD_WVOFF:
            value->rValue = model->FEFET1wvoff;
            return(OK);
        case FEFET1_MOD_WTVOFF:
            value->rValue = model->FEFET1wtvoff;
            return(OK);
        case FEFET1_MOD_WMINV:
            value->rValue = model->FEFET1wminv;
            return(OK);
        case FEFET1_MOD_WFPROUT:
            value->rValue = model->FEFET1wfprout;
            return(OK);
        case FEFET1_MOD_WPDITS:
            value->rValue = model->FEFET1wpdits;
            return(OK);
        case FEFET1_MOD_WPDITSD:
            value->rValue = model->FEFET1wpditsd;
            return(OK);
        case FEFET1_MOD_WDELTA:
            value->rValue = model->FEFET1wdelta;
            return(OK);
        case FEFET1_MOD_WRDSW:
            value->rValue = model->FEFET1wrdsw; 
            return(OK);             
        case FEFET1_MOD_WRDW:
            value->rValue = model->FEFET1wrdw;
            return(OK);
        case FEFET1_MOD_WRSW:
            value->rValue = model->FEFET1wrsw;
            return(OK);
        case FEFET1_MOD_WPRWB:
            value->rValue = model->FEFET1wprwb; 
            return(OK);             
        case FEFET1_MOD_WPRWG:
            value->rValue = model->FEFET1wprwg; 
            return(OK);             
        case FEFET1_MOD_WPRT:
            value->rValue = model->FEFET1wprt; 
            return(OK);              
        case FEFET1_MOD_WETA0:
            value->rValue = model->FEFET1weta0; 
            return(OK);               
        case FEFET1_MOD_WETAB:
            value->rValue = model->FEFET1wetab; 
            return(OK);               
        case FEFET1_MOD_WPCLM:
            value->rValue = model->FEFET1wpclm; 
            return(OK);               
        case FEFET1_MOD_WPDIBL1:
            value->rValue = model->FEFET1wpdibl1; 
            return(OK);               
        case FEFET1_MOD_WPDIBL2:
            value->rValue = model->FEFET1wpdibl2; 
            return(OK);               
        case FEFET1_MOD_WPDIBLB:
            value->rValue = model->FEFET1wpdiblb; 
            return(OK);               
        case FEFET1_MOD_WPSCBE1:
            value->rValue = model->FEFET1wpscbe1; 
            return(OK);               
        case FEFET1_MOD_WPSCBE2:
            value->rValue = model->FEFET1wpscbe2; 
            return(OK);               
        case FEFET1_MOD_WPVAG:
            value->rValue = model->FEFET1wpvag; 
            return(OK);               
        case FEFET1_MOD_WWR:
            value->rValue = model->FEFET1wwr;
            return(OK);
        case FEFET1_MOD_WDWG:
            value->rValue = model->FEFET1wdwg;
            return(OK);
        case FEFET1_MOD_WDWB:
            value->rValue = model->FEFET1wdwb;
            return(OK);
        case FEFET1_MOD_WB0:
            value->rValue = model->FEFET1wb0;
            return(OK);
        case FEFET1_MOD_WB1:
            value->rValue = model->FEFET1wb1;
            return(OK);
        case FEFET1_MOD_WALPHA0:
            value->rValue = model->FEFET1walpha0;
            return(OK);
        case FEFET1_MOD_WALPHA1:
            value->rValue = model->FEFET1walpha1;
            return(OK);
        case FEFET1_MOD_WBETA0:
            value->rValue = model->FEFET1wbeta0;
            return(OK);
        case FEFET1_MOD_WAGIDL:
            value->rValue = model->FEFET1wagidl;
            return(OK);
        case FEFET1_MOD_WBGIDL:
            value->rValue = model->FEFET1wbgidl;
            return(OK);
        case FEFET1_MOD_WCGIDL:
            value->rValue = model->FEFET1wcgidl;
            return(OK);
        case FEFET1_MOD_WEGIDL:
            value->rValue = model->FEFET1wegidl;
            return(OK);
        case FEFET1_MOD_WAIGC:
            value->rValue = model->FEFET1waigc;
            return(OK);
        case FEFET1_MOD_WBIGC:
            value->rValue = model->FEFET1wbigc;
            return(OK);
        case FEFET1_MOD_WCIGC:
            value->rValue = model->FEFET1wcigc;
            return(OK);
        case FEFET1_MOD_WAIGSD:
            value->rValue = model->FEFET1waigsd;
            return(OK);
        case FEFET1_MOD_WBIGSD:
            value->rValue = model->FEFET1wbigsd;
            return(OK);
        case FEFET1_MOD_WCIGSD:
            value->rValue = model->FEFET1wcigsd;
            return(OK);
        case FEFET1_MOD_WAIGBACC:
            value->rValue = model->FEFET1waigbacc;
            return(OK);
        case FEFET1_MOD_WBIGBACC:
            value->rValue = model->FEFET1wbigbacc;
            return(OK);
        case FEFET1_MOD_WCIGBACC:
            value->rValue = model->FEFET1wcigbacc;
            return(OK);
        case FEFET1_MOD_WAIGBINV:
            value->rValue = model->FEFET1waigbinv;
            return(OK);
        case FEFET1_MOD_WBIGBINV:
            value->rValue = model->FEFET1wbigbinv;
            return(OK);
        case FEFET1_MOD_WCIGBINV:
            value->rValue = model->FEFET1wcigbinv;
            return(OK);
        case FEFET1_MOD_WNIGC:
            value->rValue = model->FEFET1wnigc;
            return(OK);
        case FEFET1_MOD_WNIGBACC:
            value->rValue = model->FEFET1wnigbacc;
            return(OK);
        case FEFET1_MOD_WNIGBINV:
            value->rValue = model->FEFET1wnigbinv;
            return(OK);
        case FEFET1_MOD_WNTOX:
            value->rValue = model->FEFET1wntox;
            return(OK);
        case FEFET1_MOD_WEIGBINV:
            value->rValue = model->FEFET1weigbinv;
            return(OK);
        case FEFET1_MOD_WPIGCD:
            value->rValue = model->FEFET1wpigcd;
            return(OK);
        case FEFET1_MOD_WPOXEDGE:
            value->rValue = model->FEFET1wpoxedge;
            return(OK);
        case FEFET1_MOD_WPHIN:
            value->rValue = model->FEFET1wphin;
            return(OK);
        case FEFET1_MOD_WXRCRG1:
            value->rValue = model->FEFET1wxrcrg1;
            return(OK);
        case FEFET1_MOD_WXRCRG2:
            value->rValue = model->FEFET1wxrcrg2;
            return(OK);
        case FEFET1_MOD_WEU:
            value->rValue = model->FEFET1weu;
            return(OK);
        case FEFET1_MOD_WVFB:
            value->rValue = model->FEFET1wvfb;
            return(OK);

        case FEFET1_MOD_WCGSL:
            value->rValue = model->FEFET1wcgsl;
            return(OK);
        case FEFET1_MOD_WCGDL:
            value->rValue = model->FEFET1wcgdl;
            return(OK);
        case FEFET1_MOD_WCKAPPAS:
            value->rValue = model->FEFET1wckappas;
            return(OK);
        case FEFET1_MOD_WCKAPPAD:
            value->rValue = model->FEFET1wckappad;
            return(OK);
        case FEFET1_MOD_WCF:
            value->rValue = model->FEFET1wcf;
            return(OK);
        case FEFET1_MOD_WCLC:
            value->rValue = model->FEFET1wclc;
            return(OK);
        case FEFET1_MOD_WCLE:
            value->rValue = model->FEFET1wcle;
            return(OK);
        case FEFET1_MOD_WVFBCV:
            value->rValue = model->FEFET1wvfbcv;
            return(OK);
        case FEFET1_MOD_WACDE:
            value->rValue = model->FEFET1wacde;
            return(OK);
        case FEFET1_MOD_WMOIN:
            value->rValue = model->FEFET1wmoin;
            return(OK);
        case FEFET1_MOD_WNOFF:
            value->rValue = model->FEFET1wnoff;
            return(OK);
        case FEFET1_MOD_WVOFFCV:
            value->rValue = model->FEFET1wvoffcv;
            return(OK);
        case FEFET1_MOD_WVFBSDOFF:
            value->rValue = model->FEFET1wvfbsdoff;
            return(OK);
        case FEFET1_MOD_WTVFBSDOFF:
            value->rValue = model->FEFET1wtvfbsdoff;
            return(OK);

	/* Cross-term dependence */
        case  FEFET1_MOD_PCDSC :
          value->rValue = model->FEFET1pcdsc;
            return(OK);
        case  FEFET1_MOD_PCDSCB :
          value->rValue = model->FEFET1pcdscb;
            return(OK);
        case  FEFET1_MOD_PCDSCD :
          value->rValue = model->FEFET1pcdscd;
            return(OK);
         case  FEFET1_MOD_PCIT :
          value->rValue = model->FEFET1pcit;
            return(OK);
        case  FEFET1_MOD_PNFACTOR :
          value->rValue = model->FEFET1pnfactor;
            return(OK);
        case FEFET1_MOD_PXJ:
            value->rValue = model->FEFET1pxj;
            return(OK);
        case FEFET1_MOD_PVSAT:
            value->rValue = model->FEFET1pvsat;
            return(OK);
        case FEFET1_MOD_PAT:
            value->rValue = model->FEFET1pat;
            return(OK);
        case FEFET1_MOD_PA0:
            value->rValue = model->FEFET1pa0;
            return(OK);
        case FEFET1_MOD_PAGS:
            value->rValue = model->FEFET1pags;
            return(OK);
        case FEFET1_MOD_PA1:
            value->rValue = model->FEFET1pa1;
            return(OK);
        case FEFET1_MOD_PA2:
            value->rValue = model->FEFET1pa2;
            return(OK);
        case FEFET1_MOD_PKETA:
            value->rValue = model->FEFET1pketa;
            return(OK);   
        case FEFET1_MOD_PNSUB:
            value->rValue = model->FEFET1pnsub;
            return(OK);
        case FEFET1_MOD_PNDEP:
            value->rValue = model->FEFET1pndep;
            return(OK);
        case FEFET1_MOD_PNSD:
            value->rValue = model->FEFET1pnsd;
            return(OK);
        case FEFET1_MOD_PNGATE:
            value->rValue = model->FEFET1pngate;
            return(OK);
        case FEFET1_MOD_PGAMMA1:
            value->rValue = model->FEFET1pgamma1;
            return(OK);
        case FEFET1_MOD_PGAMMA2:
            value->rValue = model->FEFET1pgamma2;
            return(OK);
        case FEFET1_MOD_PVBX:
            value->rValue = model->FEFET1pvbx;
            return(OK);
        case FEFET1_MOD_PVBM:
            value->rValue = model->FEFET1pvbm;
            return(OK);
        case FEFET1_MOD_PXT:
            value->rValue = model->FEFET1pxt;
            return(OK);
        case  FEFET1_MOD_PK1:
          value->rValue = model->FEFET1pk1;
            return(OK);
        case  FEFET1_MOD_PKT1:
          value->rValue = model->FEFET1pkt1;
            return(OK);
        case  FEFET1_MOD_PKT1L:
          value->rValue = model->FEFET1pkt1l;
            return(OK);
        case  FEFET1_MOD_PKT2 :
          value->rValue = model->FEFET1pkt2;
            return(OK);
        case  FEFET1_MOD_PK2 :
          value->rValue = model->FEFET1pk2;
            return(OK);
        case  FEFET1_MOD_PK3:
          value->rValue = model->FEFET1pk3;
            return(OK);
        case  FEFET1_MOD_PK3B:
          value->rValue = model->FEFET1pk3b;
            return(OK);
        case  FEFET1_MOD_PW0:
          value->rValue = model->FEFET1pw0;
            return(OK);
        case  FEFET1_MOD_PLPE0:
          value->rValue = model->FEFET1plpe0;
            return(OK);
        case  FEFET1_MOD_PLPEB:
          value->rValue = model->FEFET1plpeb;
            return(OK);
        case  FEFET1_MOD_PDVTP0:
          value->rValue = model->FEFET1pdvtp0;
            return(OK);
        case  FEFET1_MOD_PDVTP1:
          value->rValue = model->FEFET1pdvtp1;
            return(OK);
        case  FEFET1_MOD_PDVT0 :                
          value->rValue = model->FEFET1pdvt0;
            return(OK);
        case  FEFET1_MOD_PDVT1 :             
          value->rValue = model->FEFET1pdvt1;
            return(OK);
        case  FEFET1_MOD_PDVT2 :             
          value->rValue = model->FEFET1pdvt2;
            return(OK);
        case  FEFET1_MOD_PDVT0W :                
          value->rValue = model->FEFET1pdvt0w;
            return(OK);
        case  FEFET1_MOD_PDVT1W :             
          value->rValue = model->FEFET1pdvt1w;
            return(OK);
        case  FEFET1_MOD_PDVT2W :             
          value->rValue = model->FEFET1pdvt2w;
            return(OK);
        case  FEFET1_MOD_PDROUT :           
          value->rValue = model->FEFET1pdrout;
            return(OK);
        case  FEFET1_MOD_PDSUB :           
          value->rValue = model->FEFET1pdsub;
            return(OK);
        case FEFET1_MOD_PVTH0:
            value->rValue = model->FEFET1pvth0; 
            return(OK);
        case FEFET1_MOD_PUA:
            value->rValue = model->FEFET1pua; 
            return(OK);
        case FEFET1_MOD_PUA1:
            value->rValue = model->FEFET1pua1; 
            return(OK);
        case FEFET1_MOD_PUB:
            value->rValue = model->FEFET1pub;  
            return(OK);
        case FEFET1_MOD_PUB1:
            value->rValue = model->FEFET1pub1;  
            return(OK);
        case FEFET1_MOD_PUC:
            value->rValue = model->FEFET1puc; 
            return(OK);
        case FEFET1_MOD_PUC1:
            value->rValue = model->FEFET1puc1; 
            return(OK);
        case FEFET1_MOD_PUD:
            value->rValue = model->FEFET1pud; 
            return(OK);
        case FEFET1_MOD_PUD1:
            value->rValue = model->FEFET1pud1; 
            return(OK);
        case FEFET1_MOD_PUP:
            value->rValue = model->FEFET1pup; 
            return(OK);
        case FEFET1_MOD_PLP:
            value->rValue = model->FEFET1plp; 
            return(OK);
        case FEFET1_MOD_PU0:
            value->rValue = model->FEFET1pu0;
            return(OK);
        case FEFET1_MOD_PUTE:
            value->rValue = model->FEFET1pute;
            return(OK);
        case FEFET1_MOD_PVOFF:
            value->rValue = model->FEFET1pvoff;
            return(OK);
        case FEFET1_MOD_PTVOFF:
            value->rValue = model->FEFET1ptvoff;
            return(OK);
        case FEFET1_MOD_PMINV:
            value->rValue = model->FEFET1pminv;
            return(OK);
        case FEFET1_MOD_PFPROUT:
            value->rValue = model->FEFET1pfprout;
            return(OK);
        case FEFET1_MOD_PPDITS:
            value->rValue = model->FEFET1ppdits;
            return(OK);
        case FEFET1_MOD_PPDITSD:
            value->rValue = model->FEFET1ppditsd;
            return(OK);
        case FEFET1_MOD_PDELTA:
            value->rValue = model->FEFET1pdelta;
            return(OK);
        case FEFET1_MOD_PRDSW:
            value->rValue = model->FEFET1prdsw; 
            return(OK);             
        case FEFET1_MOD_PRDW:
            value->rValue = model->FEFET1prdw;
            return(OK);
        case FEFET1_MOD_PRSW:
            value->rValue = model->FEFET1prsw;
            return(OK);
        case FEFET1_MOD_PPRWB:
            value->rValue = model->FEFET1pprwb; 
            return(OK);             
        case FEFET1_MOD_PPRWG:
            value->rValue = model->FEFET1pprwg; 
            return(OK);             
        case FEFET1_MOD_PPRT:
            value->rValue = model->FEFET1pprt; 
            return(OK);              
        case FEFET1_MOD_PETA0:
            value->rValue = model->FEFET1peta0; 
            return(OK);               
        case FEFET1_MOD_PETAB:
            value->rValue = model->FEFET1petab; 
            return(OK);               
        case FEFET1_MOD_PPCLM:
            value->rValue = model->FEFET1ppclm; 
            return(OK);               
        case FEFET1_MOD_PPDIBL1:
            value->rValue = model->FEFET1ppdibl1; 
            return(OK);               
        case FEFET1_MOD_PPDIBL2:
            value->rValue = model->FEFET1ppdibl2; 
            return(OK);               
        case FEFET1_MOD_PPDIBLB:
            value->rValue = model->FEFET1ppdiblb; 
            return(OK);               
        case FEFET1_MOD_PPSCBE1:
            value->rValue = model->FEFET1ppscbe1; 
            return(OK);               
        case FEFET1_MOD_PPSCBE2:
            value->rValue = model->FEFET1ppscbe2; 
            return(OK);               
        case FEFET1_MOD_PPVAG:
            value->rValue = model->FEFET1ppvag; 
            return(OK);               
        case FEFET1_MOD_PWR:
            value->rValue = model->FEFET1pwr;
            return(OK);
        case FEFET1_MOD_PDWG:
            value->rValue = model->FEFET1pdwg;
            return(OK);
        case FEFET1_MOD_PDWB:
            value->rValue = model->FEFET1pdwb;
            return(OK);
        case FEFET1_MOD_PB0:
            value->rValue = model->FEFET1pb0;
            return(OK);
        case FEFET1_MOD_PB1:
            value->rValue = model->FEFET1pb1;
            return(OK);
        case FEFET1_MOD_PALPHA0:
            value->rValue = model->FEFET1palpha0;
            return(OK);
        case FEFET1_MOD_PALPHA1:
            value->rValue = model->FEFET1palpha1;
            return(OK);
        case FEFET1_MOD_PBETA0:
            value->rValue = model->FEFET1pbeta0;
            return(OK);
        case FEFET1_MOD_PAGIDL:
            value->rValue = model->FEFET1pagidl;
            return(OK);
        case FEFET1_MOD_PBGIDL:
            value->rValue = model->FEFET1pbgidl;
            return(OK);
        case FEFET1_MOD_PCGIDL:
            value->rValue = model->FEFET1pcgidl;
            return(OK);
        case FEFET1_MOD_PEGIDL:
            value->rValue = model->FEFET1pegidl;
            return(OK);
        case FEFET1_MOD_PAIGC:
            value->rValue = model->FEFET1paigc;
            return(OK);
        case FEFET1_MOD_PBIGC:
            value->rValue = model->FEFET1pbigc;
            return(OK);
        case FEFET1_MOD_PCIGC:
            value->rValue = model->FEFET1pcigc;
            return(OK);
        case FEFET1_MOD_PAIGSD:
            value->rValue = model->FEFET1paigsd;
            return(OK);
        case FEFET1_MOD_PBIGSD:
            value->rValue = model->FEFET1pbigsd;
            return(OK);
        case FEFET1_MOD_PCIGSD:
            value->rValue = model->FEFET1pcigsd;
            return(OK);
        case FEFET1_MOD_PAIGBACC:
            value->rValue = model->FEFET1paigbacc;
            return(OK);
        case FEFET1_MOD_PBIGBACC:
            value->rValue = model->FEFET1pbigbacc;
            return(OK);
        case FEFET1_MOD_PCIGBACC:
            value->rValue = model->FEFET1pcigbacc;
            return(OK);
        case FEFET1_MOD_PAIGBINV:
            value->rValue = model->FEFET1paigbinv;
            return(OK);
        case FEFET1_MOD_PBIGBINV:
            value->rValue = model->FEFET1pbigbinv;
            return(OK);
        case FEFET1_MOD_PCIGBINV:
            value->rValue = model->FEFET1pcigbinv;
            return(OK);
        case FEFET1_MOD_PNIGC:
            value->rValue = model->FEFET1pnigc;
            return(OK);
        case FEFET1_MOD_PNIGBACC:
            value->rValue = model->FEFET1pnigbacc;
            return(OK);
        case FEFET1_MOD_PNIGBINV:
            value->rValue = model->FEFET1pnigbinv;
            return(OK);
        case FEFET1_MOD_PNTOX:
            value->rValue = model->FEFET1pntox;
            return(OK);
        case FEFET1_MOD_PEIGBINV:
            value->rValue = model->FEFET1peigbinv;
            return(OK);
        case FEFET1_MOD_PPIGCD:
            value->rValue = model->FEFET1ppigcd;
            return(OK);
        case FEFET1_MOD_PPOXEDGE:
            value->rValue = model->FEFET1ppoxedge;
            return(OK);
        case FEFET1_MOD_PPHIN:
            value->rValue = model->FEFET1pphin;
            return(OK);
        case FEFET1_MOD_PXRCRG1:
            value->rValue = model->FEFET1pxrcrg1;
            return(OK);
        case FEFET1_MOD_PXRCRG2:
            value->rValue = model->FEFET1pxrcrg2;
            return(OK);
        case FEFET1_MOD_PEU:
            value->rValue = model->FEFET1peu;
            return(OK);
        case FEFET1_MOD_PVFB:
            value->rValue = model->FEFET1pvfb;
            return(OK);

        case FEFET1_MOD_PCGSL:
            value->rValue = model->FEFET1pcgsl;
            return(OK);
        case FEFET1_MOD_PCGDL:
            value->rValue = model->FEFET1pcgdl;
            return(OK);
        case FEFET1_MOD_PCKAPPAS:
            value->rValue = model->FEFET1pckappas;
            return(OK);
        case FEFET1_MOD_PCKAPPAD:
            value->rValue = model->FEFET1pckappad;
            return(OK);
        case FEFET1_MOD_PCF:
            value->rValue = model->FEFET1pcf;
            return(OK);
        case FEFET1_MOD_PCLC:
            value->rValue = model->FEFET1pclc;
            return(OK);
        case FEFET1_MOD_PCLE:
            value->rValue = model->FEFET1pcle;
            return(OK);
        case FEFET1_MOD_PVFBCV:
            value->rValue = model->FEFET1pvfbcv;
            return(OK);
        case FEFET1_MOD_PACDE:
            value->rValue = model->FEFET1pacde;
            return(OK);
        case FEFET1_MOD_PMOIN:
            value->rValue = model->FEFET1pmoin;
            return(OK);
        case FEFET1_MOD_PNOFF:
            value->rValue = model->FEFET1pnoff;
            return(OK);
        case FEFET1_MOD_PVOFFCV:
            value->rValue = model->FEFET1pvoffcv;
            return(OK);
        case FEFET1_MOD_PVFBSDOFF:
            value->rValue = model->FEFET1pvfbsdoff;
            return(OK);
        case FEFET1_MOD_PTVFBSDOFF:
            value->rValue = model->FEFET1ptvfbsdoff;
            return(OK);

        case  FEFET1_MOD_TNOM :
          value->rValue = model->FEFET1tnom;
            return(OK);
        case FEFET1_MOD_CGSO:
            value->rValue = model->FEFET1cgso; 
            return(OK);
        case FEFET1_MOD_CGDO:
            value->rValue = model->FEFET1cgdo; 
            return(OK);
        case FEFET1_MOD_CGBO:
            value->rValue = model->FEFET1cgbo; 
            return(OK);
        case FEFET1_MOD_XPART:
            value->rValue = model->FEFET1xpart; 
            return(OK);
        case FEFET1_MOD_RSH:
            value->rValue = model->FEFET1sheetResistance; 
            return(OK);
        case FEFET1_MOD_JSS:
            value->rValue = model->FEFET1SjctSatCurDensity; 
            return(OK);
        case FEFET1_MOD_JSWS:
            value->rValue = model->FEFET1SjctSidewallSatCurDensity; 
            return(OK);
        case FEFET1_MOD_JSWGS:
            value->rValue = model->FEFET1SjctGateSidewallSatCurDensity;
            return(OK);
        case FEFET1_MOD_PBS:
            value->rValue = model->FEFET1SbulkJctPotential; 
            return(OK);
        case FEFET1_MOD_MJS:
            value->rValue = model->FEFET1SbulkJctBotGradingCoeff; 
            return(OK);
        case FEFET1_MOD_PBSWS:
            value->rValue = model->FEFET1SsidewallJctPotential; 
            return(OK);
        case FEFET1_MOD_MJSWS:
            value->rValue = model->FEFET1SbulkJctSideGradingCoeff; 
            return(OK);
        case FEFET1_MOD_CJS:
            value->rValue = model->FEFET1SunitAreaJctCap; 
            return(OK);
        case FEFET1_MOD_CJSWS:
            value->rValue = model->FEFET1SunitLengthSidewallJctCap; 
            return(OK);
        case FEFET1_MOD_PBSWGS:
            value->rValue = model->FEFET1SGatesidewallJctPotential; 
            return(OK);
        case FEFET1_MOD_MJSWGS:
            value->rValue = model->FEFET1SbulkJctGateSideGradingCoeff; 
            return(OK);
        case FEFET1_MOD_CJSWGS:
            value->rValue = model->FEFET1SunitLengthGateSidewallJctCap; 
            return(OK);
        case FEFET1_MOD_NJS:
            value->rValue = model->FEFET1SjctEmissionCoeff; 
            return(OK);
        case FEFET1_MOD_XTIS:
            value->rValue = model->FEFET1SjctTempExponent; 
            return(OK);
        case FEFET1_MOD_JSD:
            value->rValue = model->FEFET1DjctSatCurDensity;
            return(OK);
        case FEFET1_MOD_JSWD:
            value->rValue = model->FEFET1DjctSidewallSatCurDensity;
            return(OK);
        case FEFET1_MOD_JSWGD:
            value->rValue = model->FEFET1DjctGateSidewallSatCurDensity;
            return(OK);
        case FEFET1_MOD_PBD:
            value->rValue = model->FEFET1DbulkJctPotential;
            return(OK);
        case FEFET1_MOD_MJD:
            value->rValue = model->FEFET1DbulkJctBotGradingCoeff;
            return(OK);
        case FEFET1_MOD_PBSWD:
            value->rValue = model->FEFET1DsidewallJctPotential;
            return(OK);
        case FEFET1_MOD_MJSWD:
            value->rValue = model->FEFET1DbulkJctSideGradingCoeff;
            return(OK);
        case FEFET1_MOD_CJD:
            value->rValue = model->FEFET1DunitAreaJctCap;
            return(OK);
        case FEFET1_MOD_CJSWD:
            value->rValue = model->FEFET1DunitLengthSidewallJctCap;
            return(OK);
        case FEFET1_MOD_PBSWGD:
            value->rValue = model->FEFET1DGatesidewallJctPotential;
            return(OK);
        case FEFET1_MOD_MJSWGD:
            value->rValue = model->FEFET1DbulkJctGateSideGradingCoeff;
            return(OK);
        case FEFET1_MOD_CJSWGD:
            value->rValue = model->FEFET1DunitLengthGateSidewallJctCap;
            return(OK);
        case FEFET1_MOD_NJD:
            value->rValue = model->FEFET1DjctEmissionCoeff;
            return(OK);
        case FEFET1_MOD_XTID:
            value->rValue = model->FEFET1DjctTempExponent;
            return(OK);
        case FEFET1_MOD_LINT:
            value->rValue = model->FEFET1Lint; 
            return(OK);
        case FEFET1_MOD_LL:
            value->rValue = model->FEFET1Ll;
            return(OK);
        case FEFET1_MOD_LLC:
            value->rValue = model->FEFET1Llc;
            return(OK);
        case FEFET1_MOD_LLN:
            value->rValue = model->FEFET1Lln;
            return(OK);
        case FEFET1_MOD_LW:
            value->rValue = model->FEFET1Lw;
            return(OK);
        case FEFET1_MOD_LWC:
            value->rValue = model->FEFET1Lwc;
            return(OK);
        case FEFET1_MOD_LWN:
            value->rValue = model->FEFET1Lwn;
            return(OK);
        case FEFET1_MOD_LWL:
            value->rValue = model->FEFET1Lwl;
            return(OK);
        case FEFET1_MOD_LWLC:
            value->rValue = model->FEFET1Lwlc;
            return(OK);
        case FEFET1_MOD_LMIN:
            value->rValue = model->FEFET1Lmin;
            return(OK);
        case FEFET1_MOD_LMAX:
            value->rValue = model->FEFET1Lmax;
            return(OK);
        case FEFET1_MOD_WINT:
            value->rValue = model->FEFET1Wint;
            return(OK);
        case FEFET1_MOD_WL:
            value->rValue = model->FEFET1Wl;
            return(OK);
        case FEFET1_MOD_WLC:
            value->rValue = model->FEFET1Wlc;
            return(OK);
        case FEFET1_MOD_WLN:
            value->rValue = model->FEFET1Wln;
            return(OK);
        case FEFET1_MOD_WW:
            value->rValue = model->FEFET1Ww;
            return(OK);
        case FEFET1_MOD_WWC:
            value->rValue = model->FEFET1Wwc;
            return(OK);
        case FEFET1_MOD_WWN:
            value->rValue = model->FEFET1Wwn;
            return(OK);
        case FEFET1_MOD_WWL:
            value->rValue = model->FEFET1Wwl;
            return(OK);
        case FEFET1_MOD_WWLC:
            value->rValue = model->FEFET1Wwlc;
            return(OK);
        case FEFET1_MOD_WMIN:
            value->rValue = model->FEFET1Wmin;
            return(OK);
        case FEFET1_MOD_WMAX:
            value->rValue = model->FEFET1Wmax;
            return(OK);

        /* stress effect */
        case FEFET1_MOD_SAREF:
            value->rValue = model->FEFET1saref;
            return(OK);
        case FEFET1_MOD_SBREF:
            value->rValue = model->FEFET1sbref;
            return(OK);
	case FEFET1_MOD_WLOD:
            value->rValue = model->FEFET1wlod;
            return(OK);
        case FEFET1_MOD_KU0:
            value->rValue = model->FEFET1ku0;
            return(OK);
        case FEFET1_MOD_KVSAT:
            value->rValue = model->FEFET1kvsat;
            return(OK);
        case FEFET1_MOD_KVTH0:
            value->rValue = model->FEFET1kvth0;
            return(OK);
        case FEFET1_MOD_TKU0:
            value->rValue = model->FEFET1tku0;
            return(OK);
        case FEFET1_MOD_LLODKU0:
            value->rValue = model->FEFET1llodku0;
            return(OK);
        case FEFET1_MOD_WLODKU0:
            value->rValue = model->FEFET1wlodku0;
            return(OK);
        case FEFET1_MOD_LLODVTH:
            value->rValue = model->FEFET1llodvth;
            return(OK);
        case FEFET1_MOD_WLODVTH:
            value->rValue = model->FEFET1wlodvth;
            return(OK);
        case FEFET1_MOD_LKU0:
            value->rValue = model->FEFET1lku0;
            return(OK);
        case FEFET1_MOD_WKU0:
            value->rValue = model->FEFET1wku0;
            return(OK);
        case FEFET1_MOD_PKU0:
            value->rValue = model->FEFET1pku0;
            return(OK);
        case FEFET1_MOD_LKVTH0:
            value->rValue = model->FEFET1lkvth0;
            return(OK);
        case FEFET1_MOD_WKVTH0:
            value->rValue = model->FEFET1wkvth0;
            return(OK);
        case FEFET1_MOD_PKVTH0:
            value->rValue = model->FEFET1pkvth0;
            return(OK);
        case FEFET1_MOD_STK2:
            value->rValue = model->FEFET1stk2;
            return(OK);
        case FEFET1_MOD_LODK2:
            value->rValue = model->FEFET1lodk2;
            return(OK);
        case FEFET1_MOD_STETA0:
            value->rValue = model->FEFET1steta0;
            return(OK);
        case FEFET1_MOD_LODETA0:
            value->rValue = model->FEFET1lodeta0;
            return(OK);

        /* Well Proximity Effect  */
        case FEFET1_MOD_WEB:
            value->rValue = model->FEFET1web;
            return(OK);
        case FEFET1_MOD_WEC:
            value->rValue = model->FEFET1wec;
            return(OK);
        case FEFET1_MOD_KVTH0WE:
            value->rValue = model->FEFET1kvth0we;
            return(OK);
        case FEFET1_MOD_K2WE:
            value->rValue = model->FEFET1k2we;
            return(OK);
        case FEFET1_MOD_KU0WE:
            value->rValue = model->FEFET1ku0we;
            return(OK);
        case FEFET1_MOD_SCREF:
            value->rValue = model->FEFET1scref;
            return(OK);
        case FEFET1_MOD_WPEMOD:
            value->rValue = model->FEFET1wpemod;
            return(OK);
        case FEFET1_MOD_LKVTH0WE:
            value->rValue = model->FEFET1lkvth0we;
            return(OK);
        case FEFET1_MOD_LK2WE:
            value->rValue = model->FEFET1lk2we;
            return(OK);
        case FEFET1_MOD_LKU0WE:
            value->rValue = model->FEFET1lku0we;
            return(OK);
        case FEFET1_MOD_WKVTH0WE:
            value->rValue = model->FEFET1wkvth0we;
            return(OK);
        case FEFET1_MOD_WK2WE:
            value->rValue = model->FEFET1wk2we;
            return(OK);
        case FEFET1_MOD_WKU0WE:
            value->rValue = model->FEFET1wku0we;
            return(OK);
        case FEFET1_MOD_PKVTH0WE:
            value->rValue = model->FEFET1pkvth0we;
            return(OK);
        case FEFET1_MOD_PK2WE:
            value->rValue = model->FEFET1pk2we;
            return(OK);
        case FEFET1_MOD_PKU0WE:
            value->rValue = model->FEFET1pku0we;
            return(OK);

        case FEFET1_MOD_NOIA:
            value->rValue = model->FEFET1oxideTrapDensityA;
            return(OK);
        case FEFET1_MOD_NOIB:
            value->rValue = model->FEFET1oxideTrapDensityB;
            return(OK);
        case FEFET1_MOD_NOIC:
            value->rValue = model->FEFET1oxideTrapDensityC;
            return(OK);
        case FEFET1_MOD_EM:
            value->rValue = model->FEFET1em;
            return(OK);
        case FEFET1_MOD_EF:
            value->rValue = model->FEFET1ef;
            return(OK);
        case FEFET1_MOD_AF:
            value->rValue = model->FEFET1af;
            return(OK);
        case FEFET1_MOD_KF:
            value->rValue = model->FEFET1kf;
            return(OK);

        case FEFET1_MOD_VGS_MAX:
            value->rValue = model->FEFET1vgsMax;
            return(OK);
        case FEFET1_MOD_VGD_MAX:
            value->rValue = model->FEFET1vgdMax;
            return(OK);
        case FEFET1_MOD_VGB_MAX:
            value->rValue = model->FEFET1vgbMax;
            return(OK);
        case FEFET1_MOD_VDS_MAX:
            value->rValue = model->FEFET1vdsMax;
            return(OK);
        case FEFET1_MOD_VBS_MAX:
            value->rValue = model->FEFET1vbsMax;
            return(OK);
        case FEFET1_MOD_VBD_MAX:
            value->rValue = model->FEFET1vbdMax;
            return(OK);
        case FEFET1_MOD_VGSR_MAX:
            value->rValue = model->FEFET1vgsrMax;
            return(OK);
        case FEFET1_MOD_VGDR_MAX:
            value->rValue = model->FEFET1vgdrMax;
            return(OK);
        case FEFET1_MOD_VGBR_MAX:
            value->rValue = model->FEFET1vgbrMax;
            return(OK);
        case FEFET1_MOD_VBSR_MAX:
            value->rValue = model->FEFET1vbsrMax;
            return(OK);
        case FEFET1_MOD_VBDR_MAX:
            value->rValue = model->FEFET1vbdrMax;
            return(OK);

        default:
            return(E_BADPARM);
    }
    /* NOTREACHED */
}



