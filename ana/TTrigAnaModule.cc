//------------------------------------------------------------------------------
// analysis module used to study trigger parameter cuts 
//-----------------------------------------------------------------------------
#include "ePlus2024/ana/TTrigAnaModule.hh"

namespace ePlus2024 {

//-----------------------------------------------------------------------------
TTrigAnaModule::TTrigAnaModule(const char* name, const char* title):
  TAnaModule(name,title)
{
  fTrackBlockNameDe   =  "TrackSeedBlockApr";
}

//-----------------------------------------------------------------------------
TTrigAnaModule::~TTrigAnaModule() {
}

//-----------------------------------------------------------------------------
int TTrigAnaModule::BeginJob() {
  
  //-----------------------------------------------------------------------------
  // register data blocks
  //-----------------------------------------------------------------------------
  RegisterDataBlock(fTrackBlockNameDe, "TStnTrackBlock", &fTrackBlockDe); 

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

  track_selection[0] = new TString("all tracks");

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

  //-----------------------------------------------------------------------------
  // 2. fill track histograms
  //-----------------------------------------------------------------------------
  for (int i=0; i<fEvtPar.fNTracksDe; i++) {
    fTrack = fTrackBlockDe->Track(i);
    InitTrackPar(fTrack,fTrkPar);
    FillTrackHistograms(fHist.fTrack[0],fTrkPar);
  }
  
}

//-----------------------------------------------------------------------------
int TTrigAnaModule::Event(int ientry) {

  // get the track block entry for the ith event
  fTrackBlockDe->GetEntry(ientry);

  // get/set event parameters
  fEvtPar.fInstLum  = GetHeaderBlock()->fInstLum;
  fEvtPar.fNTracksDe = fTrackBlockDe->NTracks();

  FillHistograms();

  return 0;		       
}


//_____________________________________________________________________________
int TTrigAnaModule::EndJob() {
  printf("----- end job: ---- %s\n",GetName());
  return 0;
}

}
