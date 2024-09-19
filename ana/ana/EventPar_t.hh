#ifndef __ePlus2024_ana_EventPar_t__
#define __ePlus2024_ana_EventPar_t__

namespace ePlus2024 {

  struct EventPar_t {
    
    float         fInstLum;
    int           fNAprTracks;
    int           fNCprTracks;

    bool          fPassedAprPath;
    bool          fPassedCprPath;

  };
}
#endif
