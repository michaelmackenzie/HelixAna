#ifndef __HelixAna_ana_SimpHist_t_hh
#define __HelixAna_ana_SimpHist_t_hh

#include "TH1.h"

namespace HelixAna {

  struct SimpHist_t {
    TH1F*    fPdgCode;
    TH1F*    fCreationCode;
    TH1F*    fMomTargetEnd;
    TH1F*    fMomTrackerFront;
    TH1F*    fNStrawHits;
  };
}
#endif
