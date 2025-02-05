//------------------------------------------------------------------------------
// analysis module used to study RMC pair conversions
//-----------------------------------------------------------------------------
#include "HelixAna/ana/TRMCConvAnaModule.hh"

namespace HelixAna {

  //-----------------------------------------------------------------------------
  TRMCConvAnaModule::TRMCConvAnaModule(const char* name, const char* title):
    TAnaModule(name,title)
  {
    fAprHelixBlockName      =  "HelixBlockAprHighP";
    fCprHelixBlockName      =  "HelixBlockCprDeHighP";
    fOfflineHelixBlockName  =  "HelixBlockDe";
    fAprTrackBlockName      =  "TrackBlockAprHighP";
    fCprTrackBlockName      =  "TrackBlockCprDeHighP";
    fOfflineTrackBlockName  =  "TrackBlockDe";
    fTriggerBlockName       =  "TriggerBlock";
    fGenpBlockName          =  "GenpBlock";
    fSimpBlockName          =  "SimpBlock";
  }

  //-----------------------------------------------------------------------------
  TRMCConvAnaModule::~TRMCConvAnaModule() {
  }

  //-----------------------------------------------------------------------------
  int TRMCConvAnaModule::BeginJob() {

    //-----------------------------------------------------------------------------
    // register data blocks
    //-----------------------------------------------------------------------------
    RegisterDataBlock(fAprHelixBlockName,     "TStnHelixBlock"  , &fAprHelixBlock     );
    RegisterDataBlock(fCprHelixBlockName,     "TStnHelixBlock"  , &fCprHelixBlock     );
    RegisterDataBlock(fOfflineHelixBlockName, "TStnHelixBlock"  , &fOfflineHelixBlock );
    RegisterDataBlock(fAprTrackBlockName,     "TStnTrackBlock"  , &fAprTrackBlock     );
    RegisterDataBlock(fCprTrackBlockName,     "TStnTrackBlock"  , &fCprTrackBlock     );
    RegisterDataBlock(fOfflineTrackBlockName, "TStnTrackBlock"  , &fOfflineTrackBlock );
    RegisterDataBlock(fTriggerBlockName,      "TStnTriggerBlock", &fTriggerBlock      );
    RegisterDataBlock(fGenpBlockName,         "TGenpBlock"      , &fGenpBlock         );
    RegisterDataBlock(fSimpBlockName,         "TSimpBlock"      , &fSimpBlock         );

    //-----------------------------------------------------------------------------
    // book histograms
    //-----------------------------------------------------------------------------
    BookHistograms();

    return 0;
  }

  //-----------------------------------------------------------------------------
  void TRMCConvAnaModule::BookRMCHistograms(HelixAna::RMCHist_t* Hist, const char* Folder) {

    HBook1F(Hist->fPairRecMom    ,"rec_pair_mom"   ,Form("%s: Rec. pair momentum"    ,Folder), 200,  0., 200., Folder);
    HBook1F(Hist->fPhotonEnergy  ,"photon_energy"  ,Form("%s: Photon energy"         ,Folder), 200, 50., 150., Folder);
    HBook1F(Hist->fPositronEnergy,"positron_energy",Form("%s: Positron energy"       ,Folder), 220,  0., 110., Folder);
    HBook1F(Hist->fElectronEnergy,"electron_energy",Form("%s: Electron energy"       ,Folder), 220,  0., 110., Folder);
    HBook1F(Hist->fPhotonRadius  ,"photon_radius"  ,Form("%s: Photon radius"         ,Folder), 200,  0., 800., Folder);
    HBook1F(Hist->fPhotonCosZ    ,"photon_cosz"    ,Form("%s: Photon cos(#theta_{z})",Folder), 200, -1.,   1., Folder);
  }

  //-----------------------------------------------------------------------------
  void TRMCConvAnaModule::BookHelixHistograms(HelixHist_t* Hist, const char* Folder) {
    TAnaModule::BookHelixHistograms(Hist, Folder);
  }

