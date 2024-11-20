#ifndef __HelixAna_ana_TrackPar_t__
#define __HelixAna_ana_TrackPar_t__

#include "Stntuple/obj/TStnTrack.hh"

namespace HelixAna {

  struct TrackPar_t {

    TStnTrack* fTrack;

    float fRMax;
    float fRadius;

  };
}
#endif
