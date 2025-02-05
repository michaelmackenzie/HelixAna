#ifndef __HelixAna_ana_RMCPar_t__
#define __HelixAna_ana_RMCPar_t__

#include "Stntuple/obj/TStnHelix.hh"
#include "Stntuple/obj/TStnTrack.hh"

namespace HelixAna {

  struct RMCPar_t {

    TStnHelix* fElectronHelix;
    TStnHelix* fPositronHelix;
    TStnTrack* fElectronTrack;
    TStnTrack* fPositronTrack;

    // Reco info
    float fRecPairMom;

    // MC-truth info
    float fPhotonEnergy;
    float fPhotonRadius;
    float fPhotonCosZ;
    float fElectronEnergy;
    float fPositronEnergy;
    float fEnergySplitting;
    float fMass;
    float fTheta;
  };
}
#endif
