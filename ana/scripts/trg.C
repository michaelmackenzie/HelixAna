///////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////
#include "ePlus2024/ana/scripts/modules.hh"

def_name ePlus2024_trg_001("ePlus2024_trg_ana");

void  ePlus2024_trg_ana(int DebugBit = -1) {
  ePlus2024::m_trg = (ePlus2024::TTrigAnaModule*) g.x->AddModule("ePlus2024::TTrigAnaModule",0);  

  if (DebugBit >= 0) ePlus2024::m_trg->SetDebugBit(DebugBit,1);
}
