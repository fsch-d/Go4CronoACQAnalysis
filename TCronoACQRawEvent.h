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

#ifndef TCronoACQRawEvent_H
#define TCronoACQRawEvent_H

#include "TGo4EventElement.h"

#define NUM_SIG_MAX 600



typedef struct {
	UInt_t channel;
	Long64_t value;
} tdcRawEvent ;


/**
 * Example for user defined raw event class.
 * This event structure corresponds to the user event source.
 */
class TCronoACQRawEvent : public TGo4EventElement {

   public:

      TCronoACQRawEvent();

      TCronoACQRawEvent(const char *name);

      virtual ~TCronoACQRawEvent();

      /**
        * Method called by the event owner (analysis step) to clear the
        * event element.
        */
      void Clear(Option_t *opt = "") override;



      Long64_t groupTime;      
	   tdcRawEvent fiTDC[NUM_SIG_MAX];
	   Int_t event_count;

   ClassDefOverride(TCronoACQRawEvent,1)
};

#endif
