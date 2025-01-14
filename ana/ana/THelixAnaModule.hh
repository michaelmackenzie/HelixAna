///////////////////////////////////////////////////////////////////////////////
// Module to study helices from APR and CPR
///////////////////////////////////////////////////////////////////////////////
#ifndef HelixAna_ana_THelixAnaModule_hh
#define HelixAna_ana_THelixAnaModule_hh

// local includes
#include "HelixAna/ana/EventPar_t.hh"
#include "HelixAna/ana/TrackPar_t.hh"
#include "HelixAna/ana/EventHist_t.hh"
#include "HelixAna/ana/TrackHist_t.hh"
#include "HelixAna/ana/HelixCompHist_t.hh"
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
class THelixAnaModule: public TAnaModule {
public:

  enum { kNEventHistSets          = 100};
  enum { kNGenpHistSets           =  10};
  enum { kNSimpHistSets           =  10};
  enum { kNTrackHistSets          = 100};
  enum { kNHelixHistSets          = 100};
  enum { kNHelixCompHistSets      = 100};
  enum { kMaxHelices              =  10}; //maximum helices matched in an event
  enum { kHelixID_P, kHelixID_T, kHelixID_R, kHelixID_MCP, kHelixID_MCMatch}; //Helix ID selections

  struct Hist_t {
    HelixAna::EventHist_t*     fEvent[kNEventHistSets];
    HelixAna::GenpHist_t*      fGenp [kNGenpHistSets ];
    HelixAna::SimpHist_t*      fSimp [kNSimpHistSets ];
    HelixAna::TrackHist_t*     fTrack[kNTrackHistSets];
    HelixAna::HelixHist_t*     fHelix[kNHelixHistSets];
    HelixAna::HelixCompHist_t* fHelixComp[kNHelixCompHistSets];
  };

  // structure for mapping corresponding helices and tracks
  struct HelixPair_t {
    TStnHelix* fAprHelix = nullptr;
    TStnHelix* fCprHelix = nullptr;
    TStnHelix* fMergedHelix = nullptr;
    TStnTrack* fAprTrack = nullptr;
    TStnTrack* fCprTrack = nullptr;
    TStnTrack* fMergedTrack = nullptr;
  };

  HelixPair_t fMatchedHelices[kMaxHelices];

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
  TStnHelixBlock*          fCprDeHelixBlock;
  TStnHelixBlock*          fCprUeHelixBlock;
  TStnHelixBlock*          fMergedHelixBlock;
  TStnTrackBlock*          fAprTrackBlock;
  TStnTrackBlock*          fCprTrackBlock;
  TStnTrackBlock*          fMergedTrackBlock;
  TString                  fAprHelixBlockName;
  TString                  fCprDeHelixBlockName;
  TString                  fCprUeHelixBlockName;
  TString                  fMergedHelixBlockName;
  TString                  fAprTrackBlockName;
  TString                  fCprTrackBlockName;
  TString                  fMergedTrackBlockName;

  TStnTrack*               fTrack;
  TStnHelix*               fHelix;
  TGenParticle*            fGen;

  HelixAna::EventPar_t    fEvtPar;
  HelixAna::TrackPar_t    fTrkPar;
  HelixAna::HelixPar_t    fHlxPar;
  Hist_t                   fHist;

//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
  THelixAnaModule(const char* name="HelixAna_HelixAna", const char* title="HelixAna");
  ~THelixAnaModule();

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
  void    BookHelixCompHistograms(HelixAna::HelixCompHist_t* Hist, const char* Folder);
  void    BookHelixHistograms(HelixAna::HelixHist_t* Hist, const char* Folder);
  void    BookHistograms();
  void    FillHelixCompHistograms(HelixAna::HelixCompHist_t* Hist, HelixPair_t& Match);
  void    FillHelixHistograms(HelixAna::HelixHist_t* Hist, HelixAna::HelixPar_t*  HlxPar);
  void    FillHistograms();

//-----------------------------------------------------------------------------
// custom functions
//-----------------------------------------------------------------------------

  void MatchHelices();
  bool CompareHelices(TStnHelix* h1, TStnHelix* h2);
  TStnTrack* GetMatchingTrack(TStnHelix* h, int h_index, TStnTrackBlock* block);
  int HelixID(TStnHelix* h, HelixPar_t* hpar);
  int TrackID(TStnTrack* t, TrackPar_t* tpar);

  void Debug();

};
}
#endif
