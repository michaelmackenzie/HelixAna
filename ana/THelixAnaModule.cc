//------------------------------------------------------------------------------
// analysis module used to study trigger parameter cuts
//-----------------------------------------------------------------------------
#include "HelixAna/ana/THelixAnaModule.hh"

namespace HelixAna {

  //-----------------------------------------------------------------------------
  THelixAnaModule::THelixAnaModule(const char* name, const char* title):
    TAnaModule(name,title)
  {
    // fAprHelixBlockName      =  "HelixBlockDeApr";
    // fCprHelixBlockName      =  "HelixBlockDeCpr";
    // fMergedHelixBlockName   =  "HelixBlockDe";
    fAprHelixBlockName      =  "HelixBlockAprHighP";
    fCprHelixBlockName      =  "HelixBlockCprDeHighP";
    fMergedHelixBlockName   =  "HelixBlockDe";
    fAprTrackBlockName      =  "TrackBlockAprHighP";
    fCprTrackBlockName      =  "TrackBlockCprDeHighP";
    fMergedTrackBlockName   =  "TrackBlockDe";
    fTriggerBlockName       =  "TriggerBlock";
    fGenpBlockName          =  "GenpBlock";
    fSimpBlockName          =  "SimpBlock";
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
    RegisterDataBlock(fGenpBlockName,         "TGenpBlock"      , &fGenpBlock         );
    RegisterDataBlock(fSimpBlockName,         "TSimpBlock"      , &fSimpBlock         );

    //-----------------------------------------------------------------------------
    // book histograms
    //-----------------------------------------------------------------------------
    BookHistograms();

    return 0;
  }

  //-----------------------------------------------------------------------------
  void THelixAnaModule::BookHelixCompHistograms(HelixAna::HelixCompHist_t* Hist, const char* Folder) {

    HBook1F(Hist->fAprHelixDeltaP,"AprHelixDeltaP",Form("%s: AprHelixDeltaP",Folder), 200,  -10, 10, Folder);
    HBook1F(Hist->fAprTrackDeltaP,"AprTrackDeltaP",Form("%s: AprTrackDeltaP",Folder), 200,   -5,  5, Folder);
    HBook1F(Hist->fCprHelixDeltaP,"CprHelixDeltaP",Form("%s: CprHelixDeltaP",Folder), 200,  -10, 10, Folder);
    HBook1F(Hist->fCprTrackDeltaP,"CprTrackDeltaP",Form("%s: CprTrackDeltaP",Folder), 200,   -5,  5, Folder);
    HBook1F(Hist->fHelixDeltaP,"HelixDeltaP",Form("%s: HelixDeltaP",Folder), 200,  -10, 10, Folder);
    HBook1F(Hist->fTrackDeltaP,"TrackDeltaP",Form("%s: TrackDeltaP",Folder), 200,   -5,  5, Folder);
  }

  //-----------------------------------------------------------------------------
  void THelixAnaModule::BookHelixHistograms(HelixHist_t* Hist, const char* Folder) {
    TAnaModule::BookHelixHistograms(Hist, Folder);
    HBook1F(Hist->fTZSlopeSigCDF[0], "tzslopesig_cdf_0", Form("%s: dZ/dT significance CDF", Folder),
            Hist->fTZSlopeSig->GetNbinsX(), Hist->fTZSlopeSig->GetXaxis()->GetXmin(), Hist->fTZSlopeSig->GetXaxis()->GetXmax(), Folder);
    HBook1F(Hist->fTZSlopeSigCDF[1], "tzslopesig_cdf_1", Form("%s: dZ/dT significance CDF", Folder),
            Hist->fTZSlopeSig->GetNbinsX(), Hist->fTZSlopeSig->GetXaxis()->GetXmin(), Hist->fTZSlopeSig->GetXaxis()->GetXmax(), Folder);
  }

