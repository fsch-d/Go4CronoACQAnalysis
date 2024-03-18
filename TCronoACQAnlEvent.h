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

#ifndef TCronoACQAnlEvent_H
#define TCronoACQAnlEvent_H

#include "TGo4EventElement.h"
#include "TCronoACQUnpackEvent.h"

class TCronoACQAnlEvent : public TGo4EventElement {
   public:
      TCronoACQAnlEvent() : TGo4EventElement() {}
      TCronoACQAnlEvent(const char *name) : TGo4EventElement(name) {}
      virtual ~TCronoACQAnlEvent() {}

      void Clear(Option_t *opt = "") override;

      Float_t frData[CronoACQ_NUM_CHAN];

   ClassDefOverride(TCronoACQAnlEvent,1)
};

#endif
