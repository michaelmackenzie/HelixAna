///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __HelixAna_ana_TAnaModule_hh__
#define __HelixAna_ana_TAnaModule_hh__

// ROOT includes
#include "TH1.h"

// CLHEP includes
#include "CLHEP/Units/PhysicalConstants.h"

// Stntuple includes
#include "Stntuple/loop/TStnModule.hh"
#include "Stntuple/alg/TStntuple.hh"
#include "Stntuple/obj/TStnTrack.hh"
#include "Stntuple/obj/TGenParticle.hh"
#include "Stntuple/obj/TSimParticle.hh"

// local includes
#include "HelixAna/ana/EventPar_t.hh"
#include "HelixAna/ana/TrackPar_t.hh"
#include "HelixAna/ana/HelixPar_t.hh"
#include "HelixAna/ana/EventHist_t.hh"
#include "HelixAna/ana/GenpHist_t.hh"
#include "HelixAna/ana/SimpHist_t.hh"
#include "HelixAna/ana/TrackHist_t.hh"
#include "HelixAna/ana/HelixHist_t.hh"

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
  void    BookGenpHistograms      (HelixAna::GenpHist_t*  Hist, const char* Folder);
  void    BookSimpHistograms      (HelixAna::SimpHist_t*  Hist, const char* Folder);
  void    BookTrackHistograms     (HelixAna::TrackHist_t* Hist, const char* Folder);
  void    BookHelixHistograms     (HelixAna::HelixHist_t* Hist, const char* Folder);

  void    FillEventHistograms     (HelixAna::EventHist_t* Hist, HelixAna::EventPar_t*  EvtPar);
  void    FillGenpHistograms      (HelixAna::GenpHist_t*  Hist, TGenParticle* Genp   );
  void    FillSimpHistograms      (HelixAna::SimpHist_t*  Hist, TSimParticle* Simp   );
  void    FillTrackHistograms     (HelixAna::TrackHist_t* Hist, HelixAna::TrackPar_t*  TrkPar);
  void    FillHelixHistograms     (HelixAna::HelixHist_t* Hist, HelixAna::HelixPar_t*  HlxPar);

  void    InitTrackPar            (TStnTrack* Trk, HelixAna::TrackPar_t* TrkPar);
  void    InitHelixPar            (TStnHelix* Hlx, HelixAna::HelixPar_t* HlxPar);

  ClassDef(TAnaModule,0)
};
}
#endif
