///////////////////////////////////////////////////////////////////////////////
// Module to study RMC pair conversions
///////////////////////////////////////////////////////////////////////////////
#ifndef HelixAna_ana_TRMCConvAnaModule_hh
#define HelixAna_ana_TRMCConvAnaModule_hh

// local includes
#include "HelixAna/ana/EventPar_t.hh"
#include "HelixAna/ana/TrackPar_t.hh"
#include "HelixAna/ana/HelixPar_t.hh"
#include "HelixAna/ana/RMCPar_t.hh"
#include "HelixAna/ana/EventHist_t.hh"
#include "HelixAna/ana/TrackHist_t.hh"
#include "HelixAna/ana/RMCHist_t.hh"
#include "HelixAna/ana/TAnaModule.hh"

// Stntuple includes
#include "Stntuple/obj/TStnHelix.hh"
#include "Stntuple/obj/TStnTrack.hh"
#include "Stntuple/obj/TStnEvent.hh"
#include "Stntuple/obj/TGenParticle.hh"
#include "Stntuple/obj/TSimParticle.hh"
#include "Stntuple/obj/TStnHeaderBlock.hh"
#include "Stntuple/obj/TStnHelixBlock.hh"
#include "Stntuple/obj/TStnTrackBlock.hh"
#include "Stntuple/obj/TStnTriggerBlock.hh"
#include "Stntuple/obj/TGenpBlock.hh"
#include "Stntuple/obj/TSimpBlock.hh"

namespace HelixAna {
class TRMCConvAnaModule: public TAnaModule {
public:

  enum { kNEventHistSets          = 100};
  enum { kNGenpHistSets           =  10};
  enum { kNSimpHistSets           =  10};
  enum { kNTrackHistSets          = 100};
  enum { kNHelixHistSets          = 100};
  enum { kNRMCHistSets            = 100};

  struct Hist_t {
    HelixAna::EventHist_t*     fEvent[kNEventHistSets];
    HelixAna::GenpHist_t*      fGenp [kNGenpHistSets ];
    HelixAna::SimpHist_t*      fSimp [kNSimpHistSets ];
    HelixAna::TrackHist_t*     fTrack[kNTrackHistSets];
    HelixAna::HelixHist_t*     fHelix[kNHelixHistSets];
    HelixAna::RMCHist_t*       fRMC  [kNRMCHistSets  ];
  };

//-----------------------------------------------------------------------------
//  data members
//-----------------------------------------------------------------------------
public:

  TStnTriggerBlock*        fTriggerBlock;
  TString                  fTriggerBlockName;
  TGenpBlock*              fGenpBlock = nullptr;
  TString                  fGenpBlockName;
  TSimpBlock*              fSimpBlock = nullptr;
  TString                  fSimpBlockName;

  TStnHelixBlock*          fAprHelixBlock;
  TStnHelixBlock*          fCprHelixBlock;
  TStnHelixBlock*          fOfflineHelixBlock;
  TStnTrackBlock*          fAprTrackBlock;
  TStnTrackBlock*          fCprTrackBlock;
  TStnTrackBlock*          fOfflineTrackBlock;
  TString                  fAprHelixBlockName;
  TString                  fCprHelixBlockName;
  TString                  fOfflineHelixBlockName;
  TString                  fAprTrackBlockName;
  TString                  fCprTrackBlockName;
  TString                  fOfflineTrackBlockName;

  TStnTrack*               fTrack;
  TStnHelix*               fHelix;
  TGenParticle*            fGen;
  TSimParticle*            fPhoton;
  TSimParticle*            fPositron;
  TSimParticle*            fElectron;

  HelixAna::EventPar_t     fEvtPar;
  HelixAna::TrackPar_t     fTrkPar;
  HelixAna::HelixPar_t     fHlxPar;
  HelixAna::RMCPar_t       fRMCPar;
  Hist_t                   fHist;

//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
  TRMCConvAnaModule(const char* name="HelixAna_RMCConvAna", const char* title="RMCConvAna");
  ~TRMCConvAnaModule();

//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  Hist_t*            GetHist        () { return &fHist;        }

//-----------------------------------------------------------------------------
// overloaded methods of TStnModule
//-----------------------------------------------------------------------------
  int     BeginJob();
  int     BeginRun();
  int     Event   (int ientry);
  int     EndJob  ();
//-----------------------------------------------------------------------------
// other methods
//-----------------------------------------------------------------------------
  void    BookRMCHistograms(HelixAna::RMCHist_t* Hist, const char* Folder);
  void    BookHelixHistograms(HelixAna::HelixHist_t* Hist, const char* Folder);
  void    BookHistograms();
  void    FillRMCHistograms(HelixAna::RMCHist_t* Hist);
  void    FillHelixHistograms(HelixAna::HelixHist_t* Hist, HelixAna::HelixPar_t*  HlxPar);
  void    FillHistograms();
  void    InitRMCPar();

//-----------------------------------------------------------------------------
// custom functions
//-----------------------------------------------------------------------------

  int HelixID(TStnHelix* h, HelixPar_t* hpar);
  int TrackID(TStnTrack* t, TrackPar_t* tpar);

  void Debug();

};
}
#endif
