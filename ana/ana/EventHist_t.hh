#ifndef __ePlus2024_ana_EventHist_t_hh
#define __ePlus2024_ana_EventHist_t_hh

#include "TH1.h"

namespace ePlus2024 {

  struct EventHist_t {
    TH1F*    fInstLumi;
    TH1F*    fInstLumiApr; // only filled if apr triggered event
    TH1F*    fInstLumiCpr; // only filled if cpr triggered event
    TH1F*    fInstLumiAprCpr; // filled if either apr or cpr triggered event
    TH1F*    fNAprTracks;
    TH1F*    fNCprTracks;
  };

}
#endif
