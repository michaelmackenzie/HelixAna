///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/scripts/global_vars.h"
#include "HelixAna/ana/scripts/modules.hh"

def_name HelixAna_tst_001("HelixAna_tst_ana");

void  HelixAna_tst_ana(int DebugBit = -1) {
  HelixAna::m_tst = (HelixAna::TTestAnaModule*) g.x->AddModule("HelixAna::TTestAnaModule",0);

  if (DebugBit >= 0) HelixAna::m_tst->SetDebugBit(DebugBit,1);
}
