#ifndef __HelixAna_ana_TrackHist_t_hh
#define __HelixAna_ana_TrackHist_t_hh

#include "TH1.h"

namespace HelixAna {

  struct TrackHist_t {
    TH1F*    fP;
    TH1F*    fPt;
    TH1F*    fPCenter;
    TH1F*    fPExit;
    TH1F*    fPST;
    TH1F*    fD0;
    TH1F*    fDP;
    TH1F*    fChi2NDof;
    TH1F*    fFitCons;
    TH1F*    fFitMomErr; //estimated uncertainty
    TH1F*    fTanDip;
    TH1F*    fRadius;
    TH1F*    fRMax;
    TH1F*    fNActive;
    TH1F*    fTrkQual;
    TH1F*    fClusterE;
    TH1F*    fDt;
    TH1F*    fEp;

    //MC truth information
    TH1F*    fMCPFront; //P(front of tracker)
    TH1F*    fMCPStOut; //P(ST exit)

  };

}
#endif
