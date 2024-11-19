// #include "TF1.h"
// #include "TCanvas.h"
// #include "TPad.h"
// #include "TEnv.h"
// #include "TSystem.h"

// #include "Stntuple/loop/TStnAna.hh"
// #include "Stntuple/obj/TStnNode.hh"
#include "Stntuple/obj/TStnHeaderBlock.hh"
// #include "Stntuple/alg/TStntuple.hh"
// #include "Stntuple/geom/TDisk.hh"
// #include "Stntuple/val/stntuple_val_functions.hh"
//------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#include "HelixAna/ana/TTestAnaModule.hh"

namespace HelixAna {

//-----------------------------------------------------------------------------
TTestAnaModule::TTestAnaModule(const char* name, const char* title):
  TAnaModule(name,title)
{
}

//-----------------------------------------------------------------------------
TTestAnaModule::~TTestAnaModule() {
}

//-----------------------------------------------------------------------------
int TTestAnaModule::BeginJob() {

//-----------------------------------------------------------------------------
// book histograms
//-----------------------------------------------------------------------------
  BookHistograms();

  return 0;
}

//_____________________________________________________________________________
void TTestAnaModule::BookHistograms() {

  //  char name [200];
  //  char title[200];

  TFolder*    fol;
  TFolder*    hist_folder;
  char        folder_name[200];
  const char* folder_title;

  DeleteHistograms();
  hist_folder = (TFolder*) GetFolder()->FindObject("Hist");

//-----------------------------------------------------------------------------
// book event histograms
//-----------------------------------------------------------------------------
  TString* event_selection[kNEventHistSets];
  for (int i=0; i<kNEventHistSets; i++) event_selection[i] = 0;

  event_selection[0] = new TString("all events");

  for (int i=0; i<kNEventHistSets; i++) {
    if (event_selection[i] != 0) {
      sprintf(folder_name,"evt_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      folder_title    = event_selection[i]->Data();
      if (! fol) fol  = hist_folder->AddFolder(folder_name,folder_title);
      fHist.fEvent[i] = new EventHist_t;
      BookEventHistograms(fHist.fEvent[i],Form("Hist/%s",folder_name));
    }
  }

}

//_____________________________________________________________________________
int TTestAnaModule::BeginRun() {
  int rn = GetHeaderBlock()->RunNumber();
  TStntuple::Init(rn);
  return 0;
}

//_____________________________________________________________________________
void TTestAnaModule::FillHistograms() {

//-----------------------------------------------------------------------------
// 1. fill event histograms
//-----------------------------------------------------------------------------
  FillEventHistograms(fHist.fEvent[0],&fEvtPar);

}

//-----------------------------------------------------------------------------
int TTestAnaModule::Event(int ientry) {

  fEvtPar.fInstLum  = GetHeaderBlock()->fInstLum;

  FillHistograms();

  return 0;
}


//_____________________________________________________________________________
int TTestAnaModule::EndJob() {
  printf("----- end job: ---- %s\n",GetName());
  return 0;
}

}
