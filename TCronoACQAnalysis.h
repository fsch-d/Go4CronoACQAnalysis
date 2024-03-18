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

#ifndef TCronoACQAnalysis_H
#define TCronoACQAnalysis_H

#include "TGo4Analysis.h"

class TH1;
//lass TGo4MbsEvent;
class TGo4WinCond;
class TCronoACQRawEvent;
class TCronoACQUnpackEvent;
class TCronoACQAnlEvent;
class TCronoACQParameter;

class TCronoACQAnalysis : public TGo4Analysis  {
   public:
      TCronoACQAnalysis();
      TCronoACQAnalysis(int argc, char **argv);
      virtual ~TCronoACQAnalysis();
      Int_t UserPreLoop() override;
      Int_t UserEventFunc() override;
      Int_t UserPostLoop() override;
   private:
      TCronoACQRawEvent       *fSourceEvent{nullptr};
      TCronoACQUnpackEvent    *fRawEvent{nullptr};
      TCronoACQAnlEvent       *fCalEvent{nullptr};
      TH1                *fSize{nullptr};
      Int_t               fEvents{0};
      Int_t               fLastEvent{0};

   ClassDefOverride(TCronoACQAnalysis,1)
};

#endif
