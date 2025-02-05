///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/scripts/global_vars.h"
#include "HelixAna/ana/scripts/modules.hh"

def_name HelixAna_rmc_001("HelixAna_cnv_ana");

void  HelixAna_cnv_ana(int DebugBit = -1) {
  HelixAna::m_cnv = (HelixAna::TRMCConvAnaModule*) g.x->AddModule("HelixAna::TRMCConvAnaModule",0);

  if (DebugBit >= 0) HelixAna::m_cnv->SetDebugBit(DebugBit,1);
}
