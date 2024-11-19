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

mkdir -p /exp/mu2e/data/users/${USER}/builds/$DIR
ln -s /exp/mu2e/data/users/${USER}/builds/$DIR build

muse setup

# on mu2ebuild02
muse build -j20

# generate trigger code
python mu2e_trig_config/python/generateMenuFromJSON.py -mf mu2e_trig_config/data/physMenu.json -o mu2e_trig_config/gen -evtMode all
```
