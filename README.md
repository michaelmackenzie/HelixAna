# HelixAna

Analysis package to study Mu2e trigger and helix configuration and performance.

## Build

```
# Set up a muse build area
mu2einit
cd /exp/mu2e/app/users/${USER}/
DIR=Yale/trigger/
mkdir -p ${DIR}
cd ${DIR}

git clone git@github.com:Mu2e/Offline.git
git clone git@github.com:Mu2e/Production.git
git clone git@github.com:Mu2e/mu2e_trig_config.git
git clone git@github.com:Mu2e/Stntuple.git
git clone git@github.com:michaelmackenzie/HelixAna.git

cd Stntuple; git checkout 'gianipez/dev'; cd ..

mkdir -p /exp/mu2e/data/users/${USER}/builds/$DIR
ln -s /exp/mu2e/data/users/${USER}/builds/$DIR build

muse setup

# on mu2ebuild02
muse build -j20

# generate trigger code
python mu2e_trig_config/python/generateMenuFromJSON.py -mf mu2e_trig_config/data/physMenu.json -o mu2e_trig_config/gen -evtMode all

# Add the library path to the rootlogon.C file:
# gSystem->Load("$MUSE_BUILD_DIR/HelixAna/lib/libHelixAna_ana.so");
# Add the package to the .rootrc file:
# +Stnana.Package                HelixAna
```

## Data processing

Example Stntuple processing with `stnana`:
```
# Using the HelixAna/ana/THelixAnaModule.cc module as defined in Helix/ana/scripts/hlx.C
$> root.exe
root> .L stnana.C
root> stnana("mdc2020ae","cele0b2s8r0000","","","HelixAna_hlx_ana()/save=cele0b2s8r0000.hist",10000)
```

## Adding a module

- Add a class definition (e.g. ana/ana/THelixAnaModule.hh)
- Implement class functions (e.g. ana/THelixAnaModule.cc)
- Add a link file (e.g. ana/ana/dict/THelixAnaModule_linkdef.h)
- Add the module to ana/scripts/modules.hh
- Add a function call for the event processing (e.g. ana/scripts/hlx.C) or a new script and add it to ana/scripts/load_stnana_scripts_HelixAna.C
