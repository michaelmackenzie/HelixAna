#ifndef HelixAna_ana_scripts_modules_hh
#define HelixAna_ana_scripts_modules_hh

#include "HelixAna/ana/TTestAnaModule.hh"
#include "HelixAna/ana/TTrigAnaModule.hh"

namespace HelixAna {

  class TTestAnaModule;
  class TTrigAnaModule;

  HelixAna::TTestAnaModule*          m_tst   = nullptr;
  HelixAna::TTrigAnaModule*          m_trg   = nullptr;

};

#endif
