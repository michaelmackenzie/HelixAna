//------------------------------------------------------------------------------
// analysis module used to study trigger parameter cuts
//-----------------------------------------------------------------------------
#include "HelixAna/ana/THelixAnaModule.hh"

namespace HelixAna {

  //-----------------------------------------------------------------------------
  THelixAnaModule::THelixAnaModule(const char* name, const char* title):
    TAnaModule(name,title)
  {
    fAprHelixBlockName      =  "HelixBlockAprHighP";
    fCprHelixBlockName      =  "HelixBlockCprDeHighP";
    fMergedHelixBlockName   =  "HelixBlockDe";
    fAprTrackBlockName      =  "TrackBlockAprHighP";
    fCprTrackBlockName      =  "TrackBlockCprDeHighP";
    fMergedTrackBlockName   =  "TrackBlockDe";
    fTriggerBlockName       =  "TriggerBlock";
  }

  //-----------------------------------------------------------------------------
  THelixAnaModule::~THelixAnaModule() {
  }

  //-----------------------------------------------------------------------------
  int THelixAnaModule::BeginJob() {

    //-----------------------------------------------------------------------------
    // register data blocks
    //-----------------------------------------------------------------------------
    RegisterDataBlock(fAprHelixBlockName,     "TStnHelixBlock"  , &fAprHelixBlock     );
    RegisterDataBlock(fCprHelixBlockName,     "TStnHelixBlock"  , &fCprHelixBlock     );
    RegisterDataBlock(fMergedHelixBlockName,  "TStnHelixBlock"  , &fMergedHelixBlock  );
    RegisterDataBlock(fAprTrackBlockName,     "TStnTrackBlock"  , &fAprTrackBlock     );
    RegisterDataBlock(fCprTrackBlockName,     "TStnTrackBlock"  , &fCprTrackBlock     );
    RegisterDataBlock(fMergedTrackBlockName,  "TStnTrackBlock"  , &fMergedTrackBlock  );
    RegisterDataBlock(fTriggerBlockName,      "TStnTriggerBlock", &fTriggerBlock      );

    //-----------------------------------------------------------------------------
    // book histograms
    //-----------------------------------------------------------------------------
    BookHistograms();

    return 0;
  }

  //-----------------------------------------------------------------------------
  void THelixAnaModule::BookHelixCompHistograms(HelixAna::HelixCompHist_t* Hist, const char* Folder) {

    HBook1F(Hist->fAprHelixDeltaP,"AprHelixDeltaP",Form("%s: AprHelixDeltaP",Folder), 200,  -20, 20, Folder);
    HBook1F(Hist->fAprTrackDeltaP,"AprTrackDeltaP",Form("%s: AprTrackDeltaP",Folder), 200,  -20, 20, Folder);
    HBook1F(Hist->fCprHelixDeltaP,"CprHelixDeltaP",Form("%s: CprHelixDeltaP",Folder), 200,  -20, 20, Folder);
    HBook1F(Hist->fCprTrackDeltaP,"CprTrackDeltaP",Form("%s: CprTrackDeltaP",Folder), 200,  -20, 20, Folder);
    HBook1F(Hist->fHelixDeltaP,"HelixDeltaP",Form("%s: HelixDeltaP",Folder), 200,  -20, 20, Folder);
    HBook1F(Hist->fTrackDeltaP,"TrackDeltaP",Form("%s: TrackDeltaP",Folder), 200,  -20, 20, Folder);
  }

  //-----------------------------------------------------------------------------
  void THelixAnaModule::FillHelixCompHistograms(HelixAna::HelixCompHist_t* Hist, HelixPair_t& Match) {
    // Fill APR helix info
    if(Match.fAprHelix) {
      const float MC_P = 104.97; //FIXME: Retrieve true particle momentum corresponding to the helix
      Hist->fAprHelixDeltaP->Fill(Match.fAprHelix->P() - MC_P);
    } else {
      Hist->fAprHelixDeltaP->Fill(-999.);
    }

    // Fill CPR helix info
    if(Match.fCprHelix) {
      const float MC_P = 104.97; //FIXME: Retrieve true particle momentum corresponding to the helix
      Hist->fCprHelixDeltaP->Fill(Match.fCprHelix->P() - MC_P);
    } else {
      Hist->fCprHelixDeltaP->Fill(-999.);
    }

    // Fill APR vs. CPR helix info
    if(Match.fAprHelix && Match.fCprHelix) {
      Hist->fHelixDeltaP->Fill(Match.fAprHelix->P() - Match.fCprHelix->P());
    } else {
      Hist->fCprHelixDeltaP->Fill(-999.);
    }
  }

