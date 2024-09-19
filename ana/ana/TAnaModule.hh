///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __ePlus2024_ana_TAnaModule_hh__
#define __ePlus2024_ana_TAnaModule_hh__

#include "TH1.h"

#include "Stntuple/loop/TStnModule.hh"

#include "Stntuple/alg/TStntuple.hh"

#include "Stntuple/obj/TStnTrack.hh"

#include "ePlus2024/ana/EventPar_t.hh"
#include "ePlus2024/ana/TrackPar_t.hh"

#include "ePlus2024/ana/EventHist_t.hh"
#include "ePlus2024/ana/TrackHist_t.hh"

namespace ePlus2024 {
class TAnaModule: public TStnModule {
public:
//-----------------------------------------------------------------------------
//  data members
//-----------------------------------------------------------------------------
public:
  ePlus2024::EventPar_t  fEvtPar;
  TStntuple*             fStnt;                   // STNTUPLE singleton
  
//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
  TAnaModule(const char* name="ePlus2024_Ana", const char* title="Ana");
  ~TAnaModule();
  
//-----------------------------------------------------------------------------
// overloaded methods of TStnModule
//-----------------------------------------------------------------------------
  virtual int     BeginJob();
  virtual int     BeginRun();

//-----------------------------------------------------------------------------
// other methods
//-----------------------------------------------------------------------------
  void    BookEventHistograms     (ePlus2024::EventHist_t* Hist, const char* Folder);
  void    BookTrackHistograms     (ePlus2024::TrackHist_t* Hist, const char* Folder);

  void    FillEventHistograms     (ePlus2024::EventHist_t* Hist, ePlus2024::EventPar_t*  EvtPar);
  void    FillTrackHistograms     (ePlus2024::TrackHist_t* Hist, ePlus2024::TrackPar_t*  TrkPar);

  void    InitTrackPar            (TStnTrack* Trk, ePlus2024::TrackPar_t* TrkPar);

  ClassDef(TAnaModule,0)
};
}
#endif
