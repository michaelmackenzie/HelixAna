#ifndef __HelixAna_ana_TrackHist_t_hh
#define __HelixAna_ana_TrackHist_t_hh

#include "TH1.h"

namespace HelixAna {

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
