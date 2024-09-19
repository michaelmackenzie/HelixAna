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
  
}

//-----------------------------------------------------------------------------
void TAnaModule::BookTrackHistograms(TrackHist_t* Hist, const char* Folder) {

  HBook1F(Hist->fP,"p",Form("%s: track momentum",Folder), 300,  0.,300,Folder);
  
}

//-----------------------------------------------------------------------------
void TAnaModule::FillEventHistograms(EventHist_t* Hist, EventPar_t* EvtPar) {

  Hist->fInstLumi->Fill(EvtPar->fInstLum);
  if (EvtPar->fPassedAprPath) { Hist->fInstLumiApr->Fill(EvtPar->fInstLum); }
  if (EvtPar->fPassedCprPath) { Hist->fInstLumiCpr->Fill(EvtPar->fInstLum); }
  if (EvtPar->fPassedAprPath || EvtPar->fPassedCprPath) { Hist->fInstLumiAprCpr->Fill(EvtPar->fInstLum); }
  Hist->fNAprTracks->Fill(EvtPar->fNAprTracks);
  Hist->fNCprTracks->Fill(EvtPar->fNCprTracks);
  
}

//-----------------------------------------------------------------------------
void TAnaModule::FillTrackHistograms(TrackHist_t* Hist, TrackPar_t* TrkPar) {

  Hist->fP->Fill(TrkPar->fTrack->fP);
  
}

void TAnaModule::InitTrackPar(TStnTrack* Trk, ePlus2024::TrackPar_t* TrkPar) {

  TrkPar->fTrack = Trk;

}

}
