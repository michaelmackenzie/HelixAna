#!/usr/bin/python

from local_classes import *

class Project(ProjectBase):
    def init_datasets(self):
#------------------------------------------------------------------------------
# init datasets - for stage 4, the input is a dataset produced at stage 3
#------------------------------------------------------------------------------        
        self.add_dataset(Dataset('dig.mu2e.mnbs0b1s41r0000.HelixAna.art','mnbs0b1s41r0000','local'));


    def __init__(self,idsid=None):
        familyID                     = 'mnbs0b1'
        
        ProjectBase.__init__(self,project='HelixAna',family_id='mnbs0b1',idsid=idsid);
        self.init_datasets();
#------------------------------------------------------------------------------
# s5:reco: 
#------------------------------------------------------------------------------
        s                            = self.new_stage('s5');
        job                          = s.new_job('reco_stn','mnbs0b1s41r0000'); #idsid);

        job.fNInputFiles             = 250                       # number of the job segments

        job.fMaxInputFilesPerSegment =  1                        # MC generator
        # job.fNEventsPerSegment       =  -1                     # defined by the input dataset
        job.fResample                = 'no'                      # yes/no
        job.fMaxMemory               = '10000MB'
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
