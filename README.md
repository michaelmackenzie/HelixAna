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
git clone git@github.com:Mu2e/pbar2m.git
git clone git@github.com:michaelmackenzie/grim.git
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
# Using the HelixAna/ana/THelixAnaModule.cc module as defined in HelixAna/ana/scripts/hlx.C
$> root.exe
root> .L stnana.C
root> stnana("mdc2020ae","cele0b2s8r0000","","","HelixAna_hlx_ana()/save=cele0b2s8r0000.hist",10000)
root> stnana("mdc2020ae","cpos0b1s8r0000","","","HelixAna_hlx_ana()/save=cpos0b1s8r0000.hist",10000)
root> stnana("mdc2020ae","fele0b1s8r0000","","","HelixAna_hlx_ana()/save=fele0b1s8r0000.hist",10000)
root> stnana("mdc2020ae","fpos0b1s8r0000","","","HelixAna_hlx_ana()/save=fpos0b1s8r0000.hist",10000)
root> stnana("HelixAna","cpos0b1s5r0000","","","HelixAna_hlx_ana()/save=HelixAna.cpos0b1s5r0000.hist",10000)
```

## Adding a Stntuple analysis module

- Add a class definition (e.g. ana/ana/THelixAnaModule.hh)
- Implement class functions (e.g. ana/THelixAnaModule.cc)
- Add a link file (e.g. ana/ana/dict/THelixAnaModule_linkdef.h)
- Add the module to ana/scripts/modules.hh
- Add a function call for the event processing (e.g. ana/scripts/hlx.C) or a new script and add it to ana/scripts/load_stnana_scripts_HelixAna.C

## Stntuple production

Description for producing the input Stntuple datasets for analysis. The grid submission and job management is performed using the [GRIM](https://github.com/pavel1murat/grim/blob/main/doc/grim.org) package.

### Interactive testing
The input to Stntuple production are digi datasets.
```
# Setup access
kx509
voms-proxy-init -noregen -rfc -voms fermilab:/fermilab/mu2e/Role=Analysis
getToken
# Setup tools
setup mu2efiletools
setup dhtools
# Find datasets
mu2eDatasetList --tier=mcs --owner=mu2e --format=art | grep mcs.mu2e.CeEndpointOnSpillTriggered.MDC2020ae_best_v1_3.art
# Get an example file
FILE=`samListLocations -d --defname=dig.mu2e.CeEndpointMix2BBTriggered.MDC2020ae_best_v1_3.art | head -n 1`
echo $FILE
mu2e -c HelixAna/datasets/common/reco_stn.fcl -s ${FILE} -n 1
```

### Create a dataset family
Create a new dataset family ID under HelixAna/datasets/ following an existing example (e.g. cpos0b1). Define the stages in init_project.py along with the corresponding fcl.
Put input dataset file lists under `catalog`.

### Submitting a dataset processing
The `grim` package is used for grid job submission and monitoring. An example workflow:
```
kinit
kx509
voms-proxy-init -noregen -rfc -voms fermilab:/fermilab/mu2e/Role=Analysis
getToken

# Add the project if needed
source grim/scripts/create_project HelixAna
# --> Edit .grid_config to add HelixAna project

# Build the code tarball
grim/scripts/build_tarball.py --project=HelixAna
# --> Edit tarball name in .grid_config

# Dataset stage to process
INDSID="cpos0b1s41r0000" #Input dataset
STAGE="s5"
JOB="reco_stn"
OUTNAME="cpos0b1s5r0000"

# Generate fcl with only 1 segment for testing
grim/scripts/gen_fcl.py --project=HelixAna --dsid=${INDSID} --stage=${STAGE} --job=${JOB} --nseg=1
# Submit the test job
grim/scripts/submit_job.py --project=HelixAna --dsid=${INDSID} --stage=${STAGE} --job=${JOB} --doit=yes
# Monitor the jobs
grim/scripts/grid_monitor.py --project=HelixAna
grim/scripts/check_completed_job.py --project=HelixAna --jobid=${GRIDID}
# For recovery:
# grim/scripts/gen_fcl.py --project=<project> --recover=<grid_id>
# grim/scripts/submit_job.py --project=<project> --recover=<grid_id>

# Test running over the stn file (in ROOT shell)
.L stnana.C
stnana("file","","","${OUTPUTFILE}","HelixAna_hlx_ana()/save=${OUTNAME}.test.hist")

# Submitting full dataset processing
grim/scripts/gen_fcl.py --project=HelixAna --dsid=${INDSID} --stage=${STAGE} --job=${JOB}
grim/scripts/submit_job.py --project=HelixAna --dsid=${INDSID} --stage=${STAGE} --job=${JOB} --doit=yes
grim/scripts/grid_monitor.py --project=HelixAna
grim/scripts/check_completed_job.py --project=HelixAna --jobid=${GRIDID}
mkdir -p /exp/mu2e/data/users/${USER}/datasets/HelixAna/${OUTNAME}
setup ifdhc
ifdh cp ${OUTPATH}/00/000*/*.stn /exp/mu2e/data/users/${USER}/datasets/HelixAna/${OUTNAME}/
Stntuple/scripts/catalog_stntuples -b HelixAna -d ${OUTNAME} -u ${USER} -D /exp/mu2e/data/users/${USER}/datasets/HelixAna/${OUTNAME} -p stn
mv AAA_CATALOG.html.* /web/sites/mu2e.fnal.gov/htdocs/atwork/computing/Stntuple/cafdfc/HelixAna/${OUTNAME}/AAA_CATALOG.html
mv cpos0b1s5r0000.files.* /web/sites/mu2e.fnal.gov/htdocs/atwork/computing/Stntuple/cafdfc/HelixAna/${OUTNAME}/AAA_FILES.txt

# Test running over the dataset (in ROOT shell)
.L stnana.C
stnana("HelixAna","${OUTNAME}","","","HelixAna_hlx_ana()/save=HelixAna.${OUTNAME}.hist")
```