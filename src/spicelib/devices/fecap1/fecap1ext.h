/**********
Copyright 2024 Xidian University.	All rights reserved.
 Author: Bo Li	at Hangzhou Institute of Technology	 
 Modified: 2020/09/09  Bo Li
 Refered to NgSPICE Res/Cap related file
**********/

extern int FECAP1setup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int FECAP1load(GENmodel*,CKTcircuit*);
extern int FECAP1acLoad(GENmodel*,CKTcircuit*);

extern int FECAP1mAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int FECAP1ask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int FECAP1getic(GENmodel*,CKTcircuit*);
extern int FECAP1mParam(int,IFvalue*,GENmodel*);
extern int FECAP1param(int,IFvalue*,GENinstance*,IFvalue*);


extern int FECAP1temp(GENmodel*,CKTcircuit*);
extern int FECAP1trunc(GENmodel*,CKTcircuit*,double*);
extern int FECAP1findBr(CKTcircuit*, GENmodel*, IFuid);
extern int FECAP1getic(GENmodel*, CKTcircuit*);
extern int FECAP1convTest(GENmodel*, CKTcircuit*);


/*
extern int CAPpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int CAPsAcLoad(GENmodel*,CKTcircuit*);
extern int CAPsLoad(GENmodel*,CKTcircuit*);
extern void CAPsPrint(GENmodel*,CKTcircuit*);
extern int CAPsSetup(SENstruct *,GENmodel*);
extern int CAPsUpdate(GENmodel*,CKTcircuit*);
extern int CAPsoaCheck(CKTcircuit *, GENmodel *);
*/