  //-----------------------------------------------------------------------------
  void THelixAnaModule::FillHelixCompHistograms(HelixAna::HelixCompHist_t* Hist, HelixPair_t& Match) {
    // Fill APR helix info
    if(Match.fAprHelix) {
      // const float MC_P = (Match.fAprTrack) ? Match.fAprTrack->fPFront : 0.; //FIXME: Retrieve true particle momentum corresponding to the helix
      const float MC_P = Match.fAprHelix->fMom1.P();
      Hist->fAprHelixDeltaP->Fill(Match.fAprHelix->P() - MC_P);
    } else {
      Hist->fAprHelixDeltaP->Fill(-999.);
    }

    // Fill CPR helix info
    if(Match.fCprHelix) {
      // const float MC_P = (Match.fCprTrack) ? Match.fCprTrack->fPFront : 0.; //FIXME: Retrieve true particle momentum corresponding to the helix
      const float MC_P = Match.fCprHelix->fMom1.P();
      Hist->fCprHelixDeltaP->Fill(Match.fCprHelix->P() - MC_P);
    } else {
      Hist->fCprHelixDeltaP->Fill(-999.);
    }

    // Fill APR vs. CPR helix info
    if(Match.fAprHelix && Match.fCprHelix) {
      Hist->fHelixDeltaP->Fill(Match.fAprHelix->P() - Match.fCprHelix->P());
    } else {
      Hist->fHelixDeltaP->Fill(-999.);
    }

    // Fill APR track info
    if(Match.fAprTrack) {
      Hist->fAprTrackDeltaP->Fill(Match.fAprTrack->fP - Match.fAprTrack->fPFront);
    } else {
      Hist->fAprTrackDeltaP->Fill(-999.);
    }

    // Fill CPR track info
    if(Match.fCprTrack) {
      Hist->fCprTrackDeltaP->Fill(Match.fCprTrack->fP - Match.fCprTrack->fPFront);
    } else {
      Hist->fCprTrackDeltaP->Fill(-999.);
    }

    // Fill APR vs. CPR track info
    if(Match.fAprTrack && Match.fCprTrack) {
      Hist->fTrackDeltaP->Fill(Match.fAprTrack->P() - Match.fCprTrack->P());
    } else {
      Hist->fTrackDeltaP->Fill(-999.);
    }

  }

