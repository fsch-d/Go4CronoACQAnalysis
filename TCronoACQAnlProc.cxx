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

#include "TCronoACQAnlProc.h"

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TRandom2.h"

#include "TGo4WinCond.h"
#include "TGo4PolyCond.h"
#include "TGo4ShapedCond.h"
#include "TGo4Analysis.h"

#include "TCronoACQAnlEvent.h"
#include "TCronoACQUnpackEvent.h"
#include "TCronoACQRawPar.h"
#include "TCronoACQCorrPar.h"
#include "TCronoACQMomCalcPar.h"
#include "TCronoACQMomCorrPar.h"

//-----------------------------------------------------------
TCronoACQAnlProc::TCronoACQAnlProc() :
   TGo4EventProcessor()
{
}

//-----------------------------------------------------------
TCronoACQAnlProc::TCronoACQAnlProc(const char *name) :
   TGo4EventProcessor(name)
{
   TGo4Log::Info("TCronoACQAnlProc: Create");
   //// init user analysis objects:

   // here already exisitng parameter will be returned
   // one not need to specify macro here - it is already executed in first step
	Text_t chis[64];
	Text_t chead[64];
	Int_t i, j;

	TH1::SetDefaultSumw2(kTRUE); //calculate proper statistical error for histograms

//raw spectra

	for (j = 0; j < 3; j++)
	{

		for (i = 0; i < CronoACQ_NUM_CHAN; i++)
		{
			sprintf(chis, "raw/Hit%1d/TDC%1d_%1d", j, i, j);  sprintf(chead, "TDC channel %1d Hit %1d in mus", i, j);
			fTdc[i][j] = MakeTH1('D', chis, chead, 10000, -200, 200);

		}

		sprintf(chis, "raw/Hit%1d/tsumxr%1d", j, j); sprintf(chead, "time sum of TDC x channels Hit %1d", j);
		ftsumxr[j] = MakeTH1('I', chis, chead, 2000, -0.5, 0.5);

		sprintf(chis, "raw/Hit%1d/tsumyr%1d", j, j); sprintf(chead, "time sum of TDC y channels Hit %1d", j);
		ftsumyr[j] = MakeTH1('I', chis, chead, 2000, -0.5, 0.5);

		sprintf(chis, "raw/Hit%1d/pat_r%1d", j, j); sprintf(chead, "recoil ion pattern Hit %1d", j);
		fPatr[j] = MakeTH1('I', chis, chead, 16, 0, 16);

		sprintf(chis, "raw/Hit%1d/tsumxe%1d", j, j); sprintf(chead, "time sum of TDC x channels Hit %1d", j);
		ftsumxe[j] = MakeTH1('I', chis, chead, 2000, -0.5, 0.5);

		sprintf(chis, "raw/Hit%1d/tsumye%1d", j, j); sprintf(chead, "time sum of TDC y channels Hit %1d", j);
		ftsumye[j] = MakeTH1('I', chis, chead, 2000, -0.5, 0.5);

		sprintf(chis, "raw/Hit%1d/pat_e%1d", j, j); sprintf(chead, "electron pattern Hit %1d", j);
		fPate[j] = MakeTH1('I', chis, chead, 16, 0, 16);
	}

	fTime = MakeTH1('I', "raw/groupTime", "Time in mus", 1000, 0, 10000);
	fTimeHD = MakeTH1('I', "raw/groupTimeHD", "Time in mus", 5000, -5000, 5000);



//coordinates
	fXYr = MakeTH2('D', "coordinates/recoil/XYr", "recoil ion position", 500, -50, 50, 500, -50, 50);
	fXTsumr = MakeTH2('D', "coordinates/recoil/XTsumr", "recoil ion DLA wire sum vs. X", 500, -50, 50, 500, 0, 0.1);
	fYTsumr = MakeTH2('D', "coordinates/recoil/YTsumr", "recoil ion DLA wire sum vs. Y", 500, -50, 50, 500, 0, 0.1);
	fTr = MakeTH1('D', "coordinates/recoil/Tr", "recoil ion time-of-flight", 100000, 0, 100);
	fTr_xycond = MakeTH1('D', "coordinates/recoil/Tr_xycond", "recoil ion time-of-flight", 10000, 0, 100);
	fTr_NOTxycond = MakeTH1('D', "coordinates/recoil/Tr_NOTxycond", "recoil ion time-of-flight", 10000, 0, 100);
	fTXr = MakeTH2('D', "coordinates/recoil/TXr", "recoil ion ToF vs. X", 500, -.2, .8, 250, -50, 50);
	fTYr = MakeTH2('D', "coordinates/recoil/TYr", "recoil ion ToF vs. Y", 500, -.2, .8, 250, -50, 50);

	fXYe = MakeTH2('D', "coordinates/electron/XYe", "electron position", 500, -50, 50, 500, -50, 50);
	fXTsume = MakeTH2('D', "coordinates/electron/XTsume", "electron DLA wire sum vs. X", 500, -50, 50, 500, 0, 0.1);
	fYTsume = MakeTH2('D', "coordinates/electron/YTsume", "electron DLA wire sum vs. Y", 500, -50, 50, 500, 0, 0.1);
	fTe = MakeTH1('D', "coordinates/electron/Te", "electron time-of-flight", 1000, 0, 1);
	fTe2 = MakeTH1('D', "coordinates/electron/Te2", "electron2 time-of-flight", 1000, 0, 1);
	fTXe = MakeTH2('D', "coordinates/electron/TXe", "electron ToF vs. X", 500, 0, 1, 250, -50, 50);
	fTYe = MakeTH2('D', "coordinates/electron/TYe", "electron ToF vs. Y", 500, 0, 1, 250, -50, 50);

	fTer = MakeTH1('D', "coordinates/coinc/Ter", "ion minus electron time-of-flight", 100000, 0, 100);
	fTer_xycond = MakeTH1('D', "coordinates/coinc/Ter_xycond", "ion minus electron time-of-flight", 10000, 0, 100);
	fTeTr = MakeTH2('D', "coordinates/coinc/TeTr", "ion vs. electron time-of-flight", 550, -.5, 5, 550, -.5, 5);
	fPipico = MakeTH2('D', "coordinates/recoil/pipico", "ion1 vs ion2 ToF", 500, -1, 1, 500, -1, 1);


//corrected coordinates
	fCorrXr = MakeTH1('D', "corr_coord/recoil/CorrXr", "recoil ion position", 500, -50, 50);
	fCorrYr = MakeTH1('D', "corr_coord/recoil/CorrYr", "recoil ion position", 500, -50, 50);
	fCorrXYr = MakeTH2('D', "corr_coord/recoil/CorrXYr", "recoil ion position", 500, -10, 10, 500, -10, 10);
	fCorrPhirRr = MakeTH2('D', "corr_coord/recoil/CorrPhirRr", "recoil phi vs radius", 250, -TMath::Pi(), TMath::Pi(), 250, 0, 10);
	fCorrTXr = MakeTH2('D', "corr_coord/recoil/CorrTXr", "recoil ion ToF vs. X", 500, -1, 1, 250, -50, 50);
	fCorrTYr = MakeTH2('D', "corr_coord/recoil/CorrTYr", "recoil ion ToF vs. Y", 500, -1, 1, 250, -50, 50);
	fCorrTRr = MakeTH2('D', "corr_coord/recoil/CorrTRr", "recoil ion ToF vs. R", 500, -1, 1, 250, 0, 50);
	fexpTRr = MakeTH1('D', "corr_coord/recoil/CorrTRr_exp", "recoil ion ToF vs. R", 500, -1, 1);


	fCorrXe = MakeTH1('D', "corr_coord/electron/CorrXe", "electron position", 500, -50, 50);
	fCorrYe = MakeTH1('D', "corr_coord/electron/CorrYe", "electron position", 500, -50, 50);
	fCorrXYe = MakeTH2('D', "corr_coord/electron/CorrXYe", "electron position", 500, -50, 50, 500, -50, 50);
	fCorrPhieRe = MakeTH2('D', "corr_coord/electron/CorrPhieRe", "electron phi vs radius", 250, -TMath::Pi(), TMath::Pi(), 250, 0, 50);
	fCorrTXe = MakeTH2('D', "corr_coord/electron/CorrTXe", "electron ToF vs. X", 500, 0, 1, 250, -50, 50);
	fCorrTYe = MakeTH2('D', "corr_coord/electron/CorrTYe", "electron ToF vs. Y", 500,0, 1, 250, -50, 50);
	fCorrTRe = MakeTH2('D', "corr_coord/electron/CorrTRe", "electron ToF vs. radius", 500, 0.1, 0.7, 250, 0, 50);
	fCorrTderivRe = MakeTH2('D', "corr_coord/electron/CorrTderivRe", "electron ToF vs. radius", 500, 0.1, 0.7, 250, 0, 50);
	fexpTRe = MakeTH1('D', "corr_coord/electron/CorrTRe_exp", "electron ToF vs. radius expected", 500, 0.1, 0.7);
	if (GetHistogram("corr_coord/electron/CorrTXYe") == 0)
	{
		fCorrTXYe = new TH3D("CorrTXYe", "electron momentum", 100, -20, 20, 100, -20, 20, 100, -.1, .1);
		AddHistogram(fCorrTXYe, "corr_coord/electron");
	}
	else fCorrTXYe = (TH3D*)GetHistogram("corr_coord/electron/CorrTXYe");

	fCorrTeTr = MakeTH2('D', "corr_coord/coinc/CorrTeTr", "ion vs. electron time-of-flight", 500, -0.5, 0.5, 500, 0, 1);
	fCorrDeltaphiTe = MakeTH2('D', "corr_coord/coinc/CorrDeltaphiTe", "ion minus electron phi vs. time-of-flight", 250, -3.14, 3.14, 250, 0, .3);
	if (GetHistogram("corr_coord/coinc/CorrTRePhir") == 0)
	{
		fCorrTRePhir = new TH3D("CorrTRePhir", "electron momentum", 100, 0, 20, 100, -180, 180, 100, -.1, .1);
		AddHistogram(fCorrTRePhir, "corr_coord/coinc");
	}
	else fCorrTRePhir = (TH3D*)GetHistogram("corr_coord/coinc/CorrTRePhir");

	ftheoPhiPperp = MakeTH2('D', "corr_coord/derivT/theoPhiPperp", "derived cot of ToF", 500, 0, 2*TMath::Pi(), 500, 0, 1);
	fTcotT = MakeTH2('D', "corr_coord/derivT/TcotT", "derived cot of ToF", 500, .101, .600, 300, -5, 5);
	fTderivT = MakeTH2('D', "corr_coord/derivT/TderivT", "Tof vs. derived ToF", 500, .01, 1, 500, -.2, 0.8);
	fderivTmT = MakeTH1('D', "corr_coord/derivT/derivTmT", "derived time minus measured", 6000, -0.999, 5);
	fderivTmTXe = MakeTH2('D', "corr_coord/derivT/derivTmTXe", "derived minus measured time vs Xe", 500, -0.1, 0.9, 250, -50, 50);
	fderivTmTYe = MakeTH2('D', "corr_coord/derivT/derivTmTYE", "derived minus measured time vs Ye", 500, -0.1, 0.9, 250, -50, 50);


//momenta
	fAllPXPerpr = MakeTH2('D', "momenta_all/recoil/AllXPerpr", "recoil x vs perp momenta", 200, -1, 1, 100, 0, 1);
	fAllPYPerpr = MakeTH2('D', "momenta_all/recoil/AllYPerpr", "recoil y vs perp momenta", 200, -1, 1, 100, 0, 1);
	fAllPZPerpr = MakeTH2('D', "momenta_all/recoil/AllZPerpr", "recoil z vs perp momenta", 200, -1, 1, 100, 0, 1);
	fAllPXYr = MakeTH2('D', "momenta_all/recoil/AllPXYr", "recoil x vs y momenta", 200, -1, 1, 200, -1, 1);
	fAllPXZr = MakeTH2('D', "momenta_all/recoil/AllPXZr", "recoil x vs z momenta", 200, -1, 1, 200, -1, 1);
	fAllPYZr = MakeTH2('D', "momenta_all/recoil/AllPYZr", "recoil y vs z momenta", 200, -1, 1, 200, -1, 1);
	fAllPhirPPerpr = MakeTH2('D', "momenta_all/recoil/AllPhirPPerpr", "recoil transv momentum vs. phi", 250, -3.14, 3.14, 250, 0, 1);

	fAllPXPerpe = MakeTH2('D', "momenta_all/electron/AllXPerpe", "electron x vs perp momenta", 500, -1, 1, 250, 0, 1);
	fAllPYPerpe = MakeTH2('D', "momenta_all/electron/AllYPerpe", "electron y vs perp momenta", 500, -1, 1, 250, 0, 1);
	fAllPZPerpe = MakeTH2('D', "momenta_all/electron/AllZPerpe", "electron z vs perp momenta", 500, -1, 1, 250, 0, 1);
	fAllPXYe = MakeTH2('D', "momenta_all/electron/AllPXYe", "electron x vs y momenta", 500, -1, 1, 500, -1, 1);
	fAllPXZe = MakeTH2('D', "momenta_all/electron/AllPXZe", "electron x vs z momenta", 500, -1, 1, 500, -1, 1);
	fAllPYZe = MakeTH2('D', "momenta_all/electron/AllPYZe", "electron y vs z momenta", 500, -1, 1, 500, -1, 1);
	fAllOmegae = MakeTH2('D', "momenta_all/electron/AllOmegae", "electron solid angle", 180, 0, 360, 90, 0, 180);
	fAllOmegae_lr = MakeTH2('D', "momenta_all/electron/AllOmegaelr", "electron solid angle", 37, 0, 370, 18, 0, 180);
	fAllEe = MakeTH1('D', "momenta_all/electron/AllEe", "electron energy", 5000, 0, 5);
	fAllEeTmT = MakeTH2('D', "momenta_all/electron/AllEeTmT", "electron energy vs. TmT", 250, 0, 5, 500, 0, 5);
	fAllEeTmTzoom = MakeTH2('D', "momenta_all/electron/AllEeTmTzoom", "electron energy vs. TmT", 250, 0, 1.5, 250, 0, 0.5);
	fAllPZeTmT = MakeTH2('D', "momenta_all/electron/AllPZeTmT", "electron z momentum vs. TmT", 250, -0.3, 0.3, 500, 0, 0.5);

	fAllEePhie = MakeTH2('D', "momenta_all/electron/AllEePhie", "electron energy vs. phi", 250, -3.14, 3.14, 250, 0, 3);
	fAllThetaeTmT = MakeTH2('D', "momenta_all/electron/AllThetaeTmT", "electron theta vs. TmT", 250, -3.14, 3.14, 250, 0, 3);

	fPXer = MakeTH2('D', "momenta_all/coinc/PXer", "electron vs recoil x momenta", 200, -1, 1, 200, -1, 1);
	fPYer = MakeTH2('D', "momenta_all/coinc/PYer", "electron vs recoil y momenta", 200, -1, 1, 200, -1, 1);
	fPZer = MakeTH2('D', "momenta_all/coinc/PZer", "electron vs recoil z momenta", 200, -1, 1, 200, -1, 1);
	fPRer = MakeTH2('D', "momenta_all/coinc/PRer", "electron vs recoil transverse momenta", 200, 0, 1, 200, 0, 1);
	fPhiePhir = MakeTH2('D', "momenta_all/coinc/PhiePhir", "electron vs recoil azimuthal angle", 200, -180, 180, 200, -180, 180);
	fPhier = MakeTH1('D', "momenta_all/coinc/Phier", "electron minus recoil azimuthal angle", 200, 0, TMath::TwoPi());

	fcTime = MakeTH1('D', "momenta_all/coinc/coinc_groupTime", "Time of swicthing cylce in mus", 1000, 0, 10000);
	fEecTime = MakeTH2('D', "momenta_all/coinc/EegroupTime", "electron energy vs Time of switching cylce in mus", 500, 0, 5, 5000, 0, 5);
	fcTimeHD = MakeTH1('D', "momenta_all/coinc/coinc_groupTimeHD", "Time of swicthing cylce in mus", 1000, 0, 1000);
	fEecTimeHD = MakeTH2('D', "momenta_all/coinc/EegroupTimeHD", "eelctron energy vs Time of swicthing cylce in mus", 200, 0, 5, 500, 0, 500);

	fDRMOD = MakeTH1('D',"momenta_all/coinc/test/DRMOD minima", "", 1000, 0, 10);

	for (j = 0; j< 10; j++)
	{
		sprintf(chis, "momenta_all/coinc/test/DRMODvT % 1d", j); sprintf(chead, "Last ten DRMod v T Hit %1d", j);
		fDRMODvT[j] = MakeTH1('D', chis, chead, (1000), 0,TMath::TwoPi());
	}

	fOptXYoff = MakeTH2('D', "momenta_all/optimized/OptXYoff", "position offset", 100, -10, 10, 100, -10, 10);



	fSPXPerpr = MakeTH2('D', "momenta_S/recoil/SXPerpr", "recoil x vs perp momenta", 200, -1, 1, 100, 0, 1);
	fSPYPerpr = MakeTH2('D', "momenta_S/recoil/SYPerpr", "recoil y vs perp momenta", 200, -1, 1, 100, 0, 1);
	fSPZPerpr = MakeTH2('D', "momenta_S/recoil/SZPerpr", "recoil z vs perp momenta", 200, -1, 1, 100, 0, 1);

	fSPXPerpe = MakeTH2('D', "momenta_S/electron/SXPerpe", "electron x vs perp momenta", 500, -1, 1, 250, 0, 1);
	fSPYPerpe = MakeTH2('D', "momenta_S/electron/SYPerpe", "electron y vs perp momenta", 500, -1, 1, 250, 0, 1);
	fSPZPerpe = MakeTH2('D', "momenta_S/electron/SZPerpe", "electron z vs perp momenta", 500, -1, 1, 250, 0, 1);
	fSPXYe = MakeTH2('D', "momenta_S/electron/SPXYe", "electron x vs y momenta", 500, -1, 1, 500, -1, 1);
	fSPXZe = MakeTH2('D', "momenta_S/electron/SPXZe", "electron x vs z momenta", 500, -1, 1, 500, -1, 1);
	fSPYZe = MakeTH2('D', "momenta_S/electron/SPYZe", "electron y vs z momenta", 500, -1, 1, 500, -1, 1);
	fSEe = MakeTH1('D', "momenta_S/electron/SEe", "electron energy", 5000, 0, 5);
	fSEeTmT = MakeTH2('D', "momenta_S/electron/SEeTmT", "S state electron energy vs. TmT", 125, 0, 5, 500, 0, 5);

	fSEePhie = MakeTH2('D', "momenta_S/electron/SEePhie", "S state electron energy vs. phi", 250, -3.14, 3.14, 250, 0, 3);

	fSOmegae = MakeTH2('D', "momenta_S/electron/SOmegae", "electron solid angle", 180, 0, 360, 90, 0, 180);
	fSOmegae_lr = MakeTH2('D', "momenta_S/electron/SOmegaelr", "electron solid angle", 37, 0, 370, 18, 0, 180);

	fPPXPerpr = MakeTH2('D', "momenta_P/recoil/PXPerpr", "recoil x vs perp momenta", 200, -1, 1, 100, 0, 1);
	fPPYPerpr = MakeTH2('D', "momenta_P/recoil/PYPerpr", "recoil y vs perp momenta", 200, -1, 1, 100, 0, 1);
	fPPZPerpr = MakeTH2('D', "momenta_P/recoil/PZPerpr", "recoil z vs perp momenta", 200, -1, 1, 100, 0, 1);

	fPPXPerpe = MakeTH2('D', "momenta_P/electron/PXPerpe", "electron x vs perp momenta", 500, -1, 1, 250, 0, 1);
	fPPYPerpe = MakeTH2('D', "momenta_P/electron/PYPerpe", "electron y vs perp momenta", 500, -1, 1, 250, 0, 1);
	fPPZPerpe = MakeTH2('D', "momenta_P/electron/PZPerpe", "electron z vs perp momenta", 500, -1, 1, 250, 0, 1);
	fPPXYe = MakeTH2('D', "momenta_P/electron/PPXYe", "electron x vs y momenta", 500, -1, 1, 500, -1, 1);
	fPPXZe = MakeTH2('D', "momenta_P/electron/PPXZe", "electron x vs z momenta", 500, -1, 1, 500, -1, 1);
	fPPYZe = MakeTH2('D', "momenta_P/electron/PPYZe", "electron y vs z momenta", 500, -1, 1, 500, -1, 1);
	fPEe = MakeTH1('D', "momenta_P/electron/PEe", "electron energy", 5000, 0, 5);
	fPEeTmT = MakeTH2('D', "momenta_P/electron/PEeTmT", "P_state electron energy vs. TmT", 125, 0, 5, 500, 0, 4);

	fPEePhie = MakeTH2('D', "momenta_P/electron/PEePhie", "P state electron energy vs. phi", 250, -3.14, 3.14, 250, 0, 3);

	fPOmegae = MakeTH2('D', "momenta_P/electron/POmegae", "electron solid angle", 180, 0, 360, 90, 0, 180);
	fPOmegae_lr = MakeTH2('D', "momenta_P/electron/POmegaelr", "electron solid angle", 37, 0, 370, 18, 0, 180);

	if (GetHistogram("momenta_all/electron/test/pe3D") == 0)
	{
		fAllPe = new TH3D("pe3D", "electron momentum", 40, -.4, .4, 40, -.4, .4, 40, -.4, .4);
		AddHistogram(fAllPe, "momenta_all/electron/test");
	}
	else fAllPe = (TH3D*)GetHistogram("momenta_all/electron/test/pe3D");

	fRawParr = (TCronoACQRawPar*)MakeParameter("raw/RawParr", "TCronoACQRawPar");
	if (IsObjMade()) {
		fRawParr->scalex = -0.0005;
		fRawParr->scaley = 0.0005;
		fRawParr->shiftx = 0;//-100;
		fRawParr->shifty = 0;//-100;
		fRawParr->x0 = 0;
		fRawParr->y0 = 0;
		fRawParr->rot_phi = 0;
	}

	fRawPare = (TCronoACQRawPar*)MakeParameter("raw/RawPare", "TCronoACQRawPar");
	if (IsObjMade()) {
		fRawPare->scalex = 0.0005;
		fRawPare->scaley = 0.0005;
		fRawPare->shiftx = 0;//-100;
		fRawPare->shifty = 0;//-100;
		fRawPare->x0 = 0;
		fRawPare->y0 = 0;
		fRawPare->rot_phi = 0;
	}

	fCorrParr = (TCronoACQCorrPar*)MakeParameter("corrections/CorrParr", "TCronoACQCorrPar");
	if (IsObjMade()) {
		fCorrParr->scalex = 1;
		fCorrParr->scaley = 1;
		fCorrParr->x0 = -1.05;
		fCorrParr->y0 = 1.04;
		fCorrParr->T0 = 40.1978;
		fCorrParr->Toff = 40.2;
		fCorrParr->rot_phi = 70;
	}

	fCorrPare = (TCronoACQCorrPar*)MakeParameter("corrections/CorrPare", "TCronoACQCorrPar");
	if (IsObjMade()) {
		fCorrPare->scalex = 1;
		fCorrPare->scaley = 1;
		fCorrPare->T0 = 0.4075;
		fCorrPare->Toff = 0.3967;
		fCorrPare->t[0] = 0.348;
		fCorrPare->t[1] = 0.485;
		fCorrPare->x[0] = 4.814;
		fCorrPare->x[1] = 5.056;
		fCorrPare->y[0] = 3.282;
		fCorrPare->y[1] = 3.863;

	}



	fMomCalcParam = (TCronoACQMomCalcPar*)MakeParameter("Parameters for momentum calculation", "TCronoACQMomCalcPar");
	if (IsObjMade()) {
		fMomCalcParam->Tcyclotron = 0.0684;
		fMomCalcParam->Mrec = 6;
		fMomCalcParam->cutfrac = 0;
		fMomCalcParam->weight = 1;
		fMomCalcParam->opt = kFALSE;
		fMomCalcParam->expE = 1;
	}

	fMomCorrPare = (TCronoACQMomCorrPar*)MakeParameter("corrections/MomCorre", "TCronoACQMomCorrPar");
	if (IsObjMade()) {
		fMomCorrPare->cPX = 1;
		fMomCorrPare->cPY = 1;
		fMomCorrPare->cPZ = 1;
		fMomCorrPare->PXoff = 0;
		fMomCorrPare->PYoff = 0;
		fMomCorrPare->PZoff = 0;
		fMomCorrPare->PRotateZ = 0;
		fMomCorrPare->PRotateY = 0;
		fMomCorrPare->PRotateX = 0;
	}

	fMomCorrParr = (TCronoACQMomCorrPar*)MakeParameter("corrections/MomCorrr", "TCronoACQMomCorrPar");
	if (IsObjMade()) {
		fMomCorrParr->cPX = 1;
		fMomCorrParr->cPY = 1;
		fMomCorrParr->cPZ = 1;
		fMomCorrParr->PXoff = 0;
		fMomCorrParr->PYoff = 0;
		fMomCorrParr->PZoff = 0;
		fMomCorrParr->PRotateZ = 0;
		fMomCorrParr->PRotateY = 0;
		fMomCorrParr->PRotateX = 0;
	}



		

	fcontsumxr = MakeWinCond("raw/ctsumxr", 0.03, 0.1, "tsumxr0");//0.04,0.06
	fcontsumyr = MakeWinCond("raw/ctsumyr", 0.03, 0.1, "tsumyr0");//0.03,0.06
	fcontsumxe = MakeWinCond("raw/ctsumxe", 0.03, 0.1, "tsumxe0");//0.04,0.06
	fcontsumye = MakeWinCond("raw/ctsumye", 0.03, 0.1, "tsumye0");//0.03,0.06
	fcontS = MakeWinCond("raw/cS", -5000, 0, "TDC9_0");//0.03,0.06
	fconte = MakeWinCond("raw/cte", 0, 0, "Te");
	fconter = MakeWinCond("coinc/cter", 39.7, 39.9, "Ter");
	fcontderive = MakeWinCond("raw/ctderive", 0, 0, "CorrTderivRe");
	fcontderiv = MakeWinCond("raw/ctderiv", 0, 2.5, "derivTmT");
	fconEe = MakeWinCond("momenta/cEe", 0.5, 1.5, "AllEe");
	fcontime = MakeWinCond("ctime", 2050, 2240, "groupTime");
	fconxye = MakeCircleCond("raw/cxye", 20, 0, 0, 50, "XYe");
	fconxyr = MakeCircleCond("raw/cxyr", 20, 0, 0, 50, "XYr");
	if (IsObjMade()) fconxyr->Disable(1);
	fconRe = MakeWinCond("raw/cRe", 5, 10.5, "CorrYe");


	fcontDRModMinima = MakeWinCond("coinc/DRModMin", 0, 4.44, "DRMOD minima");

	Double_t points[4][2] = { {-1,-1}, {1,-1}, {1,1} ,{-1,1} };
	Double_t pointsPZer[4][2] = { {-0.15,-0.15}, {0.15,-15}, {0.15,0.15} ,{-0.15,0.15} };

	fconpxer = MakePolyCond("momenta/cPxer", 4, points, "PXer");
	fconpyer = MakePolyCond("momenta/cPyer", 4, points, "PYer");
	fconpzer = MakePolyCond("momenta/cPzer", 4, pointsPZer, "PZer");

	cycleStart = LONG_MIN;


	//Fill a histogram to show the expected radial distribution of the electron for a given energy
	ftheoPhiPperp->Reset();
	TAxis* x = fexpTRe->GetXaxis();
	for (i = 0; i < x->GetNbins(); i++)
	{
		Double_t T = x->GetXmin() + i * (x->GetXmax() - x->GetXmin()) / x->GetNbins();
		Double_t R = CalculateRad(T, fMomCalcParam->expE, fCorrPare->T0 + fCorrPare->Toff, fCorrPare->t[0] + fCorrPare->Toff, -1, 1);
		fexpTRe->SetBinContent(i + 1, R);
		TVector3 r(R, 0, T);
		TVector3 p;
		Double_t alpha = CalculateMomentum(r, p, fCorrPare->T0 + fCorrPare->Toff, fCorrPare->t[0] + fCorrPare->Toff, -1, 1);
		if (R > 0) ftheoPhiPperp->Fill(fmod(alpha + 10 * TMath::Pi(), 2 * TMath::Pi()), p.Perp());
	}

	x = fexpTRr->GetXaxis();
	for (i = 0; i < x->GetNbins(); i++)
	{
		Double_t T = x->GetXmin() + i * (x->GetXmax() - x->GetXmin()) / x->GetNbins() + (fCorrParr->Toff + fCorrParr->T0);
		Double_t R = CalculateRad(T, fMomCalcParam->expE, fCorrParr->T0 + fCorrParr->Toff, 0, 1, fMomCalcParam->Mrec * 1836);
		fexpTRr->SetBinContent(i + 1, R);
	}
	event = 0;
}

