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

#ifndef TCronoACQUnpackEvent_H
#define TCronoACQUnpackEvent_H

#define CronoACQ_NUM_CHAN 20

#include "TGo4EventElement.h"

class TCronoACQUnpackEvent : public TGo4EventElement {
   public:
      TCronoACQUnpackEvent() : TGo4EventElement() {}
      TCronoACQUnpackEvent(const char *name) : TGo4EventElement(name) {}
      virtual ~TCronoACQUnpackEvent() {}

      /**
       * Method called by the event owner (analysis step) to clear the
       * event element.
       */
      void Clear(Option_t *opt = "") override;

	  Long64_t tdc[CronoACQ_NUM_CHAN][3];
	  Long64_t groupTime;

   ClassDefOverride(TCronoACQUnpackEvent,1)
};

#endif
