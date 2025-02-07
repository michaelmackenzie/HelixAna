#!/usr/bin/python

from local_classes import *
import os

class Project(ProjectBase):
    def init_datasets(self):
#------------------------------------------------------------------------------
# init datasets - for stage 4, the input is a dataset produced at stage 3
#------------------------------------------------------------------------------        
        self.add_dataset(Dataset('dig.mu2e.fpos0b1s41r0000.HelixAna.art','fpos0b1s41r0000','local'));


    def __init__(self,idsid=None):
        familyID                     = 'fpos0b1'
        
        ProjectBase.__init__(self,project='HelixAna',family_id='fpos0b1',idsid=idsid);
        self.init_datasets();

#------------------------------------------------------------------------------
# s5:reco_stn: 
#------------------------------------------------------------------------------
        s                            = self.new_stage('s5');
        job                          = s.new_job('reco_stn','fpos0b1s41r0000'); #idsid);

        job.fNInputFiles             = 1854                     # number of the job segments

        job.fMaxInputFilesPerSegment =  20                      # MC generator
        # job.fNEventsPerSegment       =  -1                    # defined by the input dataset
        job.fResample                = 'no'                     # yes/no
        job.fMaxMemory               = '30000MB'
        job.fRequestedTime           = '12h'
        job.fIfdh                    = 'xrootd'                 # ifdh/xrootd
        job.fOutputPath              = [ 'out' ]

        job.fOutputStream            = [ 'defaultOutput'                ]
        job.fOutputDsID              = [ familyID+s.name()+'r0000'      ]
        job.fOutputFnPattern         = [ 'nts.'+os.getenv('USER')+'.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = [ 'stn'                          ]
#------------------------------------------------------------------------------
# end
#------------------------------------------------------------------------------
