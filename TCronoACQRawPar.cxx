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

#include "TCronoACQRawPar.h"
#include "TGo4Log.h"

TCronoACQRawPar::TCronoACQRawPar(const char *name) :
   TGo4Parameter(name)
{
    scalex=1;
    scaley=1;
    shiftx=0;
    shifty=0;
    x0=0;
    y0=0;
    rot_phi=0; 
}
