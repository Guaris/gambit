//
// FILE: egobit.h -- Interface to extensive form Gobit solution module
//
// $Id$
//

#ifndef EGOBIT_H
#define EGOBIT_H

#include "gstream.h"
#include "gstatus.h"
#include "glist.h"

#include "efg.h"
#include "behavsol.h"

class EFQreParams   {
  public:
    int trace, powLam, maxits1, maxitsN;
    double minLam, maxLam, delLam, tol1, tolN;
    bool fullGraph;
    gOutput *tracefile, *pxifile;
    gStatus &status;

    EFQreParams(gStatus & = gstatus);
    EFQreParams(gOutput &out, gOutput &pxi, gStatus & = gstatus);
};


void Qre(const Efg &, EFQreParams &,
	   const BehavProfile<gNumber> &, gList<BehavSolution > &,
	   long &nevals, long &nits);

void KQre(const Efg &E, EFQreParams &params,
	    const BehavProfile<gNumber> &start,
	    gList<BehavSolution> &solutions, 
	    long &nevals, long &nits);

#endif    // NGOBIT_H