//-----------------------------------------------------------
TCronoACQAnlProc::~TCronoACQAnlProc()
{
   TGo4Log::Info("TCronoACQAnlProc: Delete");
}

//-----------------------------------------------------------
Bool_t TCronoACQAnlProc::BuildEvent(TGo4EventElement *dest)
{
	Bool_t isValid = kFALSE; // validity of output event

   TCronoACQUnpackEvent* inp_evt  = (TCronoACQUnpackEvent*) GetInputEvent();
   TCronoACQAnlEvent* out_evt = (TCronoACQAnlEvent*) dest;

   // see comments in UnpackProc
   if(!inp_evt || !inp_evt->IsValid()){ // input invalid
	   out_evt->SetValid(isValid); // invalid
	   return isValid; // must be same is for SetValid
   }
   isValid = kTRUE;

   //
   Int_t pat_e[3] = { -1 };
   Bool_t flag_e[3] = { kFALSE };
   Bool_t flage;
   Int_t pat_r[3] = { -1 };
   Bool_t flag_r[3] = { kFALSE };
   Bool_t flagr;
   Bool_t flag_s = kFALSE;
  //
 
  	Double_t tdc[CronoACQ_NUM_CHAN][3]={ LONG_MIN };
	Double_t Tr, Te, Te2;
	TVector2 rr;
	TVector2 re;
	TVector3 CorrR;
	TVector3 CorrE;
	TVector3 pe, pr;


   for(Int_t i=0; i<CronoACQ_NUM_CHAN; i++)
	{
		for(Int_t j=0; j<3; j++) 
		{
			tdc[i][j] = inp_evt->tdc[i][j]; //time in ps
		}
	}
   if (fcontS->Test(tdc[9][0] * 1e-6)) {
	   flag_s = kTRUE;
   }
   Int_t cycleTime = (inp_evt->groupTime - cycleStart)*1e-6;
   fTime->Fill(cycleTime);
   if(tdc[14][0]!= LONG_MIN && tdc[2][0] != LONG_MIN) fTimeHD->Fill((tdc[14][0]-tdc[2][0])/1000);

/******* TDC channel assignment *********/
//tdc[0][*]: Ion MCP	
//tdc[1][*]: Electron MCP	
//tdc[2][*]: Photodiode	
	
//tdc[5][*]: Ion DLA y1	
//tdc[6][*]: Ion DLA x1	
//tdc[7][*]: Ion DLA y2	
//tdc[8][*]: Ion DLA x2	

//tdc[10][*]: electron DLA y1	
//tdc[11][*]: electron DLA x1	
//tdc[12][*]: electron DLA y2	
//tdc[13][*]: electron DLA x2	

 	// fill raw histograms
  	for(Int_t i=0;i<3;i++)
	{
		
		for(Int_t j=0;j<CronoACQ_NUM_CHAN;j++){ 
				fTdc[j][i]->Fill(tdc[j][i]*1e-6); // time in mus

		}

		ftsumxr[i]->Fill((tdc[6][i] + tdc[8][i] - 2 * tdc[0][i])*1e-6);
		ftsumyr[i]->Fill((tdc[5][i] + tdc[7][i] - 2 * tdc[0][i])*1e-6);
		flag_r[i] = kFALSE;
		pat_r[i] =
			(tdc[5][i] != LONG_MIN ? 0x1 : 0) +
			(tdc[6][i] != LONG_MIN ? 0x2 : 0) +
			(tdc[7][i] != LONG_MIN ? 0x4 : 0) +
			(tdc[8][i] != LONG_MIN ? 0x8 : 0);
		if (pat_r[i] == 15) flag_r[i] = kTRUE;
		fPatr[i]->Fill(pat_r[i]);
		
		ftsumxe[i]->Fill((tdc[10][i] + tdc[12][i] - 2 * tdc[1][i])*1e-6);
		ftsumye[i]->Fill((tdc[11][i] + tdc[13][i] - 2 * tdc[1][i])*1e-6);
		flag_e[i] = kFALSE;
		pat_e[i] =
			(tdc[10][i] != LONG_MIN && tdc[10][i] < 1 << 21 ? 0x1 : 0) +
			(tdc[11][i] != LONG_MIN && tdc[11][i] < 1 << 21 ? 0x2 : 0) +
			(tdc[12][i] != LONG_MIN && tdc[12][i] < 1 << 21 ? 0x4 : 0) +
			(tdc[13][i] != LONG_MIN && tdc[13][i] < 1 << 21 ? 0x8 : 0);
		if (pat_e[i] == 15) flag_e[i] = kTRUE;
		fPate[i]->Fill(pat_e[i]);
		
	}

	// recoil ion coordinates
	flagr = kFALSE;
	if (fcontsumxr->Test((tdc[6][0] + tdc[8][0] - 2 * tdc[0][0])*1e-6) && fcontsumyr->Test((tdc[5][0] + tdc[7][0] - 2 * tdc[0][0])*1e-6) && flag_r[0])
	{
		rr.Set(fRawParr->scalex*(tdc[7][0] - tdc[5][0]) + fRawParr->shiftx, fRawParr->scaley*((Double_t)(tdc[8][0] - tdc[6][0])) + fRawParr->shifty);
		fXTsumr->Fill(rr.X(), (tdc[5][0] + tdc[7][0] - 2 * tdc[0][0])*1e-6);
		fYTsumr->Fill(rr.Y(), (tdc[6][0] + tdc[8][0] - 2 * tdc[0][0])*1e-6);

		rr -= TVector2(fRawParr->x0, fRawParr->y0);
		rr = rr.Rotate(fRawParr->rot_phi*TMath::Pi() / 180);
		rr += TVector2(fRawParr->x0, fRawParr->y0);

		Tr = (tdc[0][0] - tdc[2][0])*1e-6;

		fXYr->Fill(rr.X(), rr.Y());
		fTr->Fill(Tr);
		if(fconxyr->Test(rr.X(),rr.Y())) fTr_xycond->Fill(Tr);
		else fTr_NOTxycond->Fill(Tr);
		fTXr->Fill(Tr - fCorrParr->T0, rr.X());
		fTYr->Fill(Tr - fCorrParr->T0, rr.Y());
		flagr = kTRUE;
	}
	else flag_r[0] = kFALSE;

	fPipico->Fill(Tr - fCorrParr->T0, (tdc[0][1] - tdc[2][0])*1e-6 - fCorrParr->T0);


	// electron coordinates
	{
		flage = kFALSE;
		Int_t i = 0;
		for (i = 0; i < 3; i++) {
			if (fcontsumxe->Test((tdc[10][i] + tdc[12][i] - 2 * tdc[1][i])*1e-6) && fcontsumye->Test((tdc[11][i] + tdc[13][i] - 2 * tdc[1][i])*1e-6) && flag_e[i])
			{
				re.Set(fRawPare->scalex*((Double_t)(tdc[10][i] - tdc[12][i])) + fRawPare->shiftx, fRawPare->scaley*((Double_t)(tdc[11][i] - tdc[13][i])) + fRawPare->shifty);
				fXTsume->Fill(re.X(), (tdc[10][i] + tdc[12][i] - 2 * tdc[1][i])*1e-6);
				fYTsume->Fill(re.Y(), (tdc[11][i] + tdc[13][i] - 2 * tdc[1][i])*1e-6);

				re -= TVector2(fRawPare->x0, fRawPare->y0);
				re = re.Rotate(fRawPare->rot_phi*TMath::Pi() / 180);
				re += TVector2(fRawPare->x0, fRawPare->y0);
				if (fconxye->Test(re.X(), re.Y())) {
					flage = kTRUE;
					break;
				}
			}
			else flag_e[i] = kFALSE;
		}

		Te = (tdc[1][i] - tdc[2][0])*1e-6;
		if (i < 2) Te2 = (tdc[1][i + 1] - tdc[2][0])*1e-6;
		else Te2 = -99999;

		if (flage) {
			fXYe->Fill(re.X(), re.Y());
			fTe->Fill(Te);
			fTe2->Fill(Te2);

			fTXe->Fill(Te, re.X());
			fTYe->Fill(Te, re.Y());
		}
	}

	// coincidences
	Double_t Ter = (tdc[0][0] - tdc[1][0])*1e-6;
	fTer->Fill(Ter);
	if(fconxyr->Test(rr.X(),rr.Y())) fTer_xycond->Fill(Ter);
	if (flag_r[0] && flage)	fTeTr->Fill(Tr - fCorrParr->T0, Te - fCorrPare->T0);


	if (!(fconter->Test(Ter) && flage && fconte->Test(Te))) return kTRUE;
	//if (fconte->Test(Te) && fconte->Test(Te2)) return kTRUE;

	//Corrections

	//recoils
	if (flagr)
	{
		CorrR.SetXYZ((rr.X() - fCorrParr->x0), (rr.Y() - fCorrParr->y0), Tr + fCorrParr->Toff);
		CorrR.RotateZ(fCorrParr->rot_phi*TMath::DegToRad());
		CorrR.SetX(CorrR.X()* fCorrParr->scalex);
		CorrR.SetY(CorrR.Y()* fCorrParr->scaley);

	}
	else CorrR.SetXYZ(-9999, -9999, -9999);

	Double_t Te_old = Te;

	//Derive time if photodiode signal is missing
	if (fMomCalcParam->bderivT)
	{
		if (!flagr) return kTRUE;
		Double_t dT = derivT(re, CorrR);
		if (dT < 0) return kTRUE;	
		//fTderivT->Fill(Te,Te-dT);
		Te = dT;
		Tr = Ter + Te;
		//CorrR.SetXYZ((rr.X() - fCorrParr->x0)*fCorrParr->scalex, (rr.Y() - fCorrParr->y0)*fCorrParr->scaley, Tr + fCorrParr->Toff);
		//CorrR.RotateZ(fCorrParr->rot_phi*TMath::DegToRad());
		CorrR.SetZ(Tr + fCorrParr->Toff);
//		fderivTmT->Fill(Te_old - Te);
		if (!fcontderiv->Test(Te_old - Te) || !fcontderive->Test(Te + fCorrPare->Toff)) return kTRUE;
	}



	//electrons
	TVector2 DeltaE;
	DeltaE = DeltaCorrElectrons(Te);

	CorrE.SetX((re.X() - DeltaE.X()));
	CorrE.SetY((re.Y() - DeltaE.Y()));
	CorrE.SetZ(Te + fCorrPare->Toff);

	CorrE.RotateZ(fCorrPare->rot_phi* TMath::DegToRad());
	CorrE.SetX(CorrE.X()* fCorrPare->scalex);
	CorrE.SetY(CorrE.Y()* fCorrPare->scaley);

	//phi-correction
	Double_t phi_o = CorrR.Phi();
	Double_t Dphi = -67.76 - 118 * sin(phi_o / 2 + 4.966) + 90.570 * sin(phi_o - 1.065) + 51.242 * sin(3 * phi_o / 2 + 2.314) - 22.046 * sin(2 * phi_o + 2.639) - 5.475 * sin(5 * phi_o / 2 - 0.224);
	CorrE.RotateZ(-Dphi*TMath::DegToRad());



	if (!fconRe->Test(CorrE.Perp())) return kTRUE;

	fderivTmT->Fill(Te_old - Te);
	fTderivT->Fill(Te_old, Te_old - Te);
	fCorrDeltaphiTe->Fill((CorrE.Phi() - CorrR.Phi()) < -TMath::Pi() ? (CorrE.Phi() - CorrR.Phi() + TMath::TwoPi()) : (CorrE.Phi() - CorrR.Phi()), Te);





	if (flagr)
	{
		fCorrXr->Fill(CorrR.X());
		fCorrYr->Fill(CorrR.Y());
		fCorrXYr->Fill(CorrR.X(), CorrR.Y());
		fCorrPhirRr->Fill(CorrR.Phi(), CorrR.Perp());
		fCorrTXr->Fill(CorrR.Z() - (fCorrParr->Toff + fCorrParr->T0), CorrR.X());
		fCorrTYr->Fill(CorrR.Z() - (fCorrParr->Toff + fCorrParr->T0), CorrR.Y());
		fCorrTRr->Fill(CorrR.Z() - (fCorrParr->Toff + fCorrParr->T0), CorrR.Perp());
	}


	fCorrXe->Fill(CorrE.X());
	fCorrYe->Fill(CorrE.Y());
	fCorrXYe->Fill(CorrE.X(), CorrE.Y());
	fCorrPhieRe->Fill(CorrE.Phi(), CorrE.Perp());
	fCorrTXe->Fill(CorrE.Z(), CorrE.X());
	fCorrTYe->Fill(CorrE.Z(), CorrE.Y());
	//fCorrTRe->Fill(Te_old + fCorrPare->Toff, CorrE.Perp());
	if (fMomCalcParam->bderivT && flagr) fCorrTderivRe->Fill(CorrE.Z(), CorrE.Perp());
	fCorrTXYe->Fill(CorrE.X(), CorrE.Y(), CorrE.Z() - (fCorrPare->Toff + fCorrPare->T0));
	fCorrTRePhir->Fill(CorrE.Perp(), CorrR.Phi()*TMath::RadToDeg(), CorrE.Z() - (fCorrPare->Toff + fCorrPare->T0));

	fCorrTeTr->Fill(CorrR.Z() - (fCorrParr->Toff + fCorrParr->T0), CorrE.Z());

	//Source point optimization
	if (fMomCalcParam->opt) {
		if(!OptSourcePos(CorrE, CorrR)) return kTRUE;
	}
	//Double_t dTe = 0;
	//Derive time if photodiode signal is missing
/*	if (fMomCalcParam->bderivT)
	{
		if (!flagr) return kTRUE;
		dTe=derivT(CorrE, CorrR, Ter);
		Double_t dTr=dTe - fCorrPare->Toff + Ter + fCorrParr->Toff;
		CorrE.SetZ(dTe);
		CorrR.SetZ(dTr);
		fCorrTderivRe->Fill(CorrE.Z(), CorrE.Perp());
		if(!fcontderiv->Test(Te - dTe) || !fcontderive->Test(CorrE.Z())) return kTRUE;
	}
	*/
	//calculate momenta
	Double_t phase_e = CalculateMomentum(CorrE, pe, fCorrPare->T0 + fCorrPare->Toff, fCorrPare->t[0] + fCorrPare->Toff, -1, 1);
	Double_t phase_r __attribute__((unused)) = CalculateMomentum(CorrR, pr, fCorrParr->T0 + fCorrParr->Toff, 0, 1, fMomCalcParam->Mrec * 1836);


	if (fMomCalcParam->bderivT) {
		fderivTmTXe->Fill(Te_old - Te, CorrE.X());
		fderivTmTYe->Fill(Te_old - Te, CorrE.Y());
	}


	//momentum corrections
	Double_t peZ_orig = pe.Z();
	pe.RotateZ(fMomCorrPare->PRotateZ*TMath::DegToRad());
	pe.RotateY(fMomCorrPare->PRotateY*TMath::DegToRad());
	pe.RotateX(fMomCorrPare->PRotateX*TMath::DegToRad());
	pe -= TVector3(fMomCorrPare->PXoff, fMomCorrPare->PYoff, fMomCorrPare->PZoff);
	pe.SetXYZ(pe.X()*fMomCorrPare->cPX, pe.Y()*fMomCorrPare->cPY, pe.Z()*fMomCorrPare->cPZ);

	pr.RotateZ(fMomCorrParr->PRotateZ * TMath::DegToRad());
	pr.RotateY(fMomCorrParr->PRotateY * TMath::DegToRad());
	pr.RotateX(fMomCorrParr->PRotateX * TMath::DegToRad());//Double
	pr -= TVector3(fMomCorrParr->PXoff, fMomCorrParr->PYoff, fMomCorrParr->PZoff);
	pr.SetXYZ(pr.X()*fMomCorrParr->cPX, pr.Y()*fMomCorrParr->cPY, pr.Z()*fMomCorrParr->cPZ);
	



	//Plot momentum spectra
	fAllPXPerpr->Fill(pr.X(), sqrt(pow(pr.Y(), 2) + pow(pr.Z(), 2)));
	fAllPYPerpr->Fill(pr.Y(), sqrt(pow(pr.X(), 2) + pow(pr.Z(), 2)));
	fAllPZPerpr->Fill(pr.Z(), sqrt(pow(pr.X(), 2) + pow(pr.Y(), 2)));


	if (ElectronNodeCut(peZ_orig, phase_e, fCorrPare->Toff+CorrE.Z()) && fconEe->Test(13.6*pe.Mag2())) {
		//some coincidence spectro for condition
		fPXer->Fill(pe.X(), pr.X() + pe.X());
		fPYer->Fill(pe.Y(), pr.Y() + pe.Y());
		fPZer->Fill(pe.Z(), pr.Z() + pe.Z());


		if (fconpxer->Test(pe.X(), pr.X() + pe.X()) && fconpyer->Test(pe.Y(), pr.Y() + pe.Y()) && fconpzer->Test(pe.Z(), pr.Z() + pe.Z())) {

			fCorrTRe->Fill(Te_old + fCorrPare->Toff, CorrE.Perp());


			Double_t w = fMomCalcParam->weight;
			fAllPXYr->Fill(pr.X(), pr.Y(), w);
			fAllPXZr->Fill(pr.X(), pr.Z(), w);
			fAllPYZr->Fill(pr.Y(), pr.Z(), w);
			fAllPhirPPerpr->Fill(pr.Phi(), pr.Perp());


			fAllPXPerpe->Fill(pe.X(), sqrt(pow(pe.Y(), 2) + pow(pe.Z(), 2)), w);
			fAllPYPerpe->Fill(pe.Y(), sqrt(pow(pe.X(), 2) + pow(pe.Z(), 2)), w);
			fAllPZPerpe->Fill(pe.Z(), sqrt(pow(pe.X(), 2) + pow(pe.Y(), 2)), w);
			fAllPXYe->Fill(pe.X(), pe.Y(), w);
			fAllPXZe->Fill(pe.X(), pe.Z(), w);
			fAllPYZe->Fill(pe.Y(), pe.Z(), w);

			fAllEe->Fill(13.6*pe.Mag2(), w);
			if (fMomCalcParam->bderivT && flagr) {
				fAllEeTmT->Fill(13.6 * pe.Mag2(), Te_old - Te, w);
				fAllEeTmTzoom->Fill(13.6 * pe.Mag2(), Te_old - Te, w);
				fAllThetaeTmT->Fill(pe.Phi(), Te_old - Te, w);
				fAllPZeTmT->Fill(pe.X(), Te_old - Te, w);
			}
			fAllPe->Fill(pe.X(), pe.Y(), pe.Z(), w);

			fAllEePhie->Fill(pe.Phi(), 13.6 * pe.Mag2(), w);

			// coincidence spectra

			fPhiePhir->Fill(((pe.Phi() - pr.Phi()) < 0 ? (pe.Phi() - pr.Phi() + TMath::TwoPi()) : (pe.Phi() - pr.Phi())) * TMath::RadToDeg()-180, pr.Phi()* TMath::RadToDeg(), w);
			fPhier->Fill((pe.Phi() - pr.Phi()) < 0 ? (pe.Phi() - pr.Phi() + TMath::TwoPi()) : (pe.Phi() - pr.Phi()), w);
			fPRer->Fill(sqrt(pow(pe.X(), 2) + pow(pe.Y(), 2)), sqrt(pow(pr.X(), 2) + pow(pr.Y(), 2)), w);

			fcTime->Fill(cycleTime, w);
			fEecTime->Fill(13.6*pe.Mag2(), Te - CorrE.Z()+.3, w);
			fcTimeHD->Fill(cycleTime, w);
			fEecTimeHD->Fill(13.6*pe.Mag2(), cycleTime, w);
			
			// only s-state
			Double_t p_weight = w;
			Double_t s_weight = w*(1 - fMomCalcParam->s_weight);
			if (flag_s) {
				fSPXPerpr->Fill(pr.X(), sqrt(pow(pr.Y(), 2) + pow(pr.Z(), 2)), s_weight);
				fSPYPerpr->Fill(pr.Y(), sqrt(pow(pr.X(), 2) + pow(pr.Z(), 2)), s_weight);
				fSPZPerpr->Fill(pr.Z(), sqrt(pow(pr.X(), 2) + pow(pr.Y(), 2)), s_weight);

				fSPXPerpe->Fill(pe.X(), sqrt(pow(pe.Y(), 2) + pow(pe.Z(), 2)), s_weight);
				fSPYPerpe->Fill(pe.Y(), sqrt(pow(pe.X(), 2) + pow(pe.Z(), 2)), s_weight);
				fSPZPerpe->Fill(pe.Z(), sqrt(pow(pe.X(), 2) + pow(pe.Y(), 2)), s_weight);
				fSPXYe->Fill(pe.X(), pe.Y(), s_weight);
				fSPXZe->Fill(pe.X(), pe.Z(), s_weight);
				fSPYZe->Fill(pe.Y(), pe.Z(), s_weight);
				fSEe->Fill(13.6*pe.Mag2(), s_weight);
				if (fMomCalcParam->bderivT && flagr) fSEeTmT->Fill(13.6 * pe.Mag2(), Te_old - Te, s_weight);
				p_weight = -w * fMomCalcParam->s_weight;

				fSEePhie->Fill(pe.Phi(), 13.6 * pe.Mag2(), s_weight);
			}

			//only p-state

			fPPXPerpr->Fill(pr.X(), sqrt(pow(pr.Y(), 2) + pow(pr.Z(), 2)), p_weight);
			fPPYPerpr->Fill(pr.Y(), sqrt(pow(pr.X(), 2) + pow(pr.Z(), 2)), p_weight);
			fPPZPerpr->Fill(pr.Z(), sqrt(pow(pr.X(), 2) + pow(pr.Y(), 2)), p_weight);

			fPPXPerpe->Fill(pe.X(), sqrt(pow(pe.Y(), 2) + pow(pe.Z(), 2)), p_weight);
			fPPYPerpe->Fill(pe.Y(), sqrt(pow(pe.X(), 2) + pow(pe.Z(), 2)), p_weight);
			fPPZPerpe->Fill(pe.Z(), sqrt(pow(pe.X(), 2) + pow(pe.Y(), 2)), p_weight);
			fPPXYe->Fill(pe.X(), pe.Y(), p_weight);
			fPPXZe->Fill(pe.X(), pe.Z(), p_weight);
			fPPYZe->Fill(pe.Y(), pe.Z(), p_weight);
			fPEe->Fill(13.6*pe.Mag2(), p_weight);
			if (fMomCalcParam->bderivT && flagr) fPEeTmT->Fill(13.6 * pe.Mag2(), Te_old - Te, p_weight);

			fPEePhie->Fill(pe.Phi(), 13.6 * pe.Mag2(), p_weight);

			// PAD
			TVector3 testvec(pe.Unit().X(), pe.Unit().Z(), pe.Unit().Y());
			for (Int_t i = 0; i < 180; i++) {
				if (fabs(testvec.Z()) < fabs(sin(1 * TMath::DegToRad())) && testvec.Phi() >= -TMath::Pi() / 2 && testvec.Phi() < TMath::Pi() / 2) {
					fAllOmegae->Fill(i * 2, testvec.Phi()*TMath::RadToDeg() + 90, w);
					if (flag_s) fSOmegae->Fill(i * 2, testvec.Phi()*TMath::RadToDeg() + 90, s_weight);
					fPOmegae->Fill(i * 2, testvec.Phi()*TMath::RadToDeg() + 90, p_weight);
				}
				testvec.RotateY(2 * TMath::DegToRad());
			}

			testvec.SetXYZ(pe.Unit().X(), pe.Unit().Z(), pe.Unit().Y());
			for (Int_t i = 0; i < 37; i++) {
				if (fabs(testvec.Z()) < fabs(sin(5 * TMath::DegToRad())) && testvec.Phi() >= -TMath::Pi() / 2 && testvec.Phi() < TMath::Pi() / 2) {
					fAllOmegae_lr->Fill((i * 10), (testvec.Phi()*TMath::RadToDeg() + 90), w);
					if (flag_s) fSOmegae_lr->Fill((i * 10), (testvec.Phi()*TMath::RadToDeg() + 90), s_weight);
					fPOmegae_lr->Fill((i * 10), (testvec.Phi()*TMath::RadToDeg() + 90), p_weight);
				}
				testvec.RotateY(10 * TMath::DegToRad());
			}

			
		}// end ElectronNodeCut


	}


   // see comments in UnpackProc
   out_evt->SetValid(isValid);
   return isValid;
}

