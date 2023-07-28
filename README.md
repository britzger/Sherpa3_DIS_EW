# Electroweak processes in DIS with Sherpa3

In this repository, runcards and instructions for electroweak processes with Sherpa 3 are collected.

The webpage for Sherpa 3 is found at https://sherpa-team.gitlab.io/sherpa/v3.0.0beta1/

## Initialization
At any common CentOS7 system (DESY,CERN,etc..) simply source the lcg setup script
```
  $ source lcgenv-openmpi.sh
```
This will provide a setup of Sherpa3, and several other recent packages, like LHAPDF, Openloops, Rivet, etc...


## Execute
In a first step, Sherpa always generates the process information. This is done by calling
```
   Sherpa Sherpa.H.LO.yaml
```
Note, that this stage generates the `Process` directory and other files. When changing the process or runcard, then these directories need to be deleted.

In a second step, on can calcuate the cross section by executing
```
   Sherpa Sherpa.H.LO.yaml -e 0
```
The option `-e 0` deactivates the event generation.


For NLO matrix elements, on in general when the Amegic matrix element generator is used, one has to compile the libraries first, and it is useful to do the calculation with MPI support
```
   $ Sherpa -e 0 Sherpa.H.NLO.yaml
   $ ./makelibs
   $ mpirun -n <n> Sherpa -e 0 Sherpa.H.NLO.yaml
```
where `<n>` should be the number of parallel processes, e.g. 15.

The NLO cross section is the sum of the *(BVI)* and *(RS)* process.

