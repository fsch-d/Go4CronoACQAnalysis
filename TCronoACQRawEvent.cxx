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

#include "TCronoACQRawEvent.h"

#include <iostream>

#include "TGo4Log.h"

TCronoACQRawEvent::TCronoACQRawEvent() :
   TGo4EventElement()
{
   // default ctor is for streamer only, avoid heap objects here!
}

TCronoACQRawEvent::TCronoACQRawEvent(const char *name) :
   TGo4EventElement(name)
{
}

TCronoACQRawEvent::~TCronoACQRawEvent()
{
}

void TCronoACQRawEvent::Clear(Option_t *)
{
   groupTime = 0;      
	memset(fiTDC, 0, sizeof(fiTDC)) ;
	event_count = 0;
}