Double_t TCronoACQAnlProc::CalculateMomentum(TVector3 r, TVector3 &p, Double_t Toff, Double_t Tnode, Int_t charge, Double_t mass)
{
	p.SetX(charge*4.56642e-4*TMath::Pi() / fMomCalcParam->Tcyclotron*(TMath::Tan((0.5 - charge*(r.Z() - Tnode) / (mass*fMomCalcParam->Tcyclotron))*TMath::Pi())*r.X() - r.Y()));
	p.SetY(charge*4.56642e-4*TMath::Pi() / fMomCalcParam->Tcyclotron*(r.X() + TMath::Tan((0.5 - charge * (r.Z() - Tnode) / (mass*fMomCalcParam->Tcyclotron))*TMath::Pi()) * r.Y()));
	p.SetZ(((charge<0)?1:-1)*4.56642e-4*430.*mass*(1/(r.Z())-fabs(charge)*(r.Z())/pow(Toff,2)));
	return (2*(r.Z() - Tnode)*TMath::Pi() / (mass*fMomCalcParam->Tcyclotron));
}

Double_t TCronoACQAnlProc::CalculateRad(Double_t T, Double_t Ee, Double_t Toff, Double_t Tnode, Int_t charge, Double_t mass)
{
	TVector3 r(0,0,T);
	TVector3 p;
	CalculateMomentum(r, p, Toff, Tnode, charge, mass);
	if (p.Mag2()*13.6 > Ee) return -10;

	Double_t pr = sqrt(Ee / 13.6 - pow(p.Z(), 2));
	Double_t R=pr*fMomCalcParam->Tcyclotron / (4.56642e-4*TMath::Pi()*fabs(charge)) * fabs(sin(charge * (T - Tnode) / (mass*fMomCalcParam->Tcyclotron)*TMath::Pi()));
	return R;
}

