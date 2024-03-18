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

#ifndef TCronoACQRawPar_H
#define TCronoACQRawPar_H

#include "TGo4Parameter.h"


class TCronoACQRawPar : public TGo4Parameter {
   public:
      TCronoACQRawPar(const char *name = nullptr);
      virtual ~TCronoACQRawPar() {}


  	   Double_t scalex;  	/*Skalierungsfaktor fuer Detektorgroesse in x-Richtung*/
      Double_t scaley;  	/*Skalierungsfaktor fuer Detektorgroesse in y-Richtung*/
	   Double_t shiftx;  	/*Shift des Detektors in x-Richtung*/ 
	   Double_t shifty;  	/*Shift des Detektors in y-Richtung*/ 
	   Double_t x0; 		/*x-Koordinate Detektormittelpunkt*/
	   Double_t y0; 		/*y-Koordinate Detektormittelpunkt*/
	   Double_t rot_phi; 	/*Drehwinkel um Detektormittelpunkt*/

   ClassDefOverride(TCronoACQRawPar,2)
};

#endif