  //-----------------------------------------------------------------------------
  void THelixAnaModule::FillHelixHistograms(HelixHist_t* Hist, HelixPar_t* HlxPar) {
    TAnaModule::FillHelixHistograms(Hist, HlxPar);
    auto Helix = HlxPar->fHelix;
    // Fill the CDF distributions
    const float sig = (Helix->TZSlopeError() > 0.f) ? Helix->TZSlope() / Helix->TZSlopeError() : -999.f;
    const int bin   = Hist->fTZSlopeSigCDF[0]->FindBin(sig);
    const int nbins = Hist->fTZSlopeSigCDF[0]->GetNbinsX();
    for(int ibin = bin; ibin <= nbins+1; ++ibin) { //cut on low end efficiency
      Hist->fTZSlopeSigCDF[0]->SetBinContent(ibin, Hist->fTZSlopeSigCDF[0]->GetBinContent(ibin) + 1);
    }
    for(int ibin = 0; ibin <= nbins; ++ibin) { //cut on high end efficiency
      Hist->fTZSlopeSigCDF[1]->SetBinContent(ibin, Hist->fTZSlopeSigCDF[1]->GetBinContent(ibin) + 1);
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
    track_selection[2] = new TString("Merged tracks");

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
    helix_selection[2] = new TString("Merged helices");
    helix_selection[10] = new TString("low |TrackDeltaP| APR helices");
    helix_selection[11] = new TString("low |TrackDeltaP| CPR helices");
    helix_selection[12] = new TString("low |TrackDeltaP| Merged helices");
    helix_selection[13] = new TString("high |TrackDeltaP| APR helices");
    helix_selection[14] = new TString("high |TrackDeltaP| CPR helices");
    helix_selection[15] = new TString("high |TrackDeltaP| Merged helices");

    helix_selection[50] = new TString("APR 80<P<110 helices");
    helix_selection[51] = new TString("APR 80<P<110 helices with clusters");
    helix_selection[52] = new TString("APR 80<P<110 helices without clusters");
    helix_selection[53] = new TString("APR 80<P<110, sig > 5");
    helix_selection[54] = new TString("APR 80<P<110, sig < -5");
    helix_selection[55] = new TString("APR 80<P<110,  1 hel");
    helix_selection[56] = new TString("APR 80<P<110, -1 hel");

    helix_selection[60] = new TString("CPR P>80 helices");
    helix_selection[61] = new TString("CPR P>80 helices with clusters");
    helix_selection[62] = new TString("CPR P>80 helices without clusters");
    helix_selection[63] = new TString("CPR 80<P<110, sig > 5");
    helix_selection[64] = new TString("CPR 80<P<110, sig < -5");

    helix_selection[70] = new TString("Merged P>80 helices");
    helix_selection[71] = new TString("Merged P>80 helices with clusters");
    helix_selection[72] = new TString("Merged P>80 helices without clusters");
    helix_selection[73] = new TString("Merged 80<P<110, sig > 5");
    helix_selection[74] = new TString("Merged 80<P<110, sig < -5");

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
    // book helix/track comparison histograms
    //-----------------------------------------------------------------------------
    TString* comp_selection[kNHelixCompHistSets];
    for (int i=0; i<kNHelixCompHistSets; i++) comp_selection[i] = nullptr;

    comp_selection[0] = new TString("All matched");
    comp_selection[1] = new TString("Good matched helices");
    comp_selection[10] = new TString("low APR |TrackDeltaP|");
    comp_selection[11] = new TString("low CPR |TrackDeltaP|");
    comp_selection[12] = new TString("low Merged |TrackDeltaP|");
    comp_selection[13] = new TString("high APR |TrackDeltaP|");
    comp_selection[14] = new TString("high CPR |TrackDeltaP|");
    comp_selection[15] = new TString("high Merged |TrackDeltaP|");

    for (int i=0; i<kNHelixCompHistSets; i++) {
      if (comp_selection[i]) {
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
      //FIXME: Add real truth matching to only look at non-pileup tracks
      const float r_origin(std::sqrt(std::pow(fHelix->fOrigin1.X()+3904., 2) + std::pow(fHelix->fOrigin1.Y(), 2))), z_origin(fHelix->fOrigin1.Z());
      const bool truth_match = fSimpBlock->NParticles() == 1 && fHelix->fMom1.P() > 80. && fHelix->fSimpPDG1 == fSimpBlock->Particle(0)->fPdgCode && r_origin < 70. && z_origin < 6400. && z_origin > 5400.;
      if(fHelix->P() > 70.f && fHelix->P() < 120.f && truth_match) { //remove most pileup helices
        FillHelixHistograms(fHist.fHelix[50],&fHlxPar);
        if(fHelix->ClusterEnergy() > 0.) FillHelixHistograms(fHist.fHelix[51],&fHlxPar);
        else                             FillHelixHistograms(fHist.fHelix[52],&fHlxPar);
        const float tz_sig = fHelix->fTZSlope/fHelix->fTZSlopeError;
        if(tz_sig >  5.f) FillHelixHistograms(fHist.fHelix[53],&fHlxPar);
        if(tz_sig < -5.f) FillHelixHistograms(fHist.fHelix[54],&fHlxPar);
        if(fHelix->Helicity() > 0) FillHelixHistograms(fHist.fHelix[55],&fHlxPar);
        else                       FillHelixHistograms(fHist.fHelix[56],&fHlxPar);
      }
    }

    //-----------------------------------------------------------------------------
    // fill cpr helix histograms
    //-----------------------------------------------------------------------------
    for (int i=0; i<fEvtPar.fNCprHelices; i++) {
      fHelix = fCprHelixBlock->Helix(i);
      InitHelixPar(fHelix,&fHlxPar);
      FillHelixHistograms(fHist.fHelix[1],&fHlxPar);
      //FIXME: Add real truth matching to only look at non-pileup tracks
      const float r_origin(std::sqrt(std::pow(fHelix->fOrigin1.X()+3904., 2) + std::pow(fHelix->fOrigin1.Y(), 2))), z_origin(fHelix->fOrigin1.Z());
      const bool truth_match = fSimpBlock->NParticles() == 1 && fHelix->fMom1.P() > 80. && fHelix->fSimpPDG1 == fSimpBlock->Particle(0)->fPdgCode && r_origin < 70. && z_origin < 6400. && z_origin > 5400.;
      if(fHelix->P() > 70.f && fHelix->P() < 120.f && truth_match) { //remove most pileup helices
        FillHelixHistograms(fHist.fHelix[60],&fHlxPar);
        if(fHelix->ClusterEnergy() > 0.) FillHelixHistograms(fHist.fHelix[61],&fHlxPar);
        else                             FillHelixHistograms(fHist.fHelix[62],&fHlxPar); //shouldn't have helices without clusters
        const float tz_sig = fHelix->fTZSlope/fHelix->fTZSlopeError;
        if(tz_sig >  5.f) FillHelixHistograms(fHist.fHelix[63],&fHlxPar);
        if(tz_sig < -5.f) FillHelixHistograms(fHist.fHelix[64],&fHlxPar);
      }
    }

    //-----------------------------------------------------------------------------
    // fill merged helix histograms
    //-----------------------------------------------------------------------------
    for (int i=0; i<fEvtPar.fNMergedHelices; i++) {
      fHelix = fMergedHelixBlock->Helix(i);
      InitHelixPar(fHelix,&fHlxPar);
      FillHelixHistograms(fHist.fHelix[2],&fHlxPar);
      //FIXME: Add real truth matching to only look at non-pileup tracks
      const float r_origin(std::sqrt(std::pow(fHelix->fOrigin1.X()+3904., 2) + std::pow(fHelix->fOrigin1.Y(), 2))), z_origin(fHelix->fOrigin1.Z());
      const bool truth_match = fSimpBlock->NParticles() == 1 && fHelix->fMom1.P() > 70. && fHelix->fSimpPDG1 == fSimpBlock->Particle(0)->fPdgCode && r_origin < 70. && z_origin < 6400. && z_origin > 5400.;
      if(fHelix->P() > 70.f && fHelix->P() < 120.f && truth_match) { //remove most pileup helices
        FillHelixHistograms(fHist.fHelix[70],&fHlxPar);
        if(fHelix->ClusterEnergy() > 0.) FillHelixHistograms(fHist.fHelix[71],&fHlxPar);
        else                             FillHelixHistograms(fHist.fHelix[72],&fHlxPar);
        const float tz_sig = fHelix->fTZSlope/fHelix->fTZSlopeError;
        if(tz_sig >  5.f) FillHelixHistograms(fHist.fHelix[73],&fHlxPar);
        if(tz_sig < -5.f) FillHelixHistograms(fHist.fHelix[74],&fHlxPar);
      }
    }

    //-----------------------------------------------------------------------------
    // fill helix/track comparison histograms
    //-----------------------------------------------------------------------------
    for(int imatch = 0; imatch < fEvtPar.fNMatchedHelices; ++imatch) {
      FillHelixCompHistograms(fHist.fHelixComp[0],fMatchedHelices[imatch]);
      if(fMatchedHelices[imatch].fAprTrack && fMatchedHelices[imatch].fCprTrack) {
        FillHelixCompHistograms(fHist.fHelixComp[1],fMatchedHelices[imatch]);

        // final track fit accuracy
        const double apr_dtp = fMatchedHelices[imatch].fAprTrack->fP - fMatchedHelices[imatch].fAprTrack->fPFront;
        const double cpr_dtp = fMatchedHelices[imatch].fCprTrack->fP - fMatchedHelices[imatch].fCprTrack->fPFront;

        // well reconstructed APR tracks
        if(std::fabs(apr_dtp) < 0.250) {
          InitHelixPar(fMatchedHelices[imatch].fAprHelix,&fHlxPar);
          FillHelixHistograms(fHist.fHelix[10],&fHlxPar);
          FillHelixCompHistograms(fHist.fHelixComp[10],fMatchedHelices[imatch]);
        }

        // well reconstructed CPR tracks
        if(std::fabs(cpr_dtp) < 0.250) {
          InitHelixPar(fMatchedHelices[imatch].fCprHelix,&fHlxPar);
          FillHelixHistograms(fHist.fHelix[11],&fHlxPar);
          FillHelixCompHistograms(fHist.fHelixComp[11],fMatchedHelices[imatch]);
        }

        // poorly reconstructed APR tracks
        if(apr_dtp > 0.500 && apr_dtp < 5.) {
          InitHelixPar(fMatchedHelices[imatch].fAprHelix,&fHlxPar);
          FillHelixHistograms(fHist.fHelix[13],&fHlxPar);
          FillHelixCompHistograms(fHist.fHelixComp[13],fMatchedHelices[imatch]);
        }

        // poorly reconstructed CPR tracks
        if(cpr_dtp > 0.500 && cpr_dtp < 5.) {
          InitHelixPar(fMatchedHelices[imatch].fCprHelix,&fHlxPar);
          FillHelixHistograms(fHist.fHelix[14],&fHlxPar);
          FillHelixCompHistograms(fHist.fHelixComp[14],fMatchedHelices[imatch]);
        }
      }
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
    fGenpBlock->GetEntry(ientry);
    fSimpBlock->GetEntry(ientry);

    // get/set event parameters
    fEvtPar.fInstLum  = GetHeaderBlock()->fInstLum;
    fEvtPar.fNAprHelices = fAprHelixBlock->NHelices();
    fEvtPar.fNCprHelices = fCprHelixBlock->NHelices();
    fEvtPar.fNMergedHelices = fMergedHelixBlock->NHelices();
    fEvtPar.fNAprTracks = fAprTrackBlock->NTracks();
    fEvtPar.fNCprTracks = fCprTrackBlock->NTracks();
    fEvtPar.fNTracks = fMergedTrackBlock->NTracks();
    fEvtPar.fPassedCprPath = fTriggerBlock->PathPassed(150);
    fEvtPar.fPassedAprPath = fTriggerBlock->PathPassed(180);

    fGen = (fGenpBlock && fGenpBlock->NParticles() > 0) ? fGenpBlock->Particle(0) : nullptr;

    // perform helix-track matching
    MatchHelices();

    Debug();

    FillHistograms();

    return 0;
  }

  //_____________________________________________________________________________
  TStnTrack* THelixAnaModule::GetMatchingTrack(TStnHelix* h, int h_index, TStnTrackBlock* block) {
    for(int t_index = 0; t_index < block->NTracks(); ++t_index) {
      auto track = block->Track(t_index);
      if(!track) continue;
      // FIXME: Check the proper technique for this matching
      // if(track->fHelixIndex == h_index && track->fSeedIndex == h->fTrackSeedIndex) return track;
      // if(track->fHelixIndex == h_index) return track;
      // temporarily just match in time and space
      bool matched(true);
      // matched &= std::fabs(h->T0() - track->T0()) / std::sqrt(std::pow(h->T0Err(),2) + std::pow(track->T0Err(),2)) < 2.;
      matched &= std::fabs(h->D0() - track->D0()) < 20.;
      matched &= std::fabs(h->P() - track->P())/(h->P() + track->P())/2. < 0.02;
      if(matched) return track;
    }
    return nullptr;
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
          fMatchedHelices[index].fAprTrack = GetMatchingTrack(apr_helix, iapr, fAprTrackBlock);
          fMatchedHelices[index].fCprTrack = GetMatchingTrack(cpr_helix, icpr, fCprTrackBlock);
          //FIXME: Get the corresponding merged helix/track collection entries if they exist
        }
      }
    }
  }

