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

#ifndef TCronoACQMomCorrPar_H
#define TCronoACQMomCorrPar_H

#include "TGo4Parameter.h"


class TCronoACQMomCorrPar : public TGo4Parameter {
   public:
      TCronoACQMomCorrPar(const char *name = nullptr);
      virtual ~TCronoACQMomCorrPar() {}


  	  Double_t cPX;  	/*scale x momentum*/
	  Double_t cPY;  	/*scale y momentum*/
	  Double_t cPZ; 	/*scale z momentum*/
	  Double_t PXoff; 	/*offset x momentum*/
	  Double_t PYoff;	/*offset y momentum*/
	  Double_t PZoff;	/*offset z momentum*/
	  Double_t PRotateZ;	/*rotate momentum around z axis*/
	  Double_t PRotateX;	/*rotate momentum around x axis*/
	  Double_t PRotateY;	/*rotate momentum around y axis*/

   ClassDefOverride(TCronoACQMomCorrPar,2)
};

#endif
