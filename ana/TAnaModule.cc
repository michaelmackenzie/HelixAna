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
#include "HelixAna/ana/TAnaModule.hh"
ClassImp(HelixAna::TAnaModule)

using std::vector;

using namespace HelixAna;

namespace HelixAna {

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
  HBook1F(Hist->fNAprHelices,"nHelicesApr",Form("%s: nHelicesApr",Folder), 50, 0.0, 50.0, Folder);
  HBook1F(Hist->fNCprHelices,"nHelicesCpr",Form("%s: nHelicesCpr",Folder), 50, 0.0, 50.0, Folder);
  HBook1F(Hist->fNMatchedHelices,"nHelicesMatched",Form("%s: nHelicesMatched",Folder), 50, 0.0, 50.0, Folder);

}

//-----------------------------------------------------------------------------
void TAnaModule::BookGenpHistograms(GenpHist_t* Hist, const char* Folder) {
//   char name [200];
//   char title[200];

  HBook1F(Hist->fP      ,"p"       ,Form("%s: Momentum"     ,Folder),1000,     0, 200,Folder);
  HBook1F(Hist->fPdgCode[0],"pdg_code_0",Form("%s: PDG Code[0]"     ,Folder),200, -100, 100,Folder);
  HBook1F(Hist->fPdgCode[1],"pdg_code_1",Form("%s: PDG Code[1]"     ,Folder),500, -2500, 2500,Folder);
  HBook1F(Hist->fGenID  ,"gen_id"  ,Form("%s: Generator ID" ,Folder), 100,     0, 100,Folder);
  HBook1F(Hist->fZ0     ,"z0"      ,Form("%s: Z0"           ,Folder), 500,  5400, 6400,Folder);
  HBook1F(Hist->fT0     ,"t0"      ,Form("%s: T0"           ,Folder), 200,     0, 2000,Folder);
  HBook1F(Hist->fR0     ,"r"       ,Form("%s: R0"           ,Folder), 100,     0,  100,Folder);
  HBook1F(Hist->fCosTh  ,"cos_th"  ,Form("%s: Cos(Theta)"   ,Folder), 200,   -1.,   1.,Folder);
}

