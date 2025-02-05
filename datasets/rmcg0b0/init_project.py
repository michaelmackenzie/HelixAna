#!/usr/bin/python

from local_classes import *
import os

class Project(ProjectBase):
    def init_datasets(self):
#------------------------------------------------------------------------------
# init datasets - for stage 3, the input is a dataset produced at stage 2
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('sim.mu2e.rmcg0b0s21r0000.HelixAna.art','rmcg0b0s21r0000','local')) # Muon stops
        self.add_dataset(Dataset('sim.mu2e.rmcg0b0s31r0000.HelixAna.art','rmcg0b0s31r0000','local')) # Gamma conversion points
        self.add_dataset(Dataset('sim.mu2e.rmcg0b0s32r0000.HelixAna.art','rmcg0b0s32r0000','local'))
        self.add_dataset(Dataset('sim.mu2e.rmcg0b0s33r0000.HelixAna.art','rmcg0b0s33r0000','local'))
        self.add_dataset(Dataset('sim.mu2e.rmcg0b0s41r0000.HelixAna.art','rmcg0b0s41r0000','local')) # Gamma conversions resampled
        self.add_dataset(Dataset('sim.mu2e.rmcg0b0s42r0000.HelixAna.art','rmcg0b0s42r0000','local'))
        self.add_dataset(Dataset('sim.mu2e.rmcg0b0s43r0000.HelixAna.art','rmcg0b0s43r0000','local'))
        self.add_dataset(Dataset('dig.mu2e.rmcg0b0s511r0000.HelixAna.art','rmcg0b0s511r0000','local')) # Two-track triggerable digi
        self.add_dataset(Dataset('dig.mu2e.rmcg0b0s521r0000.HelixAna.art','rmcg0b0s521r0000','local'))
        self.add_dataset(Dataset('dig.mu2e.rmcg0b0s531r0000.HelixAna.art','rmcg0b0s531r0000','local'))
        self.add_dataset(Dataset('dig.mu2e.rmcg0b0s512r0000.HelixAna.art','rmcg0b0s512r0000','local')) # Triggerable digi
        self.add_dataset(Dataset('dig.mu2e.rmcg0b0s522r0000.HelixAna.art','rmcg0b0s522r0000','local'))
        self.add_dataset(Dataset('dig.mu2e.rmcg0b0s532r0000.HelixAna.art','rmcg0b0s532r0000','local'))
        self.add_dataset(Dataset('dig.mu2e.rmcg0b0s513r0000.HelixAna.art','rmcg0b0s513r0000','local')) # Triggered digi
        self.add_dataset(Dataset('dig.mu2e.rmcg0b0s523r0000.HelixAna.art','rmcg0b0s523r0000','local'))
        self.add_dataset(Dataset('dig.mu2e.rmcg0b0s533r0000.HelixAna.art','rmcg0b0s533r0000','local'))


    def __init__(self,idsid=None):
        familyID                     = 'rmcg0b0'
        version                      = 'r0000'
        
        ProjectBase.__init__(self,project='HelixAna',family_id='rmcg0b0',idsid=idsid);
        self.init_datasets();
        user = os.getenv('USER')

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
        job.fOutputFnPattern         = [ 'sim.'+user+'.RMCFlatGammaStops', 'sim.'+user+'.RMCFlatWireGammaStops', 'sim.'+user+'.RMCFlatIPAGammaStops' ]
        job.fOutputFormat            = [ 'art'                     ]

#------------------------------------------------------------------------------
# s4:sim_conversions (Stopping target)
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
        job.fOutputFnPattern         = [ 'dts.'+user+'.RMCFlatGammaConversions' ]
        job.fOutputFormat            = [ 'art'           ]

#------------------------------------------------------------------------------
# s4:sim_wire_conversions (Stopping target wires)
#------------------------------------------------------------------------------
        s                            = self.new_stage('s4');
        job                          = s.new_job('sim_wire_conversions','rmcg0b0s32r0000');

        job.fNInputFiles             =  20                       # N(jobs) to submit

        job.fMaxInputFilesPerSegment =  1                        # MC generator
        job.fNEventsPerSegment       =  50000                    # N(events / job)
        job.fResample                = 'yes'                     # yes/no (all events in 1 file, so no need to use fcl resampling technology
        job.fUseRangeMaxSkip         =  1
        job.fResamplingModuleLabel   = 'GammaConversionResampler'
        job.fRunNumber               = 1210

        job.fMaxMemory               = '3000MB'
        job.fRequestedTime           = '12h'
        job.fIfdh                    = 'ifdh'                    # ifdh/xrootd
        job.fOutputPath              = [ 'out' ]

        job.fOutputStream            = [ 'PrimaryOutput'                            ]
        job.fOutputDsID              = [ familyID+'s42r0000'                        ]
        job.fOutputFnPattern         = [ 'dts.'+user+'.RMCFlatWireGammaConversions' ]
        job.fOutputFormat            = [ 'art'                                      ]

