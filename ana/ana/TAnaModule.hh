///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __ePlus2024_analysisModules_TAnaModule_hh__
#define __ePlus2024_analysisModules_TAnaModule_hh__

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "Math/PdfFuncMathCore.h"
#include "Math/ProbFuncMathCore.h"

#include "Stntuple/loop/TStnModule.hh"

#include "Stntuple/obj/TStnTimeClusterBlock.hh"
#include "Stntuple/obj/TStnHelixBlock.hh"
#include "Stntuple/obj/TStnTrackSeedBlock.hh"
#include "Stntuple/obj/TStnTrackBlock.hh"
#include "Stntuple/obj/TStnClusterBlock.hh"
#include "Stntuple/obj/TCalDataBlock.hh"
#include "Stntuple/obj/TStepPointMCBlock.hh"
#include "Stntuple/obj/TStrawDataBlock.hh"
#include "Stntuple/obj/TGenpBlock.hh"
#include "Stntuple/obj/TSimpBlock.hh"
#include "Stntuple/obj/TCrvClusterBlock.hh"
#include "Stntuple/obj/TCrvPulseBlock.hh"

#include "Stntuple/base/TStnArrayI.hh"

#include "Stntuple/geom/TStnCrystal.hh"
#include "Stntuple/alg/TStnTrackID.hh"
#include "Stntuple/alg/TEmuLogLH.hh"
#include "Stntuple/alg/TStntuple.hh"

#include "ePlus2024/analysisModules/include/EventPar_t.hh"

#include "ePlus2024/analysisModules/include/EventHist_t.hh"

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

  void    FillEventHistograms     (ePlus2024::EventHist_t* Hist, ePlus2024::EventPar_t*  EvtPar);

};
}
#endif
