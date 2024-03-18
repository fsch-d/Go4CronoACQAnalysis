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

#ifndef TCronoACQAnlProc_H
#define TCronoACQAnlProc_H

#include "TGo4EventProcessor.h"

#include "TH3.h"
#include "TVector2.h"
#include "TVector3.h"



#include "TCronoACQUnpackEvent.h"

//class TCronoACQParameter;
class TCronoACQRawPar;
class TCronoACQCorrPar;
class TCronoACQMomCalcPar;
class TCronoACQMomCorrPar;
class TCronoACQAnlEvent;

class TCronoACQAnlProc : public TGo4EventProcessor {
   public:
      TCronoACQAnlProc();
      TCronoACQAnlProc(const char *name);
      virtual ~TCronoACQAnlProc();

      Bool_t BuildEvent(TGo4EventElement *dest) override;


   private:
	  Double_t CalculateMomentum(TVector3 r, TVector3 &p, Double_t Toff, Double_t Tnode, Int_t charge, Double_t mass);
	  Double_t CalculateRad(Double_t T, Double_t Ee, Double_t Toff, Double_t Tnode, Int_t charge, Double_t mass);
	  Double_t derivT(TVector2 re, TVector3 CorrR);
	  Bool_t ElectronNodeCut(Double_t pz, Double_t phase, Double_t Te);
	  Bool_t OptSourcePos(TVector3 &re, TVector3 &rr);
	  TVector2 DeltaCorrElectrons(Double_t Te);
	  Int_t event;

  	  TCronoACQRawPar *fRawParr;
	  TCronoACQRawPar *fRawPare;
     TCronoACQCorrPar *fCorrParr;
	  TCronoACQCorrPar *fCorrPare;
	  TCronoACQMomCalcPar *fMomCalcParam;
     TCronoACQMomCorrPar *fMomCorrPare;
     TCronoACQMomCorrPar *fMomCorrParr;
	  
	  //raw spectra
	  TH1       *fTdc[CronoACQ_NUM_CHAN][3];
	  TH1		   *ftsumxr[3];
	  TH1		   *ftsumyr[3];
	  TH1		   *ftsumxe[3];
	  TH1		   *ftsumye[3];
	  TH1		   *fPate[3];
	  TH1		   *fPatr[3];


	  TH1          *fTime;
	  TH1          *fTimeHD;


	  //coordinates
	  TH2 *fXYr;
	  TH2 *fXTsumr;
	  TH2 *fYTsumr;
	  TH1 *fTr;
	  TH1 *fTr_xycond;
	  TH1 *fTr_NOTxycond;
	  TH2 *fTXr;
	  TH2 *fTYr;
	  TH2 *fPipico;

	  TH2 *fXYe;
	  TH2 *fXTsume;
	  TH2 *fYTsume;
	  TH1 *fTe;
	  TH1 *fTe2;
	  TH2 *fTXe;
	  TH2 *fTYe;

	  TH1 *fTer;
	  TH1 *fTer_xycond;
	  TH2 *fTeTr;


	  // corrected coordinates
	  TH1 *fCorrXr;
	  TH1 *fCorrYr;
	  TH2 *fCorrXYr;
	  TH2* fCorrPhirRr;
	  TH2 *fCorrTXr;
	  TH2 *fCorrTYr;
	  TH2 *fCorrTRr;
	  TH1 *fexpTRr;

	  TH1 *fCorrXe;
	  TH1 *fCorrYe;
	  TH2 *fCorrXYe;
	  TH2* fCorrPhieRe;
	  TH2 *fCorrTXe;
	  TH2 *fCorrTYe;
	  TH2 *fCorrTRe;
	  TH2 *fCorrTderivRe;
	  TH1 *fexpTRe;
	  TH3 *fCorrTXYe;

	  TH2 *fCorrTeTr;
	  TH2 *fCorrDeltaphiTe;
	  TH3 *fCorrTRePhir;

	  TH2 *fTcotT;
	  TH2 *fTderivT;
	  TH1 *fderivTmT;
	  TH2 *fderivTmTXe;
	  TH2 *fderivTmTYe;
	  TH2 *ftheoPhiPperp;


	//momenta
	  TH2 *fAllPXPerpr;
	  TH2 *fAllPYPerpr;
	  TH2 *fAllPZPerpr;
	  TH2 *fAllPXYr;
	  TH2 *fAllPXZr;
	  TH2 *fAllPYZr;
	  TH2 *fAllPhirPPerpr;

	  TH2 *fAllPXPerpe;
	  TH2 *fAllPYPerpe;
	  TH2 *fAllPZPerpe;
	  TH2 *fAllPXYe;
	  TH2 *fAllPXZe;
	  TH2 *fAllPYZe;
	  TH1 *fAllEe;
	  TH2* fAllEeTmT;
	  TH2* fAllEeTmTzoom;
	  TH2* fAllPZeTmT;
	  TH2* fAllEePhie;
	  TH2* fAllThetaeTmT;
	  TH2 *fAllOmegae;
	  TH2 *fAllOmegae_lr;

	  TH2 *fPXer;
	  TH2 *fPYer;
	  TH2 *fPZer;
	  TH2 *fPRer;
	  TH2* fPhiePhir;
	  TH1* fPhier;
	  TH1 *fcTime;
	  TH1 *fcTimeHD;
	  TH2 *fEecTime;
	  TH2 *fEecTimeHD;

	  TH1* fDRMOD;
	  TH1* fDRMODvT[10];

	  TH3 *fAllPe;


	  TH2 *fSPXPerpr;
	  TH2 *fSPYPerpr;
	  TH2 *fSPZPerpr;

	  TH2 *fSPXPerpe;
	  TH2 *fSPYPerpe;
	  TH2 *fSPZPerpe;
	  TH2 *fSPXYe;
	  TH2 *fSPXZe;
	  TH2 *fSPYZe;
	  TH1 *fSEe;
	  TH2 *fSEeTmT;
	  TH2 *fSEePhie;
	  TH2 *fSOmegae;
	  TH2 *fSOmegae_lr;


	  TH2 *fPPXPerpr;
	  TH2 *fPPYPerpr;
	  TH2 *fPPZPerpr;

	  TH2 *fPPXPerpe;
	  TH2 *fPPYPerpe;
	  TH2 *fPPZPerpe;
	  TH2 *fPPXYe;
	  TH2 *fPPXZe;
	  TH2 *fPPYZe;
	  TH1 *fPEe;
	  TH2 *fPEeTmT;
	  TH2 *fPEePhie;
	  TH2 *fPOmegae;
	  TH2 *fPOmegae_lr;

	  //optimization

	  TH2 *fOptXYoff;


	  TGo4WinCond  *fcontsumxr;
	  TGo4WinCond  *fcontsumyr;
	  TGo4WinCond  *fcontsumxe;
	  TGo4WinCond  *fcontsumye;
	  TGo4WinCond  *fcontS;
	  TGo4WinCond  *fconter;
	  TGo4WinCond  *fconte;
	  TGo4WinCond  *fcontderive;
	  TGo4WinCond  *fcontderiv;
	  TGo4WinCond  *fconEe;
	  TGo4WinCond  *fcontime;
	  TGo4WinCond  *fconRe; 
	  TGo4ShapedCond *fconxye;
	  TGo4ShapedCond *fconxyr;

	  TGo4PolyCond *fconpxer;
	  TGo4PolyCond *fconpyer;
	  TGo4PolyCond *fconpzer;

	  TGo4WinCond* fcontDRModMinima;

	  //

	  Long64_t	cycleStart;
	  

   ClassDefOverride(TCronoACQAnlProc, 1)
};

#endif
