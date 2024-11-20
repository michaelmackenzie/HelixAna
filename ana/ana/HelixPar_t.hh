#ifndef __HelixAna_ana_HelixPar_t__
#define __HelixAna_ana_HelixPar_t__

#include "Stntuple/obj/TStnHelix.hh"

namespace HelixAna {

  struct HelixPar_t {

    TStnHelix* fHelix;

    float fRMax;
    float fRadius;

  };
}
#endif
