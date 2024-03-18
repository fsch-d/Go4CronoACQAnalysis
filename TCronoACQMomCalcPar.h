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

#ifndef TCronoACQMomCalcPar_H
#define TCronoACQMomCalcPar_H

#include "TGo4Parameter.h"


class TCronoACQMomCalcPar : public TGo4Parameter {
   public:
      TCronoACQMomCalcPar(const char *name = nullptr);
      virtual ~TCronoACQMomCalcPar() {}


  	  Double_t Tcyclotron;  	/*Cyclotron time for an electron*/
	  Double_t Mrec;  	/*Mass of the Ion*/
	  Double_t cutfrac;  	/*fraction of electron node cut*/
	  Bool_t opt; // Apply position optimization algorithm
	  Double_t weight;  	/*wheighing factor for electron node cutout*/
	  Double_t Tcycl_alt;  	/*Cyclotron period for alternative spectrometer settings*/
	  Double_t Toff_alt;  	/*Electron ToF offset for alternative spectrometer settings*/
	  Double_t T0_alt;  	/*Electron measured ToF for zero energy and alternative spectrometer settings*/
	  Double_t Tnode_alt;  	/*Node time for alternative spectrometer settings*/

	  Double_t s_weight;  	/*wheighing factor for s-state*/

	  Bool_t bderivT; /* Apply time derivation algorithm (if Photodiode signal is missing or wrong)*/
	  Double_t expE; /* expected electron energy for time derivation algorithm */

   ClassDefOverride(TCronoACQMomCalcPar,2)
};

#endif
