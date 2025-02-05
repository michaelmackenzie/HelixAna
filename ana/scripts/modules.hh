#ifndef HelixAna_ana_scripts_modules_hh
#define HelixAna_ana_scripts_modules_hh

#include "HelixAna/ana/TTestAnaModule.hh"
#include "HelixAna/ana/TTrigAnaModule.hh"
#include "HelixAna/ana/THelixAnaModule.hh"
#include "HelixAna/ana/TRMCConvAnaModule.hh"

namespace HelixAna {

  class TTestAnaModule;
  class TTrigAnaModule;
  class THelixAnaModule;

  HelixAna::TTestAnaModule*          m_tst   = nullptr;
  HelixAna::TTrigAnaModule*          m_trg   = nullptr;
  HelixAna::THelixAnaModule*         m_hlx   = nullptr;
  HelixAna::TRMCConvAnaModule*       m_cnv   = nullptr;

};

#endif
