// $Id$
//-----------------------------------------------------------------------
//       The GSI Online Offline Object Oriented (Go4) Project
//         Experiment Data Processing at EE department, GSI
//-----------------------------------------------------------------------
// Copyright (C) 2000- GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
//                     Planckstr. 1, 64291 Darmstadt, Germany
// Contact:            http://go4.gsi.de
//-----------------------------------------------------------------------
// This software can be used under the license agreements as stated
// in Go4License.txt file which is part of the distribution.
//-----------------------------------------------------------------------

#include "TCronoACQMomCalcPar.h"
#include "TGo4Log.h"

TCronoACQMomCalcPar::TCronoACQMomCalcPar(const char *name) :
   TGo4Parameter(name)
{
  Tcyclotron=-9999;
  Mrec = -9999;
  cutfrac = -9999;
  opt = kFALSE;
  weight = -9999;
  Tcycl_alt = -9999;
  Toff_alt = -9999;
  T0_alt = -9999;
  Tnode_alt = -9999;
  s_weight = 0;
  bderivT = kFALSE;
  expE = 0;
}