  //_____________________________________________________________________________
  void THelixAnaModule::BookHistograms() {

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

    event_selection[0] = new TString("All events");
    event_selection[1] = new TString("Events with a matched helix");

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

    track_selection[0] = new TString("APR tracks");
    track_selection[1] = new TString("CPR tracks");
    track_selection[2] = new TString("Merged tracks");

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

    //-----------------------------------------------------------------------------
    // book helix histograms
    //-----------------------------------------------------------------------------
    TString* helix_selection[kNHelixHistSets];
    for (int i=0; i<kNHelixHistSets; i++) helix_selection[i] = 0;

    helix_selection[0] = new TString("APR helices");
    helix_selection[1] = new TString("CPR helices");
    helix_selection[2] = new TString("Merged helices");

    for (int i=0; i<kNHelixHistSets; i++) {
      if (helix_selection[i] != 0) {
        sprintf(folder_name,"hlx_%i",i);
        fol = (TFolder*) hist_folder->FindObject(folder_name);
        folder_title    = helix_selection[i]->Data();
        if (! fol) fol  = hist_folder->AddFolder(folder_name,folder_title);
        fHist.fHelix[i] = new HelixHist_t;
        BookHelixHistograms(fHist.fHelix[i],Form("Hist/%s",folder_name));
      }
    }

    //-----------------------------------------------------------------------------
    // book helix/track comparison histograms
    //-----------------------------------------------------------------------------
    TString* comp_selection[kNHelixCompHistSets];
    for (int i=0; i<kNHelixCompHistSets; i++) comp_selection[i] = 0;

    comp_selection[0] = new TString("All matched");
    comp_selection[1] = new TString("Good matched helices");

    for (int i=0; i<kNHelixCompHistSets; i++) {
      if (comp_selection[i] != 0) {
        sprintf(folder_name,"cmp_%i",i);
        fol = (TFolder*) hist_folder->FindObject(folder_name);
        folder_title    = comp_selection[i]->Data();
        if (! fol) fol  = hist_folder->AddFolder(folder_name,folder_title);
        fHist.fHelixComp[i] = new HelixCompHist_t;
        BookHelixCompHistograms(fHist.fHelixComp[i],Form("Hist/%s",folder_name));
      }
    }

  }

  //_____________________________________________________________________________
  int THelixAnaModule::BeginRun() {
    int rn = GetHeaderBlock()->RunNumber();
    TStntuple::Init(rn);
    return 0;
  }

  //_____________________________________________________________________________
  void THelixAnaModule::FillHistograms() {

    //-----------------------------------------------------------------------------
    // fill event histograms
    //-----------------------------------------------------------------------------
    FillEventHistograms(fHist.fEvent[0],&fEvtPar);
    if(fEvtPar.fNMatchedHelices > 0) FillEventHistograms(fHist.fEvent[1],&fEvtPar);

    //-----------------------------------------------------------------------------
    // fill apr track histograms
    //-----------------------------------------------------------------------------
    for (int i=0; i<fEvtPar.fNAprTracks; i++) {
      fTrack = fAprTrackBlock->Track(i);
      InitTrackPar(fTrack,&fTrkPar);
      FillTrackHistograms(fHist.fTrack[0],&fTrkPar);
    }

    //-----------------------------------------------------------------------------
    // fill cpr track histograms
    //-----------------------------------------------------------------------------
    for (int i=0; i<fEvtPar.fNCprTracks; i++) {
      fTrack = fCprTrackBlock->Track(i);
      InitTrackPar(fTrack,&fTrkPar);
      FillTrackHistograms(fHist.fTrack[1],&fTrkPar);
    }

    //-----------------------------------------------------------------------------
    // fill merged track histograms
    //-----------------------------------------------------------------------------
    for (int i=0; i<fEvtPar.fNTracks; i++) {
      fTrack = fMergedTrackBlock->Track(i);
      InitTrackPar(fTrack,&fTrkPar);
      FillTrackHistograms(fHist.fTrack[2],&fTrkPar);
    }

    //-----------------------------------------------------------------------------
    // fill apr helix histograms
    //-----------------------------------------------------------------------------
    for (int i=0; i<fEvtPar.fNAprHelices; i++) {
      fHelix = fAprHelixBlock->Helix(i);
      InitHelixPar(fHelix,&fHlxPar);
      FillHelixHistograms(fHist.fHelix[0],&fHlxPar);
    }

    //-----------------------------------------------------------------------------
    // fill cpr helix histograms
    //-----------------------------------------------------------------------------
    for (int i=0; i<fEvtPar.fNCprHelices; i++) {
      fHelix = fCprHelixBlock->Helix(i);
      InitHelixPar(fHelix,&fHlxPar);
      FillHelixHistograms(fHist.fHelix[1],&fHlxPar);
    }

    //-----------------------------------------------------------------------------
    // fill merged helix histograms
    //-----------------------------------------------------------------------------
    for (int i=0; i<fEvtPar.fNMergedHelices; i++) {
      fHelix = fMergedHelixBlock->Helix(i);
      InitHelixPar(fHelix,&fHlxPar);
      FillHelixHistograms(fHist.fHelix[2],&fHlxPar);
    }

    //-----------------------------------------------------------------------------
    // fill helix/track comparison histograms
    //-----------------------------------------------------------------------------
    for(int imatch = 0; imatch < fEvtPar.fNMatchedHelices; ++imatch) {
      FillHelixCompHistograms(fHist.fHelixComp[0],fMatchedHelices[imatch]);
    }

  }

