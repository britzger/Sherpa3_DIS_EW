# Runcards for rivet

To run Sherpa with rivet, the run card needs to contain the following block:

ANALYSIS: Rivet
RIVET:
  --ignore-beams: 1
  ANALYSES:
    - <your_analysis_tag 1>
    - <your analysis_tag 2>

The tags can thereby include generic Rivet analysis, shipped with the release, or user analyses
that have been compiled as a plugin before. 

Assuming your analysis file is MyNiceAnalysis.cc, the analysis name declared in there defines the
tag from above, simplye execute

rivet-buildplugin MyNiceAnalysis.cc

Once successfully compiled this should produce a RivetAnalysis.so shared lib, now simply add the 
path to this lib to the RIVET_ANALYSIS_PATH variable, e.g. via

export RIVET_ANALYSIS_PATH=<path where the lib resides>:$RIVET_ANALYSIS_PATH

this should do the trick ... 
