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

#include "TCronoACQUnpackProc.h"

#include "TH1.h"
#include "TMath.h"
#include "TCanvas.h"

#include "TGo4Log.h"
#include "TCronoACQRawEvent.h"
#include "TGo4WinCond.h"

#include "TCronoACQUnpackEvent.h"

//-----------------------------------------------------------
TCronoACQUnpackProc::TCronoACQUnpackProc() :
   TGo4EventProcessor()
{
}

//-----------------------------------------------------------
TCronoACQUnpackProc::TCronoACQUnpackProc(const char *name) :
   TGo4EventProcessor(name)
{
   TGo4Log::Info("TCronoACQUnpackProc: Create");

   //// init user analysis objects:
   // create and load parameter here
   // set_Par.C macro executed after parameter load from auto-save file
   //fParam = (TCronoACQParameter*) MakeParameter("CronoACQParameter", "TCronoACQParameter", "set_Par.C");

	Text_t chis[64];
	Text_t chead[64];
	Text_t ccon[64];
	Int_t j;


	for (j = 0; j < CronoACQ_NUM_CHAN; j++)
	{
		sprintf(chis, "Unpack/TDC%1d", j);  sprintf(chead, "TDC channel %1d", j);
		fTdc[j] = MakeTH1('I', chis, chead, 10000, -200000000, 200000000);
		sprintf(ccon, "Unpack/TDC%1dWinCond", j);  sprintf(chis, "TDC%1d", j);
		fTDCWinCond[j] = MakeWinCond(ccon, -100000000, 1000000, chis);//0.04,0.06
		fTDCWinCond[j]->Disable(1);
	}
	fNSignals = MakeTH1('I', "Unpack/NSignals", "Number of Signals per event", 200, 1, 200);

   //ExecuteLine("$blabla.py"); // JAM test internal python call
   //ExecutePython("blabla.py");
}

//-----------------------------------------------------------
TCronoACQUnpackProc::~TCronoACQUnpackProc()
{
   TGo4Log::Info("TCronoACQUnpackProc: Delete");
}

//-----------------------------------------------------------
Bool_t TCronoACQUnpackProc::BuildEvent(TGo4EventElement *dest)
{
   Bool_t isValid = kFALSE; // validity of output event

   TCronoACQRawEvent *inp_evt = (TCronoACQRawEvent *) GetInputEvent(); // from this
   TCronoACQUnpackEvent* out_evt = (TCronoACQUnpackEvent*) dest;

   if (!inp_evt) {
      TGo4Log::Error("CronoACQUnpackProc: no input event !");
      out_evt->SetValid(isValid); // to store or not to store
      // default calling Fill method will set validity of out_evt to return value!
      return isValid;
   }
   isValid = kTRUE;

   Int_t hit[CronoACQ_NUM_CHAN];

   for (Int_t i = 0; i < CronoACQ_NUM_CHAN; i++) {
	   hit[i] = 0;
	   for (Int_t j = 0; j < 3; j++) out_evt->tdc[i][j] = LONG_MIN;
   }


   out_evt->groupTime = inp_evt->groupTime;
   for(Int_t i=0; i<inp_evt->event_count; i++)
	{
	   Int_t channel = inp_evt->fiTDC[i].channel;
	   Long64_t value = inp_evt->fiTDC[i].value;

      if ( channel > CronoACQ_NUM_CHAN - 1 ) continue;
	   fTdc[channel]->Fill(value); //time in ps

	   if (fTDCWinCond[channel]->Test(value) && hit[channel] < 3)
	   {
		   out_evt->tdc[channel][hit[channel]] = value;
	   }
	   hit[channel]++;
   }
   fNSignals->Fill(inp_evt->event_count);



   out_evt->SetValid(isValid); // to store or not to store
   // default calling Fill method will set validity of out_evt to return value!

   return isValid; // this will overwrite out_evt->SetValid
   // except one would have a Fill method implemented in the output event class!
   // In this case the return value can be handled differently there.
}
