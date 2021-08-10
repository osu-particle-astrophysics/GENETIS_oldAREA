#!/bin/bash
#SBATCH -A PAS0654
#SBATCH -t 8:00:00
#SBATCH -N 1
#SBATCH -n 8
#SBATCH --output=/users/PAS0654/fahimi5/AREA/GA/%x/output/oscRun_%a.out
#SBATCH --error=/users/PAS0654/fahimi5/AREA/GA/%x/error/oscRun_%a.err

# What number individual we're running
INDIV=$(($((${SLURM_ARRAY_TASK_ID}-1))/${SEEDS}))

# What number job for the individual
JOB=$(($((${SLURM_ARRAY_TASK_ID}-1))%${SEEDS}+1))


source /fs/project/PAS0654/BiconeEvolutionOSC/new_root/new_root_setup.sh

GAPATH='/users/PAS0654/fahimi5/AREA/GA'
ARAPATH='/users/PAS0654/fahimi5/AREA/AraSim'

cd $ARAPATH

echo "Running AraSim for generation $GENER and child $INDIV and job $JOB"
#./AraSim setup.txt 1 ./outputs $GAPATH/$RUNPATH/gen_$GENER/child_${KID}.txt | tee $GAPATH/$RUNPATH/gen_$GENER/temp_${KID}.txt
./AraSim setup.txt $SLURM_ARRAY_TASK_ID ./outputs $GAPATH/$RUNPATH/gen_$GENER/child_${INDIV}.txt > $TMPDIR/temp_${INDIV}_${JOB}.txt

cd $TMPDIR
sleep 30

#pbsdcp -g * $GAPATH/$RUNPATH/gen_$GENER/temp_${KID}.txt
cp -R * $GAPATH/$RUNPATH/gen_$GENER/temp_${INDIV}_${JOB}.txt


cd $GAPATH/$RUNPATH/Flag_Files
echo "Creating Flag File for child ${INDIV} and job ${JOB}"
touch ${INDIV}_${JOB}.txt
