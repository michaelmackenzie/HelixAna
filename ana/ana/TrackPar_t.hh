#ifndef __ePlus2024_ana_TrackPar_t__
#define __ePlus2024_ana_TrackPar_t__

#include "Stntuple/obj/TStnTrack.hh"

namespace ePlus2024 {

  struct TrackPar_t {
    
    TStnTrack* fTrack;

    float fMaxApproach;
    float fRadius;

  };
}
#endif
