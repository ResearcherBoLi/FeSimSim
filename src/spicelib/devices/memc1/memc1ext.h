/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/
/**********
Comment by Bo Li
this header file is called by memdefs.h, modified by resext.h
need further modify the parser interface 
---20191024 

**********/
/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/


extern int MEMC1ask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int MEMC1load(GENmodel*,CKTcircuit*);
//extern int RESacload(GENmodel*,CKTcircuit*);
extern int MEMC1modAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int MEMC1mParam(int,IFvalue*,GENmodel*);
extern int MEMC1param(int,IFvalue*,GENinstance*,IFvalue*);
//extern int RESpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
//extern int RESsAcLoad(GENmodel*,CKTcircuit*);
//extern int MEMsLoad(GENmodel*,CKTcircuit*);
//extern int MEMsSetup(SENstruct*,GENmodel*);
//extern void MEMsPrint(GENmodel*,CKTcircuit*);
extern int MEMC1setup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int MEMC1temp(GENmodel*,CKTcircuit*);
extern int MEMC1trunc(GENmodel*, CKTcircuit*, double *);
//extern int MEMnoise(int,int,GENmodel*,CKTcircuit*,Ndata*,double*);
//extern int MEMsoaCheck(CKTcircuit *, GENmodel *);
extern int MEMC1findBr(CKTcircuit*, GENmodel*, IFuid);
extern int MEMC1getic(GENmodel*, CKTcircuit*);
extern int MEMC1convTest(GENmodel *, CKTcircuit *);