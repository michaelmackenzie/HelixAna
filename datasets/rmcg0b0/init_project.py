#!/usr/bin/python

from local_classes import *

class Project(ProjectBase):
    def init_datasets(self):
#------------------------------------------------------------------------------
# init datasets - for stage 3, the input is a dataset produced at stage 2
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('dig.mu2e.rmcg0b0s21r0000.HelixAna.art','rmcg0b0s21r0000','local'));
        self.add_dataset(Dataset('dig.mu2e.rmcg0b0s31r0000.HelixAna.art','rmcg0b0s31r0000','local'));
        self.add_dataset(Dataset('dig.mu2e.rmcg0b0s32r0000.HelixAna.art','rmcg0b0s32r0000','local'));
        self.add_dataset(Dataset('dig.mu2e.rmcg0b0s33r0000.HelixAna.art','rmcg0b0s33r0000','local'));


    def __init__(self,idsid=None):
        familyID                     = 'rmcg0b0'
        
        ProjectBase.__init__(self,project='HelixAna',family_id='rmcg0b0',idsid=idsid);
        self.init_datasets();
#------------------------------------------------------------------------------
# s3:sim_stops 
#------------------------------------------------------------------------------
        s                            = self.new_stage('s3');
        job                          = s.new_job('sim_stops','rmcg0b0s21r0000');

        job.fNInputFiles             = -1                        # placeholder

        job.fMaxInputFilesPerSegment =  1                        # MC generator
        job.fNEventsPerSegment       =  100000                   # 
        job.fResample                = 'yes'                     # yes/no
        job.fResamplingModuleLabel   = 'TargetStopResampler'

        job.fMaxMemory               = '3000MB'
        job.fRequestedTime           = '12h'
        job.fIfdh                    = 'ifdh'                    # ifdh/xrootd
        job.fOutputPath              = [ 'out' ]

        job.fOutputStream            = [ 'TargetOutput', 'WireOutput', 'IPAOutput' ]
        job.fOutputDsID              = [ 'RMCFlat'      ]
        job.fOutputFnPattern         = [ 'sim.mu2e.RMCFlatGammaStops', 'sim.mu2e.RMCFlatWireGammaStops', 'sim.mu2e.RMCFlatIPAGammaStops' ]
        job.fOutputFormat            = [ 'art'                     ]
#------------------------------------------------------------------------------
# s4:sim_conversions
#------------------------------------------------------------------------------
        s                            = self.new_stage('s4');
        job                          = s.new_job('sim_conversions','rmcg0b0s31r0000');

        job.fNInputFiles             = -1                        # placeholder

        job.fMaxInputFilesPerSegment =  1                        # MC generator
        job.fNEventsPerSegment       =  10000                    # 
        job.fResample                = 'yes'                     # yes/no
        job.fResamplingModuleLabel   = 'GammaConversionResampler'

        job.fMaxMemory               = '3000MB'
        job.fRequestedTime           = '12h'
        job.fIfdh                    = 'ifdh'                    # ifdh/xrootd
        job.fOutputPath              = [ 'out' ]

        job.fOutputStream            = [ 'PrimaryOutput' ]
        job.fOutputDsID              = [ 'RMCFlat'       ]
        job.fOutputFnPattern         = [ 'dts.mu2e.RMCFlatGammaConversions' ]
        job.fOutputFormat            = [ 'art'           ]
#------------------------------------------------------------------------------
# s5:reco: 
#------------------------------------------------------------------------------
        s                            = self.new_stage('s5');
        job                          = s.new_job('reco_stn','rmcg0b0s41r0000');

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
        job.fOutputFormat            = [ 'stn:root'                     ]
#------------------------------------------------------------------------------
# end
#------------------------------------------------------------------------------
