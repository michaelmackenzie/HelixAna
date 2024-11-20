///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef HelixAna_ana_TTestAnaModule_hh
#define HelixAna_ana_TTestAnaModule_hh

#include "HelixAna/ana/EventPar_t.hh"

#include "HelixAna/ana/TAnaModule.hh"

namespace HelixAna {
class TTestAnaModule: public TAnaModule {
public:

  enum { kNEventHistSets          =   1 };

  struct Hist_t {
    HelixAna::EventHist_t*           fEvent[kNEventHistSets];
  };

//-----------------------------------------------------------------------------
//  data members
//-----------------------------------------------------------------------------
public:

  HelixAna::EventPar_t    fEvtPar;        // defined in TAnaModule.hh
  Hist_t                   fHist;          // histograms fille

//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
  TTestAnaModule(const char* name="HelixAna_TestAna", const char* title="TestAna");
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
