#!/usr/bin/python

from local_classes import *

class Project(ProjectBase):
    def init_datasets(self):
#------------------------------------------------------------------------------
# init datasets - for stage 4, the input is a dataset produced at stage 3
#------------------------------------------------------------------------------        
        self.add_dataset(Dataset('dig.mu2e.cpos0b1s41r0000.ePlus2024.art','cpos0b1s41r0000','local'));


    def __init__(self,idsid=None):
        familyID                     = 'cpos0b1'
        
        ProjectBase.__init__(self,project='ePlus2024',family_id='cpos0b1',idsid=idsid);
        self.init_datasets();
#------------------------------------------------------------------------------
# s5:reco: 
#------------------------------------------------------------------------------
        s                            = self.new_stage('s5');
        job                          = s.new_job('reco_stn','cpos0b1s41r0000');

        job.fNInputFiles             = 1000                      # number of the job segments

        job.fMaxInputFilesPerSegment =  10                       # MC generator
        # job.fNEventsPerSegment       =  -1                     # defined by the input dataset
        job.fResample                = 'no'                      # yes/no
        job.fMaxMemory               = '30000MB'
        job.fRequestedTime           = '12h'
        job.fIfdh                    = 'ifdh'                    # ifdh/xrootd
        job.fOutputPath              = [ 'out' ]

        job.fOutputStream            = [ 'defaultOutput'                ]
        job.fOutputDsID              = [ familyID+s.name()+'r0000'      ]
        job.fOutputFnPattern         = [ 'dig.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = [ 'art'                          ]
#------------------------------------------------------------------------------
# end
#------------------------------------------------------------------------------
