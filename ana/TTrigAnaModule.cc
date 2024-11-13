//------------------------------------------------------------------------------
// analysis module used to study trigger parameter cuts 
//-----------------------------------------------------------------------------
#include "ePlus2024/ana/TTrigAnaModule.hh"

namespace ePlus2024 {

//-----------------------------------------------------------------------------
TTrigAnaModule::TTrigAnaModule(const char* name, const char* title):
  TAnaModule(name,title)
{
  fAprTrackBlockName      =  "TrackBlockAprHighP";
  fCprTrackBlockName      =  "TrackBlockCprDeHighP";
  fOfflineTrackBlockName  =  "TrackBlockDe";
  fTriggerBlockName       =  "TriggerBlock";
}

//-----------------------------------------------------------------------------
TTrigAnaModule::~TTrigAnaModule() {
}

//-----------------------------------------------------------------------------
int TTrigAnaModule::BeginJob() {
  
  //-----------------------------------------------------------------------------
  // register data blocks
  //-----------------------------------------------------------------------------
  RegisterDataBlock(fAprTrackBlockName,     "TStnTrackBlock"  , &fAprTrackBlock     );
  RegisterDataBlock(fCprTrackBlockName,     "TStnTrackBlock"  , &fCprTrackBlock     );
  RegisterDataBlock(fOfflineTrackBlockName, "TStnTrackBlock"  , &fOfflineTrackBlock );
  RegisterDataBlock(fTriggerBlockName,      "TStnTriggerBlock", &fTriggerBlock      );

  //-----------------------------------------------------------------------------
  // book histograms
  //-----------------------------------------------------------------------------
  BookHistograms();

  return 0;
}

//_____________________________________________________________________________
void TTrigAnaModule::BookHistograms() {
  
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
  event_selection[1] = new TString("events where valid Offline track exists");

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

  //-----------------------------------------------------------------------------
  // book track histograms
  //-----------------------------------------------------------------------------
  TString* track_selection[kNTrackHistSets];
  for (int i=0; i<kNTrackHistSets; i++) track_selection[i] = 0;

  track_selection[0] = new TString("apr tracks");
  track_selection[1] = new TString("cpr tracks");
  track_selection[2] = new TString("Offline tracks");
  track_selection[3] = new TString("Best Offline track if track is good");
  track_selection[4] = new TString("Best Offline track if track is good and apr or cpr triggered");
  track_selection[5] = new TString("Best Offline track if track is good and neither apr or cpr trigger");
  
  for (int i=0; i<kNTrackHistSets; i++) {
    if (track_selection[i] != 0) {
      sprintf(folder_name,"trk_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      folder_title    = track_selection[i]->Data();
      if (! fol) fol  = hist_folder->AddFolder(folder_name,folder_title);
      fHist.fTrack[i] = new TrackHist_t;
      BookTrackHistograms(fHist.fTrack[i],Form("Hist/%s",folder_name));
    }
  }

}

//_____________________________________________________________________________
int TTrigAnaModule::BeginRun() {
  int rn = GetHeaderBlock()->RunNumber();
  TStntuple::Init(rn);
  return 0;
}

//_____________________________________________________________________________
void TTrigAnaModule::FillHistograms() {

  //-----------------------------------------------------------------------------
  // 1. fill event histograms
  //-----------------------------------------------------------------------------
  FillEventHistograms(fHist.fEvent[0],&fEvtPar);
  if (fGoodOfflineTrackExists) FillEventHistograms(fHist.fEvent[1],&fEvtPar);

  //-----------------------------------------------------------------------------
  // 2. fill apr track histograms
  //-----------------------------------------------------------------------------
  for (int i=0; i<fEvtPar.fNAprTracks; i++) {
    fTrack = fAprTrackBlock->Track(i);
    InitTrackPar(fTrack,&fTrkPar);
    FillTrackHistograms(fHist.fTrack[0],&fTrkPar);
  }

  //-----------------------------------------------------------------------------
  // 2. fill cpr track histograms
  //-----------------------------------------------------------------------------
  for (int i=0; i<fEvtPar.fNCprTracks; i++) {
    fTrack = fCprTrackBlock->Track(i);
    InitTrackPar(fTrack,&fTrkPar);
    FillTrackHistograms(fHist.fTrack[1],&fTrkPar);
  }

  //-----------------------------------------------------------------------------
  // 3. fill offline track histograms
  //-----------------------------------------------------------------------------
  for (int i=0; i<fEvtPar.fNTracks; i++) {
    fTrack = fOfflineTrackBlock->Track(i);
    InitTrackPar(fTrack,&fTrkPar);
    FillTrackHistograms(fHist.fTrack[2],&fTrkPar);
    // fill folder 3 with best offline track, if a good one exists
    if (fGoodOfflineTrackExists && i==fGoodOfflineTrackIndex) {
      FillTrackHistograms(fHist.fTrack[3],&fTrkPar);
      // fill folder 4 with best offline track if apr or cpr triggered
      if (fEvtPar.fPassedCprPath || fEvtPar.fPassedAprPath) {
        FillTrackHistograms(fHist.fTrack[4],&fTrkPar);
      }
      // fill folder 5 with best offline track when neither apr or cpr trigger
      if (!fEvtPar.fPassedCprPath && !fEvtPar.fPassedAprPath) {
        FillTrackHistograms(fHist.fTrack[5],&fTrkPar);
      }
    }
  }
  
}

//-----------------------------------------------------------------------------
bool TTrigAnaModule::GoodOfflineTrackExists() {

  // specify momentum range and particle charge
  int   charge = 1;
  float minP   = 90.0;
  float maxP   = 95.0;

  // specify minimum number of active hits
  int nActiveMin = 20;

  // check that at least one track exists to begin with
  if (fEvtPar.fNTracks == 0) return false;

  // check that a track matches some criteria to be considered "good"
  // TODO : make selection cuts more precise
  for (int i=0; i<fEvtPar.fNTracks; i++) {
    fTrack = fOfflineTrackBlock->Track(i);
    if (fTrack->NActive() < nActiveMin) continue;
    if (fTrack->fCharge != charge) continue;
    if (fTrack->fP < minP) continue;
    if (fTrack->fP > maxP) continue;
    fGoodOfflineTrackIndex = i; // set the index of this good track
    return true;
  }

  // if no good track was found above then we return false
  return false;

}

//-----------------------------------------------------------------------------
int TTrigAnaModule::Event(int ientry) {

  // get entry for the ith event for each data block
  fAprTrackBlock->GetEntry(ientry);
  fCprTrackBlock->GetEntry(ientry);
  fOfflineTrackBlock->GetEntry(ientry);
  fTriggerBlock->GetEntry(ientry);

  // get/set event parameters
  fEvtPar.fInstLum  = GetHeaderBlock()->fInstLum;
  fEvtPar.fNAprTracks = fAprTrackBlock->NTracks();
  fEvtPar.fNCprTracks = fCprTrackBlock->NTracks();
  fEvtPar.fNTracks = fOfflineTrackBlock->NTracks();
  fEvtPar.fPassedCprPath = false;
  fEvtPar.fPassedAprPath = false;
  if (fTriggerBlock->PathPassed(150)) { fEvtPar.fPassedCprPath = true; }
  if (fTriggerBlock->PathPassed(180)) { fEvtPar.fPassedAprPath = true; }

  // determine whether a good offline track exists
  fGoodOfflineTrackExists = GoodOfflineTrackExists();
  
  FillHistograms();

  return 0;		       
}

//_____________________________________________________________________________
int TTrigAnaModule::EndJob() {
  printf("----- end job: ---- %s\n",GetName());
  return 0;
}

}
