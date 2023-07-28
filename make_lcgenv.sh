#!/bin/bash

OUTFILE=lcgenvg-openmpi.sh
 

export LCGENV_PATH=/cvmfs/sft.cern.ch/lcg/releases
#export LCG_VERSION=LCG_101_ATLAS_20
export LCG_VERSION=LCG_102b_ATLAS_23 
export PLATFORM=x86_64-centos7-gcc12-opt

#$LCGENV_PATH/lcgenv/latest/lcgenv_py2 -p $LCG_VERSION $PLATFORM sherap > $OUTFILE 
#$LCGENV_PATH/lcgenv/latest/lcgenv -p $LCG_VERSION $PLATFORM sherpa > $OUTFILE 
$LCGENV_PATH/lcgenv/latest/lcgenv -p $LCG_VERSION $PLATFORM  sherpa-openmpi > $OUTFILE 
$LCGENV_PATH/lcgenv/latest/lcgenv -p $LCG_VERSION $PLATFORM lhapdf >> $OUTFILE 

echo "export LHAPDF_DATA_PATH=\$LHAPDF_DATA_PATH:\$LHAPDF__HOME/share/LHAPDF" >> $OUTFILE
echo "export OL_PREFIX=\$OPENLOOPS__HOME" >> $OUTFILE
echo "export OPAL_PREFIX=\$OPENMPI__HOME" >> $OUTFILE
echo "run with mpi, usnig:  mpirun -n <n> Sherpa -e 0 Sherpa.WWbb.NLO.yaml" >> $OUTFILE
