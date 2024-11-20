#ifndef __HelixAna_ana_EventPar_t__
#define __HelixAna_ana_EventPar_t__

namespace HelixAna {

  struct EventPar_t {

    float         fInstLum;
    int           fNAprHelices;
    int           fNCprHelices;
    int           fNMergedHelices;
    int           fNAprTracks;  // for trigger analysis
    int           fNCprTracks;  // for trigger analysis
    int           fNTracks;
    int           fNMatchedHelices;

    bool          fPassedAprPath;
    bool          fPassedCprPath;

  };
}
#endif
