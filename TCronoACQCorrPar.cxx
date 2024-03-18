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

#include "TCronoACQCorrPar.h"
#include "TGo4Log.h"

TCronoACQCorrPar::TCronoACQCorrPar(const char *name) :
   TGo4Parameter(name)
{
  scalex=-9999;
  scaley=-9999;
  x0=-9999;
  y0=-9999;
  rot_phi=-9999;
  node_n = 0;
  for (Int_t i = 0; i < 10; i++)
  {
	  t[i] = -9999;
	  x[i] = -9999;
	  y[i] = -9999;
  }
  Toff = -9999;
  T0 = -9999;
}
