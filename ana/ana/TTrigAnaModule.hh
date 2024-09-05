///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef ePlus2024_ana_TTrigAnaModule_hh
#define ePlus2024_ana_TTrigAnaModule_hh

#include "ePlus2024/ana/EventPar_t.hh"
#include "ePlus2024/ana/TrackPar_t.hh"
#include "ePlus2024/ana/TAnaModule.hh"

#include "Stntuple/obj/TStnHeaderBlock.hh"
#include "Stntuple/obj/TStnTrackBlock.hh"

namespace ePlus2024 {
class TTrigAnaModule: public TAnaModule {
public:

  enum { kNEventHistSets          =   1 };
  enum { kNTrackHistSets          =   1 };

  struct Hist_t {
    ePlus2024::EventHist_t*    fEvent[kNEventHistSets];
    ePlus2024::TrackHist_t*    fTrack[kNTrackHistSets];
  };
  
//-----------------------------------------------------------------------------
//  data members
//-----------------------------------------------------------------------------
public:

  TStnTrackBlock*          fTrackBlockDe;
  TStnTrack*               fTrack;
  TString                  fTrackBlockNameDe;

  ePlus2024::EventPar_t    fEvtPar;
  ePlus2024::TrackPar_t*   fTrkPar;
  Hist_t                   fHist;
  
//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
  TTrigAnaModule(const char* name="ePlus2024_TrigAna", const char* title="TrigAna");
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

};
}
#endif