Double_t TCronoACQAnlProc::derivT(TVector2 re, TVector3 CorrR)
{
	TVector3 pr, pe;
	Double_t Te, phase_e;
	
	CorrR.SetZ(fCorrParr->T0 + fCorrParr->Toff); 
	Double_t phase_r __attribute__((unused)) = CalculateMomentum(CorrR, pr, fCorrParr->T0 + fCorrParr->Toff, 0, 1, fMomCalcParam->Mrec * 1836);
	pr.RotateZ(fMomCorrParr->PRotateZ * TMath::DegToRad());
	//pr.RotateY(fMomCorrParr->PRotateY * TMath::DegToRad());
	//pr.RotateX(fMomCorrParr->PRotateX * TMath::DegToRad());//Double
	pr -= TVector3(fMomCorrParr->PXoff, fMomCorrParr->PYoff, fMomCorrParr->PZoff);
	pr.SetXYZ(pr.X() * fMomCorrParr->cPX, pr.Y() * fMomCorrParr->cPY, pr.Z() * fMomCorrParr->cPZ);


	Int_t npx=10000;

	Int_t imax = (Int_t)(npx * fMomCalcParam->Tcyclotron);
	Int_t hn = event % 10;
	Int_t flag = 0;
	event++;
	fDRMODvT[hn]->Reset();

	TRandom2 rndm;
	Double_t minimum = FLT_MAX, locm;
	for (Int_t i = 0; i <= imax; i++) {
		phase_e =  i*TMath::TwoPi() / (npx*fMomCalcParam->Tcyclotron);
		Te = fCorrPare->t[0] + ((Double_t)i) / npx;
		if (0) { //position method
			Double_t xc = fMomCalcParam->Tcyclotron / (2 * 4.56642e-4 * TMath::Pi()) * (-sin(phase_e) * pr.X() + (1 - cos(phase_e)) * pr.Y());
			Double_t yc = fMomCalcParam->Tcyclotron / (2 * 4.56642e-4 * TMath::Pi()) * ((cos(phase_e) - 1) * pr.X() - sin(phase_e) * pr.Y());
			TVector2 DR(xc, yc);
			DR = DR + DeltaCorrElectrons(Te) - re;
			if (DR.Mod() < minimum) {
				minimum = DR.Mod();
				locm = Te;
			}

			fDRMODvT[hn]->Fill(phase_e, (1000 / imax) * DR.Mod());

		}
		else
		{
			//calculate momenta instead
			TVector2 DeltaE;
			TVector3 CorrE;
			DeltaE = DeltaCorrElectrons(Te);
			CorrE.SetX((re.X() - DeltaE.X()));
			CorrE.SetY((re.Y() - DeltaE.Y()));
			CorrE.SetZ(Te + fCorrPare->Toff);
			CorrE.RotateZ(fCorrPare->rot_phi * TMath::DegToRad());
			CorrE.SetX(CorrE.X() * fCorrPare->scalex);
			CorrE.SetY(CorrE.Y() * fCorrPare->scaley);
			//phi-correction
			Double_t phi_o = CorrR.Phi();
			Double_t Dphi = -67.76 - 118 * sin(phi_o / 2 + 4.966) + 90.570 * sin(phi_o - 1.065) + 51.242 * sin(3 * phi_o / 2 + 2.314) - 22.046 * sin(2 * phi_o + 2.639) - 5.475 * sin(5 * phi_o / 2 - 0.224);
			CorrE.RotateZ(-Dphi * TMath::DegToRad());

			Double_t phase_e = CalculateMomentum(CorrE, pe, fCorrPare->T0 + fCorrPare->Toff, fCorrPare->t[0] + fCorrPare->Toff, -1, 1);
			pe.RotateZ(fMomCorrPare->PRotateZ * TMath::DegToRad());
			//pe.RotateY(fMomCorrPare->PRotateY * TMath::DegToRad());
			//pe.RotateX(fMomCorrPare->PRotateX * TMath::DegToRad());
			pe -= TVector3(fMomCorrPare->PXoff, fMomCorrPare->PYoff, fMomCorrPare->PZoff);
			pe.SetXYZ(pe.X() * fMomCorrPare->cPX, pe.Y() * fMomCorrPare->cPY, pe.Z() * fMomCorrPare->cPZ);

			TVector3 DR = pr + pe;

			//fAbsPPePPr->Fill(phase_e, DR.Mod());

			if (DR.Perp() < minimum) {
				minimum = DR.Perp();
				locm = Te;
			}

			fDRMODvT[hn]->Fill(phase_e, (1000 / imax) * DR.Perp());

		}
	}

	fDRMOD-> Fill((1000 / imax) * minimum);

	if (!fcontDRModMinima->Test((1000 / imax)*minimum)) {
		flag = -1;
		fDRMODvT[hn]->Reset();
	}

	//cout << gMin << endl;
	if (flag == -1) {
		return flag;
	}
	else if (flag == 0) {
		return locm;
	}
	else return 0;
}

