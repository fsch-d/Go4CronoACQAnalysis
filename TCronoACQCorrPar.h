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

#ifndef TCronoACQCorrPar_H
#define TCronoACQCorrPar_H

#include "TGo4Parameter.h"


class TCronoACQCorrPar : public TGo4Parameter {
   public:
      TCronoACQCorrPar(const char *name = nullptr);
      virtual ~TCronoACQCorrPar() {}


  	  Double_t scalex;  	/*Skalierungsfaktor fuer Detektorgroesse in x-Richtung*/
      Double_t scaley;  	/*Skalierungsfaktor fuer Detektorgroesse in y-Richtung*/
	  Double_t x0; 		/*x-Koordinate Detektormittelpunkt*/
	  Double_t y0; 		/*y-Koordinate Detektormittelpunkt*/
	  Int_t node_n;		/*number of nodes*/
	  Double_t x[10];	/*wiggle x positions*/
	  Double_t y[10];	/*wiggle y positions*/
	  Double_t t[10];	/*wiggle times*/
	  Double_t rot_phi; 	/*Drehwinkel um Detektormittelpunkt*/
	  Double_t Toff;
	  Double_t T0;

   ClassDefOverride(TCronoACQCorrPar,2)
};

#endif