  //-----------------------------------------------------------------------------
  int THelixAnaModule::Event(int ientry) {

    // get entry for the ith event for each data block
    fAprHelixBlock->GetEntry(ientry);
    fCprHelixBlock->GetEntry(ientry);
    fMergedHelixBlock->GetEntry(ientry);
    fAprTrackBlock->GetEntry(ientry);
    fCprTrackBlock->GetEntry(ientry);
    fMergedTrackBlock->GetEntry(ientry);
    fTriggerBlock->GetEntry(ientry);

    // get/set event parameters
    fEvtPar.fInstLum  = GetHeaderBlock()->fInstLum;
    fEvtPar.fNAprHelices = fAprHelixBlock->NHelices();
    fEvtPar.fNCprHelices = fCprHelixBlock->NHelices();
    fEvtPar.fNAprTracks = fAprTrackBlock->NTracks();
    fEvtPar.fNCprTracks = fCprTrackBlock->NTracks();
    fEvtPar.fNTracks = fMergedTrackBlock->NTracks();
    fEvtPar.fPassedCprPath = fTriggerBlock->PathPassed(150);
    fEvtPar.fPassedAprPath = fTriggerBlock->PathPassed(180);

    // perform helix matching
    MatchHelices();

    FillHistograms();

    return 0;
  }

  //_____________________________________________________________________________
  bool THelixAnaModule::CompareHelices(TStnHelix* h1, TStnHelix* h2) {
    // printf("THelixAnaModule::%s: Function not yet implemented\n", __func__);
    if(!h1 || !h2) return false;
    // FIXME: Implement a better matching algorithm
    // For now, just check if they're close in time and space

    bool matched(true);
    matched &= std::fabs(h1->T0() - h2->T0()) / std::sqrt(std::pow(h1->T0Err(),2) + std::pow(h2->T0Err(),2)) < 2.;
    matched &= std::fabs(h1->CenterX() - h2->CenterX()) < 50.;
    matched &= std::fabs(h1->CenterY() - h2->CenterY()) < 50.;
    matched &= h1->PropDir() == h2->PropDir();

    return matched;
  }

  //_____________________________________________________________________________
  void THelixAnaModule::MatchHelices() {
    fEvtPar.fNMatchedHelices = 0;

    // loop through all helices, identifying helices in both collections
    for(int iapr = 0; iapr < fEvtPar.fNAprHelices; ++iapr) {
      for(int icpr = 0; icpr < fEvtPar.fNCprHelices; ++icpr) {
        // check if the two helices match
        auto apr_helix = fAprHelixBlock->Helix(iapr);
        auto cpr_helix = fCprHelixBlock->Helix(iapr);
        if(CompareHelices(apr_helix, cpr_helix)) {
          ++fEvtPar.fNMatchedHelices;
          if(fEvtPar.fNMatchedHelices > kMaxHelices) {
            throw std::runtime_error(Form("THelixAnaModule::%s: Exceeded max helix matches (%i matched, %i APR, %i CPR)\n",
                                          __func__, fEvtPar.fNMatchedHelices, fEvtPar.fNAprHelices, fEvtPar.fNCprHelices));
          }
          const int index = fEvtPar.fNMatchedHelices - 1;
          fMatchedHelices[index].fAprHelix = apr_helix;
          fMatchedHelices[index].fCprHelix = cpr_helix;
          //FIXME: Get the corresponding tracks if they exist
          //FIXME: Get the corresponding merged helix/track collection entries if they exist
        }
      }
    }
  }

  //_____________________________________________________________________________
  int THelixAnaModule::EndJob() {
    printf("----- end job: ---- %s\n",GetName());
    return 0;
  }

}