Bool_t TCronoACQAnlProc::ElectronNodeCut(Double_t pz, Double_t phase, Double_t Te)
{
	if (fMomCalcParam->cutfrac<0) {
		Double_t pzprime = pz * (fMomCalcParam->Toff_alt + fMomCalcParam->T0_alt) / (2 * 4.56642e-4*430.);
		Double_t T = (fMomCalcParam->Toff_alt + fMomCalcParam->T0_alt)*(sqrt(pow(pzprime, 2) + 1) - pzprime);
		Double_t phase_alt = (T - (fMomCalcParam->Toff_alt + fMomCalcParam->Tnode_alt)) * 2 * TMath::Pi() / (fMomCalcParam->Tcycl_alt);
		if (pow(sin(phase_alt/2), 2) < pow(sin(phase/2), 2)) return kTRUE;
		else return kFALSE;
	}
	else {
		if (pow(sin(phase/2), 2) > pow(sin(fMomCalcParam->cutfrac*TMath::Pi() / 2), 2)) return kTRUE;
		else return kFALSE;
	}
}

Bool_t TCronoACQAnlProc::OptSourcePos(TVector3 &re, TVector3 &rr)
{
	//optimization algorithm
	Double_t max_off = 2;

	TVector3 offs_tot(0, 0, 0);
	TVector3 offs(0.05, 0, 0);
	TVector3 petest;
	TVector3 prtest;
	TRandom2 rnd;
	offs.RotateZ((2 * rnd.Rndm() - 1)*TMath::Pi());

	CalculateMomentum(re, petest, fCorrPare->T0 + fCorrPare->Toff, fCorrPare->t[0] + fCorrPare->Toff, -1, 1);
	CalculateMomentum(rr, prtest, fCorrParr->T0 + fCorrParr->Toff, 0, 1, fMomCalcParam->Mrec * 1836);

	Double_t p_d = (petest + prtest).Perp();

	Int_t n1 = 0;
	Int_t n2 = 0;

	while (n2 < 5 && n1 < 150 )  //optimization loop
	{
		CalculateMomentum(re + offs_tot + offs, petest, fCorrPare->T0 + fCorrPare->Toff, fCorrPare->t[0] + fCorrPare->Toff, -1, 1);
		CalculateMomentum(rr + offs_tot + offs, prtest, fCorrParr->T0 + fCorrParr->Toff, 0, 1, fMomCalcParam->Mrec * 1836);

		if ((petest + prtest).Perp() < p_d) {
			n2 = 0;
			n1++;
			offs_tot += offs;
			p_d = (petest + prtest).Perp();
			offs.RotateZ((2 * rnd.Rndm() - 1)*TMath::Pi() / 2);
		}
		else {
			offs.RotateZ((2 * rnd.Rndm() + 1)*TMath::Pi() / 2);
			n2++;
		}
	}
	fOptXYoff->Fill(offs_tot.X(), offs_tot.Y());
	if (offs_tot.Mag() < max_off) {
		re += offs_tot;
		rr += offs_tot;
		return kTRUE;
	}
	else return kFALSE;
}