  //_____________________________________________________________________________
  void THelixAnaModule::Debug() {
    int n_high_p_apr_hel(0), n_high_apr_sig(0);
    for (int i=0; i<fEvtPar.fNAprHelices; i++) {
      const auto helix = fAprHelixBlock->Helix(i);
      const auto simp = (fSimpBlock->NParticles() == 1) ? fSimpBlock->Particle(0) : nullptr;
      if(helix->P() > 70.f) ++n_high_p_apr_hel;
      //FIXME: Add real truth matching to only look at non-pileup tracks
      const bool truth_match = simp && helix->fSimpPDG1 == simp->fPdgCode;
      if(truth_match && simp && abs(simp->fPdgCode) < 14 && helix->fTZSlope/helix->fTZSlopeError*simp->fPdgCode/abs(simp->fPdgCode) > 5.f) ++n_high_apr_sig;
    }

    const bool print_event = ((GetDebugBit(0)) //all events
                              || (GetDebugBit(1) && fEvtPar.fNMatchedHelices) //events with matches
                              || (GetDebugBit(2) && fEvtPar.fNMatchedHelices > 1) //events with multiple matches
                              || (GetDebugBit(3) && n_high_p_apr_hel > 1) //events with multiple high momentum helices
                              || (GetDebugBit(4) && n_high_apr_sig > 0) //events with significantly wrong TZ slope
                              );
    if(print_event) { //print event info
      auto event = GetEvent();
      printf(">>> Event %5i/%5i/%6i:\n", event->fRunNumber, event->fSectionNumber, event->fEventNumber);
      printf(" Helices: %2i %2i %2i: %2i matches\n",
             fEvtPar.fNAprHelices, fEvtPar.fNCprHelices, fEvtPar.fNMergedHelices, fEvtPar.fNMatchedHelices);
      printf(" Tracks : %2i %2i %2i\n",
             fEvtPar.fNAprTracks, fEvtPar.fNCprTracks, fEvtPar.fNTracks);
      printf(" SIM particles:\n");
      fSimpBlock->Print();
      printf(" APR helices:\n");
      fAprHelixBlock->Print();
      printf(" More APR helix info:\n");
      printf("--------------------------------------------------------------------------------------------------------------------------------\n");
      printf(" i  p(sim_1) ID(sim_1) nh(sim_1)  p(sim_2) ID(sim_2)  nh(sim_2)   dZdT    sig(dZdT)\n");
      //       1  206.67     2212        1       151.09    2212         1     7.07e-03    0.00

      printf("--------------------------------------------------------------------------------------------------------------------------------\n");
      for (int i=0; i<fEvtPar.fNAprHelices; i++) {
        const auto helix = fAprHelixBlock->Helix(i);
        printf("%2i  %6.2f     %4i      %3i       %6.2f    %4i       %3i     %8.2e   %4.2f\n",
               i, helix->fMom1.P(), helix->fSimpPDG1, helix->fSimpId1Hits,
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
      printf(" Matched helices:\n");
      for(int imatch = 0; imatch < fEvtPar.fNMatchedHelices; ++imatch) {
        printf("  %2i:\n", imatch);
        if(fMatchedHelices[imatch].fAprHelix)    fMatchedHelices[imatch].fAprHelix->Print("banner data");
        else                                     printf("   --> No APR helix found!\n");
        if(fMatchedHelices[imatch].fCprHelix)    fMatchedHelices[imatch].fCprHelix->Print("data");
        else                                     printf("   --> No CPR helix found!\n");
        if(fMatchedHelices[imatch].fMergedHelix) fMatchedHelices[imatch].fMergedHelix->Print("data");
        else                                     printf("   --> No merged helix found!\n");
        if(fMatchedHelices[imatch].fAprTrack)    fMatchedHelices[imatch].fAprTrack->Print("banner data");
        else                                     printf("   --> No APR track found!\n");
        if(fMatchedHelices[imatch].fCprTrack)    fMatchedHelices[imatch].fCprTrack->Print("data");
        else                                     printf("   --> No CPR track found!\n");
        if(fMatchedHelices[imatch].fMergedTrack) fMatchedHelices[imatch].fMergedTrack->Print("data");
        else                                     printf("   --> No merged track found!\n");
      }
    }
  }

  //_____________________________________________________________________________
  int THelixAnaModule::EndJob() {
    printf("----- end job: ---- %s\n",GetName());
    // Normalize the CDF distributions
    for (int i=0; i<kNHelixHistSets; i++) {
      if(!fHist.fHelix[i]) continue;
      auto Hist = fHist.fHelix[i];
      const float norm = Hist->fTZSlopeSigCDF[1]->GetBinContent(0);
      Hist->fTZSlopeSigCDF[0]->Scale((norm > 0.f) ? 1.f/norm : 1.f);
      Hist->fTZSlopeSigCDF[1]->Scale((norm > 0.f) ? 1.f/norm : 1.f);
    }
    return 0;
  }
}
