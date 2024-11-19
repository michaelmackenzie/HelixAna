///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/scripts/global_vars.h"
#include "HelixAna/ana/scripts/modules.hh"

def_name HelixAna_trg_001("HelixAna_trg_ana");

void  HelixAna_trg_ana(int DebugBit = -1) {
  HelixAna::m_trg = (HelixAna::TTrigAnaModule*) g.x->AddModule("HelixAna::TTrigAnaModule",0);

  if (DebugBit >= 0) HelixAna::m_trg->SetDebugBit(DebugBit,1);
}
