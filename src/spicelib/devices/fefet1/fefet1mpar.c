/**********
Copyright 2024 Xidian University.	All rights reserved.
 Author: Bo Li	at Hangzhou Institute of Technology	 
 Modified: 2024/1/27  Bo Li
 Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "fefet1def.h"
#include "ngspice/ifsim.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"
#include "ngspice/const.h"

int
FEFET1mParam(
int param,
IFvalue *value,
GENmodel *inMod)
{
    FEFET1model *mod = (FEFET1model*)inMod;
    switch(param)
    {   
        
	case FECAP1_MOD_FECAP: 
		if(value->iValue){ 
			mod->FECAP1type = 1; 
			mod->FECAP1typeGiven = TRUE; 
		}
		break;

    case FECAP1_MOD_Thickness: 
		mod->FECAP1_Thickness = value->rValue; 
		mod->FECAP1_ThicknessGiven = TRUE; 
		break;

    case FECAP1_MOD_Ec: 
		mod->FECAP1_Ec = value->rValue; 
		mod->FECAP1_EcGiven = TRUE; 
		break;

    case FECAP1_MOD_Ps: 
		mod->FECAP1_Ps = value->rValue; 
		mod->FECAP1_PsGiven = TRUE; 
		break;

    case FECAP1_MOD_Alpha: 
		mod->FECAP1_Alpha = value->rValue; 
		mod->FECAP1_AlphaGiven = TRUE; 
		break;     

    case FECAP1_MOD_Beta: 
		mod->FECAP1_Beta = value->rValue; 
		mod->FECAP1_BetaGiven = TRUE; 
		break;     

    case FECAP1_MOD_Gamma: 
		mod->FECAP1_Gamma = value->rValue; 
		mod->FECAP1_GammaGiven = TRUE; 
		break;     		

    case FECAP1_MOD_Rho: 
		mod->FECAP1_Rho = value->rValue; 
		mod->FECAP1_RhoGiven = TRUE; 
		break;         
        
        case  FEFET1_MOD_MOBMOD :
            mod->FEFET1mobMod = value->iValue;
            mod->FEFET1mobModGiven = TRUE;
            break;
        case  FEFET1_MOD_BINUNIT :
            mod->FEFET1binUnit = value->iValue;
            mod->FEFET1binUnitGiven = TRUE;
            break;
        case  FEFET1_MOD_PARAMCHK :
            mod->FEFET1paramChk = value->iValue;
            mod->FEFET1paramChkGiven = TRUE;
            break;
        case  FEFET1_MOD_CAPMOD :
            mod->FEFET1capMod = value->iValue;
            mod->FEFET1capModGiven = TRUE;
            break;
        case  FEFET1_MOD_DIOMOD :
            mod->FEFET1dioMod = value->iValue;
            mod->FEFET1dioModGiven = TRUE;
            break;
        case  FEFET1_MOD_RDSMOD :
            mod->FEFET1rdsMod = value->iValue;
            mod->FEFET1rdsModGiven = TRUE;
            break;
        case  FEFET1_MOD_TRNQSMOD :
            mod->FEFET1trnqsMod = value->iValue;
            mod->FEFET1trnqsModGiven = TRUE;
            break;
        case  FEFET1_MOD_ACNQSMOD :
            mod->FEFET1acnqsMod = value->iValue;
            mod->FEFET1acnqsModGiven = TRUE;
            break;
        case  FEFET1_MOD_RBODYMOD :
            mod->FEFET1rbodyMod = value->iValue;
            mod->FEFET1rbodyModGiven = TRUE;
            break;
        case  FEFET1_MOD_RGATEMOD :
            mod->FEFET1rgateMod = value->iValue;
            mod->FEFET1rgateModGiven = TRUE;
            break;
        case  FEFET1_MOD_PERMOD :
            mod->FEFET1perMod = value->iValue;
            mod->FEFET1perModGiven = TRUE;
            break;
        case  FEFET1_MOD_GEOMOD :
            mod->FEFET1geoMod = value->iValue;
            mod->FEFET1geoModGiven = TRUE;
            break;
        case  FEFET1_MOD_RGEOMOD :
            mod->FEFET1rgeoMod = value->iValue;
            mod->FEFET1rgeoModGiven = TRUE;
            break;
        case  FEFET1_MOD_FNOIMOD :
            mod->FEFET1fnoiMod = value->iValue;
            mod->FEFET1fnoiModGiven = TRUE;
            break;
        case  FEFET1_MOD_TNOIMOD :
            mod->FEFET1tnoiMod = value->iValue;
            mod->FEFET1tnoiModGiven = TRUE;
            break;
        case  FEFET1_MOD_IGCMOD :
            mod->FEFET1igcMod = value->iValue;
            mod->FEFET1igcModGiven = TRUE;
            break;
        case  FEFET1_MOD_IGBMOD :
            mod->FEFET1igbMod = value->iValue;
            mod->FEFET1igbModGiven = TRUE;
            break;
        case  FEFET1_MOD_TEMPMOD :
            mod->FEFET1tempMod = value->iValue;
            mod->FEFET1tempModGiven = TRUE;
            break;

        case  FEFET1_MOD_VERSION :
            mod->FEFET1version = value->sValue;
            mod->FEFET1versionGiven = TRUE;
            break;
        case  FEFET1_MOD_TOXREF :
            mod->FEFET1toxref = value->rValue;
            mod->FEFET1toxrefGiven = TRUE;
            break;
        case  FEFET1_MOD_TOXE :
            mod->FEFET1toxe = value->rValue;
            mod->FEFET1toxeGiven = TRUE;
            break;
        case  FEFET1_MOD_TOXP :
            mod->FEFET1toxp = value->rValue;
            mod->FEFET1toxpGiven = TRUE;
            break;
        case  FEFET1_MOD_TOXM :
            mod->FEFET1toxm = value->rValue;
            mod->FEFET1toxmGiven = TRUE;
            break;
        case  FEFET1_MOD_DTOX :
            mod->FEFET1dtox = value->rValue;
            mod->FEFET1dtoxGiven = TRUE;
            break;
        case  FEFET1_MOD_EPSROX :
            mod->FEFET1epsrox = value->rValue;
            mod->FEFET1epsroxGiven = TRUE;
            break;

        case  FEFET1_MOD_CDSC :
            mod->FEFET1cdsc = value->rValue;
            mod->FEFET1cdscGiven = TRUE;
            break;
        case  FEFET1_MOD_CDSCB :
            mod->FEFET1cdscb = value->rValue;
            mod->FEFET1cdscbGiven = TRUE;
            break;

        case  FEFET1_MOD_CDSCD :
            mod->FEFET1cdscd = value->rValue;
            mod->FEFET1cdscdGiven = TRUE;
            break;

        case  FEFET1_MOD_CIT :
            mod->FEFET1cit = value->rValue;
            mod->FEFET1citGiven = TRUE;
            break;
        case  FEFET1_MOD_NFACTOR :
            mod->FEFET1nfactor = value->rValue;
            mod->FEFET1nfactorGiven = TRUE;
            break;
        case FEFET1_MOD_XJ:
            mod->FEFET1xj = value->rValue;
            mod->FEFET1xjGiven = TRUE;
            break;
        case FEFET1_MOD_VSAT:
            mod->FEFET1vsat = value->rValue;
            mod->FEFET1vsatGiven = TRUE;
            break;
        case FEFET1_MOD_A0:
            mod->FEFET1a0 = value->rValue;
            mod->FEFET1a0Given = TRUE;
            break;
        
        case FEFET1_MOD_AGS:
            mod->FEFET1ags= value->rValue;
            mod->FEFET1agsGiven = TRUE;
            break;
        
        case FEFET1_MOD_A1:
            mod->FEFET1a1 = value->rValue;
            mod->FEFET1a1Given = TRUE;
            break;
        case FEFET1_MOD_A2:
            mod->FEFET1a2 = value->rValue;
            mod->FEFET1a2Given = TRUE;
            break;
        case FEFET1_MOD_AT:
            mod->FEFET1at = value->rValue;
            mod->FEFET1atGiven = TRUE;
            break;
        case FEFET1_MOD_KETA:
            mod->FEFET1keta = value->rValue;
            mod->FEFET1ketaGiven = TRUE;
            break;    
        case FEFET1_MOD_NSUB:
            mod->FEFET1nsub = value->rValue;
            mod->FEFET1nsubGiven = TRUE;
            break;
        case FEFET1_MOD_NDEP:
            mod->FEFET1ndep = value->rValue;
            mod->FEFET1ndepGiven = TRUE;
	    if (mod->FEFET1ndep > 1.0e20)
		mod->FEFET1ndep *= 1.0e-6;
            break;
        case FEFET1_MOD_NSD:
            mod->FEFET1nsd = value->rValue;
            mod->FEFET1nsdGiven = TRUE;
            if (mod->FEFET1nsd > 1.0e23)
                mod->FEFET1nsd *= 1.0e-6;
            break;
        case FEFET1_MOD_NGATE:
            mod->FEFET1ngate = value->rValue;
            mod->FEFET1ngateGiven = TRUE;
	    if (mod->FEFET1ngate > 1.0e23)
		mod->FEFET1ngate *= 1.0e-6;
            break;
        case FEFET1_MOD_GAMMA1:
            mod->FEFET1gamma1 = value->rValue;
            mod->FEFET1gamma1Given = TRUE;
            break;
        case FEFET1_MOD_GAMMA2:
            mod->FEFET1gamma2 = value->rValue;
            mod->FEFET1gamma2Given = TRUE;
            break;
        case FEFET1_MOD_VBX:
            mod->FEFET1vbx = value->rValue;
            mod->FEFET1vbxGiven = TRUE;
            break;
        case FEFET1_MOD_VBM:
            mod->FEFET1vbm = value->rValue;
            mod->FEFET1vbmGiven = TRUE;
            break;
        case FEFET1_MOD_XT:
            mod->FEFET1xt = value->rValue;
            mod->FEFET1xtGiven = TRUE;
            break;
        case  FEFET1_MOD_K1:
            mod->FEFET1k1 = value->rValue;
            mod->FEFET1k1Given = TRUE;
            break;
        case  FEFET1_MOD_KT1:
            mod->FEFET1kt1 = value->rValue;
            mod->FEFET1kt1Given = TRUE;
            break;
        case  FEFET1_MOD_KT1L:
            mod->FEFET1kt1l = value->rValue;
            mod->FEFET1kt1lGiven = TRUE;
            break;
        case  FEFET1_MOD_KT2:
            mod->FEFET1kt2 = value->rValue;
            mod->FEFET1kt2Given = TRUE;
            break;
        case  FEFET1_MOD_K2:
            mod->FEFET1k2 = value->rValue;
            mod->FEFET1k2Given = TRUE;
            break;
        case  FEFET1_MOD_K3:
            mod->FEFET1k3 = value->rValue;
            mod->FEFET1k3Given = TRUE;
            break;
        case  FEFET1_MOD_K3B:
            mod->FEFET1k3b = value->rValue;
            mod->FEFET1k3bGiven = TRUE;
            break;
        case  FEFET1_MOD_LPE0:
            mod->FEFET1lpe0 = value->rValue;
            mod->FEFET1lpe0Given = TRUE;
            break;
        case  FEFET1_MOD_LPEB:
            mod->FEFET1lpeb = value->rValue;
            mod->FEFET1lpebGiven = TRUE;
            break;
        case  FEFET1_MOD_DVTP0:
            mod->FEFET1dvtp0 = value->rValue;
            mod->FEFET1dvtp0Given = TRUE;
            break;
        case  FEFET1_MOD_DVTP1:
            mod->FEFET1dvtp1 = value->rValue;
            mod->FEFET1dvtp1Given = TRUE;
            break;
        case  FEFET1_MOD_W0:
            mod->FEFET1w0 = value->rValue;
            mod->FEFET1w0Given = TRUE;
            break;
        case  FEFET1_MOD_DVT0:               
            mod->FEFET1dvt0 = value->rValue;
            mod->FEFET1dvt0Given = TRUE;
            break;
        case  FEFET1_MOD_DVT1:             
            mod->FEFET1dvt1 = value->rValue;
            mod->FEFET1dvt1Given = TRUE;
            break;
        case  FEFET1_MOD_DVT2:             
            mod->FEFET1dvt2 = value->rValue;
            mod->FEFET1dvt2Given = TRUE;
            break;
        case  FEFET1_MOD_DVT0W:               
            mod->FEFET1dvt0w = value->rValue;
            mod->FEFET1dvt0wGiven = TRUE;
            break;
        case  FEFET1_MOD_DVT1W:             
            mod->FEFET1dvt1w = value->rValue;
            mod->FEFET1dvt1wGiven = TRUE;
            break;
        case  FEFET1_MOD_DVT2W:             
            mod->FEFET1dvt2w = value->rValue;
            mod->FEFET1dvt2wGiven = TRUE;
            break;
        case  FEFET1_MOD_DROUT:             
            mod->FEFET1drout = value->rValue;
            mod->FEFET1droutGiven = TRUE;
            break;
        case  FEFET1_MOD_DSUB:             
            mod->FEFET1dsub = value->rValue;
            mod->FEFET1dsubGiven = TRUE;
            break;
        case FEFET1_MOD_VTH0:
            mod->FEFET1vth0 = value->rValue;
            mod->FEFET1vth0Given = TRUE;
            break;
        case FEFET1_MOD_EU:
            mod->FEFET1eu = value->rValue;
            mod->FEFET1euGiven = TRUE;
            break;
        case FEFET1_MOD_UA:
            mod->FEFET1ua = value->rValue;
            mod->FEFET1uaGiven = TRUE;
            break;
        case FEFET1_MOD_UA1:
            mod->FEFET1ua1 = value->rValue;
            mod->FEFET1ua1Given = TRUE;
            break;
        case FEFET1_MOD_UB:
            mod->FEFET1ub = value->rValue;
            mod->FEFET1ubGiven = TRUE;
            break;
        case FEFET1_MOD_UB1:
            mod->FEFET1ub1 = value->rValue;
            mod->FEFET1ub1Given = TRUE;
            break;
        case FEFET1_MOD_UC:
            mod->FEFET1uc = value->rValue;
            mod->FEFET1ucGiven = TRUE;
            break;
        case FEFET1_MOD_UC1:
            mod->FEFET1uc1 = value->rValue;
            mod->FEFET1uc1Given = TRUE;
            break;
        case  FEFET1_MOD_U0 :
            mod->FEFET1u0 = value->rValue;
            mod->FEFET1u0Given = TRUE;
            break;
        case  FEFET1_MOD_UTE :
            mod->FEFET1ute = value->rValue;
            mod->FEFET1uteGiven = TRUE;
            break;

        case FEFET1_MOD_UD:
            mod->FEFET1ud = value->rValue;
            mod->FEFET1udGiven = TRUE;
            break;
        case FEFET1_MOD_UD1:
            mod->FEFET1ud1 = value->rValue;
            mod->FEFET1ud1Given = TRUE;
            break;
        case FEFET1_MOD_UP:
            mod->FEFET1up = value->rValue;
            mod->FEFET1upGiven = TRUE;
            break;
        case FEFET1_MOD_LP:
            mod->FEFET1lp = value->rValue;
            mod->FEFET1lpGiven = TRUE;
            break;
        case FEFET1_MOD_LUD:
            mod->FEFET1lud = value->rValue;
            mod->FEFET1ludGiven = TRUE;
            break;
        case FEFET1_MOD_LUD1:
            mod->FEFET1lud1 = value->rValue;
            mod->FEFET1lud1Given = TRUE;
            break;
        case FEFET1_MOD_LUP:
            mod->FEFET1lup = value->rValue;
            mod->FEFET1lupGiven = TRUE;
            break;
        case FEFET1_MOD_LLP:
            mod->FEFET1llp = value->rValue;
            mod->FEFET1llpGiven = TRUE;
            break;
        case FEFET1_MOD_WUD:
            mod->FEFET1wud = value->rValue;
            mod->FEFET1wudGiven = TRUE;
            break;
        case FEFET1_MOD_WUD1:
            mod->FEFET1wud1 = value->rValue;
            mod->FEFET1wud1Given = TRUE;
            break;
        case FEFET1_MOD_WUP:
            mod->FEFET1wup = value->rValue;
            mod->FEFET1wupGiven = TRUE;
            break;
        case FEFET1_MOD_WLP:
            mod->FEFET1wlp = value->rValue;
            mod->FEFET1wlpGiven = TRUE;
            break;
        case FEFET1_MOD_PUD:
            mod->FEFET1pud = value->rValue;
            mod->FEFET1pudGiven = TRUE;
            break;
        case FEFET1_MOD_PUD1:
            mod->FEFET1pud1 = value->rValue;
            mod->FEFET1pud1Given = TRUE;
            break;
        case FEFET1_MOD_PUP:
            mod->FEFET1pup = value->rValue;
            mod->FEFET1pupGiven = TRUE;
            break;
        case FEFET1_MOD_PLP:
            mod->FEFET1plp = value->rValue;
            mod->FEFET1plpGiven = TRUE;
            break;


        case FEFET1_MOD_VOFF:
            mod->FEFET1voff = value->rValue;
            mod->FEFET1voffGiven = TRUE;
            break;
        case FEFET1_MOD_TVOFF:
            mod->FEFET1tvoff = value->rValue;
            mod->FEFET1tvoffGiven = TRUE;
            break;
        case FEFET1_MOD_VOFFL:
            mod->FEFET1voffl = value->rValue;
            mod->FEFET1vofflGiven = TRUE;
            break;
        case FEFET1_MOD_MINV:
            mod->FEFET1minv = value->rValue;
            mod->FEFET1minvGiven = TRUE;
            break;
        case FEFET1_MOD_FPROUT:
            mod->FEFET1fprout = value->rValue;
            mod->FEFET1fproutGiven = TRUE;
            break;
        case FEFET1_MOD_PDITS:
            mod->FEFET1pdits = value->rValue;
            mod->FEFET1pditsGiven = TRUE;
            break;
        case FEFET1_MOD_PDITSD:
            mod->FEFET1pditsd = value->rValue;
            mod->FEFET1pditsdGiven = TRUE;
            break;
        case FEFET1_MOD_PDITSL:
            mod->FEFET1pditsl = value->rValue;
            mod->FEFET1pditslGiven = TRUE;
            break;
        case  FEFET1_MOD_DELTA :
            mod->FEFET1delta = value->rValue;
            mod->FEFET1deltaGiven = TRUE;
            break;
        case FEFET1_MOD_RDSW:
            mod->FEFET1rdsw = value->rValue;
            mod->FEFET1rdswGiven = TRUE;
            break;                     
        case FEFET1_MOD_RDSWMIN:
            mod->FEFET1rdswmin = value->rValue;
            mod->FEFET1rdswminGiven = TRUE;
            break;
        case FEFET1_MOD_RDWMIN:
            mod->FEFET1rdwmin = value->rValue;
            mod->FEFET1rdwminGiven = TRUE;
            break;
        case FEFET1_MOD_RSWMIN:
            mod->FEFET1rswmin = value->rValue;
            mod->FEFET1rswminGiven = TRUE;
            break;
        case FEFET1_MOD_RDW:
            mod->FEFET1rdw = value->rValue;
            mod->FEFET1rdwGiven = TRUE;
            break;
        case FEFET1_MOD_RSW:
            mod->FEFET1rsw = value->rValue;
            mod->FEFET1rswGiven = TRUE;
            break;
        case FEFET1_MOD_PRWG:
            mod->FEFET1prwg = value->rValue;
            mod->FEFET1prwgGiven = TRUE;
            break;                     
        case FEFET1_MOD_PRWB:
            mod->FEFET1prwb = value->rValue;
            mod->FEFET1prwbGiven = TRUE;
            break;                     
        case FEFET1_MOD_PRT:
            mod->FEFET1prt = value->rValue;
            mod->FEFET1prtGiven = TRUE;
            break;                     
        case FEFET1_MOD_ETA0:
            mod->FEFET1eta0 = value->rValue;
            mod->FEFET1eta0Given = TRUE;
            break;                 
        case FEFET1_MOD_ETAB:
            mod->FEFET1etab = value->rValue;
            mod->FEFET1etabGiven = TRUE;
            break;                 
        case FEFET1_MOD_PCLM:
            mod->FEFET1pclm = value->rValue;
            mod->FEFET1pclmGiven = TRUE;
            break;                 
        case FEFET1_MOD_PDIBL1:
            mod->FEFET1pdibl1 = value->rValue;
            mod->FEFET1pdibl1Given = TRUE;
            break;                 
        case FEFET1_MOD_PDIBL2:
            mod->FEFET1pdibl2 = value->rValue;
            mod->FEFET1pdibl2Given = TRUE;
            break;                 
        case FEFET1_MOD_PDIBLB:
            mod->FEFET1pdiblb = value->rValue;
            mod->FEFET1pdiblbGiven = TRUE;
            break;                 
        case FEFET1_MOD_PSCBE1:
            mod->FEFET1pscbe1 = value->rValue;
            mod->FEFET1pscbe1Given = TRUE;
            break;                 
        case FEFET1_MOD_PSCBE2:
            mod->FEFET1pscbe2 = value->rValue;
            mod->FEFET1pscbe2Given = TRUE;
            break;                 
        case FEFET1_MOD_PVAG:
            mod->FEFET1pvag = value->rValue;
            mod->FEFET1pvagGiven = TRUE;
            break;                 
        case  FEFET1_MOD_WR :
            mod->FEFET1wr = value->rValue;
            mod->FEFET1wrGiven = TRUE;
            break;
        case  FEFET1_MOD_DWG :
            mod->FEFET1dwg = value->rValue;
            mod->FEFET1dwgGiven = TRUE;
            break;
        case  FEFET1_MOD_DWB :
            mod->FEFET1dwb = value->rValue;
            mod->FEFET1dwbGiven = TRUE;
            break;
        case  FEFET1_MOD_B0 :
            mod->FEFET1b0 = value->rValue;
            mod->FEFET1b0Given = TRUE;
            break;
        case  FEFET1_MOD_B1 :
            mod->FEFET1b1 = value->rValue;
            mod->FEFET1b1Given = TRUE;
            break;
        case  FEFET1_MOD_ALPHA0 :
            mod->FEFET1alpha0 = value->rValue;
            mod->FEFET1alpha0Given = TRUE;
            break;
        case  FEFET1_MOD_ALPHA1 :
            mod->FEFET1alpha1 = value->rValue;
            mod->FEFET1alpha1Given = TRUE;
            break;
        case  FEFET1_MOD_AGIDL :
            mod->FEFET1agidl = value->rValue;
            mod->FEFET1agidlGiven = TRUE;
            break;
        case  FEFET1_MOD_BGIDL :
            mod->FEFET1bgidl = value->rValue;
            mod->FEFET1bgidlGiven = TRUE;
            break;
        case  FEFET1_MOD_CGIDL :
            mod->FEFET1cgidl = value->rValue;
            mod->FEFET1cgidlGiven = TRUE;
            break;
        case  FEFET1_MOD_PHIN :
            mod->FEFET1phin = value->rValue;
            mod->FEFET1phinGiven = TRUE;
            break;
        case  FEFET1_MOD_EGIDL :
            mod->FEFET1egidl = value->rValue;
            mod->FEFET1egidlGiven = TRUE;
            break;
        case  FEFET1_MOD_AIGC :
            mod->FEFET1aigc = value->rValue;
            mod->FEFET1aigcGiven = TRUE;
            break;
        case  FEFET1_MOD_BIGC :
            mod->FEFET1bigc = value->rValue;
            mod->FEFET1bigcGiven = TRUE;
            break;
        case  FEFET1_MOD_CIGC :
            mod->FEFET1cigc = value->rValue;
            mod->FEFET1cigcGiven = TRUE;
            break;
        case  FEFET1_MOD_AIGSD :
            mod->FEFET1aigsd = value->rValue;
            mod->FEFET1aigsdGiven = TRUE;
            break;
        case  FEFET1_MOD_BIGSD :
            mod->FEFET1bigsd = value->rValue;
            mod->FEFET1bigsdGiven = TRUE;
            break;
        case  FEFET1_MOD_CIGSD :
            mod->FEFET1cigsd = value->rValue;
            mod->FEFET1cigsdGiven = TRUE;
            break;
        case  FEFET1_MOD_AIGBACC :
            mod->FEFET1aigbacc = value->rValue;
            mod->FEFET1aigbaccGiven = TRUE;
            break;
        case  FEFET1_MOD_BIGBACC :
            mod->FEFET1bigbacc = value->rValue;
            mod->FEFET1bigbaccGiven = TRUE;
            break;
        case  FEFET1_MOD_CIGBACC :
            mod->FEFET1cigbacc = value->rValue;
            mod->FEFET1cigbaccGiven = TRUE;
            break;
        case  FEFET1_MOD_AIGBINV :
            mod->FEFET1aigbinv = value->rValue;
            mod->FEFET1aigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_BIGBINV :
            mod->FEFET1bigbinv = value->rValue;
            mod->FEFET1bigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_CIGBINV :
            mod->FEFET1cigbinv = value->rValue;
            mod->FEFET1cigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_NIGC :
            mod->FEFET1nigc = value->rValue;
            mod->FEFET1nigcGiven = TRUE;
            break;
        case  FEFET1_MOD_NIGBINV :
            mod->FEFET1nigbinv = value->rValue;
            mod->FEFET1nigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_NIGBACC :
            mod->FEFET1nigbacc = value->rValue;
            mod->FEFET1nigbaccGiven = TRUE;
            break;
        case  FEFET1_MOD_NTOX :
            mod->FEFET1ntox = value->rValue;
            mod->FEFET1ntoxGiven = TRUE;
            break;
        case  FEFET1_MOD_EIGBINV :
            mod->FEFET1eigbinv = value->rValue;
            mod->FEFET1eigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_PIGCD :
            mod->FEFET1pigcd = value->rValue;
            mod->FEFET1pigcdGiven = TRUE;
            break;
        case  FEFET1_MOD_POXEDGE :
            mod->FEFET1poxedge = value->rValue;
            mod->FEFET1poxedgeGiven = TRUE;
            break;
        case  FEFET1_MOD_XRCRG1 :
            mod->FEFET1xrcrg1 = value->rValue;
            mod->FEFET1xrcrg1Given = TRUE;
            break;
        case  FEFET1_MOD_XRCRG2 :
            mod->FEFET1xrcrg2 = value->rValue;
            mod->FEFET1xrcrg2Given = TRUE;
            break;
        case  FEFET1_MOD_LAMBDA :
            mod->FEFET1lambda = value->rValue;
            mod->FEFET1lambdaGiven = TRUE;
            break;
        case  FEFET1_MOD_VTL :
            mod->FEFET1vtl = value->rValue;
            mod->FEFET1vtlGiven = TRUE;
            break;
        case  FEFET1_MOD_XN:
            mod->FEFET1xn = value->rValue;
            mod->FEFET1xnGiven = TRUE;
            break;
        case  FEFET1_MOD_LC:
            mod->FEFET1lc = value->rValue;
            mod->FEFET1lcGiven = TRUE;
            break;
        case  FEFET1_MOD_TNOIA :
            mod->FEFET1tnoia = value->rValue;
            mod->FEFET1tnoiaGiven = TRUE;
            break;
        case  FEFET1_MOD_TNOIB :
            mod->FEFET1tnoib = value->rValue;
            mod->FEFET1tnoibGiven = TRUE;
            break;
        case  FEFET1_MOD_RNOIA :
            mod->FEFET1rnoia = value->rValue;
            mod->FEFET1rnoiaGiven = TRUE;
            break;
        case  FEFET1_MOD_RNOIB :
            mod->FEFET1rnoib = value->rValue;
            mod->FEFET1rnoibGiven = TRUE;
            break;
        case  FEFET1_MOD_NTNOI :
            mod->FEFET1ntnoi = value->rValue;
            mod->FEFET1ntnoiGiven = TRUE;
            break;
        case  FEFET1_MOD_VFBSDOFF:
            mod->FEFET1vfbsdoff = value->rValue;
            mod->FEFET1vfbsdoffGiven = TRUE;
            break;
        case  FEFET1_MOD_TVFBSDOFF:
            mod->FEFET1tvfbsdoff = value->rValue;
            mod->FEFET1tvfbsdoffGiven = TRUE;
            break;
        case  FEFET1_MOD_LINTNOI:
            mod->FEFET1lintnoi = value->rValue;
            mod->FEFET1lintnoiGiven = TRUE;
            break;

        /* stress effect */
        case  FEFET1_MOD_SAREF :
            mod->FEFET1saref = value->rValue;
            mod->FEFET1sarefGiven = TRUE;
            break;
        case  FEFET1_MOD_SBREF :
            mod->FEFET1sbref = value->rValue;
            mod->FEFET1sbrefGiven = TRUE;
            break;
        case  FEFET1_MOD_WLOD :
            mod->FEFET1wlod = value->rValue;
            mod->FEFET1wlodGiven = TRUE;
            break;
        case  FEFET1_MOD_KU0 :
            mod->FEFET1ku0 = value->rValue;
            mod->FEFET1ku0Given = TRUE;
            break;
        case  FEFET1_MOD_KVSAT :
            mod->FEFET1kvsat = value->rValue;
            mod->FEFET1kvsatGiven = TRUE;
            break;
        case  FEFET1_MOD_KVTH0 :
            mod->FEFET1kvth0 = value->rValue;
            mod->FEFET1kvth0Given = TRUE;
            break;
        case  FEFET1_MOD_TKU0 :
            mod->FEFET1tku0 = value->rValue;
            mod->FEFET1tku0Given = TRUE;
            break;
        case  FEFET1_MOD_LLODKU0 :
            mod->FEFET1llodku0 = value->rValue;
            mod->FEFET1llodku0Given = TRUE;
            break;
        case  FEFET1_MOD_WLODKU0 :
            mod->FEFET1wlodku0 = value->rValue;
            mod->FEFET1wlodku0Given = TRUE;
            break;
        case  FEFET1_MOD_LLODVTH :
            mod->FEFET1llodvth = value->rValue;
            mod->FEFET1llodvthGiven = TRUE;
            break;
        case  FEFET1_MOD_WLODVTH :
            mod->FEFET1wlodvth = value->rValue;
            mod->FEFET1wlodvthGiven = TRUE;
            break;
        case  FEFET1_MOD_LKU0 :
            mod->FEFET1lku0 = value->rValue;
            mod->FEFET1lku0Given = TRUE;
            break;
        case  FEFET1_MOD_WKU0 :
            mod->FEFET1wku0 = value->rValue;
            mod->FEFET1wku0Given = TRUE;
            break;
        case  FEFET1_MOD_PKU0 :
            mod->FEFET1pku0 = value->rValue;
            mod->FEFET1pku0Given = TRUE;
            break;
        case  FEFET1_MOD_LKVTH0 :
            mod->FEFET1lkvth0 = value->rValue;
            mod->FEFET1lkvth0Given = TRUE;
            break;
        case  FEFET1_MOD_WKVTH0 :
            mod->FEFET1wkvth0 = value->rValue;
            mod->FEFET1wkvth0Given = TRUE;
            break;
        case  FEFET1_MOD_PKVTH0 :
            mod->FEFET1pkvth0 = value->rValue;
            mod->FEFET1pkvth0Given = TRUE;
            break;
        case  FEFET1_MOD_STK2 :
            mod->FEFET1stk2 = value->rValue;
            mod->FEFET1stk2Given = TRUE;
            break;
        case  FEFET1_MOD_LODK2 :
            mod->FEFET1lodk2 = value->rValue;
            mod->FEFET1lodk2Given = TRUE;
            break;
        case  FEFET1_MOD_STETA0 :
            mod->FEFET1steta0 = value->rValue;
            mod->FEFET1steta0Given = TRUE;
            break;
        case  FEFET1_MOD_LODETA0 :
            mod->FEFET1lodeta0 = value->rValue;
            mod->FEFET1lodeta0Given = TRUE;
            break;

        case  FEFET1_MOD_WEB :
            mod->FEFET1web = value->rValue;
            mod->FEFET1webGiven = TRUE;
            break;
	case FEFET1_MOD_WEC :
            mod->FEFET1wec = value->rValue;
            mod->FEFET1wecGiven = TRUE;
            break;
        case  FEFET1_MOD_KVTH0WE :
            mod->FEFET1kvth0we = value->rValue;
            mod->FEFET1kvth0weGiven = TRUE;
            break;
        case  FEFET1_MOD_K2WE :
            mod->FEFET1k2we = value->rValue;
            mod->FEFET1k2weGiven = TRUE;
            break;
        case  FEFET1_MOD_KU0WE :
            mod->FEFET1ku0we = value->rValue;
            mod->FEFET1ku0weGiven = TRUE;
            break;
        case  FEFET1_MOD_SCREF :
            mod->FEFET1scref = value->rValue;
            mod->FEFET1screfGiven = TRUE;
            break;
        case  FEFET1_MOD_WPEMOD :
            mod->FEFET1wpemod = value->rValue;
            mod->FEFET1wpemodGiven = TRUE;
            break;
        case  FEFET1_MOD_LKVTH0WE :
            mod->FEFET1lkvth0we = value->rValue;
            mod->FEFET1lkvth0weGiven = TRUE;
            break;
        case  FEFET1_MOD_LK2WE :
            mod->FEFET1lk2we = value->rValue;
            mod->FEFET1lk2weGiven = TRUE;
            break;
        case  FEFET1_MOD_LKU0WE :
            mod->FEFET1lku0we = value->rValue;
            mod->FEFET1lku0weGiven = TRUE;
            break;
        case  FEFET1_MOD_WKVTH0WE :
            mod->FEFET1wkvth0we = value->rValue;
            mod->FEFET1wkvth0weGiven = TRUE;
            break;
        case  FEFET1_MOD_WK2WE :
            mod->FEFET1wk2we = value->rValue;
            mod->FEFET1wk2weGiven = TRUE;
            break;
        case  FEFET1_MOD_WKU0WE :
            mod->FEFET1wku0we = value->rValue;
            mod->FEFET1wku0weGiven = TRUE;
            break;
        case  FEFET1_MOD_PKVTH0WE :
            mod->FEFET1pkvth0we = value->rValue;
            mod->FEFET1pkvth0weGiven = TRUE;
            break;
        case  FEFET1_MOD_PK2WE :
            mod->FEFET1pk2we = value->rValue;
            mod->FEFET1pk2weGiven = TRUE;
            break;
        case  FEFET1_MOD_PKU0WE :
            mod->FEFET1pku0we = value->rValue;
            mod->FEFET1pku0weGiven = TRUE;
            break;

        case  FEFET1_MOD_BETA0 :
            mod->FEFET1beta0 = value->rValue;
            mod->FEFET1beta0Given = TRUE;
            break;
        case  FEFET1_MOD_IJTHDFWD :
            mod->FEFET1ijthdfwd = value->rValue;
            mod->FEFET1ijthdfwdGiven = TRUE;
            break;
        case  FEFET1_MOD_IJTHSFWD :
            mod->FEFET1ijthsfwd = value->rValue;
            mod->FEFET1ijthsfwdGiven = TRUE;
            break;
        case  FEFET1_MOD_IJTHDREV :
            mod->FEFET1ijthdrev = value->rValue;
            mod->FEFET1ijthdrevGiven = TRUE;
            break;
        case  FEFET1_MOD_IJTHSREV :
            mod->FEFET1ijthsrev = value->rValue;
            mod->FEFET1ijthsrevGiven = TRUE;
            break;
        case  FEFET1_MOD_XJBVD :
            mod->FEFET1xjbvd = value->rValue;
            mod->FEFET1xjbvdGiven = TRUE;
            break;
        case  FEFET1_MOD_XJBVS :
            mod->FEFET1xjbvs = value->rValue;
            mod->FEFET1xjbvsGiven = TRUE;
            break;
        case  FEFET1_MOD_BVD :
            mod->FEFET1bvd = value->rValue;
            mod->FEFET1bvdGiven = TRUE;
            break;
        case  FEFET1_MOD_BVS :
            mod->FEFET1bvs = value->rValue;
            mod->FEFET1bvsGiven = TRUE;
            break;
        
        /* reverse diode */
        case  FEFET1_MOD_JTSS :
            mod->FEFET1jtss = value->rValue;
            mod->FEFET1jtssGiven = TRUE;
            break;
        case  FEFET1_MOD_JTSD :
            mod->FEFET1jtsd = value->rValue;
            mod->FEFET1jtsdGiven = TRUE;
            break;
        case  FEFET1_MOD_JTSSWS :
            mod->FEFET1jtssws = value->rValue;
            mod->FEFET1jtsswsGiven = TRUE;
            break;
        case  FEFET1_MOD_JTSSWD :
            mod->FEFET1jtsswd = value->rValue;
            mod->FEFET1jtsswdGiven = TRUE;
            break;
        case  FEFET1_MOD_JTSSWGS :
            mod->FEFET1jtsswgs = value->rValue;
            mod->FEFET1jtsswgsGiven = TRUE;
            break;
        case  FEFET1_MOD_JTSSWGD :
            mod->FEFET1jtsswgd = value->rValue;
            mod->FEFET1jtsswgdGiven = TRUE;
            break;
        case  FEFET1_MOD_NJTS :
            mod->FEFET1njts = value->rValue;
            mod->FEFET1njtsGiven = TRUE;
            break;
        case  FEFET1_MOD_NJTSSW :
            mod->FEFET1njtssw = value->rValue;
            mod->FEFET1njtsswGiven = TRUE;
            break;
        case  FEFET1_MOD_NJTSSWG :
            mod->FEFET1njtsswg = value->rValue;
            mod->FEFET1njtsswgGiven = TRUE;
            break;
        case  FEFET1_MOD_XTSS :
            mod->FEFET1xtss = value->rValue;
            mod->FEFET1xtssGiven = TRUE;
            break;
        case  FEFET1_MOD_XTSD :
            mod->FEFET1xtsd = value->rValue;
            mod->FEFET1xtsdGiven = TRUE;
            break;
        case  FEFET1_MOD_XTSSWS :
            mod->FEFET1xtssws = value->rValue;
            mod->FEFET1xtsswsGiven = TRUE;
            break;
        case  FEFET1_MOD_XTSSWD :
            mod->FEFET1xtsswd = value->rValue;
            mod->FEFET1xtsswdGiven = TRUE;
            break;
        case  FEFET1_MOD_XTSSWGS :
            mod->FEFET1xtsswgs = value->rValue;
            mod->FEFET1xtsswgsGiven = TRUE;
            break;
        case  FEFET1_MOD_XTSSWGD :
            mod->FEFET1xtsswgd = value->rValue;
            mod->FEFET1xtsswgdGiven = TRUE;
            break;
        case  FEFET1_MOD_TNJTS :
            mod->FEFET1tnjts = value->rValue;
            mod->FEFET1tnjtsGiven = TRUE;
            break;
        case  FEFET1_MOD_TNJTSSW :
            mod->FEFET1tnjtssw = value->rValue;
            mod->FEFET1tnjtsswGiven = TRUE;
            break;
        case  FEFET1_MOD_TNJTSSWG :
            mod->FEFET1tnjtsswg = value->rValue;
            mod->FEFET1tnjtsswgGiven = TRUE;
            break;
        case  FEFET1_MOD_VTSS :
            mod->FEFET1vtss = value->rValue;
            mod->FEFET1vtssGiven = TRUE;
            break;
        case  FEFET1_MOD_VTSD :
            mod->FEFET1vtsd = value->rValue;
            mod->FEFET1vtsdGiven = TRUE;
            break;
        case  FEFET1_MOD_VTSSWS :
            mod->FEFET1vtssws = value->rValue;
            mod->FEFET1vtsswsGiven = TRUE;
            break;
        case  FEFET1_MOD_VTSSWD :
            mod->FEFET1vtsswd = value->rValue;
            mod->FEFET1vtsswdGiven = TRUE;
            break;
        case  FEFET1_MOD_VTSSWGS :
            mod->FEFET1vtsswgs = value->rValue;
            mod->FEFET1vtsswgsGiven = TRUE;
            break;
        case  FEFET1_MOD_VTSSWGD :
            mod->FEFET1vtsswgd = value->rValue;
            mod->FEFET1vtsswgdGiven = TRUE;
            break;

        case  FEFET1_MOD_VFB :
            mod->FEFET1vfb = value->rValue;
            mod->FEFET1vfbGiven = TRUE;
            break;

        case  FEFET1_MOD_GBMIN :
            mod->FEFET1gbmin = value->rValue;
            mod->FEFET1gbminGiven = TRUE;
            break;
        case  FEFET1_MOD_RBDB :
            mod->FEFET1rbdb = value->rValue;
            mod->FEFET1rbdbGiven = TRUE;
            break;
        case  FEFET1_MOD_RBPB :
            mod->FEFET1rbpb = value->rValue;
            mod->FEFET1rbpbGiven = TRUE;
            break;
        case  FEFET1_MOD_RBSB :
            mod->FEFET1rbsb = value->rValue;
            mod->FEFET1rbsbGiven = TRUE;
            break;
        case  FEFET1_MOD_RBPS :
            mod->FEFET1rbps = value->rValue;
            mod->FEFET1rbpsGiven = TRUE;
            break;
        case  FEFET1_MOD_RBPD :
            mod->FEFET1rbpd = value->rValue;
            mod->FEFET1rbpdGiven = TRUE;
            break;

        case  FEFET1_MOD_RBPS0 :
            mod->FEFET1rbps0 = value->rValue;
            mod->FEFET1rbps0Given = TRUE;
            break;
        case  FEFET1_MOD_RBPSL :
            mod->FEFET1rbpsl = value->rValue;
            mod->FEFET1rbpslGiven = TRUE;
            break;
        case  FEFET1_MOD_RBPSW :
            mod->FEFET1rbpsw = value->rValue;
            mod->FEFET1rbpswGiven = TRUE;
            break;
        case  FEFET1_MOD_RBPSNF :
            mod->FEFET1rbpsnf = value->rValue;
            mod->FEFET1rbpsnfGiven = TRUE;
            break;

        case  FEFET1_MOD_RBPD0 :
            mod->FEFET1rbpd0 = value->rValue;
            mod->FEFET1rbpd0Given = TRUE;
            break;
        case  FEFET1_MOD_RBPDL :
            mod->FEFET1rbpdl = value->rValue;
            mod->FEFET1rbpdlGiven = TRUE;
            break;
        case  FEFET1_MOD_RBPDW :
            mod->FEFET1rbpdw = value->rValue;
            mod->FEFET1rbpdwGiven = TRUE;
            break;
        case  FEFET1_MOD_RBPDNF :
            mod->FEFET1rbpdnf = value->rValue;
            mod->FEFET1rbpdnfGiven = TRUE;
            break;

        case  FEFET1_MOD_RBPBX0 :
            mod->FEFET1rbpbx0 = value->rValue;
            mod->FEFET1rbpbx0Given = TRUE;
            break;
        case  FEFET1_MOD_RBPBXL :
            mod->FEFET1rbpbxl = value->rValue;
            mod->FEFET1rbpbxlGiven = TRUE;
            break;
        case  FEFET1_MOD_RBPBXW :
            mod->FEFET1rbpbxw = value->rValue;
            mod->FEFET1rbpbxwGiven = TRUE;
            break;
        case  FEFET1_MOD_RBPBXNF :
            mod->FEFET1rbpbxnf = value->rValue;
            mod->FEFET1rbpbxnfGiven = TRUE;
            break;
        case  FEFET1_MOD_RBPBY0 :
            mod->FEFET1rbpby0 = value->rValue;
            mod->FEFET1rbpby0Given = TRUE;
            break;
        case  FEFET1_MOD_RBPBYL :
            mod->FEFET1rbpbyl = value->rValue;
            mod->FEFET1rbpbylGiven = TRUE;
            break;
        case  FEFET1_MOD_RBPBYW :
            mod->FEFET1rbpbyw = value->rValue;
            mod->FEFET1rbpbywGiven = TRUE;
            break;
        case  FEFET1_MOD_RBPBYNF :
            mod->FEFET1rbpbynf = value->rValue;
            mod->FEFET1rbpbynfGiven = TRUE;
            break;
       case  FEFET1_MOD_RBSBX0 :
            mod->FEFET1rbsbx0 = value->rValue;
            mod->FEFET1rbsbx0Given = TRUE;
            break;
       case  FEFET1_MOD_RBSBY0 :
            mod->FEFET1rbsby0 = value->rValue;
            mod->FEFET1rbsby0Given = TRUE;
            break;
       case  FEFET1_MOD_RBDBX0 :
            mod->FEFET1rbdbx0 = value->rValue;
            mod->FEFET1rbdbx0Given = TRUE;
            break;
       case  FEFET1_MOD_RBDBY0 :
            mod->FEFET1rbdby0 = value->rValue;
            mod->FEFET1rbdby0Given = TRUE;
            break;


       case  FEFET1_MOD_RBSDBXL :
            mod->FEFET1rbsdbxl = value->rValue;
            mod->FEFET1rbsdbxlGiven = TRUE;
            break;
       case  FEFET1_MOD_RBSDBXW :
            mod->FEFET1rbsdbxw = value->rValue;
            mod->FEFET1rbsdbxwGiven = TRUE;
            break;
       case  FEFET1_MOD_RBSDBXNF :
            mod->FEFET1rbsdbxnf = value->rValue;
            mod->FEFET1rbsdbxnfGiven = TRUE;
            break;
       case  FEFET1_MOD_RBSDBYL :
            mod->FEFET1rbsdbyl = value->rValue;
            mod->FEFET1rbsdbylGiven = TRUE;
            break;
       case  FEFET1_MOD_RBSDBYW :
            mod->FEFET1rbsdbyw = value->rValue;
            mod->FEFET1rbsdbywGiven = TRUE;
            break;
       case  FEFET1_MOD_RBSDBYNF :
            mod->FEFET1rbsdbynf = value->rValue;
            mod->FEFET1rbsdbynfGiven = TRUE;
            break;
 
        case  FEFET1_MOD_CGSL :
            mod->FEFET1cgsl = value->rValue;
            mod->FEFET1cgslGiven = TRUE;
            break;
        case  FEFET1_MOD_CGDL :
            mod->FEFET1cgdl = value->rValue;
            mod->FEFET1cgdlGiven = TRUE;
            break;
        case  FEFET1_MOD_CKAPPAS :
            mod->FEFET1ckappas = value->rValue;
            mod->FEFET1ckappasGiven = TRUE;
            break;
        case  FEFET1_MOD_CKAPPAD :
            mod->FEFET1ckappad = value->rValue;
            mod->FEFET1ckappadGiven = TRUE;
            break;
        case  FEFET1_MOD_CF :
            mod->FEFET1cf = value->rValue;
            mod->FEFET1cfGiven = TRUE;
            break;
        case  FEFET1_MOD_CLC :
            mod->FEFET1clc = value->rValue;
            mod->FEFET1clcGiven = TRUE;
            break;
        case  FEFET1_MOD_CLE :
            mod->FEFET1cle = value->rValue;
            mod->FEFET1cleGiven = TRUE;
            break;
        case  FEFET1_MOD_DWC :
            mod->FEFET1dwc = value->rValue;
            mod->FEFET1dwcGiven = TRUE;
            break;
        case  FEFET1_MOD_DLC :
            mod->FEFET1dlc = value->rValue;
            mod->FEFET1dlcGiven = TRUE;
            break;
        case  FEFET1_MOD_XW :
            mod->FEFET1xw = value->rValue;
            mod->FEFET1xwGiven = TRUE;
            break;
        case  FEFET1_MOD_XL :
            mod->FEFET1xl = value->rValue;
            mod->FEFET1xlGiven = TRUE;
            break;
        case  FEFET1_MOD_DLCIG :
            mod->FEFET1dlcig = value->rValue;
            mod->FEFET1dlcigGiven = TRUE;
            break;
        case  FEFET1_MOD_DWJ :
            mod->FEFET1dwj = value->rValue;
            mod->FEFET1dwjGiven = TRUE;
            break;
        case  FEFET1_MOD_VFBCV :
            mod->FEFET1vfbcv = value->rValue;
            mod->FEFET1vfbcvGiven = TRUE;
            break;
        case  FEFET1_MOD_ACDE :
            mod->FEFET1acde = value->rValue;
            mod->FEFET1acdeGiven = TRUE;
            break;
        case  FEFET1_MOD_MOIN :
            mod->FEFET1moin = value->rValue;
            mod->FEFET1moinGiven = TRUE;
            break;
        case  FEFET1_MOD_NOFF :
            mod->FEFET1noff = value->rValue;
            mod->FEFET1noffGiven = TRUE;
            break;
        case  FEFET1_MOD_VOFFCV :
            mod->FEFET1voffcv = value->rValue;
            mod->FEFET1voffcvGiven = TRUE;
            break;
        case  FEFET1_MOD_DMCG :
            mod->FEFET1dmcg = value->rValue;
            mod->FEFET1dmcgGiven = TRUE;
            break;
        case  FEFET1_MOD_DMCI :
            mod->FEFET1dmci = value->rValue;
            mod->FEFET1dmciGiven = TRUE;
            break;
        case  FEFET1_MOD_DMDG :
            mod->FEFET1dmdg = value->rValue;
            mod->FEFET1dmdgGiven = TRUE;
            break;
        case  FEFET1_MOD_DMCGT :
            mod->FEFET1dmcgt = value->rValue;
            mod->FEFET1dmcgtGiven = TRUE;
            break;
        case  FEFET1_MOD_XGW :
            mod->FEFET1xgw = value->rValue;
            mod->FEFET1xgwGiven = TRUE;
            break;
        case  FEFET1_MOD_XGL :
            mod->FEFET1xgl = value->rValue;
            mod->FEFET1xglGiven = TRUE;
            break;
        case  FEFET1_MOD_RSHG :
            mod->FEFET1rshg = value->rValue;
            mod->FEFET1rshgGiven = TRUE;
            break;
        case  FEFET1_MOD_NGCON :
            mod->FEFET1ngcon = value->rValue;
            mod->FEFET1ngconGiven = TRUE;
            break;
        case  FEFET1_MOD_TCJ :
            mod->FEFET1tcj = value->rValue;
            mod->FEFET1tcjGiven = TRUE;
            break;
        case  FEFET1_MOD_TPB :
            mod->FEFET1tpb = value->rValue;
            mod->FEFET1tpbGiven = TRUE;
            break;
        case  FEFET1_MOD_TCJSW :
            mod->FEFET1tcjsw = value->rValue;
            mod->FEFET1tcjswGiven = TRUE;
            break;
        case  FEFET1_MOD_TPBSW :
            mod->FEFET1tpbsw = value->rValue;
            mod->FEFET1tpbswGiven = TRUE;
            break;
        case  FEFET1_MOD_TCJSWG :
            mod->FEFET1tcjswg = value->rValue;
            mod->FEFET1tcjswgGiven = TRUE;
            break;
        case  FEFET1_MOD_TPBSWG :
            mod->FEFET1tpbswg = value->rValue;
            mod->FEFET1tpbswgGiven = TRUE;
            break;

	/* Length dependence */
        case  FEFET1_MOD_LCDSC :
            mod->FEFET1lcdsc = value->rValue;
            mod->FEFET1lcdscGiven = TRUE;
            break;


        case  FEFET1_MOD_LCDSCB :
            mod->FEFET1lcdscb = value->rValue;
            mod->FEFET1lcdscbGiven = TRUE;
            break;
        case  FEFET1_MOD_LCDSCD :
            mod->FEFET1lcdscd = value->rValue;
            mod->FEFET1lcdscdGiven = TRUE;
            break;
        case  FEFET1_MOD_LCIT :
            mod->FEFET1lcit = value->rValue;
            mod->FEFET1lcitGiven = TRUE;
            break;
        case  FEFET1_MOD_LNFACTOR :
            mod->FEFET1lnfactor = value->rValue;
            mod->FEFET1lnfactorGiven = TRUE;
            break;
        case FEFET1_MOD_LXJ:
            mod->FEFET1lxj = value->rValue;
            mod->FEFET1lxjGiven = TRUE;
            break;
        case FEFET1_MOD_LVSAT:
            mod->FEFET1lvsat = value->rValue;
            mod->FEFET1lvsatGiven = TRUE;
            break;
        
        
        case FEFET1_MOD_LA0:
            mod->FEFET1la0 = value->rValue;
            mod->FEFET1la0Given = TRUE;
            break;
        case FEFET1_MOD_LAGS:
            mod->FEFET1lags = value->rValue;
            mod->FEFET1lagsGiven = TRUE;
            break;
        case FEFET1_MOD_LA1:
            mod->FEFET1la1 = value->rValue;
            mod->FEFET1la1Given = TRUE;
            break;
        case FEFET1_MOD_LA2:
            mod->FEFET1la2 = value->rValue;
            mod->FEFET1la2Given = TRUE;
            break;
        case FEFET1_MOD_LAT:
            mod->FEFET1lat = value->rValue;
            mod->FEFET1latGiven = TRUE;
            break;
        case FEFET1_MOD_LKETA:
            mod->FEFET1lketa = value->rValue;
            mod->FEFET1lketaGiven = TRUE;
            break;    
        case FEFET1_MOD_LNSUB:
            mod->FEFET1lnsub = value->rValue;
            mod->FEFET1lnsubGiven = TRUE;
            break;
        case FEFET1_MOD_LNDEP:
            mod->FEFET1lndep = value->rValue;
            mod->FEFET1lndepGiven = TRUE;
	    if (mod->FEFET1lndep > 1.0e20)
		mod->FEFET1lndep *= 1.0e-6;
            break;
        case FEFET1_MOD_LNSD:
            mod->FEFET1lnsd = value->rValue;
            mod->FEFET1lnsdGiven = TRUE;
            if (mod->FEFET1lnsd > 1.0e23)
                mod->FEFET1lnsd *= 1.0e-6;
            break;
        case FEFET1_MOD_LNGATE:
            mod->FEFET1lngate = value->rValue;
            mod->FEFET1lngateGiven = TRUE;
	    if (mod->FEFET1lngate > 1.0e23)
		mod->FEFET1lngate *= 1.0e-6;
            break;
        case FEFET1_MOD_LGAMMA1:
            mod->FEFET1lgamma1 = value->rValue;
            mod->FEFET1lgamma1Given = TRUE;
            break;
        case FEFET1_MOD_LGAMMA2:
            mod->FEFET1lgamma2 = value->rValue;
            mod->FEFET1lgamma2Given = TRUE;
            break;
        case FEFET1_MOD_LVBX:
            mod->FEFET1lvbx = value->rValue;
            mod->FEFET1lvbxGiven = TRUE;
            break;
        case FEFET1_MOD_LVBM:
            mod->FEFET1lvbm = value->rValue;
            mod->FEFET1lvbmGiven = TRUE;
            break;
        case FEFET1_MOD_LXT:
            mod->FEFET1lxt = value->rValue;
            mod->FEFET1lxtGiven = TRUE;
            break;
        case  FEFET1_MOD_LK1:
            mod->FEFET1lk1 = value->rValue;
            mod->FEFET1lk1Given = TRUE;
            break;
        case  FEFET1_MOD_LKT1:
            mod->FEFET1lkt1 = value->rValue;
            mod->FEFET1lkt1Given = TRUE;
            break;
        case  FEFET1_MOD_LKT1L:
            mod->FEFET1lkt1l = value->rValue;
            mod->FEFET1lkt1lGiven = TRUE;
            break;
        case  FEFET1_MOD_LKT2:
            mod->FEFET1lkt2 = value->rValue;
            mod->FEFET1lkt2Given = TRUE;
            break;
        case  FEFET1_MOD_LK2:
            mod->FEFET1lk2 = value->rValue;
            mod->FEFET1lk2Given = TRUE;
            break;
        case  FEFET1_MOD_LK3:
            mod->FEFET1lk3 = value->rValue;
            mod->FEFET1lk3Given = TRUE;
            break;
        case  FEFET1_MOD_LK3B:
            mod->FEFET1lk3b = value->rValue;
            mod->FEFET1lk3bGiven = TRUE;
            break;
        case  FEFET1_MOD_LLPE0:
            mod->FEFET1llpe0 = value->rValue;
            mod->FEFET1llpe0Given = TRUE;
            break;
        case  FEFET1_MOD_LLPEB:
            mod->FEFET1llpeb = value->rValue;
            mod->FEFET1llpebGiven = TRUE;
            break;
        case  FEFET1_MOD_LDVTP0:
            mod->FEFET1ldvtp0 = value->rValue;
            mod->FEFET1ldvtp0Given = TRUE;
            break;
        case  FEFET1_MOD_LDVTP1:
            mod->FEFET1ldvtp1 = value->rValue;
            mod->FEFET1ldvtp1Given = TRUE;
            break;
        case  FEFET1_MOD_LW0:
            mod->FEFET1lw0 = value->rValue;
            mod->FEFET1lw0Given = TRUE;
            break;
        case  FEFET1_MOD_LDVT0:               
            mod->FEFET1ldvt0 = value->rValue;
            mod->FEFET1ldvt0Given = TRUE;
            break;
        case  FEFET1_MOD_LDVT1:             
            mod->FEFET1ldvt1 = value->rValue;
            mod->FEFET1ldvt1Given = TRUE;
            break;
        case  FEFET1_MOD_LDVT2:             
            mod->FEFET1ldvt2 = value->rValue;
            mod->FEFET1ldvt2Given = TRUE;
            break;
        case  FEFET1_MOD_LDVT0W:               
            mod->FEFET1ldvt0w = value->rValue;
            mod->FEFET1ldvt0wGiven = TRUE;
            break;
        case  FEFET1_MOD_LDVT1W:             
            mod->FEFET1ldvt1w = value->rValue;
            mod->FEFET1ldvt1wGiven = TRUE;
            break;
        case  FEFET1_MOD_LDVT2W:             
            mod->FEFET1ldvt2w = value->rValue;
            mod->FEFET1ldvt2wGiven = TRUE;
            break;
        case  FEFET1_MOD_LDROUT:             
            mod->FEFET1ldrout = value->rValue;
            mod->FEFET1ldroutGiven = TRUE;
            break;
        case  FEFET1_MOD_LDSUB:             
            mod->FEFET1ldsub = value->rValue;
            mod->FEFET1ldsubGiven = TRUE;
            break;
        case FEFET1_MOD_LVTH0:
            mod->FEFET1lvth0 = value->rValue;
            mod->FEFET1lvth0Given = TRUE;
            break;
        case FEFET1_MOD_LUA:
            mod->FEFET1lua = value->rValue;
            mod->FEFET1luaGiven = TRUE;
            break;
        case FEFET1_MOD_LUA1:
            mod->FEFET1lua1 = value->rValue;
            mod->FEFET1lua1Given = TRUE;
            break;
        case FEFET1_MOD_LUB:
            mod->FEFET1lub = value->rValue;
            mod->FEFET1lubGiven = TRUE;
            break;
        case FEFET1_MOD_LUB1:
            mod->FEFET1lub1 = value->rValue;
            mod->FEFET1lub1Given = TRUE;
            break;
        case FEFET1_MOD_LUC:
            mod->FEFET1luc = value->rValue;
            mod->FEFET1lucGiven = TRUE;
            break;
        case FEFET1_MOD_LUC1:
            mod->FEFET1luc1 = value->rValue;
            mod->FEFET1luc1Given = TRUE;
            break;
        case  FEFET1_MOD_LU0 :
            mod->FEFET1lu0 = value->rValue;
            mod->FEFET1lu0Given = TRUE;
            break;
        case  FEFET1_MOD_LUTE :
            mod->FEFET1lute = value->rValue;
            mod->FEFET1luteGiven = TRUE;
            break;
        case FEFET1_MOD_LVOFF:
            mod->FEFET1lvoff = value->rValue;
            mod->FEFET1lvoffGiven = TRUE;
            break;
        case FEFET1_MOD_LTVOFF:
            mod->FEFET1ltvoff = value->rValue;
            mod->FEFET1ltvoffGiven = TRUE;
            break;
        case FEFET1_MOD_LMINV:
            mod->FEFET1lminv = value->rValue;
            mod->FEFET1lminvGiven = TRUE;
            break;
        case FEFET1_MOD_LFPROUT:
            mod->FEFET1lfprout = value->rValue;
            mod->FEFET1lfproutGiven = TRUE;
            break;
        case FEFET1_MOD_LPDITS:
            mod->FEFET1lpdits = value->rValue;
            mod->FEFET1lpditsGiven = TRUE;
            break;
        case FEFET1_MOD_LPDITSD:
            mod->FEFET1lpditsd = value->rValue;
            mod->FEFET1lpditsdGiven = TRUE;
            break;
        case  FEFET1_MOD_LDELTA :
            mod->FEFET1ldelta = value->rValue;
            mod->FEFET1ldeltaGiven = TRUE;
            break;
        case FEFET1_MOD_LRDSW:
            mod->FEFET1lrdsw = value->rValue;
            mod->FEFET1lrdswGiven = TRUE;
            break;                     
        case FEFET1_MOD_LRDW:
            mod->FEFET1lrdw = value->rValue;
            mod->FEFET1lrdwGiven = TRUE;
            break;
        case FEFET1_MOD_LRSW:
            mod->FEFET1lrsw = value->rValue;
            mod->FEFET1lrswGiven = TRUE;
            break;
        case FEFET1_MOD_LPRWB:
            mod->FEFET1lprwb = value->rValue;
            mod->FEFET1lprwbGiven = TRUE;
            break;                     
        case FEFET1_MOD_LPRWG:
            mod->FEFET1lprwg = value->rValue;
            mod->FEFET1lprwgGiven = TRUE;
            break;                     
        case FEFET1_MOD_LPRT:
            mod->FEFET1lprt = value->rValue;
            mod->FEFET1lprtGiven = TRUE;
            break;                     
        case FEFET1_MOD_LETA0:
            mod->FEFET1leta0 = value->rValue;
            mod->FEFET1leta0Given = TRUE;
            break;                 
        case FEFET1_MOD_LETAB:
            mod->FEFET1letab = value->rValue;
            mod->FEFET1letabGiven = TRUE;
            break;                 
        case FEFET1_MOD_LPCLM:
            mod->FEFET1lpclm = value->rValue;
            mod->FEFET1lpclmGiven = TRUE;
            break;                 
        case FEFET1_MOD_LPDIBL1:
            mod->FEFET1lpdibl1 = value->rValue;
            mod->FEFET1lpdibl1Given = TRUE;
            break;                 
        case FEFET1_MOD_LPDIBL2:
            mod->FEFET1lpdibl2 = value->rValue;
            mod->FEFET1lpdibl2Given = TRUE;
            break;                 
        case FEFET1_MOD_LPDIBLB:
            mod->FEFET1lpdiblb = value->rValue;
            mod->FEFET1lpdiblbGiven = TRUE;
            break;                 
        case FEFET1_MOD_LPSCBE1:
            mod->FEFET1lpscbe1 = value->rValue;
            mod->FEFET1lpscbe1Given = TRUE;
            break;                 
        case FEFET1_MOD_LPSCBE2:
            mod->FEFET1lpscbe2 = value->rValue;
            mod->FEFET1lpscbe2Given = TRUE;
            break;                 
        case FEFET1_MOD_LPVAG:
            mod->FEFET1lpvag = value->rValue;
            mod->FEFET1lpvagGiven = TRUE;
            break;                 
        case  FEFET1_MOD_LWR :
            mod->FEFET1lwr = value->rValue;
            mod->FEFET1lwrGiven = TRUE;
            break;
        case  FEFET1_MOD_LDWG :
            mod->FEFET1ldwg = value->rValue;
            mod->FEFET1ldwgGiven = TRUE;
            break;
        case  FEFET1_MOD_LDWB :
            mod->FEFET1ldwb = value->rValue;
            mod->FEFET1ldwbGiven = TRUE;
            break;
        case  FEFET1_MOD_LB0 :
            mod->FEFET1lb0 = value->rValue;
            mod->FEFET1lb0Given = TRUE;
            break;
        case  FEFET1_MOD_LB1 :
            mod->FEFET1lb1 = value->rValue;
            mod->FEFET1lb1Given = TRUE;
            break;
        case  FEFET1_MOD_LALPHA0 :
            mod->FEFET1lalpha0 = value->rValue;
            mod->FEFET1lalpha0Given = TRUE;
            break;
        case  FEFET1_MOD_LALPHA1 :
            mod->FEFET1lalpha1 = value->rValue;
            mod->FEFET1lalpha1Given = TRUE;
            break;
        case  FEFET1_MOD_LBETA0 :
            mod->FEFET1lbeta0 = value->rValue;
            mod->FEFET1lbeta0Given = TRUE;
            break;
        case  FEFET1_MOD_LAGIDL :
            mod->FEFET1lagidl = value->rValue;
            mod->FEFET1lagidlGiven = TRUE;
            break;
        case  FEFET1_MOD_LBGIDL :
            mod->FEFET1lbgidl = value->rValue;
            mod->FEFET1lbgidlGiven = TRUE;
            break;
        case  FEFET1_MOD_LCGIDL :
            mod->FEFET1lcgidl = value->rValue;
            mod->FEFET1lcgidlGiven = TRUE;
            break;
        case  FEFET1_MOD_LPHIN :
            mod->FEFET1lphin = value->rValue;
            mod->FEFET1lphinGiven = TRUE;
            break;
        case  FEFET1_MOD_LEGIDL :
            mod->FEFET1legidl = value->rValue;
            mod->FEFET1legidlGiven = TRUE;
            break;
        case  FEFET1_MOD_LAIGC :
            mod->FEFET1laigc = value->rValue;
            mod->FEFET1laigcGiven = TRUE;
            break;
        case  FEFET1_MOD_LBIGC :
            mod->FEFET1lbigc = value->rValue;
            mod->FEFET1lbigcGiven = TRUE;
            break;
        case  FEFET1_MOD_LCIGC :
            mod->FEFET1lcigc = value->rValue;
            mod->FEFET1lcigcGiven = TRUE;
            break;
        case  FEFET1_MOD_LAIGSD :
            mod->FEFET1laigsd = value->rValue;
            mod->FEFET1laigsdGiven = TRUE;
            break;
        case  FEFET1_MOD_LBIGSD :
            mod->FEFET1lbigsd = value->rValue;
            mod->FEFET1lbigsdGiven = TRUE;
            break;
        case  FEFET1_MOD_LCIGSD :
            mod->FEFET1lcigsd = value->rValue;
            mod->FEFET1lcigsdGiven = TRUE;
            break;
        case  FEFET1_MOD_LAIGBACC :
            mod->FEFET1laigbacc = value->rValue;
            mod->FEFET1laigbaccGiven = TRUE;
            break;
        case  FEFET1_MOD_LBIGBACC :
            mod->FEFET1lbigbacc = value->rValue;
            mod->FEFET1lbigbaccGiven = TRUE;
            break;
        case  FEFET1_MOD_LCIGBACC :
            mod->FEFET1lcigbacc = value->rValue;
            mod->FEFET1lcigbaccGiven = TRUE;
            break;
        case  FEFET1_MOD_LAIGBINV :
            mod->FEFET1laigbinv = value->rValue;
            mod->FEFET1laigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_LBIGBINV :
            mod->FEFET1lbigbinv = value->rValue;
            mod->FEFET1lbigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_LCIGBINV :
            mod->FEFET1lcigbinv = value->rValue;
            mod->FEFET1lcigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_LNIGC :
            mod->FEFET1lnigc = value->rValue;
            mod->FEFET1lnigcGiven = TRUE;
            break;
        case  FEFET1_MOD_LNIGBINV :
            mod->FEFET1lnigbinv = value->rValue;
            mod->FEFET1lnigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_LNIGBACC :
            mod->FEFET1lnigbacc = value->rValue;
            mod->FEFET1lnigbaccGiven = TRUE;
            break;
        case  FEFET1_MOD_LNTOX :
            mod->FEFET1lntox = value->rValue;
            mod->FEFET1lntoxGiven = TRUE;
            break;
        case  FEFET1_MOD_LEIGBINV :
            mod->FEFET1leigbinv = value->rValue;
            mod->FEFET1leigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_LPIGCD :
            mod->FEFET1lpigcd = value->rValue;
            mod->FEFET1lpigcdGiven = TRUE;
            break;
        case  FEFET1_MOD_LPOXEDGE :
            mod->FEFET1lpoxedge = value->rValue;
            mod->FEFET1lpoxedgeGiven = TRUE;
            break;
        case  FEFET1_MOD_LXRCRG1 :
            mod->FEFET1lxrcrg1 = value->rValue;
            mod->FEFET1lxrcrg1Given = TRUE;
            break;
        case  FEFET1_MOD_LXRCRG2 :
            mod->FEFET1lxrcrg2 = value->rValue;
            mod->FEFET1lxrcrg2Given = TRUE;
            break;
        case  FEFET1_MOD_LLAMBDA :
            mod->FEFET1llambda = value->rValue;
            mod->FEFET1llambdaGiven = TRUE;
            break;
        case  FEFET1_MOD_LVTL :
            mod->FEFET1lvtl = value->rValue;
            mod->FEFET1lvtlGiven = TRUE;
            break;
        case  FEFET1_MOD_LXN:
            mod->FEFET1lxn = value->rValue;
            mod->FEFET1lxnGiven = TRUE;
            break;
        case  FEFET1_MOD_LVFBSDOFF:
            mod->FEFET1lvfbsdoff = value->rValue;
            mod->FEFET1lvfbsdoffGiven = TRUE;
            break;
        case  FEFET1_MOD_LTVFBSDOFF:
            mod->FEFET1ltvfbsdoff = value->rValue;
            mod->FEFET1ltvfbsdoffGiven = TRUE;
            break;
        case  FEFET1_MOD_LEU :
            mod->FEFET1leu = value->rValue;
            mod->FEFET1leuGiven = TRUE;
            break;
        case  FEFET1_MOD_LVFB :
            mod->FEFET1lvfb = value->rValue;
            mod->FEFET1lvfbGiven = TRUE;
            break;
        case  FEFET1_MOD_LCGSL :
            mod->FEFET1lcgsl = value->rValue;
            mod->FEFET1lcgslGiven = TRUE;
            break;
        case  FEFET1_MOD_LCGDL :
            mod->FEFET1lcgdl = value->rValue;
            mod->FEFET1lcgdlGiven = TRUE;
            break;
        case  FEFET1_MOD_LCKAPPAS :
            mod->FEFET1lckappas = value->rValue;
            mod->FEFET1lckappasGiven = TRUE;
            break;
        case  FEFET1_MOD_LCKAPPAD :
            mod->FEFET1lckappad = value->rValue;
            mod->FEFET1lckappadGiven = TRUE;
            break;
        case  FEFET1_MOD_LCF :
            mod->FEFET1lcf = value->rValue;
            mod->FEFET1lcfGiven = TRUE;
            break;
        case  FEFET1_MOD_LCLC :
            mod->FEFET1lclc = value->rValue;
            mod->FEFET1lclcGiven = TRUE;
            break;
        case  FEFET1_MOD_LCLE :
            mod->FEFET1lcle = value->rValue;
            mod->FEFET1lcleGiven = TRUE;
            break;
        case  FEFET1_MOD_LVFBCV :
            mod->FEFET1lvfbcv = value->rValue;
            mod->FEFET1lvfbcvGiven = TRUE;
            break;
        case  FEFET1_MOD_LACDE :
            mod->FEFET1lacde = value->rValue;
            mod->FEFET1lacdeGiven = TRUE;
            break;
        case  FEFET1_MOD_LMOIN :
            mod->FEFET1lmoin = value->rValue;
            mod->FEFET1lmoinGiven = TRUE;
            break;
        case  FEFET1_MOD_LNOFF :
            mod->FEFET1lnoff = value->rValue;
            mod->FEFET1lnoffGiven = TRUE;
            break;
        case  FEFET1_MOD_LVOFFCV :
            mod->FEFET1lvoffcv = value->rValue;
            mod->FEFET1lvoffcvGiven = TRUE;
            break;

	/* Width dependence */
        case  FEFET1_MOD_WCDSC :
            mod->FEFET1wcdsc = value->rValue;
            mod->FEFET1wcdscGiven = TRUE;
            break;
       
       
         case  FEFET1_MOD_WCDSCB :
            mod->FEFET1wcdscb = value->rValue;
            mod->FEFET1wcdscbGiven = TRUE;
            break;
         case  FEFET1_MOD_WCDSCD :
            mod->FEFET1wcdscd = value->rValue;
            mod->FEFET1wcdscdGiven = TRUE;
            break;
        case  FEFET1_MOD_WCIT :
            mod->FEFET1wcit = value->rValue;
            mod->FEFET1wcitGiven = TRUE;
            break;
        case  FEFET1_MOD_WNFACTOR :
            mod->FEFET1wnfactor = value->rValue;
            mod->FEFET1wnfactorGiven = TRUE;
            break;
        case FEFET1_MOD_WXJ:
            mod->FEFET1wxj = value->rValue;
            mod->FEFET1wxjGiven = TRUE;
            break;
        case FEFET1_MOD_WVSAT:
            mod->FEFET1wvsat = value->rValue;
            mod->FEFET1wvsatGiven = TRUE;
            break;


        case FEFET1_MOD_WA0:
            mod->FEFET1wa0 = value->rValue;
            mod->FEFET1wa0Given = TRUE;
            break;
        case FEFET1_MOD_WAGS:
            mod->FEFET1wags = value->rValue;
            mod->FEFET1wagsGiven = TRUE;
            break;
        case FEFET1_MOD_WA1:
            mod->FEFET1wa1 = value->rValue;
            mod->FEFET1wa1Given = TRUE;
            break;
        case FEFET1_MOD_WA2:
            mod->FEFET1wa2 = value->rValue;
            mod->FEFET1wa2Given = TRUE;
            break;
        case FEFET1_MOD_WAT:
            mod->FEFET1wat = value->rValue;
            mod->FEFET1watGiven = TRUE;
            break;
        case FEFET1_MOD_WKETA:
            mod->FEFET1wketa = value->rValue;
            mod->FEFET1wketaGiven = TRUE;
            break;    
        case FEFET1_MOD_WNSUB:
            mod->FEFET1wnsub = value->rValue;
            mod->FEFET1wnsubGiven = TRUE;
            break;
        case FEFET1_MOD_WNDEP:
            mod->FEFET1wndep = value->rValue;
            mod->FEFET1wndepGiven = TRUE;
	    if (mod->FEFET1wndep > 1.0e20)
		mod->FEFET1wndep *= 1.0e-6;
            break;
        case FEFET1_MOD_WNSD:
            mod->FEFET1wnsd = value->rValue;
            mod->FEFET1wnsdGiven = TRUE;
            if (mod->FEFET1wnsd > 1.0e23)
                mod->FEFET1wnsd *= 1.0e-6;
            break;
        case FEFET1_MOD_WNGATE:
            mod->FEFET1wngate = value->rValue;
            mod->FEFET1wngateGiven = TRUE;
	    if (mod->FEFET1wngate > 1.0e23)
		mod->FEFET1wngate *= 1.0e-6;
            break;
        case FEFET1_MOD_WGAMMA1:
            mod->FEFET1wgamma1 = value->rValue;
            mod->FEFET1wgamma1Given = TRUE;
            break;
        case FEFET1_MOD_WGAMMA2:
            mod->FEFET1wgamma2 = value->rValue;
            mod->FEFET1wgamma2Given = TRUE;
            break;
        case FEFET1_MOD_WVBX:
            mod->FEFET1wvbx = value->rValue;
            mod->FEFET1wvbxGiven = TRUE;
            break;
        case FEFET1_MOD_WVBM:
            mod->FEFET1wvbm = value->rValue;
            mod->FEFET1wvbmGiven = TRUE;
            break;
        case FEFET1_MOD_WXT:
            mod->FEFET1wxt = value->rValue;
            mod->FEFET1wxtGiven = TRUE;
            break;
        case  FEFET1_MOD_WK1:
            mod->FEFET1wk1 = value->rValue;
            mod->FEFET1wk1Given = TRUE;
            break;
        case  FEFET1_MOD_WKT1:
            mod->FEFET1wkt1 = value->rValue;
            mod->FEFET1wkt1Given = TRUE;
            break;
        case  FEFET1_MOD_WKT1L:
            mod->FEFET1wkt1l = value->rValue;
            mod->FEFET1wkt1lGiven = TRUE;
            break;
        case  FEFET1_MOD_WKT2:
            mod->FEFET1wkt2 = value->rValue;
            mod->FEFET1wkt2Given = TRUE;
            break;
        case  FEFET1_MOD_WK2:
            mod->FEFET1wk2 = value->rValue;
            mod->FEFET1wk2Given = TRUE;
            break;
        case  FEFET1_MOD_WK3:
            mod->FEFET1wk3 = value->rValue;
            mod->FEFET1wk3Given = TRUE;
            break;
        case  FEFET1_MOD_WK3B:
            mod->FEFET1wk3b = value->rValue;
            mod->FEFET1wk3bGiven = TRUE;
            break;
        case  FEFET1_MOD_WLPE0:
            mod->FEFET1wlpe0 = value->rValue;
            mod->FEFET1wlpe0Given = TRUE;
            break;
        case  FEFET1_MOD_WLPEB:
            mod->FEFET1wlpeb = value->rValue;
            mod->FEFET1wlpebGiven = TRUE;
            break;
        case  FEFET1_MOD_WDVTP0:
            mod->FEFET1wdvtp0 = value->rValue;
            mod->FEFET1wdvtp0Given = TRUE;
            break;
        case  FEFET1_MOD_WDVTP1:
            mod->FEFET1wdvtp1 = value->rValue;
            mod->FEFET1wdvtp1Given = TRUE;
            break;
        case  FEFET1_MOD_WW0:
            mod->FEFET1ww0 = value->rValue;
            mod->FEFET1ww0Given = TRUE;
            break;
        case  FEFET1_MOD_WDVT0:               
            mod->FEFET1wdvt0 = value->rValue;
            mod->FEFET1wdvt0Given = TRUE;
            break;
        case  FEFET1_MOD_WDVT1:             
            mod->FEFET1wdvt1 = value->rValue;
            mod->FEFET1wdvt1Given = TRUE;
            break;
        case  FEFET1_MOD_WDVT2:             
            mod->FEFET1wdvt2 = value->rValue;
            mod->FEFET1wdvt2Given = TRUE;
            break;
        case  FEFET1_MOD_WDVT0W:               
            mod->FEFET1wdvt0w = value->rValue;
            mod->FEFET1wdvt0wGiven = TRUE;
            break;
        case  FEFET1_MOD_WDVT1W:             
            mod->FEFET1wdvt1w = value->rValue;
            mod->FEFET1wdvt1wGiven = TRUE;
            break;
        case  FEFET1_MOD_WDVT2W:             
            mod->FEFET1wdvt2w = value->rValue;
            mod->FEFET1wdvt2wGiven = TRUE;
            break;
        case  FEFET1_MOD_WDROUT:             
            mod->FEFET1wdrout = value->rValue;
            mod->FEFET1wdroutGiven = TRUE;
            break;
        case  FEFET1_MOD_WDSUB:             
            mod->FEFET1wdsub = value->rValue;
            mod->FEFET1wdsubGiven = TRUE;
            break;
        case FEFET1_MOD_WVTH0:
            mod->FEFET1wvth0 = value->rValue;
            mod->FEFET1wvth0Given = TRUE;
            break;
        case FEFET1_MOD_WUA:
            mod->FEFET1wua = value->rValue;
            mod->FEFET1wuaGiven = TRUE;
            break;
        case FEFET1_MOD_WUA1:
            mod->FEFET1wua1 = value->rValue;
            mod->FEFET1wua1Given = TRUE;
            break;
        case FEFET1_MOD_WUB:
            mod->FEFET1wub = value->rValue;
            mod->FEFET1wubGiven = TRUE;
            break;
        case FEFET1_MOD_WUB1:
            mod->FEFET1wub1 = value->rValue;
            mod->FEFET1wub1Given = TRUE;
            break;
        case FEFET1_MOD_WUC:
            mod->FEFET1wuc = value->rValue;
            mod->FEFET1wucGiven = TRUE;
            break;
        case FEFET1_MOD_WUC1:
            mod->FEFET1wuc1 = value->rValue;
            mod->FEFET1wuc1Given = TRUE;
            break;
        case  FEFET1_MOD_WU0 :
            mod->FEFET1wu0 = value->rValue;
            mod->FEFET1wu0Given = TRUE;
            break;
        case  FEFET1_MOD_WUTE :
            mod->FEFET1wute = value->rValue;
            mod->FEFET1wuteGiven = TRUE;
            break;
        case FEFET1_MOD_WVOFF:
            mod->FEFET1wvoff = value->rValue;
            mod->FEFET1wvoffGiven = TRUE;
            break;
        case FEFET1_MOD_WTVOFF:
            mod->FEFET1wtvoff = value->rValue;
            mod->FEFET1wtvoffGiven = TRUE;
            break;
        case FEFET1_MOD_WMINV:
            mod->FEFET1wminv = value->rValue;
            mod->FEFET1wminvGiven = TRUE;
            break;
        case FEFET1_MOD_WFPROUT:
            mod->FEFET1wfprout = value->rValue;
            mod->FEFET1wfproutGiven = TRUE;
            break;
        case FEFET1_MOD_WPDITS:
            mod->FEFET1wpdits = value->rValue;
            mod->FEFET1wpditsGiven = TRUE;
            break;
        case FEFET1_MOD_WPDITSD:
            mod->FEFET1wpditsd = value->rValue;
            mod->FEFET1wpditsdGiven = TRUE;
            break;
        case  FEFET1_MOD_WDELTA :
            mod->FEFET1wdelta = value->rValue;
            mod->FEFET1wdeltaGiven = TRUE;
            break;
        case FEFET1_MOD_WRDSW:
            mod->FEFET1wrdsw = value->rValue;
            mod->FEFET1wrdswGiven = TRUE;
            break;                     
        case FEFET1_MOD_WRDW:
            mod->FEFET1wrdw = value->rValue;
            mod->FEFET1wrdwGiven = TRUE;
            break;
        case FEFET1_MOD_WRSW:
            mod->FEFET1wrsw = value->rValue;
            mod->FEFET1wrswGiven = TRUE;
            break;
        case FEFET1_MOD_WPRWB:
            mod->FEFET1wprwb = value->rValue;
            mod->FEFET1wprwbGiven = TRUE;
            break;                     
        case FEFET1_MOD_WPRWG:
            mod->FEFET1wprwg = value->rValue;
            mod->FEFET1wprwgGiven = TRUE;
            break;                     
        case FEFET1_MOD_WPRT:
            mod->FEFET1wprt = value->rValue;
            mod->FEFET1wprtGiven = TRUE;
            break;                     
        case FEFET1_MOD_WETA0:
            mod->FEFET1weta0 = value->rValue;
            mod->FEFET1weta0Given = TRUE;
            break;                 
        case FEFET1_MOD_WETAB:
            mod->FEFET1wetab = value->rValue;
            mod->FEFET1wetabGiven = TRUE;
            break;                 
        case FEFET1_MOD_WPCLM:
            mod->FEFET1wpclm = value->rValue;
            mod->FEFET1wpclmGiven = TRUE;
            break;                 
        case FEFET1_MOD_WPDIBL1:
            mod->FEFET1wpdibl1 = value->rValue;
            mod->FEFET1wpdibl1Given = TRUE;
            break;                 
        case FEFET1_MOD_WPDIBL2:
            mod->FEFET1wpdibl2 = value->rValue;
            mod->FEFET1wpdibl2Given = TRUE;
            break;                 
        case FEFET1_MOD_WPDIBLB:
            mod->FEFET1wpdiblb = value->rValue;
            mod->FEFET1wpdiblbGiven = TRUE;
            break;                 
        case FEFET1_MOD_WPSCBE1:
            mod->FEFET1wpscbe1 = value->rValue;
            mod->FEFET1wpscbe1Given = TRUE;
            break;                 
        case FEFET1_MOD_WPSCBE2:
            mod->FEFET1wpscbe2 = value->rValue;
            mod->FEFET1wpscbe2Given = TRUE;
            break;                 
        case FEFET1_MOD_WPVAG:
            mod->FEFET1wpvag = value->rValue;
            mod->FEFET1wpvagGiven = TRUE;
            break;                 
        case  FEFET1_MOD_WWR :
            mod->FEFET1wwr = value->rValue;
            mod->FEFET1wwrGiven = TRUE;
            break;
        case  FEFET1_MOD_WDWG :
            mod->FEFET1wdwg = value->rValue;
            mod->FEFET1wdwgGiven = TRUE;
            break;
        case  FEFET1_MOD_WDWB :
            mod->FEFET1wdwb = value->rValue;
            mod->FEFET1wdwbGiven = TRUE;
            break;
        case  FEFET1_MOD_WB0 :
            mod->FEFET1wb0 = value->rValue;
            mod->FEFET1wb0Given = TRUE;
            break;
        case  FEFET1_MOD_WB1 :
            mod->FEFET1wb1 = value->rValue;
            mod->FEFET1wb1Given = TRUE;
            break;
        case  FEFET1_MOD_WALPHA0 :
            mod->FEFET1walpha0 = value->rValue;
            mod->FEFET1walpha0Given = TRUE;
            break;
        case  FEFET1_MOD_WALPHA1 :
            mod->FEFET1walpha1 = value->rValue;
            mod->FEFET1walpha1Given = TRUE;
            break;
        case  FEFET1_MOD_WBETA0 :
            mod->FEFET1wbeta0 = value->rValue;
            mod->FEFET1wbeta0Given = TRUE;
            break;
        case  FEFET1_MOD_WAGIDL :
            mod->FEFET1wagidl = value->rValue;
            mod->FEFET1wagidlGiven = TRUE;
            break;
        case  FEFET1_MOD_WBGIDL :
            mod->FEFET1wbgidl = value->rValue;
            mod->FEFET1wbgidlGiven = TRUE;
            break;
        case  FEFET1_MOD_WCGIDL :
            mod->FEFET1wcgidl = value->rValue;
            mod->FEFET1wcgidlGiven = TRUE;
            break;
        case  FEFET1_MOD_WPHIN :
            mod->FEFET1wphin = value->rValue;
            mod->FEFET1wphinGiven = TRUE;
            break;
        case  FEFET1_MOD_WEGIDL :
            mod->FEFET1wegidl = value->rValue;
            mod->FEFET1wegidlGiven = TRUE;
            break;
        case  FEFET1_MOD_WAIGC :
            mod->FEFET1waigc = value->rValue;
            mod->FEFET1waigcGiven = TRUE;
            break;
        case  FEFET1_MOD_WBIGC :
            mod->FEFET1wbigc = value->rValue;
            mod->FEFET1wbigcGiven = TRUE;
            break;
        case  FEFET1_MOD_WCIGC :
            mod->FEFET1wcigc = value->rValue;
            mod->FEFET1wcigcGiven = TRUE;
            break;
        case  FEFET1_MOD_WAIGSD :
            mod->FEFET1waigsd = value->rValue;
            mod->FEFET1waigsdGiven = TRUE;
            break;
        case  FEFET1_MOD_WBIGSD :
            mod->FEFET1wbigsd = value->rValue;
            mod->FEFET1wbigsdGiven = TRUE;
            break;
        case  FEFET1_MOD_WCIGSD :
            mod->FEFET1wcigsd = value->rValue;
            mod->FEFET1wcigsdGiven = TRUE;
            break;
        case  FEFET1_MOD_WAIGBACC :
            mod->FEFET1waigbacc = value->rValue;
            mod->FEFET1waigbaccGiven = TRUE;
            break;
        case  FEFET1_MOD_WBIGBACC :
            mod->FEFET1wbigbacc = value->rValue;
            mod->FEFET1wbigbaccGiven = TRUE;
            break;
        case  FEFET1_MOD_WCIGBACC :
            mod->FEFET1wcigbacc = value->rValue;
            mod->FEFET1wcigbaccGiven = TRUE;
            break;
        case  FEFET1_MOD_WAIGBINV :
            mod->FEFET1waigbinv = value->rValue;
            mod->FEFET1waigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_WBIGBINV :
            mod->FEFET1wbigbinv = value->rValue;
            mod->FEFET1wbigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_WCIGBINV :
            mod->FEFET1wcigbinv = value->rValue;
            mod->FEFET1wcigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_WNIGC :
            mod->FEFET1wnigc = value->rValue;
            mod->FEFET1wnigcGiven = TRUE;
            break;
        case  FEFET1_MOD_WNIGBINV :
            mod->FEFET1wnigbinv = value->rValue;
            mod->FEFET1wnigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_WNIGBACC :
            mod->FEFET1wnigbacc = value->rValue;
            mod->FEFET1wnigbaccGiven = TRUE;
            break;
        case  FEFET1_MOD_WNTOX :
            mod->FEFET1wntox = value->rValue;
            mod->FEFET1wntoxGiven = TRUE;
            break;
        case  FEFET1_MOD_WEIGBINV :
            mod->FEFET1weigbinv = value->rValue;
            mod->FEFET1weigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_WPIGCD :
            mod->FEFET1wpigcd = value->rValue;
            mod->FEFET1wpigcdGiven = TRUE;
            break;
        case  FEFET1_MOD_WPOXEDGE :
            mod->FEFET1wpoxedge = value->rValue;
            mod->FEFET1wpoxedgeGiven = TRUE;
            break;
        case  FEFET1_MOD_WXRCRG1 :
            mod->FEFET1wxrcrg1 = value->rValue;
            mod->FEFET1wxrcrg1Given = TRUE;
            break;
        case  FEFET1_MOD_WXRCRG2 :
            mod->FEFET1wxrcrg2 = value->rValue;
            mod->FEFET1wxrcrg2Given = TRUE;
            break;
        case  FEFET1_MOD_WLAMBDA :
            mod->FEFET1wlambda = value->rValue;
            mod->FEFET1wlambdaGiven = TRUE;
            break;
        case  FEFET1_MOD_WVTL :
            mod->FEFET1wvtl = value->rValue;
            mod->FEFET1wvtlGiven = TRUE;
            break;
        case  FEFET1_MOD_WXN:
            mod->FEFET1wxn = value->rValue;
            mod->FEFET1wxnGiven = TRUE;
            break;
        case  FEFET1_MOD_WVFBSDOFF:
            mod->FEFET1wvfbsdoff = value->rValue;
            mod->FEFET1wvfbsdoffGiven = TRUE;
            break;
        case  FEFET1_MOD_WTVFBSDOFF:
            mod->FEFET1wtvfbsdoff = value->rValue;
            mod->FEFET1wtvfbsdoffGiven = TRUE;
            break;
        case  FEFET1_MOD_WEU :
            mod->FEFET1weu = value->rValue;
            mod->FEFET1weuGiven = TRUE;
            break;
        case  FEFET1_MOD_WVFB :
            mod->FEFET1wvfb = value->rValue;
            mod->FEFET1wvfbGiven = TRUE;
            break;
        case  FEFET1_MOD_WCGSL :
            mod->FEFET1wcgsl = value->rValue;
            mod->FEFET1wcgslGiven = TRUE;
            break;
        case  FEFET1_MOD_WCGDL :
            mod->FEFET1wcgdl = value->rValue;
            mod->FEFET1wcgdlGiven = TRUE;
            break;
        case  FEFET1_MOD_WCKAPPAS :
            mod->FEFET1wckappas = value->rValue;
            mod->FEFET1wckappasGiven = TRUE;
            break;
        case  FEFET1_MOD_WCKAPPAD :
            mod->FEFET1wckappad = value->rValue;
            mod->FEFET1wckappadGiven = TRUE;
            break;
        case  FEFET1_MOD_WCF :
            mod->FEFET1wcf = value->rValue;
            mod->FEFET1wcfGiven = TRUE;
            break;
        case  FEFET1_MOD_WCLC :
            mod->FEFET1wclc = value->rValue;
            mod->FEFET1wclcGiven = TRUE;
            break;
        case  FEFET1_MOD_WCLE :
            mod->FEFET1wcle = value->rValue;
            mod->FEFET1wcleGiven = TRUE;
            break;
        case  FEFET1_MOD_WVFBCV :
            mod->FEFET1wvfbcv = value->rValue;
            mod->FEFET1wvfbcvGiven = TRUE;
            break;
        case  FEFET1_MOD_WACDE :
            mod->FEFET1wacde = value->rValue;
            mod->FEFET1wacdeGiven = TRUE;
            break;
        case  FEFET1_MOD_WMOIN :
            mod->FEFET1wmoin = value->rValue;
            mod->FEFET1wmoinGiven = TRUE;
            break;
        case  FEFET1_MOD_WNOFF :
            mod->FEFET1wnoff = value->rValue;
            mod->FEFET1wnoffGiven = TRUE;
            break;
        case  FEFET1_MOD_WVOFFCV :
            mod->FEFET1wvoffcv = value->rValue;
            mod->FEFET1wvoffcvGiven = TRUE;
            break;

	/* Cross-term dependence */
        case  FEFET1_MOD_PCDSC :
            mod->FEFET1pcdsc = value->rValue;
            mod->FEFET1pcdscGiven = TRUE;
            break;


        case  FEFET1_MOD_PCDSCB :
            mod->FEFET1pcdscb = value->rValue;
            mod->FEFET1pcdscbGiven = TRUE;
            break;
        case  FEFET1_MOD_PCDSCD :
            mod->FEFET1pcdscd = value->rValue;
            mod->FEFET1pcdscdGiven = TRUE;
            break;
        case  FEFET1_MOD_PCIT :
            mod->FEFET1pcit = value->rValue;
            mod->FEFET1pcitGiven = TRUE;
            break;
        case  FEFET1_MOD_PNFACTOR :
            mod->FEFET1pnfactor = value->rValue;
            mod->FEFET1pnfactorGiven = TRUE;
            break;
        case FEFET1_MOD_PXJ:
            mod->FEFET1pxj = value->rValue;
            mod->FEFET1pxjGiven = TRUE;
            break;
        case FEFET1_MOD_PVSAT:
            mod->FEFET1pvsat = value->rValue;
            mod->FEFET1pvsatGiven = TRUE;
            break;


        case FEFET1_MOD_PA0:
            mod->FEFET1pa0 = value->rValue;
            mod->FEFET1pa0Given = TRUE;
            break;
        case FEFET1_MOD_PAGS:
            mod->FEFET1pags = value->rValue;
            mod->FEFET1pagsGiven = TRUE;
            break;
        case FEFET1_MOD_PA1:
            mod->FEFET1pa1 = value->rValue;
            mod->FEFET1pa1Given = TRUE;
            break;
        case FEFET1_MOD_PA2:
            mod->FEFET1pa2 = value->rValue;
            mod->FEFET1pa2Given = TRUE;
            break;
        case FEFET1_MOD_PAT:
            mod->FEFET1pat = value->rValue;
            mod->FEFET1patGiven = TRUE;
            break;
        case FEFET1_MOD_PKETA:
            mod->FEFET1pketa = value->rValue;
            mod->FEFET1pketaGiven = TRUE;
            break;    
        case FEFET1_MOD_PNSUB:
            mod->FEFET1pnsub = value->rValue;
            mod->FEFET1pnsubGiven = TRUE;
            break;
        case FEFET1_MOD_PNDEP:
            mod->FEFET1pndep = value->rValue;
            mod->FEFET1pndepGiven = TRUE;
	    if (mod->FEFET1pndep > 1.0e20)
		mod->FEFET1pndep *= 1.0e-6;
            break;
        case FEFET1_MOD_PNSD:
            mod->FEFET1pnsd = value->rValue;
            mod->FEFET1pnsdGiven = TRUE;
            if (mod->FEFET1pnsd > 1.0e23)
                mod->FEFET1pnsd *= 1.0e-6;
            break;
        case FEFET1_MOD_PNGATE:
            mod->FEFET1pngate = value->rValue;
            mod->FEFET1pngateGiven = TRUE;
	    if (mod->FEFET1pngate > 1.0e23)
		mod->FEFET1pngate *= 1.0e-6;
            break;
        case FEFET1_MOD_PGAMMA1:
            mod->FEFET1pgamma1 = value->rValue;
            mod->FEFET1pgamma1Given = TRUE;
            break;
        case FEFET1_MOD_PGAMMA2:
            mod->FEFET1pgamma2 = value->rValue;
            mod->FEFET1pgamma2Given = TRUE;
            break;
        case FEFET1_MOD_PVBX:
            mod->FEFET1pvbx = value->rValue;
            mod->FEFET1pvbxGiven = TRUE;
            break;
        case FEFET1_MOD_PVBM:
            mod->FEFET1pvbm = value->rValue;
            mod->FEFET1pvbmGiven = TRUE;
            break;
        case FEFET1_MOD_PXT:
            mod->FEFET1pxt = value->rValue;
            mod->FEFET1pxtGiven = TRUE;
            break;
        case  FEFET1_MOD_PK1:
            mod->FEFET1pk1 = value->rValue;
            mod->FEFET1pk1Given = TRUE;
            break;
        case  FEFET1_MOD_PKT1:
            mod->FEFET1pkt1 = value->rValue;
            mod->FEFET1pkt1Given = TRUE;
            break;
        case  FEFET1_MOD_PKT1L:
            mod->FEFET1pkt1l = value->rValue;
            mod->FEFET1pkt1lGiven = TRUE;
            break;
        case  FEFET1_MOD_PKT2:
            mod->FEFET1pkt2 = value->rValue;
            mod->FEFET1pkt2Given = TRUE;
            break;
        case  FEFET1_MOD_PK2:
            mod->FEFET1pk2 = value->rValue;
            mod->FEFET1pk2Given = TRUE;
            break;
        case  FEFET1_MOD_PK3:
            mod->FEFET1pk3 = value->rValue;
            mod->FEFET1pk3Given = TRUE;
            break;
        case  FEFET1_MOD_PK3B:
            mod->FEFET1pk3b = value->rValue;
            mod->FEFET1pk3bGiven = TRUE;
            break;
        case  FEFET1_MOD_PLPE0:
            mod->FEFET1plpe0 = value->rValue;
            mod->FEFET1plpe0Given = TRUE;
            break;
        case  FEFET1_MOD_PLPEB:
            mod->FEFET1plpeb = value->rValue;
            mod->FEFET1plpebGiven = TRUE;
            break;
        case  FEFET1_MOD_PDVTP0:
            mod->FEFET1pdvtp0 = value->rValue;
            mod->FEFET1pdvtp0Given = TRUE;
            break;
        case  FEFET1_MOD_PDVTP1:
            mod->FEFET1pdvtp1 = value->rValue;
            mod->FEFET1pdvtp1Given = TRUE;
            break;
        case  FEFET1_MOD_PW0:
            mod->FEFET1pw0 = value->rValue;
            mod->FEFET1pw0Given = TRUE;
            break;
        case  FEFET1_MOD_PDVT0:               
            mod->FEFET1pdvt0 = value->rValue;
            mod->FEFET1pdvt0Given = TRUE;
            break;
        case  FEFET1_MOD_PDVT1:             
            mod->FEFET1pdvt1 = value->rValue;
            mod->FEFET1pdvt1Given = TRUE;
            break;
        case  FEFET1_MOD_PDVT2:             
            mod->FEFET1pdvt2 = value->rValue;
            mod->FEFET1pdvt2Given = TRUE;
            break;
        case  FEFET1_MOD_PDVT0W:               
            mod->FEFET1pdvt0w = value->rValue;
            mod->FEFET1pdvt0wGiven = TRUE;
            break;
        case  FEFET1_MOD_PDVT1W:             
            mod->FEFET1pdvt1w = value->rValue;
            mod->FEFET1pdvt1wGiven = TRUE;
            break;
        case  FEFET1_MOD_PDVT2W:             
            mod->FEFET1pdvt2w = value->rValue;
            mod->FEFET1pdvt2wGiven = TRUE;
            break;
        case  FEFET1_MOD_PDROUT:             
            mod->FEFET1pdrout = value->rValue;
            mod->FEFET1pdroutGiven = TRUE;
            break;
        case  FEFET1_MOD_PDSUB:             
            mod->FEFET1pdsub = value->rValue;
            mod->FEFET1pdsubGiven = TRUE;
            break;
        case FEFET1_MOD_PVTH0:
            mod->FEFET1pvth0 = value->rValue;
            mod->FEFET1pvth0Given = TRUE;
            break;
        case FEFET1_MOD_PUA:
            mod->FEFET1pua = value->rValue;
            mod->FEFET1puaGiven = TRUE;
            break;
        case FEFET1_MOD_PUA1:
            mod->FEFET1pua1 = value->rValue;
            mod->FEFET1pua1Given = TRUE;
            break;
        case FEFET1_MOD_PUB:
            mod->FEFET1pub = value->rValue;
            mod->FEFET1pubGiven = TRUE;
            break;
        case FEFET1_MOD_PUB1:
            mod->FEFET1pub1 = value->rValue;
            mod->FEFET1pub1Given = TRUE;
            break;
        case FEFET1_MOD_PUC:
            mod->FEFET1puc = value->rValue;
            mod->FEFET1pucGiven = TRUE;
            break;
        case FEFET1_MOD_PUC1:
            mod->FEFET1puc1 = value->rValue;
            mod->FEFET1puc1Given = TRUE;
            break;
        case  FEFET1_MOD_PU0 :
            mod->FEFET1pu0 = value->rValue;
            mod->FEFET1pu0Given = TRUE;
            break;
        case  FEFET1_MOD_PUTE :
            mod->FEFET1pute = value->rValue;
            mod->FEFET1puteGiven = TRUE;
            break;
        case FEFET1_MOD_PVOFF:
            mod->FEFET1pvoff = value->rValue;
            mod->FEFET1pvoffGiven = TRUE;
            break;
        case FEFET1_MOD_PTVOFF:
            mod->FEFET1ptvoff = value->rValue;
            mod->FEFET1ptvoffGiven = TRUE;
            break;
        case FEFET1_MOD_PMINV:
            mod->FEFET1pminv = value->rValue;
            mod->FEFET1pminvGiven = TRUE;
            break;
        case FEFET1_MOD_PFPROUT:
            mod->FEFET1pfprout = value->rValue;
            mod->FEFET1pfproutGiven = TRUE;
            break;
        case FEFET1_MOD_PPDITS:
            mod->FEFET1ppdits = value->rValue;
            mod->FEFET1ppditsGiven = TRUE;
            break;
        case FEFET1_MOD_PPDITSD:
            mod->FEFET1ppditsd = value->rValue;
            mod->FEFET1ppditsdGiven = TRUE;
            break;
        case  FEFET1_MOD_PDELTA :
            mod->FEFET1pdelta = value->rValue;
            mod->FEFET1pdeltaGiven = TRUE;
            break;
        case FEFET1_MOD_PRDSW:
            mod->FEFET1prdsw = value->rValue;
            mod->FEFET1prdswGiven = TRUE;
            break;                     
        case FEFET1_MOD_PRDW:
            mod->FEFET1prdw = value->rValue;
            mod->FEFET1prdwGiven = TRUE;
            break;
        case FEFET1_MOD_PRSW:
            mod->FEFET1prsw = value->rValue;
            mod->FEFET1prswGiven = TRUE;
            break;
        case FEFET1_MOD_PPRWB:
            mod->FEFET1pprwb = value->rValue;
            mod->FEFET1pprwbGiven = TRUE;
            break;                     
        case FEFET1_MOD_PPRWG:
            mod->FEFET1pprwg = value->rValue;
            mod->FEFET1pprwgGiven = TRUE;
            break;                     
        case FEFET1_MOD_PPRT:
            mod->FEFET1pprt = value->rValue;
            mod->FEFET1pprtGiven = TRUE;
            break;                     
        case FEFET1_MOD_PETA0:
            mod->FEFET1peta0 = value->rValue;
            mod->FEFET1peta0Given = TRUE;
            break;                 
        case FEFET1_MOD_PETAB:
            mod->FEFET1petab = value->rValue;
            mod->FEFET1petabGiven = TRUE;
            break;                 
        case FEFET1_MOD_PPCLM:
            mod->FEFET1ppclm = value->rValue;
            mod->FEFET1ppclmGiven = TRUE;
            break;                 
        case FEFET1_MOD_PPDIBL1:
            mod->FEFET1ppdibl1 = value->rValue;
            mod->FEFET1ppdibl1Given = TRUE;
            break;                 
        case FEFET1_MOD_PPDIBL2:
            mod->FEFET1ppdibl2 = value->rValue;
            mod->FEFET1ppdibl2Given = TRUE;
            break;                 
        case FEFET1_MOD_PPDIBLB:
            mod->FEFET1ppdiblb = value->rValue;
            mod->FEFET1ppdiblbGiven = TRUE;
            break;                 
        case FEFET1_MOD_PPSCBE1:
            mod->FEFET1ppscbe1 = value->rValue;
            mod->FEFET1ppscbe1Given = TRUE;
            break;                 
        case FEFET1_MOD_PPSCBE2:
            mod->FEFET1ppscbe2 = value->rValue;
            mod->FEFET1ppscbe2Given = TRUE;
            break;                 
        case FEFET1_MOD_PPVAG:
            mod->FEFET1ppvag = value->rValue;
            mod->FEFET1ppvagGiven = TRUE;
            break;                 
        case  FEFET1_MOD_PWR :
            mod->FEFET1pwr = value->rValue;
            mod->FEFET1pwrGiven = TRUE;
            break;
        case  FEFET1_MOD_PDWG :
            mod->FEFET1pdwg = value->rValue;
            mod->FEFET1pdwgGiven = TRUE;
            break;
        case  FEFET1_MOD_PDWB :
            mod->FEFET1pdwb = value->rValue;
            mod->FEFET1pdwbGiven = TRUE;
            break;
        case  FEFET1_MOD_PB0 :
            mod->FEFET1pb0 = value->rValue;
            mod->FEFET1pb0Given = TRUE;
            break;
        case  FEFET1_MOD_PB1 :
            mod->FEFET1pb1 = value->rValue;
            mod->FEFET1pb1Given = TRUE;
            break;
        case  FEFET1_MOD_PALPHA0 :
            mod->FEFET1palpha0 = value->rValue;
            mod->FEFET1palpha0Given = TRUE;
            break;
        case  FEFET1_MOD_PALPHA1 :
            mod->FEFET1palpha1 = value->rValue;
            mod->FEFET1palpha1Given = TRUE;
            break;
        case  FEFET1_MOD_PBETA0 :
            mod->FEFET1pbeta0 = value->rValue;
            mod->FEFET1pbeta0Given = TRUE;
            break;
        case  FEFET1_MOD_PAGIDL :
            mod->FEFET1pagidl = value->rValue;
            mod->FEFET1pagidlGiven = TRUE;
            break;
        case  FEFET1_MOD_PBGIDL :
            mod->FEFET1pbgidl = value->rValue;
            mod->FEFET1pbgidlGiven = TRUE;
            break;
        case  FEFET1_MOD_PCGIDL :
            mod->FEFET1pcgidl = value->rValue;
            mod->FEFET1pcgidlGiven = TRUE;
            break;
        case  FEFET1_MOD_PPHIN :
            mod->FEFET1pphin = value->rValue;
            mod->FEFET1pphinGiven = TRUE;
            break;
        case  FEFET1_MOD_PEGIDL :
            mod->FEFET1pegidl = value->rValue;
            mod->FEFET1pegidlGiven = TRUE;
            break;
        case  FEFET1_MOD_PAIGC :
            mod->FEFET1paigc = value->rValue;
            mod->FEFET1paigcGiven = TRUE;
            break;
        case  FEFET1_MOD_PBIGC :
            mod->FEFET1pbigc = value->rValue;
            mod->FEFET1pbigcGiven = TRUE;
            break;
        case  FEFET1_MOD_PCIGC :
            mod->FEFET1pcigc = value->rValue;
            mod->FEFET1pcigcGiven = TRUE;
            break;
        case  FEFET1_MOD_PAIGSD :
            mod->FEFET1paigsd = value->rValue;
            mod->FEFET1paigsdGiven = TRUE;
            break;
        case  FEFET1_MOD_PBIGSD :
            mod->FEFET1pbigsd = value->rValue;
            mod->FEFET1pbigsdGiven = TRUE;
            break;
        case  FEFET1_MOD_PCIGSD :
            mod->FEFET1pcigsd = value->rValue;
            mod->FEFET1pcigsdGiven = TRUE;
            break;
        case  FEFET1_MOD_PAIGBACC :
            mod->FEFET1paigbacc = value->rValue;
            mod->FEFET1paigbaccGiven = TRUE;
            break;
        case  FEFET1_MOD_PBIGBACC :
            mod->FEFET1pbigbacc = value->rValue;
            mod->FEFET1pbigbaccGiven = TRUE;
            break;
        case  FEFET1_MOD_PCIGBACC :
            mod->FEFET1pcigbacc = value->rValue;
            mod->FEFET1pcigbaccGiven = TRUE;
            break;
        case  FEFET1_MOD_PAIGBINV :
            mod->FEFET1paigbinv = value->rValue;
            mod->FEFET1paigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_PBIGBINV :
            mod->FEFET1pbigbinv = value->rValue;
            mod->FEFET1pbigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_PCIGBINV :
            mod->FEFET1pcigbinv = value->rValue;
            mod->FEFET1pcigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_PNIGC :
            mod->FEFET1pnigc = value->rValue;
            mod->FEFET1pnigcGiven = TRUE;
            break;
        case  FEFET1_MOD_PNIGBINV :
            mod->FEFET1pnigbinv = value->rValue;
            mod->FEFET1pnigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_PNIGBACC :
            mod->FEFET1pnigbacc = value->rValue;
            mod->FEFET1pnigbaccGiven = TRUE;
            break;
        case  FEFET1_MOD_PNTOX :
            mod->FEFET1pntox = value->rValue;
            mod->FEFET1pntoxGiven = TRUE;
            break;
        case  FEFET1_MOD_PEIGBINV :
            mod->FEFET1peigbinv = value->rValue;
            mod->FEFET1peigbinvGiven = TRUE;
            break;
        case  FEFET1_MOD_PPIGCD :
            mod->FEFET1ppigcd = value->rValue;
            mod->FEFET1ppigcdGiven = TRUE;
            break;
        case  FEFET1_MOD_PPOXEDGE :
            mod->FEFET1ppoxedge = value->rValue;
            mod->FEFET1ppoxedgeGiven = TRUE;
            break;
        case  FEFET1_MOD_PXRCRG1 :
            mod->FEFET1pxrcrg1 = value->rValue;
            mod->FEFET1pxrcrg1Given = TRUE;
            break;
        case  FEFET1_MOD_PXRCRG2 :
            mod->FEFET1pxrcrg2 = value->rValue;
            mod->FEFET1pxrcrg2Given = TRUE;
            break;
        case  FEFET1_MOD_PLAMBDA :
            mod->FEFET1plambda = value->rValue;
            mod->FEFET1plambdaGiven = TRUE;
            break;
        case  FEFET1_MOD_PVTL :
            mod->FEFET1pvtl = value->rValue;
            mod->FEFET1pvtlGiven = TRUE;
            break;
        case  FEFET1_MOD_PXN:
            mod->FEFET1pxn = value->rValue;
            mod->FEFET1pxnGiven = TRUE;
            break;
        case  FEFET1_MOD_PVFBSDOFF:
            mod->FEFET1pvfbsdoff = value->rValue;
            mod->FEFET1pvfbsdoffGiven = TRUE;
            break;
        case  FEFET1_MOD_PTVFBSDOFF:
            mod->FEFET1ptvfbsdoff = value->rValue;
            mod->FEFET1ptvfbsdoffGiven = TRUE;
            break;
        case  FEFET1_MOD_PEU :
            mod->FEFET1peu = value->rValue;
            mod->FEFET1peuGiven = TRUE;
            break;
        case  FEFET1_MOD_PVFB :
            mod->FEFET1pvfb = value->rValue;
            mod->FEFET1pvfbGiven = TRUE;
            break;
        case  FEFET1_MOD_PCGSL :
            mod->FEFET1pcgsl = value->rValue;
            mod->FEFET1pcgslGiven = TRUE;
            break;
        case  FEFET1_MOD_PCGDL :
            mod->FEFET1pcgdl = value->rValue;
            mod->FEFET1pcgdlGiven = TRUE;
            break;
        case  FEFET1_MOD_PCKAPPAS :
            mod->FEFET1pckappas = value->rValue;
            mod->FEFET1pckappasGiven = TRUE;
            break;
        case  FEFET1_MOD_PCKAPPAD :
            mod->FEFET1pckappad = value->rValue;
            mod->FEFET1pckappadGiven = TRUE;
            break;
        case  FEFET1_MOD_PCF :
            mod->FEFET1pcf = value->rValue;
            mod->FEFET1pcfGiven = TRUE;
            break;
        case  FEFET1_MOD_PCLC :
            mod->FEFET1pclc = value->rValue;
            mod->FEFET1pclcGiven = TRUE;
            break;
        case  FEFET1_MOD_PCLE :
            mod->FEFET1pcle = value->rValue;
            mod->FEFET1pcleGiven = TRUE;
            break;
        case  FEFET1_MOD_PVFBCV :
            mod->FEFET1pvfbcv = value->rValue;
            mod->FEFET1pvfbcvGiven = TRUE;
            break;
        case  FEFET1_MOD_PACDE :
            mod->FEFET1pacde = value->rValue;
            mod->FEFET1pacdeGiven = TRUE;
            break;
        case  FEFET1_MOD_PMOIN :
            mod->FEFET1pmoin = value->rValue;
            mod->FEFET1pmoinGiven = TRUE;
            break;
        case  FEFET1_MOD_PNOFF :
            mod->FEFET1pnoff = value->rValue;
            mod->FEFET1pnoffGiven = TRUE;
            break;
        case  FEFET1_MOD_PVOFFCV :
            mod->FEFET1pvoffcv = value->rValue;
            mod->FEFET1pvoffcvGiven = TRUE;
            break;

        case  FEFET1_MOD_TNOM :
            mod->FEFET1tnom = value->rValue + CONSTCtoK;
            mod->FEFET1tnomGiven = TRUE;
            break;
        case  FEFET1_MOD_CGSO :
            mod->FEFET1cgso = value->rValue;
            mod->FEFET1cgsoGiven = TRUE;
            break;
        case  FEFET1_MOD_CGDO :
            mod->FEFET1cgdo = value->rValue;
            mod->FEFET1cgdoGiven = TRUE;
            break;
        case  FEFET1_MOD_CGBO :
            mod->FEFET1cgbo = value->rValue;
            mod->FEFET1cgboGiven = TRUE;
            break;
        case  FEFET1_MOD_XPART :
            mod->FEFET1xpart = value->rValue;
            mod->FEFET1xpartGiven = TRUE;
            break;
        case  FEFET1_MOD_RSH :
            mod->FEFET1sheetResistance = value->rValue;
            mod->FEFET1sheetResistanceGiven = TRUE;
            break;
        case  FEFET1_MOD_JSS :
            mod->FEFET1SjctSatCurDensity = value->rValue;
            mod->FEFET1SjctSatCurDensityGiven = TRUE;
            break;
        case  FEFET1_MOD_JSWS :
            mod->FEFET1SjctSidewallSatCurDensity = value->rValue;
            mod->FEFET1SjctSidewallSatCurDensityGiven = TRUE;
            break;
        case  FEFET1_MOD_JSWGS :
            mod->FEFET1SjctGateSidewallSatCurDensity = value->rValue;
            mod->FEFET1SjctGateSidewallSatCurDensityGiven = TRUE;
            break;
        case  FEFET1_MOD_PBS :
            mod->FEFET1SbulkJctPotential = value->rValue;
            mod->FEFET1SbulkJctPotentialGiven = TRUE;
            break;
        case  FEFET1_MOD_MJS :
            mod->FEFET1SbulkJctBotGradingCoeff = value->rValue;
            mod->FEFET1SbulkJctBotGradingCoeffGiven = TRUE;
            break;
        case  FEFET1_MOD_PBSWS :
            mod->FEFET1SsidewallJctPotential = value->rValue;
            mod->FEFET1SsidewallJctPotentialGiven = TRUE;
            break;
        case  FEFET1_MOD_MJSWS :
            mod->FEFET1SbulkJctSideGradingCoeff = value->rValue;
            mod->FEFET1SbulkJctSideGradingCoeffGiven = TRUE;
            break;
        case  FEFET1_MOD_CJS :
            mod->FEFET1SunitAreaJctCap = value->rValue;
            mod->FEFET1SunitAreaJctCapGiven = TRUE;
            break;
        case  FEFET1_MOD_CJSWS :
            mod->FEFET1SunitLengthSidewallJctCap = value->rValue;
            mod->FEFET1SunitLengthSidewallJctCapGiven = TRUE;
            break;
        case  FEFET1_MOD_NJS :
            mod->FEFET1SjctEmissionCoeff = value->rValue;
            mod->FEFET1SjctEmissionCoeffGiven = TRUE;
            break;
        case  FEFET1_MOD_PBSWGS :
            mod->FEFET1SGatesidewallJctPotential = value->rValue;
            mod->FEFET1SGatesidewallJctPotentialGiven = TRUE;
            break;
        case  FEFET1_MOD_MJSWGS :
            mod->FEFET1SbulkJctGateSideGradingCoeff = value->rValue;
            mod->FEFET1SbulkJctGateSideGradingCoeffGiven = TRUE;
            break;
        case  FEFET1_MOD_CJSWGS :
            mod->FEFET1SunitLengthGateSidewallJctCap = value->rValue;
            mod->FEFET1SunitLengthGateSidewallJctCapGiven = TRUE;
            break;
        case  FEFET1_MOD_XTIS :
            mod->FEFET1SjctTempExponent = value->rValue;
            mod->FEFET1SjctTempExponentGiven = TRUE;
            break;
        case  FEFET1_MOD_JSD :
            mod->FEFET1DjctSatCurDensity = value->rValue;
            mod->FEFET1DjctSatCurDensityGiven = TRUE;
            break;
        case  FEFET1_MOD_JSWD :
            mod->FEFET1DjctSidewallSatCurDensity = value->rValue;
            mod->FEFET1DjctSidewallSatCurDensityGiven = TRUE;
            break;
        case  FEFET1_MOD_JSWGD :
            mod->FEFET1DjctGateSidewallSatCurDensity = value->rValue;
            mod->FEFET1DjctGateSidewallSatCurDensityGiven = TRUE;
            break;
        case  FEFET1_MOD_PBD :
            mod->FEFET1DbulkJctPotential = value->rValue;
            mod->FEFET1DbulkJctPotentialGiven = TRUE;
            break;
        case  FEFET1_MOD_MJD :
            mod->FEFET1DbulkJctBotGradingCoeff = value->rValue;
            mod->FEFET1DbulkJctBotGradingCoeffGiven = TRUE;
            break;
        case  FEFET1_MOD_PBSWD :
            mod->FEFET1DsidewallJctPotential = value->rValue;
            mod->FEFET1DsidewallJctPotentialGiven = TRUE;
            break;
        case  FEFET1_MOD_MJSWD :
            mod->FEFET1DbulkJctSideGradingCoeff = value->rValue;
            mod->FEFET1DbulkJctSideGradingCoeffGiven = TRUE;
            break;
        case  FEFET1_MOD_CJD :
            mod->FEFET1DunitAreaJctCap = value->rValue;
            mod->FEFET1DunitAreaJctCapGiven = TRUE;
            break;
        case  FEFET1_MOD_CJSWD :
            mod->FEFET1DunitLengthSidewallJctCap = value->rValue;
            mod->FEFET1DunitLengthSidewallJctCapGiven = TRUE;
            break;
        case  FEFET1_MOD_NJD :
            mod->FEFET1DjctEmissionCoeff = value->rValue;
            mod->FEFET1DjctEmissionCoeffGiven = TRUE;
            break;
        case  FEFET1_MOD_PBSWGD :
            mod->FEFET1DGatesidewallJctPotential = value->rValue;
            mod->FEFET1DGatesidewallJctPotentialGiven = TRUE;
            break;
        case  FEFET1_MOD_MJSWGD :
            mod->FEFET1DbulkJctGateSideGradingCoeff = value->rValue;
            mod->FEFET1DbulkJctGateSideGradingCoeffGiven = TRUE;
            break;
        case  FEFET1_MOD_CJSWGD :
            mod->FEFET1DunitLengthGateSidewallJctCap = value->rValue;
            mod->FEFET1DunitLengthGateSidewallJctCapGiven = TRUE;
            break;
        case  FEFET1_MOD_XTID :
            mod->FEFET1DjctTempExponent = value->rValue;
            mod->FEFET1DjctTempExponentGiven = TRUE;
            break;
        case  FEFET1_MOD_LINT :
            mod->FEFET1Lint = value->rValue;
            mod->FEFET1LintGiven = TRUE;
            break;
        case  FEFET1_MOD_LL :
            mod->FEFET1Ll = value->rValue;
            mod->FEFET1LlGiven = TRUE;
            break;
        case  FEFET1_MOD_LLC :
            mod->FEFET1Llc = value->rValue;
            mod->FEFET1LlcGiven = TRUE;
            break;
        case  FEFET1_MOD_LLN :
            mod->FEFET1Lln = value->rValue;
            mod->FEFET1LlnGiven = TRUE;
            break;
        case  FEFET1_MOD_LW :
            mod->FEFET1Lw = value->rValue;
            mod->FEFET1LwGiven = TRUE;
            break;
        case  FEFET1_MOD_LWC :
            mod->FEFET1Lwc = value->rValue;
            mod->FEFET1LwcGiven = TRUE;
            break;
        case  FEFET1_MOD_LWN :
            mod->FEFET1Lwn = value->rValue;
            mod->FEFET1LwnGiven = TRUE;
            break;
        case  FEFET1_MOD_LWL :
            mod->FEFET1Lwl = value->rValue;
            mod->FEFET1LwlGiven = TRUE;
            break;
        case  FEFET1_MOD_LWLC :
            mod->FEFET1Lwlc = value->rValue;
            mod->FEFET1LwlcGiven = TRUE;
            break;
        case  FEFET1_MOD_LMIN :
            mod->FEFET1Lmin = value->rValue;
            mod->FEFET1LminGiven = TRUE;
            break;
        case  FEFET1_MOD_LMAX :
            mod->FEFET1Lmax = value->rValue;
            mod->FEFET1LmaxGiven = TRUE;
            break;
        case  FEFET1_MOD_WINT :
            mod->FEFET1Wint = value->rValue;
            mod->FEFET1WintGiven = TRUE;
            break;
        case  FEFET1_MOD_WL :
            mod->FEFET1Wl = value->rValue;
            mod->FEFET1WlGiven = TRUE;
            break;
        case  FEFET1_MOD_WLC :
            mod->FEFET1Wlc = value->rValue;
            mod->FEFET1WlcGiven = TRUE;
            break;
        case  FEFET1_MOD_WLN :
            mod->FEFET1Wln = value->rValue;
            mod->FEFET1WlnGiven = TRUE;
            break;
        case  FEFET1_MOD_WW :
            mod->FEFET1Ww = value->rValue;
            mod->FEFET1WwGiven = TRUE;
            break;
        case  FEFET1_MOD_WWC :
            mod->FEFET1Wwc = value->rValue;
            mod->FEFET1WwcGiven = TRUE;
            break;
        case  FEFET1_MOD_WWN :
            mod->FEFET1Wwn = value->rValue;
            mod->FEFET1WwnGiven = TRUE;
            break;
        case  FEFET1_MOD_WWL :
            mod->FEFET1Wwl = value->rValue;
            mod->FEFET1WwlGiven = TRUE;
            break;
        case  FEFET1_MOD_WWLC :
            mod->FEFET1Wwlc = value->rValue;
            mod->FEFET1WwlcGiven = TRUE;
            break;
        case  FEFET1_MOD_WMIN :
            mod->FEFET1Wmin = value->rValue;
            mod->FEFET1WminGiven = TRUE;
            break;
        case  FEFET1_MOD_WMAX :
            mod->FEFET1Wmax = value->rValue;
            mod->FEFET1WmaxGiven = TRUE;
            break;

        case  FEFET1_MOD_NOIA :
            mod->FEFET1oxideTrapDensityA = value->rValue;
            mod->FEFET1oxideTrapDensityAGiven = TRUE;
            break;
        case  FEFET1_MOD_NOIB :
            mod->FEFET1oxideTrapDensityB = value->rValue;
            mod->FEFET1oxideTrapDensityBGiven = TRUE;
            break;
        case  FEFET1_MOD_NOIC :
            mod->FEFET1oxideTrapDensityC = value->rValue;
            mod->FEFET1oxideTrapDensityCGiven = TRUE;
            break;
        case  FEFET1_MOD_EM :
            mod->FEFET1em = value->rValue;
            mod->FEFET1emGiven = TRUE;
            break;
        case  FEFET1_MOD_EF :
            mod->FEFET1ef = value->rValue;
            mod->FEFET1efGiven = TRUE;
            break;
        case  FEFET1_MOD_AF :
            mod->FEFET1af = value->rValue;
            mod->FEFET1afGiven = TRUE;
            break;
        case  FEFET1_MOD_KF :
            mod->FEFET1kf = value->rValue;
            mod->FEFET1kfGiven = TRUE;
            break;

        case FEFET1_MOD_VGS_MAX:
            mod->FEFET1vgsMax = value->rValue;
            mod->FEFET1vgsMaxGiven = TRUE;
            break;
        case FEFET1_MOD_VGD_MAX:
            mod->FEFET1vgdMax = value->rValue;
            mod->FEFET1vgdMaxGiven = TRUE;
            break;
        case FEFET1_MOD_VGB_MAX:
            mod->FEFET1vgbMax = value->rValue;
            mod->FEFET1vgbMaxGiven = TRUE;
            break;
        case FEFET1_MOD_VDS_MAX:
            mod->FEFET1vdsMax = value->rValue;
            mod->FEFET1vdsMaxGiven = TRUE;
            break;
        case FEFET1_MOD_VBS_MAX:
            mod->FEFET1vbsMax = value->rValue;
            mod->FEFET1vbsMaxGiven = TRUE;
            break;
        case FEFET1_MOD_VBD_MAX:
            mod->FEFET1vbdMax = value->rValue;
            mod->FEFET1vbdMaxGiven = TRUE;
            break;
        case FEFET1_MOD_VGSR_MAX:
            mod->FEFET1vgsrMax = value->rValue;
            mod->FEFET1vgsrMaxGiven = TRUE;
            break;
        case FEFET1_MOD_VGDR_MAX:
            mod->FEFET1vgdrMax = value->rValue;
            mod->FEFET1vgdrMaxGiven = TRUE;
            break;
        case FEFET1_MOD_VGBR_MAX:
            mod->FEFET1vgbrMax = value->rValue;
            mod->FEFET1vgbrMaxGiven = TRUE;
            break;
        case FEFET1_MOD_VBSR_MAX:
            mod->FEFET1vbsrMax = value->rValue;
            mod->FEFET1vbsrMaxGiven = TRUE;
            break;
        case FEFET1_MOD_VBDR_MAX:
            mod->FEFET1vbdrMax = value->rValue;
            mod->FEFET1vbdrMaxGiven = TRUE;
            break;

        case  FEFET1_MOD_NMOS  :
            if(value->iValue) {
                mod->FEFET1type = 1;
                mod->FEFET1typeGiven = TRUE;
            }
            break;
        case  FEFET1_MOD_PMOS  :
            if(value->iValue) {
                mod->FEFET1type = - 1;
                mod->FEFET1typeGiven = TRUE;
            }
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}


