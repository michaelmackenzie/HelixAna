#!/usr/bin/env python
#
import os, re, string, subprocess, sys, importlib
Import('env')
#
sys.path.append(os.getenv("MUSE_WORK_DIR")+'/site_scons')
#------------------------------------------------------------------------------
# print("Stntuple/SConscript:muse branch: PWD:"+os.getenv("PWD"))

x = subprocess.call(os.getenv("MUSE_WORK_DIR")+'/HelixAna/scripts/build_config_muse HelixAna',shell=True)
HelixAna_env = env.Clone()
#------------------------------------------------------------------------------
# done
#------------------------------------------------------------------------------
exec(open(os.environ['MUSE_WORK_DIR']+"/site_scons/stntuple_site_init.py").read())
from stntuple_helper import *
#------------------------------------------------------------------------------
# last two components of the path. Ex: /not/this/but/THIS/AND_THIS
#                                      "AND_THIS" is usually "src"
#------------------------------------------------------------------------------
w   = os.getenv('MUSE_BUILD_DIR').split('/')
w1  = w[0:len(w)-2]
dir = '/'.join(w1)
# print('dir = ',dir)

HelixAna_env['CPPPATH' ].append('-I'+dir+'/include');
HelixAna_env['CXXFLAGS'].append('-I'+dir+'/include');
#------------------------------------------------------------------------------
# done
#------------------------------------------------------------------------------
HelixAna_env.Append(BUILDERS = {'StntupleCodegen'  : stntuple_codegen})
HelixAna_env.Append(BUILDERS = {'StntupleRootCint' : stntuple_rootcint})

Export('HelixAna_env')
Export('stntuple_helper')
