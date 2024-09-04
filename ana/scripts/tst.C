///////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////
#include "ePlus2024/ana/scripts/modules.hh"

def_name ePlus2024_tst_001("ePlus2024_tst_ana");

void  ePlus2024_tst_ana(int DebugBit = -1) {
  ePlus2024::m_tst = (ePlus2024::TTestAnaModule*) g.x->AddModule("ePlus2024::TTestAnaModule",0);  

  if (DebugBit >= 0) ePlus2024::m_tst->SetDebugBit(DebugBit,1);
}
