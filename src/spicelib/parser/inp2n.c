/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Comment: INP2N is totally changed compared to NgSpice original ones
**********/

/**********
The N type device is redefined as Memristor or RRAM type, 
codes are refered to INP2r.c/INP2c.c/INP2v.c/INP2m.c
**********/
#include "ngspice/ngspice.h"
#include "ngspice/ifsim.h"
#include "ngspice/inpdefs.h"
#include "ngspice/inpmacs.h"
#include "ngspice/fteext.h"
#include "inpxx.h"
#include "ngspice/stringskip.h"

/* undefine to add tracing to this file */
/* #define TRACE */

void INP2N(CKTcircuit *ckt, INPtables * tab, struct card *current)
{
/* parse a memristor card */
/* Nname <node> <node> [<mname>] [<initValue>]  
// refer to below definition
/* Rname <node> <node> [<val>][<mname>][w=<val>][l=<val>][ac=<val>] */

    static int mytype = -1;	/* the type we determine resistors are */
    int type = 0;		/* the type the model says it is */
    char *line;			/* the part of the current line left to parse */
    char *saveline;		/* ... just in case we need to go back... */
    char *name;			/* the memristor's name */
    char *model;		/* the name of the memsistor's model */
    char *nname1;		/* the first node's name */
    char *nname2;		/* the second node's name */
    CKTnode *node1;		/* the first node's node pointer */
    CKTnode *node2;		/* the second node's node pointer */
    double val;			/* temp to held mem-resistance */
    int error;			/* error code temporary */
    int error1;			/* secondary error code temporary */
    INPmodel *thismodel;	/* pointer to model structure describing our model */
    GENmodel *mdfast = NULL;	/* pointer to the actual model */
    GENinstance *fast;		/* pointer to the actual instance */
    IFvalue ptemp;		/* a value structure to package mem-resistance into */
    int waslead;		/* flag to indicate that funny unlabeled number was found */
    double leadval;		/* actual value of unlabeled number */
    IFuid uid;			/* uid for default model */

    char *s;   /* Temporary buffer and pointer for translation */

#ifdef TRACE
    printf("In INP2N, Current line: %s\n", current->line);
#endif



	/*
    if (mytype < 0) {
        if ((mytype = INPtypelook("Memristor")) < 0) {

		
            LITERR("Device type Memristor not supported by this binary\n");
            return;
        }
    }

    INPgetNetTok(&line, &model, 1);
	INPinsert(&model, tab);
	current->error = INPgetMod(ckt, model, &thismodel, tab);

    if (thismodel->INPmodType != INPtypelook("HP") &&
        thismodel->INPmodType != INPtypelook("MEM1") ){
	    LITERR ("incorrect model type");
        return;        	
        
    }
    */
   /*
	mytype = INPtypelook("MEMR1");
    if (mytype < 0) {
		LITERR("Device type memristor not supported by this binary\n");
		return;
    }*/	
    line = current->line;
    INPgetNetTok(&line, &name, 1);			/* Nname */
    INPinsert(&name, tab);
    
    INPgetNetTok(&line, &nname1, 1);		/* <node> */
    INPtermInsert(ckt, &nname1, tab, &node1);
    INPgetNetTok(&line, &nname2, 1);		/* <node> */
    INPtermInsert(ckt, &nname2, tab, &node2);

    saveline = line;		/* save then old pointer */

	INPgetNetTok(&line, &model, 1);
    INPinsert(&model, tab);
    current->error = INPgetMod(ckt, model, &thismodel, tab);


	if (thismodel != NULL) {
		if ( thismodel->INPmodType != INPtypelook("MEMR1") &&
		   thismodel->INPmodType != INPtypelook("MEMR2") &&
		   thismodel->INPmodType != INPtypelook("MEMR3") &&
		   thismodel->INPmodType != INPtypelook("MEMC1") &&
		   thismodel->INPmodType != INPtypelook("MEMI1") &&
           thismodel->INPmodType != INPtypelook("FECAP1") &&
           thismodel->INPmodType != INPtypelook("FECAP2") &&           
           thismodel->INPmodType != INPtypelook("FECAP3") 
		   ){
			LITERR ("incorrect model type");
			return;		  	
		}
		mdfast = thismodel->INPmodfast;
 		type = thismodel->INPmodType; 
	}else{
		tfree(model);
        /* It is not a model */
        line = saveline;	/* go back */
		type = mytype;
		if (!tab->defNmod) {
	    /* create default D model */
	    	IFnewUid(ckt, &uid, NULL, "N", UID_MODEL, NULL);
	    	IFC(newModel, (ckt, type, &(tab->defNmod), uid));

		}
		mdfast = tab->defNmod;

	}

	IFC(newInstance, (ckt, mdfast, &fast, name));
    IFC(bindNode, (ckt, fast, 1, node1));
    IFC(bindNode, (ckt, fast, 2, node2));
    PARSECALL((&line, ckt, type, fast, &leadval, &waslead, tab));
    if (waslead) {
      ptemp.rValue = leadval;
      GCA(INPpName, ("mem-resistance", &ptemp, ckt, type, fast));
    }

    return;
}

