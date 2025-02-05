// Histograms specific to RMC studies

#ifndef __HelixAna_ana_RMCHist_t_hh
#define __HelixAna_ana_RMCHist_t_hh

#include "TH1.h"
#include "TH1F.h"

namespace HelixAna {

  struct RMCHist_t {
    TH1F*    fPairRecMom    ; //Reco info

    TH1F*    fPhotonEnergy  ; //MC-truth info
    TH1F*    fPositronEnergy;
    TH1F*    fElectronEnergy;
    TH1F*    fPhotonRadius  ;
    TH1F*    fPhotonCosZ    ;
  };

}
#endif
