///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __HelixAna_ana_TAnaModule_hh__
#define __HelixAna_ana_TAnaModule_hh__

#include "TH1.h"

#include "Stntuple/loop/TStnModule.hh"

#include "Stntuple/alg/TStntuple.hh"

#include "Stntuple/obj/TStnTrack.hh"

#include "HelixAna/ana/EventPar_t.hh"
#include "HelixAna/ana/TrackPar_t.hh"

#include "HelixAna/ana/EventHist_t.hh"
#include "HelixAna/ana/TrackHist_t.hh"

#include "CLHEP/Units/PhysicalConstants.h"

namespace HelixAna {
class TAnaModule: public TStnModule {
public:

//-----------------------------------------------------------------------------
//  define constexpr
//-----------------------------------------------------------------------------
  static constexpr float mmTconversion = CLHEP::c_light/1000.0;
  static constexpr float bz0           = 1.0;
  
//-----------------------------------------------------------------------------
//  data members
//-----------------------------------------------------------------------------
public:
  HelixAna::EventPar_t  fEvtPar;
  TStntuple*             fStnt;                   // STNTUPLE singleton
  
//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
  TAnaModule(const char* name="HelixAna_Ana", const char* title="Ana");
  ~TAnaModule();
  
//-----------------------------------------------------------------------------
// overloaded methods of TStnModule
//-----------------------------------------------------------------------------
  virtual int     BeginJob();
  virtual int     BeginRun();

//-----------------------------------------------------------------------------
// other methods
//-----------------------------------------------------------------------------
  void    BookEventHistograms     (HelixAna::EventHist_t* Hist, const char* Folder);
  void    BookTrackHistograms     (HelixAna::TrackHist_t* Hist, const char* Folder);

  void    FillEventHistograms     (HelixAna::EventHist_t* Hist, HelixAna::EventPar_t*  EvtPar);
  void    FillTrackHistograms     (HelixAna::TrackHist_t* Hist, HelixAna::TrackPar_t*  TrkPar);

  void    InitTrackPar            (TStnTrack* Trk, HelixAna::TrackPar_t* TrkPar);

  ClassDef(TAnaModule,0)
};
}
#endif