TVector2 TCronoACQAnlProc::DeltaCorrElectrons(Double_t Te)
{
	Int_t i = 0;
	Double_t Delta_Xe, Delta_Ye;

	if (fCorrPare->node_n <= 0) {// non nodes defined
		Delta_Xe = 0;
		Delta_Ye = 0;
	}
	else {
		while (Te > fCorrPare->t[i] && i < fCorrPare->node_n && i < 10) i++; // find the next larger node
		if (i == 0) {// left of all nodes
			Delta_Xe = fCorrPare->x[0];
			Delta_Ye = fCorrPare->y[0];
		}
		else if (i == min(fCorrPare->node_n, 10)) { //right of all nodes
			Delta_Xe = fCorrPare->x[min(fCorrPare->node_n, 10) - 1];
			Delta_Ye = fCorrPare->y[min(fCorrPare->node_n, 10) - 1];
		}
		else {//between two nodes
			if (fCorrPare->t[i] > fCorrPare->t[i - 1])
			{
				Delta_Xe = fCorrPare->x[i - 1] + (fCorrPare->x[i] - fCorrPare->x[i - 1])*(Te - fCorrPare->t[i - 1]) / (fCorrPare->t[i] - fCorrPare->t[i - 1]);
				Delta_Ye = fCorrPare->y[i - 1] + (fCorrPare->y[i] - fCorrPare->y[i - 1])*(Te - fCorrPare->t[i - 1]) / (fCorrPare->t[i] - fCorrPare->t[i - 1]);
			}
		}
	}
	TVector2 Delta(Delta_Xe, Delta_Ye);

	return Delta;
}