  //-----------------------------------------------------------------------------
  void TRMCConvAnaModule::FillRMCHistograms(HelixAna::RMCHist_t* Hist) {
    Hist->fPairRecMom    ->Fill(fRMCPar.fRecPairMom    );
    Hist->fPhotonEnergy  ->Fill(fRMCPar.fPhotonEnergy  );
    Hist->fPositronEnergy->Fill(fRMCPar.fPositronEnergy);
    Hist->fElectronEnergy->Fill(fRMCPar.fElectronEnergy);
    Hist->fPhotonRadius  ->Fill(fRMCPar.fPhotonRadius  );
    Hist->fPhotonCosZ    ->Fill(fRMCPar.fPhotonCosZ    );
  }

  //-----------------------------------------------------------------------------
  void TRMCConvAnaModule::FillHelixHistograms(HelixHist_t* Hist, HelixPar_t* HlxPar) {
    TAnaModule::FillHelixHistograms(Hist, HlxPar);
  }

  //_____________________________________________________________________________
  void TRMCConvAnaModule::BookHistograms() {

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
    for (int i=0; i<kNEventHistSets; i++) event_selection[i] = nullptr;

    event_selection[0] = new TString("All events");
    event_selection[1] = new TString("Events with a matched helix");

    for (int i=0; i<kNEventHistSets; i++) {
      if (event_selection[i]) {
        sprintf(folder_name,"evt_%i",i);
        fol = (TFolder*) hist_folder->FindObject(folder_name);
        folder_title    = event_selection[i]->Data();
        if (! fol) fol  = hist_folder->AddFolder(folder_name,folder_title);
        fHist.fEvent[i] = new EventHist_t;
        BookEventHistograms(fHist.fEvent[i],Form("Hist/%s",folder_name));
      }
    }

    //-----------------------------------------------------------------------------
    // book genp histograms
    //-----------------------------------------------------------------------------
    TString* genp_selection[kNGenpHistSets];
    for (int i=0; i<kNGenpHistSets; i++) genp_selection[i] = nullptr;

    genp_selection[0] = new TString("All gen partilces");

    for (int i=0; i<kNGenpHistSets; i++) {
      if (genp_selection[i]) {
        sprintf(folder_name,"gen_%i",i);
        fol = (TFolder*) hist_folder->FindObject(folder_name);
        folder_title    = genp_selection[i]->Data();
        if (! fol) fol  = hist_folder->AddFolder(folder_name,folder_title);
        fHist.fGenp[i] = new GenpHist_t;
        BookGenpHistograms(fHist.fGenp[i],Form("Hist/%s",folder_name));
      }
    }

    //-----------------------------------------------------------------------------
    // book simp histograms
    //-----------------------------------------------------------------------------
    TString* simp_selection[kNSimpHistSets];
    for (int i=0; i<kNSimpHistSets; i++) simp_selection[i] = nullptr;

    simp_selection[0] = new TString("All sim partilces");

    for (int i=0; i<kNSimpHistSets; i++) {
      if (simp_selection[i]) {
        sprintf(folder_name,"sim_%i",i);
        fol = (TFolder*) hist_folder->FindObject(folder_name);
        folder_title    = simp_selection[i]->Data();
        if (! fol) fol  = hist_folder->AddFolder(folder_name,folder_title);
        fHist.fSimp[i] = new SimpHist_t;
        BookSimpHistograms(fHist.fSimp[i],Form("Hist/%s",folder_name));
      }
    }

    //-----------------------------------------------------------------------------
    // book track histograms
    //-----------------------------------------------------------------------------
    TString* track_selection[kNTrackHistSets];
    for (int i=0; i<kNTrackHistSets; i++) track_selection[i] = nullptr;

    track_selection[0] = new TString("APR tracks");
    track_selection[1] = new TString("CPR tracks");
    track_selection[2] = new TString("Offline tracks");

    track_selection[50] = new TString("APR, track ID");
    track_selection[60] = new TString("CPR, track ID");
    track_selection[70] = new TString("Offline, track ID");

    for (int i=0; i<kNTrackHistSets; i++) {
      if (track_selection[i]) {
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
    for (int i=0; i<kNHelixHistSets; i++) helix_selection[i] = nullptr;

    helix_selection[0] = new TString("APR helices");
    helix_selection[1] = new TString("CPR helices");
    helix_selection[2] = new TString("Offline helices");

    for (int i=0; i<kNHelixHistSets; i++) {
      fHist.fHelix[i] = nullptr;
      if (helix_selection[i]) {
        sprintf(folder_name,"hlx_%i",i);
        fol = (TFolder*) hist_folder->FindObject(folder_name);
        folder_title    = helix_selection[i]->Data();
        if (! fol) fol  = hist_folder->AddFolder(folder_name,folder_title);
        fHist.fHelix[i] = new HelixHist_t;
        BookHelixHistograms(fHist.fHelix[i],Form("Hist/%s",folder_name));
      }
    }

    //-----------------------------------------------------------------------------
    // book RMC histograms
    //-----------------------------------------------------------------------------
    TString* rmc_selection[kNRMCHistSets];
    for (int i=0; i<kNRMCHistSets; i++) rmc_selection[i] = nullptr;

    rmc_selection[0] = new TString("All events");

    for (int i=0; i<kNRMCHistSets; i++) {
      if (rmc_selection[i]) {
        sprintf(folder_name,"rmc_%i",i);
        fol = (TFolder*) hist_folder->FindObject(folder_name);
        folder_title    = rmc_selection[i]->Data();
        if (! fol) fol  = hist_folder->AddFolder(folder_name,folder_title);
        fHist.fRMC[i] = new RMCHist_t;
        BookRMCHistograms(fHist.fRMC[i],Form("Hist/%s",folder_name));
      }
    }

  }

  //_____________________________________________________________________________
  int TRMCConvAnaModule::BeginRun() {
    int rn = GetHeaderBlock()->RunNumber();
    TStntuple::Init(rn);
    return 0;
  }

  //_____________________________________________________________________________
  void TRMCConvAnaModule::FillHistograms() {

    //-----------------------------------------------------------------------------
    // fill event histograms
    //-----------------------------------------------------------------------------
    FillEventHistograms(fHist.fEvent[0],&fEvtPar);
    if(fEvtPar.fNMatchedHelices > 0) FillEventHistograms(fHist.fEvent[1],&fEvtPar);

    //-----------------------------------------------------------------------------
    // fill genp histograms
    //-----------------------------------------------------------------------------
    if(fGenpBlock) {
      for (int i=0; i<fGenpBlock->NParticles(); i++) {
        FillGenpHistograms(fHist.fGenp[0],fGenpBlock->Particle(i));
      }
    }

    //-----------------------------------------------------------------------------
    // fill simp histograms
    //-----------------------------------------------------------------------------
    if(fSimpBlock) {
      for (int i=0; i<fSimpBlock->NParticles(); i++) {
        FillSimpHistograms(fHist.fSimp[0],fSimpBlock->Particle(i));
      }
    }

    //-----------------------------------------------------------------------------
    // fill apr track histograms
    //-----------------------------------------------------------------------------
    for (int i=0; i<fEvtPar.fNAprTracks; i++) {
      fTrack = fAprTrackBlock->Track(i);
      InitTrackPar(fTrack,&fTrkPar);
      FillTrackHistograms(fHist.fTrack[0],&fTrkPar);
      const int ID = TrackID(fTrack, &fTrkPar);
      if(ID == 0) {
        FillTrackHistograms(fHist.fTrack[50],&fTrkPar);
      }
    }

    //-----------------------------------------------------------------------------
    // fill cpr track histograms
    //-----------------------------------------------------------------------------
    for (int i=0; i<fEvtPar.fNCprTracks; i++) {
      fTrack = fCprTrackBlock->Track(i);
      InitTrackPar(fTrack,&fTrkPar);
      FillTrackHistograms(fHist.fTrack[1],&fTrkPar);
      const int ID = TrackID(fTrack, &fTrkPar);
      if(ID == 0) {
        FillTrackHistograms(fHist.fTrack[60],&fTrkPar);
      }
    }

    //-----------------------------------------------------------------------------
    // fill merged track histograms
    //-----------------------------------------------------------------------------
    for (int i=0; i<fEvtPar.fNTracks; i++) {
      fTrack = fOfflineTrackBlock->Track(i);
      InitTrackPar(fTrack,&fTrkPar);
      FillTrackHistograms(fHist.fTrack[2],&fTrkPar);
      const int ID = TrackID(fTrack, &fTrkPar);
      if(ID == 0) {
        FillTrackHistograms(fHist.fTrack[70],&fTrkPar);
      }
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
    // fill Offline helix histograms
    //-----------------------------------------------------------------------------
    for (int i=0; i<fEvtPar.fNOfflineHelices; i++) {
      fHelix = fOfflineHelixBlock->Helix(i);
      InitHelixPar(fHelix,&fHlxPar);
      FillHelixHistograms(fHist.fHelix[2],&fHlxPar);
    }

    //-----------------------------------------------------------------------------
    // fill RMC histograms
    //-----------------------------------------------------------------------------
    FillRMCHistograms(fHist.fRMC[0]);

  }

  //-----------------------------------------------------------------------------
  int TRMCConvAnaModule::Event(int ientry) {

    // get entry for the ith event for each data block
    fAprHelixBlock->GetEntry(ientry);
    fCprHelixBlock->GetEntry(ientry);
    fOfflineHelixBlock->GetEntry(ientry);
    fAprTrackBlock->GetEntry(ientry);
    fCprTrackBlock->GetEntry(ientry);
    fOfflineTrackBlock->GetEntry(ientry);
    fTriggerBlock->GetEntry(ientry);
    fGenpBlock->GetEntry(ientry);
    fSimpBlock->GetEntry(ientry);

    // get/set event parameters
    fEvtPar.fInstLum  = GetHeaderBlock()->fInstLum;
    fEvtPar.fNAprHelices = fAprHelixBlock->NHelices();
    fEvtPar.fNCprHelices = fCprHelixBlock->NHelices();
    fEvtPar.fNOfflineHelices = fOfflineHelixBlock->NHelices();
    fEvtPar.fNAprTracks = fAprTrackBlock->NTracks();
    fEvtPar.fNCprTracks = fCprTrackBlock->NTracks();
    fEvtPar.fNTracks = fOfflineTrackBlock->NTracks();
    fEvtPar.fPassedCprPath = fTriggerBlock->PathPassed(150);
    fEvtPar.fPassedAprPath = fTriggerBlock->PathPassed(180);

    fGen = (fGenpBlock && fGenpBlock->NParticles() > 0) ? fGenpBlock->Particle(0) : nullptr;

    // Find the relevant SIM particles
    fPhoton = nullptr;
    fElectron = nullptr;
    fPositron = nullptr;
    for(int isim = 0; isim < fSimpBlock->NParticles(); ++isim) {
      auto sim = fSimpBlock->Particle(isim);
      if(sim->PDGCode() ==  22) fPhoton   = sim;
      if(sim->PDGCode() ==  11) fElectron = sim;
      if(sim->PDGCode() == -11) fPositron = sim;
    }

    InitRMCPar();

    Debug();

    FillHistograms();

    return 0;
  }

  //-----------------------------------------------------------------------------
  void TRMCConvAnaModule::InitRMCPar() {
    if(!fPhoton || !fElectron || !fPositron) {
      fRMCPar.fPhotonEnergy   = -1.;
      fRMCPar.fPhotonRadius   = -1.;
      fRMCPar.fPositronEnergy = -1.;
      fRMCPar.fElectronEnergy = -1.;
      return;
    }

    fRMCPar.fPhotonEnergy = fPhoton->fStartMom.P();
    fRMCPar.fPhotonRadius = std::sqrt(std::pow(fPhoton->fEndPos.X()+3904., 2) + std::pow(fPhoton->fEndPos.Y(), 2));
    fRMCPar.fPositronEnergy = fPositron->fStartMom.P();
    fRMCPar.fElectronEnergy = fElectron->fStartMom.P();
  }

  //_____________________________________________________________________________
  // Apply selection cuts to the helix
  int TRMCConvAnaModule::HelixID(TStnHelix* h, HelixPar_t* hpar) {
    int ID(0);
    return ID;
  }

  //_____________________________________________________________________________
  // Apply selection cuts to a track
  int TRMCConvAnaModule::TrackID(TStnTrack* track, TrackPar_t* tpar) {
    int ID(0);
    return ID;
  }

  //_____________________________________________________________________________
  void TRMCConvAnaModule::Debug() {
    bool print_event = GetDebugBit(0); //all events

    if(print_event) { //print event info
      auto event = GetEvent();
      printf(">>> Event %5i/%5i/%6i:\n", event->fRunNumber, event->fSectionNumber, event->fEventNumber);
      printf(" Helices: %2i %2i %2i\n",
             fEvtPar.fNAprHelices, fEvtPar.fNCprHelices, fEvtPar.fNOfflineHelices);
      printf(" Tracks : %2i %2i %2i\n",
             fEvtPar.fNAprTracks, fEvtPar.fNCprTracks, fEvtPar.fNTracks);
      printf(" SIM particles:\n");
      fSimpBlock->Print();
      printf(" APR helices:\n");
      fAprHelixBlock->Print();
      printf(" More APR helix info:\n");
      printf("--------------------------------------------------------------------------------------------------------------------------------\n");
      printf(" i  p(sim_1) pz(sim_1) ID(sim_1) nh(sim_1)  p(sim_2) ID(sim_2)  nh(sim_2)   dZdT    sig(dZdT)\n");
      //       0  112.94   -104.82     -11        5       164.16    2212         2     -3.36e-03   -3.43

      printf("--------------------------------------------------------------------------------------------------------------------------------\n");
      for (int i=0; i<fEvtPar.fNAprHelices; i++) {
        const auto helix = fAprHelixBlock->Helix(i);
        InitHelixPar(helix,&fHlxPar);
        printf("%2i  %6.2f   %7.2f    %4i      %3i       %6.2f    %4i       %3i     %9.2e   %5.2f\n",
               i, helix->fMom1.P(), helix->fMom1.Pz(), helix->fSimpPDG1, helix->fSimpId1Hits,
               (helix->fSimpId2Hits > 0) ? helix->fMom2.P() : -1.f, helix->fSimpPDG2, helix->fSimpId2Hits,
               helix->TZSlope(), helix->TZSlope()/helix->TZSlopeError());
      }
      printf(" CPR helices:\n");
      fCprHelixBlock->Print();
      printf(" APR tracks:\n");
      fAprTrackBlock->Print();
      for(int itrack = 0; itrack < fEvtPar.fNAprTracks; ++itrack) printf(" %2i: helix index = %2i\n", itrack, fAprTrackBlock->Track(itrack)->fHelixIndex);
      printf(" CPR tracks:\n");
      fCprTrackBlock->Print();
      for(int itrack = 0; itrack < fEvtPar.fNCprTracks; ++itrack) printf(" %2i: helix index = %2i\n", itrack, fCprTrackBlock->Track(itrack)->fHelixIndex);
      fOfflineTrackBlock->Print();
      for(int itrack = 0; itrack < fEvtPar.fNTracks; ++itrack) printf(" %2i: helix index = %2i\n", itrack, fOfflineTrackBlock->Track(itrack)->fHelixIndex);
    }
  }

  //_____________________________________________________________________________
  int TRMCConvAnaModule::EndJob() {
    printf("----- end job: ---- %s\n",GetName());
    return 0;
  }
}
