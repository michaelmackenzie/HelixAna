#ifndef __HelixAna_ana_HelixCompHist_t_hh
#define __HelixAna_ana_HelixCompHist_t_hh

#include "TH1.h"

namespace HelixAna {

  struct HelixCompHist_t {
    TH1F* fAprHelixDeltaP;
    TH1F* fAprTrackDeltaP;
    TH1F* fCprHelixDeltaP;
    TH1F* fCprTrackDeltaP;
    TH1F* fHelixDeltaP; //between the algorithms
    TH1F* fTrackDeltaP;
  };

}
#endif
