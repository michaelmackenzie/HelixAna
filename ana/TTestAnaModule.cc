#include "TF1.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TEnv.h"
#include "TSystem.h"

#include "Stntuple/loop/TStnAna.hh"
#include "Stntuple/obj/TStnNode.hh"
#include "Stntuple/obj/TStnHeaderBlock.hh"
#include "Stntuple/alg/TStntuple.hh"
#include "Stntuple/geom/TDisk.hh"
#include "Stntuple/val/stntuple_val_functions.hh"
//------------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
#include "ePlus2024/analysisModules/include/TTestAnaModule.hh"

namespace su2020 {

//-----------------------------------------------------------------------------
TTestAnaModule::TTestAnaModule(const char* name, const char* title):
  TAnaModule(name,title)
{
}

//-----------------------------------------------------------------------------
TTestAnaModule::~TTestAnaModule() {
}

//-----------------------------------------------------------------------------
int TTestAnaModule::BeginJob() {

//-----------------------------------------------------------------------------
// book histograms
//-----------------------------------------------------------------------------
  BookHistograms();

  return 0;
}

//_____________________________________________________________________________
void TTestAnaModule::BookHistograms() {

  //  char name [200];
  //  char title[200];
  
  TFolder*    fol;
  TFolder*    hist_folder;
  char        folder_name[200];
  const char* folder_title;
  
  DeleteHistograms();
  hist_folder = (TFolder*) GetFolder()->FindObject("Hist");
  
//-----------------------------------------------------------------------------
// book event histograms
//-----------------------------------------------------------------------------
  TString* event_selection[kNEventHistSets];
  for (int i=0; i<kNEventHistSets; i++) event_selection[i] = 0;

  event_selection[0] = new TString("all events");

  for (int i=0; i<kNEventHistSets; i++) {
    if (event_selection[i] != 0) {
      sprintf(folder_name,"evt_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      folder_title    = event_selection[i]->Data();
      if (! fol) fol  = hist_folder->AddFolder(folder_name,folder_title);
      fHist.fEvent[i] = new EventHist_t;
      BookEventHistograms(fHist.fEvent[i],Form("Hist/%s",folder_name));
    }
  }

}

//_____________________________________________________________________________
int TTestAnaModule::BeginRun() {
  int rn = GetHeaderBlock()->RunNumber();
  TStntuple::Init(rn);
  return 0;
}

//_____________________________________________________________________________
void TTestAnaModule::FillHistograms() {

//-----------------------------------------------------------------------------
// 1. fill event histograms
//-----------------------------------------------------------------------------
  FillEventHistograms(fHist.fEvent[0],&fEvtPar);
  
}

//-----------------------------------------------------------------------------
int TTestAnaModule::Event(int ientry) {

  fEvtPar.fCutCounter[0] = 0;
  fEvtPar.fCutCounter[1] = 0;
  fEvtPar.fCutCounter[2] = 0;
  fEvtPar.fCutCounter[3] = 0;

  TLorentzVector        mom;

  //  TDiskCalorimeter::GeomData_t disk_geom;

  fGenpBlock->GetEntry(ientry);
  fSimpBlock->GetEntry(ientry);
  fSpmcBlockVDet->GetEntry(ientry);

  fCrvClusterBlock->GetEntry(ientry);
  fCrvPulseBlock->GetEntry(ientry);

  fEventWeight              = 1.;
  fEvtPar.fDioLOWt          = 1.;
  fEvtPar.fDioLLWt          = 1.;

  fEvtPar.fNCrvClusters     = fCrvClusterBlock->NClusters();
  fEvtPar.fNCrvPulses       = fCrvPulseBlock->NPulses();
  fEvtPar.fNCrvCoincidences = fCrvPulseBlock->NCoincidences();
//-----------------------------------------------------------------------------
// assume electron in the first particle, otherwise the logic will need to 
// be changed
//-----------------------------------------------------------------------------
  fEvtPar.fNGenp          = fGenpBlock->NParticles();
  fEvtPar.fNStrawHits     = GetHeaderBlock()->fNStrawHits;
  fEvtPar.fInstLum        = GetHeaderBlock()->fInstLum;
  fEvtPar.fOneBatchWeight = BatchModeWeight(fEvtPar.fInstLum, 1); //1 batch mode
  fEvtPar.fTwoBatchWeight = BatchModeWeight(fEvtPar.fInstLum, 2); //2 batch mode
  fEvtPar.fOneBatchBeamWeight  = BeamProcessWeight(fEvtPar.fInstLum, 1); //beam process weight for 1 batch mode
  fEvtPar.fTwoBatchBeamWeight  = BeamProcessWeight(fEvtPar.fInstLum, 2); //beam process weight for 2 batch mode
//-----------------------------------------------------------------------------
// MC generator info
//-----------------------------------------------------------------------------
  fEvtPar.fParticle = NULL;
  for (int i=fEvtPar.fNGenp-1; i>=0; i--) {
    TGenParticle* genp = fGenpBlock->Particle(i);
    int pdg_code       = genp->GetPdgCode();
    int generator_code = genp->GetStatusCode();
    if ((abs(pdg_code) == fPDGCode) && (generator_code == fMCProcessCode)) {
      fEvtPar.fParticle = genp;
      break;
    }
  }
//-----------------------------------------------------------------------------
// may want to revisit the definition of fSimp and remove overlaps with fEvtPar
//-----------------------------------------------------------------------------
  fSimPar.fParticle = fSimpBlock->Particle(0);
  fSimPar.fVD10     = NULL;
  fSimPar.fTFront   = NULL;
  fSimPar.fTMid     = NULL;
  fSimPar.fTBack    = NULL;
  fSimPar.fGenp     = fEvtPar.fParticle;

  if (fSimPar.fGenp) fSimPar.fEleE = fSimPar.fGenp->Energy();
  else               fSimPar.fEleE = -1;
//-----------------------------------------------------------------------------
// virtual detectors - for fSimp need parameters at the tracker front
//-----------------------------------------------------------------------------
  int nsteps = fSpmcBlockVDet->NStepPoints();
  
  if (fSimPar.fParticle) {
    for (int i=0; i<nsteps; i++) {
      TStepPointMC* step = fSpmcBlockVDet->StepPointMC(i);
      if (step->PDGCode() == fSimPar.fParticle->fPdgCode) {
	if ((step->VolumeID() == 13) || (step->VolumeID() == 14)) {
	  fSimPar.fTFront = step;
	}
	else if ((step->VolumeID() == 11) || (step->VolumeID() == 12)) {
	  fSimPar.fTMid = step;
	}
      }
    }
  }

  fEvtPar.fNTracksDe   = 0;
  fEvtPar.fNGoodTracks = 0;

  InitCrvStubPar(fCrvClusterBlock, fCrvStubPar);

  FillHistograms();

  Debug();

  return 0;		       
}


//_____________________________________________________________________________
int TTestAnaModule::EndJob() {
  printf("----- end job: ---- %s\n",GetName());
  return 0;
}

}
