#ifndef __ePlus2024_ana_TrackHist_t_hh
#define __ePlus2024_ana_TrackHist_t_hh

#include "TH1.h"

namespace ePlus2024 {

  struct TrackHist_t {
    TH1F*    fP;
    TH1F*    fPt;
    TH1F*    fD0;
    TH1F*    fDP;
    TH1F*    fChi2NDof;
    TH1F*    fTanDip;
    TH1F*    fRadius;
    TH1F*    fRMax;
    TH1F*    fNActive;
  };

}
#endif
