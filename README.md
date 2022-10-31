# AREA

## Introduction

This README documents instructions on how to setup and run the AREA software to optimize antenna gain patterns using genetic algorithms and AraSim. Some of the functionality of the AREA software is still under construction, so check back frequently for updates.

## Initial Setup

To clone the AREA repository for the first time, navigate to the desired directory in your OSC space and execute:
```
git clone https://github.com/osu-particle-astrophysics/GENETIS_AREA
```

Because the AraSim software is too large to be held within this Github repository, it does not automatically come with the repo when cloned. If you do not have a copy of AraSim accessible within your OSC area and need to copy one, a stable version can be copied from this location:
```
/fs/ess/PAS1960/BiconeEvolutionOSC/AraSim/
```

## Setup for AREA Jobs

AREA jobs are controlled using the bash script, ```GA_controller_job.sh```. This script will require some initial edits to several user-specific variables:
- General: Make sure that any environment variables at the top of the script point to your User (OSC username) and Project (PAS1960, PAS0654, etc.) space.
- Line #35: 'USER' must be changed to '$[Your OSC Username]'
- Line #40: 'RunName' must be changed to the path of the desired output directory. Should be changed for each run, or else previous jobs will be overwritten.
- Line #43: 'GAPATH' must be changed to the filepath of the subdirectory containing the GA. By default, this should be the ```(...)/AREA/GA``` subdirectory in our newly cloned AREA repository.
- Line #44: 'ARAPATH' must be changed to the filepath of the subdirectory containing the desired version of AraSim.
- Line #45: 'MAINPATH' must be changed to the path to your AREA directory.

There are also some environment variables that should be changed to reflect your user-specific information in the ```oscRun.sh``` script:
- General: Make sure that any environment variables at the top of the script point to your User (OSC username) and Project (PAS1960, PAS0654, etc.) space.

Some settings important to job submission on OSC should also be checked prior to submiting any jobs:
- Set the maximum time of your controller job:
-- This is set at the top of ```GA_controller_job.sh``` (Line #4) in hr:mm:ss format.
- Set the maximum time for each AraSim job that will be launched:
-- This is set at the top of ```oscRun.sh``` (Line #3) in hr:mm:ss format.
- Set the number of neutrinos to simulate in each AraSim job (NNU):
-- This is set in ```AraSim/setup.txt``` (Line #8). Note that this is a different setup.txt file than the one that exists inside of the main AREA directory.

## Launching AREA Jobs

After all of the necessary setup steps have been completed, AREA jobs can be submited. Navigate to your main ```/AREA/``` directory and execute a command of the form:

```
sbatch --export=ALL,A=$A,B=$B,C=$C,D=$D,E=$E,F=$F GA_controller_job.sh
```

For example:

```
sbatch --export=ALL,A=1,B=1,C=1,D=1,E=3,F=0 GA_controller_job.sh
```

The above command includes the following information for the submited job:
- A: number of roulette cross-over chromosomes
- B: number of roulette mutation chromosomes
- C: number of tournament cross-over chromosomes
- D: number of tournament mutation chromosomes
- E: number of generations to evolve
- F: starting generation (a value of zero indicates to start a new run)

So, the above example command would submit a new run comprised of 1 chromosome of each selection type that evolves for 3 generations. This would be a sufficient test job to make sure that everthing is running as expected.