//-----------------------------------------------------------------------------
void TAnaModule::BookSimpHistograms(SimpHist_t* Hist, const char* Folder) {
  //  char name [200];
  //  char title[200];

  HBook1F(Hist->fPdgCode         ,"pdg"         ,Form("%s: PDG code"                     ,Folder),200,-100,100,Folder);
  HBook1F(Hist->fCreationCode    ,"code"        ,Form("%s: Creation code"                ,Folder),200,   0,200,Folder);
  HBook1F(Hist->fNStrawHits      ,"nsth"        ,Form("%s: n straw hits"                 ,Folder),200,   0,200,Folder);
  HBook1F(Hist->fMomTargetEnd    ,"ptarg"       ,Form("%s: CE mom after Stopping Target" ,Folder),400,  70,110,Folder);
  HBook1F(Hist->fMomTrackerFront ,"pfront"      ,Form("%s: CE mom at the Tracker Front"  ,Folder),400,  70,110,Folder);
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
void TAnaModule::BookHelixHistograms(HelixHist_t* Hist, const char* Folder) {

  HBook1F(Hist->fNHits         ,"nhits"      ,Form("%s: # of straw hits"              ,Folder),  150,     0,  150,Folder);
  HBook1F(Hist->fHelicity      ,"hel"        ,Form("%s: Helicity"                     ,Folder),   10,    -5,    5,Folder);
  HBook1F(Hist->fClusterTime   ,"clusterTime",Form("%s: cluster time; t_{cluster}[ns]",Folder),  400,     0, 2000,Folder);
  HBook1F(Hist->fClusterEnergy ,"clusterE"   ,Form("%s: cluster energy; E [MeV]      ",Folder),  400,     0,  200,Folder);
  HBook1F(Hist->fRadius        ,"radius"     ,Form("%s: helix radius; r [mm]"         ,Folder),  500,     0,  500,Folder);
  HBook1F(Hist->fMom           ,"p"          ,Form("%s: momentum; p [MeV/c]"          ,Folder),  300,    50,  200,Folder);
  HBook1F(Hist->fPt            ,"pT"         ,Form("%s: pT; pT [MeV/c]"               ,Folder),  600,     0,  150,Folder);
  HBook1F(Hist->fGenMom        ,"simMom"     ,Form("%s: Sim particle P"               ,Folder),  400,     0,  200,Folder);
  HBook1F(Hist->fGenID         ,"simPDG"     ,Form("%s: Sim particle PDG type"        ,Folder),   60,   -30,   30,Folder);
  HBook1F(Hist->fLambda        ,"lambda"     ,Form("%s: lambda; #lambda"              ,Folder),  200, -1000, 1000,Folder);
  HBook1F(Hist->fTanDip        ,"tanDip"     ,Form("%s: tanDip"                       ,Folder),  200,  -2.0,  2.0,Folder);
  HBook1F(Hist->fT0            ,"t0"         ,Form("%s: t0; t0[ns]"                   ,Folder),  400,     0, 2000,Folder);
  HBook1F(Hist->fT0Err         ,"t0err"      ,Form("%s: t0err; t0err [ns]"            ,Folder),  100,     0,   10,Folder);
  HBook1F(Hist->fD0            ,"d0"         ,Form("%s: D0; d0 [mm]"                  ,Folder), 1600,  -400,  400,Folder);
  HBook1F(Hist->fAlgMask       ,"algmask"    ,Form("%s: Algorithm Mask"               ,Folder),   10,     0,   10,Folder);
  HBook1F(Hist->fBestAlg       ,"bestalg"    ,Form("%s: Best Algorithm"               ,Folder),   10,     0,   10,Folder);
  HBook1F(Hist->fChi2XY        ,"chi2xy"     ,Form("%s: Chi2(XY)/DOF"                 ,Folder),  100,     0,   10,Folder);
  HBook1F(Hist->fChi2ZPhi      ,"chi2zphi"   ,Form("%s: Chi2(ZPhi)/DOF"               ,Folder),  100,     0,   10,Folder);
  HBook1F(Hist->fTZSlope       ,"tzslope"    ,Form("%s: dz/dt"                        ,Folder),  100,  -0.1,  0.1,Folder);
  HBook1F(Hist->fTZSlopeErr    ,"tzslopeerr" ,Form("%s: dz/dt error"                  ,Folder),  100,   0.1,  0.1,Folder);
  HBook1F(Hist->fTZSlopeSig    ,"tzslopesig" ,Form("%s: dz/dt significance"           ,Folder),  200,  -10,    10,Folder);

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
  Hist->fNAprHelices->Fill(EvtPar->fNAprHelices);
  Hist->fNCprHelices->Fill(EvtPar->fNCprHelices);
  Hist->fNMatchedHelices->Fill(EvtPar->fNMatchedHelices);

}

//-----------------------------------------------------------------------------
void TAnaModule::FillGenpHistograms(GenpHist_t* Hist, TGenParticle* Genp) {
  int    gen_id;
  float  p, cos_th, z0, t0, r0, x0, y0;

  TLorentzVector mom, v;

  Genp->Momentum(mom);
  //  Genp->ProductionVertex(v);

  p      = mom.P();
  cos_th = mom.CosTheta();

  x0     = Genp->Vx()+3904.;
  y0     = Genp->Vy();

  z0     = Genp->Vz();
  t0     = Genp->T();
  r0     = sqrt(x0*x0+y0*y0);
  gen_id = Genp->GetStatusCode();

  Hist->fPdgCode[0]->Fill(Genp->GetPdgCode());
  Hist->fPdgCode[1]->Fill(Genp->GetPdgCode());
  Hist->fGenID->Fill(gen_id);
  Hist->fZ0->Fill(z0);
  Hist->fT0->Fill(t0);
  Hist->fR0->Fill(r0);
  Hist->fP->Fill(p);
  Hist->fCosTh->Fill(cos_th);
}

//-----------------------------------------------------------------------------
void TAnaModule::FillSimpHistograms(SimpHist_t* Hist, TSimParticle* Simp) {

  Hist->fPdgCode->Fill(Simp->fPdgCode);
  Hist->fCreationCode->Fill(Simp->fCreationCode);
  Hist->fMomTargetEnd->Fill(Simp->fMomTargetEnd);
  Hist->fMomTrackerFront->Fill(Simp->fMomTrackerFront);
  Hist->fNStrawHits->Fill(Simp->fNStrawHits);
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

//-----------------------------------------------------------------------------
void TAnaModule::FillHelixHistograms(HelixHist_t* Hist, HelixPar_t* HlxPar) {
  if(!Hist) throw std::runtime_error(Form("TAnaModule::%s: Attempting to fill undefined histograms\n", __func__));
  if(!HlxPar) throw std::runtime_error(Form("TAnaModule::%s: Attempting to fill histograms with undefinded helix par\n", __func__));
  auto Helix = HlxPar->fHelix;

  const int    nhits    = Helix->NHits      ();
  const double clusterT = Helix->ClusterTime();
  const double clusterE = Helix->ClusterEnergy();

  const double radius   = Helix->Radius();

  const double lambda   = Helix->Lambda();
  const double tanDip   = lambda/radius;
  const double pT       = Helix->Pt();
  int genID = Helix->fSimpPDG1;
  if(abs(genID) == 2212) genID = genID*25*((genID < 0) ? -1 : 1); //compress some particle IDs
  if(abs(genID) ==  211) genID = genID*26*((genID < 0) ? -1 : 1);
  const double p        = Helix->P();


  Hist->fHelicity      ->Fill(Helix->Helicity());
  Hist->fNHits         ->Fill(nhits);
  Hist->fClusterTime   ->Fill(clusterT);
  Hist->fClusterEnergy ->Fill(clusterE);

  Hist->fRadius        ->Fill(radius);
  Hist->fMom           ->Fill(p);
  Hist->fPt            ->Fill(pT);
  Hist->fLambda        ->Fill(lambda);
  Hist->fTanDip        ->Fill(tanDip);

  Hist->fGenMom        ->Fill(Helix->fMom1.P());
  Hist->fGenID         ->Fill(genID);

  Hist->fBestAlg       ->Fill(Helix->BestAlg());
  Hist->fAlgMask       ->Fill(Helix->AlgMask());
  Hist->fD0            ->Fill(Helix->D0());
  Hist->fT0            ->Fill(Helix->T0());
  Hist->fT0Err         ->Fill(Helix->T0Err());
  Hist->fChi2XY        ->Fill(Helix->Chi2XY());
  Hist->fChi2ZPhi      ->Fill(Helix->Chi2ZPhi());
  Hist->fTZSlope       ->Fill(Helix->TZSlope());
  Hist->fTZSlopeErr    ->Fill(Helix->TZSlopeError());
  Hist->fTZSlopeSig    ->Fill((Helix->TZSlopeError() > 0.) ? Helix->TZSlope() / Helix->TZSlopeError() : -999.); //sign the significance by the direction

}

void TAnaModule::InitTrackPar(TStnTrack* Trk, HelixAna::TrackPar_t* TrkPar) {

  // set pointer to TStnTrack
  TrkPar->fTrack = Trk;

  // compute parameters not contained in TStnTrack
  TrkPar->fRadius = Trk->fPt / (mmTconversion*bz0);
  TrkPar->fRMax = Trk->fD0 + 2*TrkPar->fRadius;

}

void TAnaModule::InitHelixPar(TStnHelix* Hlx, HelixAna::HelixPar_t* HlxPar) {

  // set pointer to TStnTrack
  HlxPar->fHelix = Hlx;

  // compute parameters not contained in TStnHelix
  HlxPar->fRMax = Hlx->D0() + 2.*Hlx->Radius();

}

}
