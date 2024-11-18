#include "TF1.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TEnv.h"
#include "TSystem.h"

#include "Stntuple/base/TStnDataset.hh"
#include "Stntuple/loop/TStnInputModule.hh"
#include "Stntuple/loop/TStnAna.hh"

#include "Stntuple/obj/TStnNode.hh"
#include "Stntuple/obj/TStnHeaderBlock.hh"
#include "Stntuple/alg/TStntuple.hh"
#include "Stntuple/geom/TCrvNumerology.hh"
#include "Stntuple/val/stntuple_val_functions.hh"
//------------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
#include "ePlus2024/ana/TAnaModule.hh"
ClassImp(ePlus2024::TAnaModule)

using std::vector;

using namespace ePlus2024;

namespace ePlus2024 {

//-----------------------------------------------------------------------------
TAnaModule::TAnaModule(const char* name, const char* title):
  TStnModule(name,title)
{
 
//-----------------------------------------------------------------------------
// TStntuple singleton
//-----------------------------------------------------------------------------
  fStnt      = TStntuple::Instance();

}

//-----------------------------------------------------------------------------
TAnaModule::~TAnaModule() {}


//-----------------------------------------------------------------------------
// common initializations
//-----------------------------------------------------------------------------
int TAnaModule::BeginJob() {
  return 0;
}

//-----------------------------------------------------------------------------
// common initializations
// assume that the input dataset is initialized. 
//-----------------------------------------------------------------------------
int TAnaModule::BeginRun() {

  //TStnDataset* ds = GetAna()->GetInputModule()->GetDataset(0);

  int rn = GetHeaderBlock()->RunNumber();
  TStntuple::Init(rn);
  
  return 0;
}

//-----------------------------------------------------------------------------
void TAnaModule::BookEventHistograms(EventHist_t* Hist, const char* Folder) {

  HBook1F(Hist->fInstLumi,"inst_lumi",Form("%s: POT",Folder), 300,  0.0, 1.5e8, Folder);
  HBook1F(Hist->fInstLumiApr,"inst_lumi_apr",Form("%s: POT",Folder), 300,  0.0, 1.5e8, Folder);
  HBook1F(Hist->fInstLumiCpr,"inst_lumi_cpr",Form("%s: POT",Folder), 300,  0.0, 1.5e8, Folder);
  HBook1F(Hist->fInstLumiAprCpr,"inst_lumi_apr_cpr",Form("%s: POT",Folder), 300,  0.0, 1.5e8, Folder);
  HBook1F(Hist->fNAprTracks,"nTracksApr",Form("%s: nTracksApr",Folder), 50, 0.0, 50.0, Folder);
  HBook1F(Hist->fNCprTracks,"nTracksCpr",Form("%s: nTracksCpr",Folder), 50, 0.0, 50.0, Folder);
  HBook1F(Hist->fNTracks,"nTracks",Form("%s: nTracks",Folder), 50, 0.0, 50.0, Folder);

}

//-----------------------------------------------------------------------------
void TAnaModule::BookTrackHistograms(TrackHist_t* Hist, const char* Folder) {

  HBook1F(Hist->fP,"p",Form("%s: track momentum",Folder), 600,  -300.0, 300.0, Folder);
  HBook1F(Hist->fPt,"pt",Form("%s: track transverse momentum",Folder), 600,  -300.0, 300.0, Folder);
  HBook1F(Hist->fD0,"d0",Form("%s: track d0",Folder), 200, -200.0, 200.0, Folder);
  HBook1F(Hist->fDP,"dP",Form("%s: track p_reco - p_mc",Folder), 400, -200.0, 200.0, Folder);
  HBook1F(Hist->fChi2NDof,"chi2NDof",Form("%s: track chi2/ndof",Folder), 200, 0.0, 10.0, Folder);
  HBook1F(Hist->fTanDip,"tanDip",Form("%s: track tanDip",Folder), 200,  0.0, 2.0, Folder);
  HBook1F(Hist->fRadius,"radius",Form("%s: track radius",Folder), 1000,  0.0, 1000, Folder);
  HBook1F(Hist->fRMax,"rMax",Form("%s: track rMax",Folder), 2000,  0.0, 2000, Folder);
  HBook1F(Hist->fNActive,"nActive",Form("%s: nHits used in fit",Folder), 150, 0.0, 150.0, Folder);
  
}

//-----------------------------------------------------------------------------
void TAnaModule::FillEventHistograms(EventHist_t* Hist, EventPar_t* EvtPar) {

  Hist->fInstLumi->Fill(EvtPar->fInstLum);
  if (EvtPar->fPassedAprPath) { Hist->fInstLumiApr->Fill(EvtPar->fInstLum); }
  if (EvtPar->fPassedCprPath) { Hist->fInstLumiCpr->Fill(EvtPar->fInstLum); }
  if (EvtPar->fPassedAprPath || EvtPar->fPassedCprPath) { Hist->fInstLumiAprCpr->Fill(EvtPar->fInstLum); }
  Hist->fNAprTracks->Fill(EvtPar->fNAprTracks);
  Hist->fNCprTracks->Fill(EvtPar->fNCprTracks);
  Hist->fNTracks->Fill(EvtPar->fNTracks);
  
}

//-----------------------------------------------------------------------------
void TAnaModule::FillTrackHistograms(TrackHist_t* Hist, TrackPar_t* TrkPar) {

  Hist->fP->Fill((TrkPar->fTrack->fP)*(TrkPar->fTrack->fCharge));
  Hist->fPt->Fill((TrkPar->fTrack->fPt)*(TrkPar->fTrack->fCharge));
  Hist->fD0->Fill(TrkPar->fTrack->fD0);
  Hist->fDP->Fill((TrkPar->fTrack->fP)-(TrkPar->fTrack->fPFront));
  Hist->fChi2NDof->Fill(TrkPar->fTrack->Chi2Dof());
  Hist->fTanDip->Fill(TrkPar->fTrack->fTanDip);
  Hist->fRadius->Fill(TrkPar->fRadius);
  Hist->fRMax->Fill(TrkPar->fRMax);
  Hist->fNActive->Fill(TrkPar->fTrack->NActive());
  
}

void TAnaModule::InitTrackPar(TStnTrack* Trk, ePlus2024::TrackPar_t* TrkPar) {
  
  // set pointer to TStnTrack
  TrkPar->fTrack = Trk;

  // compute parameters not contained in TStnTrack
  TrkPar->fRadius = Trk->fPt / (mmTconversion*bz0);
  TrkPar->fRMax = Trk->fD0 + 2*TrkPar->fRadius;
  
}

}
