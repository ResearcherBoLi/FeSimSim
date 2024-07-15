/**********
Copyright 2024 Xidian University
Author: 2024 Bo Li
Modified: 2024/01/27  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/ifsim.h"
#include "fefet1def.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"
#include "ngspice/fteext.h"

int
FEFET1param(
int param,
IFvalue *value,
GENinstance *inst,
IFvalue *select)
{
    double scale;

    FEFET1instance *here = (FEFET1instance*)inst;

    NG_IGNORE(select);

    if (!cp_getvar("scale", CP_REAL, &scale, 0))
        scale = 1;

    switch (param) {

        case FECAP1_IC:
            here->FECAP1initCondP = value->rValue;
            here->FECAP1_icGiven = TRUE;
            break;  

        case FECAP1_AREA:
            here->FECAP1_Area = value->rValue * scale;
            here->FECAP1_AreaGiven = TRUE;
            break;



        case FEFET1_W:
            here->FEFET1w = value->rValue*scale;
            here->FEFET1wGiven = TRUE;
            break;
        case FEFET1_L:
            here->FEFET1l = value->rValue*scale;
            here->FEFET1lGiven = TRUE;
            break;
        case FEFET1_M:
            here->FEFET1m = value->rValue;
            here->FEFET1mGiven = TRUE;
            break;
        case FEFET1_NF:
            here->FEFET1nf = value->rValue;
            here->FEFET1nfGiven = TRUE;
            break;
        case FEFET1_MIN:
            here->FEFET1min = value->iValue;
            here->FEFET1minGiven = TRUE;
            break;
        case FEFET1_AS:
            here->FEFET1sourceArea = value->rValue*scale*scale;
            here->FEFET1sourceAreaGiven = TRUE;
            break;
        case FEFET1_AD:
            here->FEFET1drainArea = value->rValue*scale*scale;
            here->FEFET1drainAreaGiven = TRUE;
            break;
        case FEFET1_PS:
            here->FEFET1sourcePerimeter = value->rValue*scale;
            here->FEFET1sourcePerimeterGiven = TRUE;
            break;
        case FEFET1_PD:
            here->FEFET1drainPerimeter = value->rValue*scale;
            here->FEFET1drainPerimeterGiven = TRUE;
            break;
        case FEFET1_NRS:
            here->FEFET1sourceSquares = value->rValue;
            here->FEFET1sourceSquaresGiven = TRUE;
            break;
        case FEFET1_NRD:
            here->FEFET1drainSquares = value->rValue;
            here->FEFET1drainSquaresGiven = TRUE;
            break;
        case FEFET1_OFF:
            here->FEFET1off = value->iValue;
            break;
        case FEFET1_SA:
            here->FEFET1sa = value->rValue*scale;
            here->FEFET1saGiven = TRUE;
            break;
        case FEFET1_SB:
            here->FEFET1sb = value->rValue*scale;
            here->FEFET1sbGiven = TRUE;
            break;
        case FEFET1_SD:
            here->FEFET1sd = value->rValue*scale;
            here->FEFET1sdGiven = TRUE;
            break;
        case FEFET1_SCA:
            here->FEFET1sca = value->rValue;
            here->FEFET1scaGiven = TRUE;
            break;
        case FEFET1_SCB:
            here->FEFET1scb = value->rValue;
            here->FEFET1scbGiven = TRUE;
            break;
        case FEFET1_SCC:
            here->FEFET1scc = value->rValue;
            here->FEFET1sccGiven = TRUE;
            break;
        case FEFET1_SC:
            here->FEFET1sc = value->rValue*scale;
            here->FEFET1scGiven = TRUE;
            break;
        case FEFET1_RBSB:
            here->FEFET1rbsb = value->rValue;
            here->FEFET1rbsbGiven = TRUE;
            break;
        case FEFET1_RBDB:
            here->FEFET1rbdb = value->rValue;
            here->FEFET1rbdbGiven = TRUE;
            break;
        case FEFET1_RBPB:
            here->FEFET1rbpb = value->rValue;
            here->FEFET1rbpbGiven = TRUE;
            break;
        case FEFET1_RBPS:
            here->FEFET1rbps = value->rValue;
            here->FEFET1rbpsGiven = TRUE;
            break;
        case FEFET1_RBPD:
            here->FEFET1rbpd = value->rValue;
            here->FEFET1rbpdGiven = TRUE;
            break;
        case FEFET1_DELVTO:
            here->FEFET1delvto = value->rValue;
            here->FEFET1delvtoGiven = TRUE;
            break;
        case FEFET1_XGW:
            here->FEFET1xgw = value->rValue;
            here->FEFET1xgwGiven = TRUE;
            break;
        case FEFET1_NGCON:
            here->FEFET1ngcon = value->rValue;
            here->FEFET1ngconGiven = TRUE;
            break;
        case FEFET1_TRNQSMOD:
            here->FEFET1trnqsMod = value->iValue;
            here->FEFET1trnqsModGiven = TRUE;
            break;
        case FEFET1_ACNQSMOD:
            here->FEFET1acnqsMod = value->iValue;
            here->FEFET1acnqsModGiven = TRUE;
            break;
        case FEFET1_RBODYMOD:
            here->FEFET1rbodyMod = value->iValue;
            here->FEFET1rbodyModGiven = TRUE;
            break;
        case FEFET1_RGATEMOD:
            here->FEFET1rgateMod = value->iValue;
            here->FEFET1rgateModGiven = TRUE;
            break;
        case FEFET1_GEOMOD:
            here->FEFET1geoMod = value->iValue;
            here->FEFET1geoModGiven = TRUE;
            break;
        case FEFET1_RGEOMOD:
            here->FEFET1rgeoMod = value->iValue;
            here->FEFET1rgeoModGiven = TRUE;
            break;
        case FEFET1_IC_VDS:
            here->FEFET1icVDS = value->rValue;
            here->FEFET1icVDSGiven = TRUE;
            break;
        case FEFET1_IC_VGS:
            here->FEFET1icVGS = value->rValue;
            here->FEFET1icVGSGiven = TRUE;
            break;
        case FEFET1_IC_VBS:
            here->FEFET1icVBS = value->rValue;
            here->FEFET1icVBSGiven = TRUE;
            break;
        case FEFET1_IC:
            /* FALLTHROUGH added to suppress GCC warning due to
             * -Wimplicit-fallthrough flag */
            switch (value->v.numValue) {
                case 3:
                    here->FEFET1icVBS = *(value->v.vec.rVec+2);
                    here->FEFET1icVBSGiven = TRUE;
                    /* FALLTHROUGH */
                case 2:
                    here->FEFET1icVGS = *(value->v.vec.rVec+1);
                    here->FEFET1icVGSGiven = TRUE;
                    /* FALLTHROUGH */
                case 1:
                    here->FEFET1icVDS = *(value->v.vec.rVec);
                    here->FEFET1icVDSGiven = TRUE;
                    break;
                default:
                    return(E_BADPARM);
            }
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
