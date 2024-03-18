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

#include "TCronoACQMomCorrPar.h"
#include "TGo4Log.h"

TCronoACQMomCorrPar::TCronoACQMomCorrPar(const char *name) :
   TGo4Parameter(name)
{
  cPX = 1;
  cPY = 1;
  cPZ = 1;
  PXoff = 0;
  PYoff = 0;
  PZoff = 0;
  PRotateZ = 0;
  PRotateY = 0;
  PRotateX = 0;
}
