#ifndef __HelixAna_ana_EventPar_t__
#define __HelixAna_ana_EventPar_t__

namespace HelixAna {

  struct EventPar_t {
    
    float         fInstLum;
    int           fNAprTracks;  // for trigger analysis
    int           fNCprTracks;  // for trigger analysis
    int           fNTracks;

    bool          fPassedAprPath;  
    bool          fPassedCprPath;

  };
}
#endif
