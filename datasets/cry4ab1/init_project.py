#!/usr/bin/python

from local_classes import *
#from mixing_inputs import *

class Project(ProjectBase):

    def init_datasets(self):
#------------------------------------------------------------------------------
# init datasets - for stage 5, the input is a dataset produced at stage 4
#------------------------------------------------------------------------------        
        self.add_dataset(Dataset('dig.mu2e.cry4ab1s41r0000.HelixAna.art' ,'cry4ab1s41r0000','local'))
        return


    def __init__(self,idsid=None):
        familyID                     = 'cry4ab1'

        ProjectBase.__init__(self,project='HelixAna',family_id='cry4ab1',idsid=idsid);
        self.init_datasets();
#------------------------------------------------------------------------------
# a Stage can have one or several jobs associated with it
# s4:pbar1b0s31r0000:mix_sim_digi : simulation with pileup
# for a job with an input dataset, 'job.fNInputFiles' is defined by the input dataset
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s5'); #s4
        job                          = s.new_job('reco_stn','cry4ab1s41r0000'); 

        job.fRunNumber               = 1202;
        job.fRecoVersion             = '00';

        job.fNInputFiles             =  -1                                        # placeholder, 
        #define_mixing_inputs(job);

        job.fMaxInputFilesPerSegment =  5                                          # process each run separately
        # job.fNEventsPerSegment       =  90000                                      # placeholder
        job.fResample                = 'no'                                        # yes/no
        job.fMaxMemory               = '30000MB'
        job.fRequestedTime           = '48h'
        job.fIfdh                    = 'xrootd'                                  # ifdh/xrootd

        odsid                        = self.fFamilyID+s.name()+'r'+job.fRecoVersion+'00'; #s41

        job.fOutputStream            = [ 'defaultOutput'                            ]
        job.fOutputDsID              = [ odsid                                      ]
        job.fOutputFnPattern         = [ 'nts.mu2e.'+job.fOutputDsID[0]+'.'+self.fProjectName ]
        job.fOutputFormat            = [ 'stn:root'                                 ]
#------------------------------------------------------------------------------
# end
#------------------------------------------------------------------------------
