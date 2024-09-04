///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef ePlus2024_ana_TTestAnaModule_hh
#define ePlus2024_ana_TTestAnaModule_hh

#include "ePlus2024/ana/ana/EventPar_t.hh"

#include "ePlus2024/ana/ana/TAnaModule.hh"

namespace ePlus2024 {
class TTestAnaModule: public TAnaModule {
public:

  enum { kNEventHistSets          =   1 };

  struct Hist_t {
    ePlus2024::EventHist_t*           fEvent[kNEventHistSets];
  };
  
//-----------------------------------------------------------------------------
//  data members
//-----------------------------------------------------------------------------
public:

  ePlus2024::EventPar_t    fEvtPar;        // defined in TAnaModule.hh
  Hist_t                   fHist;          // histograms fille
  
//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
  TTestAnaModule(const char* name="ePlus2024_TestAna", const char* title="TestAna");
  ~TTestAnaModule();
  
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
