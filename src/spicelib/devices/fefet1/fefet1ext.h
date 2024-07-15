/**********
Copyright 2024 Xidian University.	All rights reserved.
 Author: Bo Li	at Hangzhou Institute of Technology	 
 Modified: 2020/09/09  Bo Li
 Refered to NgSPICE Res/Cap related file
**********/


extern int FEFET1acLoad(GENmodel *,CKTcircuit*);
extern int FEFET1ask(CKTcircuit *,GENinstance*,int,IFvalue*,IFvalue*);
extern int FEFET1convTest(GENmodel *,CKTcircuit*);
extern int FEFET1getic(GENmodel*,CKTcircuit*);
extern int FEFET1load(GENmodel*,CKTcircuit*);
extern int FEFET1mAsk(CKTcircuit*,GENmodel *,int, IFvalue*);
extern int FEFET1mDelete(GENmodel*);
extern int FEFET1mParam(int,IFvalue*,GENmodel*);
extern void FEFET1mosCap(CKTcircuit*, double, double, double, double,
        double, double, double, double, double, double, double,
        double, double, double, double, double, double, double*,
        double*, double*, double*, double*, double*, double*, double*,
        double*, double*, double*, double*, double*, double*, double*, 
        double*);
extern int FEFET1param(int,IFvalue*,GENinstance*,IFvalue*);
extern int FEFET1pzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int FEFET1setup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int FEFET1temp(GENmodel*,CKTcircuit*);
extern int FEFET1trunc(GENmodel*,CKTcircuit*,double*);
extern int FEFET1noise(int,int,GENmodel*,CKTcircuit*,Ndata*,double*);
extern int FEFET1unsetup(GENmodel*,CKTcircuit*);
extern int FEFET1soaCheck(CKTcircuit *, GENmodel *);
