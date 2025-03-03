///////////////////////////////////////////////////////////////////////////////
// the first parameter is the script, the second - env.var telling whether the script has to be loaded,
// and if so, which directory the script resides in.
// If the corresponding env. variable is not defined, the script is not loaded. PWD is always defined
///////////////////////////////////////////////////////////////////////////////
#include "TInterpreter.h"
#include "HelixAna/ana/scripts/modules.hh"

int load_stnana_scripts_HelixAna() {
  char        macro[200];

  const char* script[] = {
    "tst.C"        , "PWD",
    "trg.C"        , "PWD",
    "hlx.C"        , "PWD",
    "rmc.C"        , "PWD",
    0
  };

  TInterpreter* cint = gROOT->GetInterpreter();

  for (int i=0; script[i] != 0; i+=2) {
    const char* dir = gSystem->Getenv(script[i+1]);
    if (dir) {
      sprintf(macro,"%s/HelixAna/ana/scripts/%s",dir,script[i]);
      if (! cint->IsLoaded(macro)) cint->LoadMacro(macro);
    }
  }

  return 0;
}
