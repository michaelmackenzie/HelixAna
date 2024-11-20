///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/scripts/global_vars.h"
#include "HelixAna/ana/scripts/modules.hh"

def_name HelixAna_hlx_001("HelixAna_hlx_ana");

void  HelixAna_hlx_ana(int DebugBit = -1) {
  HelixAna::m_hlx = (HelixAna::THelixAnaModule*) g.x->AddModule("HelixAna::THelixAnaModule",0);

  if (DebugBit >= 0) HelixAna::m_hlx->SetDebugBit(DebugBit,1);
}
