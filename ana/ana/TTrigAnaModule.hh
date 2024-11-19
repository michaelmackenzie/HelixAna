///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef HelixAna_ana_TTrigAnaModule_hh
#define HelixAna_ana_TTrigAnaModule_hh

#include "HelixAna/ana/EventPar_t.hh"
#include "HelixAna/ana/TrackPar_t.hh"
#include "HelixAna/ana/TAnaModule.hh"

#include "Stntuple/obj/TStnHeaderBlock.hh"
#include "Stntuple/obj/TStnTrackBlock.hh"
#include "Stntuple/obj/TStnTriggerBlock.hh"

namespace HelixAna {
class TTrigAnaModule: public TAnaModule {
public:

  enum { kNEventHistSets          =   2 };
  enum { kNTrackHistSets          =   6 };

  struct Hist_t {
    HelixAna::EventHist_t*    fEvent[kNEventHistSets];
    HelixAna::TrackHist_t*    fTrack[kNTrackHistSets];
  };
  
//-----------------------------------------------------------------------------
//  data members
//-----------------------------------------------------------------------------
public:

  TStnTriggerBlock*        fTriggerBlock;
  TString                  fTriggerBlockName;
  
  TStnTrackBlock*          fAprTrackBlock;
  TStnTrackBlock*          fCprTrackBlock;
  TStnTrackBlock*          fOfflineTrackBlock;
  TString                  fAprTrackBlockName;
  TString                  fCprTrackBlockName;
  TString                  fOfflineTrackBlockName;
  TStnTrack*               fTrack;

  bool                     fGoodOfflineTrackExists;
  int                      fGoodOfflineTrackIndex;

  HelixAna::EventPar_t    fEvtPar;
  HelixAna::TrackPar_t    fTrkPar;
  Hist_t                   fHist;
  
//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
  TTrigAnaModule(const char* name="HelixAna_TrigAna", const char* title="TrigAna");
  ~TTrigAnaModule();
  
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
  void    BookHistograms();
  void    FillHistograms();

//-----------------------------------------------------------------------------
// custom functions
//-----------------------------------------------------------------------------
  // function used to set fGoodOfflineTrackExists as well as fGoodOfflineTrackIndex
  bool    GoodOfflineTrackExists();

};
}
#endif