#------------------------------------------------------------------------------
# s5:digi: (Stopping target conversions)
#------------------------------------------------------------------------------
        s                            = self.new_stage('s5');
        job                          = s.new_job('digi','rmcg0b0s41r0000');

        job.fNInputFiles             = 100                       # number of the job segments

        job.fMaxInputFilesPerSegment =  10                       # MC generator
        # job.fNEventsPerSegment       =  -1                     # defined by the input dataset
        job.fResample                = 'no'                      # yes/no
        job.fMaxMemory               = '3000MB'
        job.fRequestedTime           = '12h'
        job.fIfdh                    = 'ifdh'                    # ifdh/xrootd
        job.fOutputPath              = [ 'out' ]

        job.fOutputStream            = [ 'TriggerableOutput'                ]
        job.fOutputDsID              = [ familyID+s.name()+version          ]
        job.fOutputFnPattern         = [ 'dig.'+user+'.'+job.fOutputDsID[0]+'-Triggerable' ]
        job.fOutputFormat            = [ 'art'                          ]

#------------------------------------------------------------------------------
# s5:digi_wire: (Stopping target wire conversions)
#------------------------------------------------------------------------------
        s                            = self.new_stage('s5');
        job                          = s.new_job('digi_wire','rmcg0b0s42r0000');

        job.fNInputFiles             = 1                         # number of the job segments from previous stage

        job.fMaxInputFilesPerSegment =  10                       # MC generator
        # job.fNEventsPerSegment       =  -1                     # defined by the input dataset
        job.fResample                = 'no'                      # yes/no
        job.fMaxMemory               = '3000MB'
        job.fRequestedTime           = '12h'
        job.fIfdh                    = 'xrootd'                  # ifdh/xrootd
        job.fOutputPath              = [ 'out' ]

        job.fOutputStream            = [ 'TriggeredOutput'                           ]
        job.fOutputDsID              = [ familyID+s.name()+'2'+version+'-Triggered'  ]
        job.fOutputFnPattern         = [ 'dig.'+user+'.'+job.fOutputDsID[0]          ]
        job.fOutputFormat            = [ 'art'                                       ]

#------------------------------------------------------------------------------
# s6:reco_stn: (Stopping target conversions)
#------------------------------------------------------------------------------
        s                            = self.new_stage('s6');
        job                          = s.new_job('reco_stn','rmcg0b0s512r0000'); # use the triggerable input

        job.fNInputFiles             = 100                       # number of the job segments

        job.fMaxInputFilesPerSegment =  10                       # MC generator
        # job.fNEventsPerSegment       =  -1                     # defined by the input dataset
        job.fResample                = 'no'                      # yes/no
        job.fMaxMemory               = '30000MB'
        job.fRequestedTime           = '12h'
        job.fIfdh                    = 'ifdh'                    # ifdh/xrootd
        job.fOutputPath              = [ 'out' ]

        job.fOutputStream            = [ 'defaultOutput'                ]
        job.fOutputDsID              = [ familyID+s.name()              ]
        job.fOutputFnPattern         = [ 'nts.' +user+'.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = [ 'stn:root'                     ]

#------------------------------------------------------------------------------
# s6:reco_stn_wire: (Stopping target wire conversions)
#------------------------------------------------------------------------------
        s                            = self.new_stage('s6');
        job                          = s.new_job('reco_stn_wire','rmcg0b0s521r0000'); # use the two-track triggerable input

        job.fNInputFiles             = 100                       # number of the job segments

        job.fMaxInputFilesPerSegment =  10                       # MC generator
        # job.fNEventsPerSegment       =  -1                     # defined by the input dataset
        job.fResample                = 'no'                      # yes/no
        job.fMaxMemory               = '30000MB'
        job.fRequestedTime           = '12h'
        job.fIfdh                    = 'ifdh'                    # ifdh/xrootd
        job.fOutputPath              = [ 'out' ]

        job.fOutputStream            = [ 'defaultOutput'                ]
        job.fOutputDsID              = [ familyID+s.name()              ]
        job.fOutputFnPattern         = [ 'nts.' +user+'.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = [ 'stn:root'                     ]

#------------------------------------------------------------------------------
# end
#------------------------------------------------------------------------------
