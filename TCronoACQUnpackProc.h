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

#ifndef TCronoACQUnpackProc_H
#define TCronoACQUnpackProc_H

#include "TGo4EventProcessor.h"

class TCronoACQUnpackEvent;

#include "TCronoACQUnpackEvent.h"

class TCronoACQUnpackProc : public TGo4EventProcessor {
   public:
      TCronoACQUnpackProc();
      TCronoACQUnpackProc(const char *name);
      virtual ~TCronoACQUnpackProc();

      Bool_t BuildEvent(TGo4EventElement *dest) override;

   protected:
	   TH1   *fTdc[CronoACQ_NUM_CHAN];
	   TH1	*fNSignals;

	   TGo4WinCond  *fTDCWinCond[CronoACQ_NUM_CHAN];

   ClassDefOverride(TCronoACQUnpackProc,1)
};

#endif
