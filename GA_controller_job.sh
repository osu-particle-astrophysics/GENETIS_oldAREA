#!/bin/bash

#SBATCH -A PAS1960
#SBATCH -t 24:00:00
#SBATCH -N 1
#SBATCH -n 8
##SBATCH -o ~
##SBATCH -e ~
###PBS -o /users/PAS1960/breynolds/work/AREA/termoutput/
###PBS -e /users/PAS1960/breynolds/work/AREA/termoutput/


#source /fs/project/PAS0654/BiconeEvolutionOSC/new_root/new_root_setup.sh


# This script runs and controls the GA and looping

# Input arguments for this controller script are:
# $1 = Roulette cross-over
# $2 = Roulette mutation
# $3 = Tournament cross-over
# $4 = Tournament mutation
# $5 = number of generations to run
# $6 = index number of the starting generation, 0 for a new run

# There will be $1 + $2 + $3 + $4 chromosomes per generation

# To run this job, do the following (giving 1 to each algorithm and for 1 generation):
## qsub -v A=1,B=1,C=1,D=1,E=1 GA_controller_job.sh
## new way:
## sbatch --export=ALL,A=1,B=1,C=1,D=1,E=1,F=10 GA_controller_job.sh

#permissions variable
#USER=$whoami
USER=$breynolds

#**
# make a variable called RunName
#RunName='Runs/20220419fahimi5run1'
RunName='Runs/20221017breynoldsrun_lowNNUFixed_5Gen_lastTestBeforePush'

#Path Variables
GAPATH='/users/PAS1960/breynolds/work/AREA/GA'
ARAPATH='/users/PAS1960/breynolds/work/AraSim'
MAINPATH='/users/PAS1960/breynolds/work/AREA'

# Read input GA parameter arguments
let CHILD_NO=$(($A+$B+$C+$D))  # number of individuals are in each generation
let TOT=$CHILD_NO-1							        GEN_CNT=$F
SEEDS=4 # Number of AraSim jobs per individual 
NUM_JOBS=$((CHILD_NO*SEEDS))

#Remove the interrupted gen so we can start it over														# start the generation number from 0
rm -r $GAPATH/$RunName/gen_$GEN_CNT/*
rm -r $GAPATH/$RunName/Flag_Files/*

#recompile GA
cd $GAPATH
g++ -std=c++11 ara_GA.cc -o ara_GA
cd $MAINPATH

export TOT=$TOT	# exporting this makes it a variable any bash script can see
export GEN_CNT=$GEN_CNT
while [ $GEN_CNT -lt $E ]
do
	if [ $GEN_CNT -eq 0 ]
	then
		# Navigate to GA directory
		cd $GAPATH

		# Make Runs directory if it isnt already there
		mkdir -m775 ./Runs

		#**
		#make directory to store information for this run
		mkdir -m775 ./$RunName
		
		#**
		#Navigate to Run directory
		cd $RunName
		
		#Creat generation error and output files
		if [ $GEN_CNT -eq 0 ]
		then
			mkdir -m775 output
			mkdir -m775 error
		fi
		
		# Make a new folder for generation 0
		mkdir -m775 ./gen_$GEN_CNT

		#navigate to GA directory
		cd $GAPATH

		# Run the GA withGA parameters passed to this script
		# $1 = Roulette cross-over
		# $2 = Roulette mutation
		# $3 = Tournament cross-over
		# $4 = Tournament mutation
		# The GA only runs one generation and the makes children but to run the next generation the loop below is needed, the far right number which is 0 in this case is the current generation and the last input when running this script is just how many times the GA program is run						# -p represents the number of input parents (0 in gen 0)
		# -gp represents the number of children for each algorithm
		# hence, we have four numbers following -gp

		./ara_GA -gp $A $B $C $D -p 0

		# Move all the children to current generation folder
		mv child_* $GAPATH/$RunName/gen_$GEN_CNT
		
		# Move back to RunName Directory
		cd $RunName
	        
		# Begin simulating each chromosome in this generation
		# Run AraSim with setup.txt, once, with each current child, and pipe the Veff to a temp.txt file
		# Placed outside loop to simulate all children at once
		
		#Loop for submitting multiple AraSim jobs to the osc
	        echo Before Arasim!	
		mkdir -m775 Flag_Files
		flag_files=0
		
		cd $GAPATH
		cd ..
		
		sbatch --array=1-$NUM_JOBS%100 --export=ALL,GENER=$GEN_CNT,RUNPATH=$RunName,SEEDS=$SEEDS --job-name=${RunName} oscRun.sh
		
		
		# this is where we want to put in a wait function
		# don't continue until number of files in flag_directory = CHILD_NO
	        
		cd $GAPATH/$RunName/Flag_Files
		
		while [[ $flag_files -lt $NUM_JOBS ]]
		do
			echo "Waiting for the first batch of jobs to complete"
			sleep 30
			flag_files=$(ls -1 --file-type | grep -v '/$' | wc -l)
		done
		
		echo "After the while loop!"
		
		
		# At the end of the looping
		cd .. 
		rm -r Flag_Files/*
		
		cd $ARAPATH
		
		module load python/2.7-conda5.2
		
		sleep 5
                
                # Call a python script that grabs the fitness score from each job for each individual
                # Take the mean of those (because of multiple seeds for each individual), put them into fitnessScores.txt
                python $MAINPATH/stringReplacement2.py -i $GEN_CNT $CHILD_NO $SEEDS $GAPATH/$RunName

		echo stringReplacement2 executed for Gen_0

		let GEN_CNT=$GEN_CNT+1
		export GEN_CNT=$GEN_CNT
		# Repeat above process for each generation until $E is reached
	else
		while [ $GEN_CNT -lt $E ]
		do
		    echo "Beginning Gen : $GEN_CNT"
		
		    cd $GAPATH/$RunName
		    	
		    #change permissions
		    #chown $USER $GAPATH/$RunName/gen_$GEN_CNT $GAPATH/$RunName/gen_$GEN_CNT/temp* $GAPATH/$RunName/gen_$GEN_CNT/child*
		    
		    mkdir -m775 ./gen_$GEN_CNT

		    echo made directory for gen_$GEN_CNT
		
		    let LAST_CNT=$GEN_CNT-1
		
		    #Navigate to GA
		    cd $GAPATH
		    
		    # Run the GA with all chromosomes from last generation as potential parents
		    echo before GA for gen_$GEN_CNT

		    ./ara_GA -gp $A $B $C $D -p $CHILD_NO ./$RunName/gen_$LAST_CNT/child* 
		
		    echo after GA for gen_$GEN_CNT

		    # Take new children and move them to current generation folder
		    mv child_* $GAPATH/$RunName/gen_$GEN_CNT
			
		    # Navigate up a directory to run AraSim (EDITED)
		    cd $ARAPATH
		
		    flag_files=0
		
		    #go to main path explicitly
		    cd $MAINPATH

		    #for i in $(seq 0 $TOT)
		    #do
		    #    sbatch --export=ALL,KID=$i,GENER=$GEN_CNT,RUNPATH=$RunName oscRun.sh
		    #done

		    sbatch --array=1-$NUM_JOBS%100 --export=ALL,GENER=$GEN_CNT,RUNPATH=$RunName,SEEDS=$SEEDS --job-name=$RunName oscRun.sh
		
		
		    # this is where we want to put in a wait function
		    # don't continue until number of files in flag_directory = CHILD_NO
		    mkdir -m775 $GAPATH/$RunName/Flag_Files
		    cd $GAPATH/$RunName/Flag_Files
		
		    while [[ $flag_files -lt $NUM_JOBS ]]
		    do
			echo "Waiting for the first batch of jobs to complete"
			sleep 30
			flag_files=$(ls -1 --file-type | grep -v '/$' | wc -l)
		    done
		
		    echo "After the while loop!"
		
		
		    # At the end of the looping 
		    cd ..
		    rm -r Flag_Files/*		    
		    
		    #navigate to ARAPATH
		    cd $ARAPATH
		    
		    module load python/2.7-conda5.2
		    
		    sleep 5
		    
		    ### Call a python script that grabs the fitness score from each job for each individual
                    ### Take the mean of those (because multiple seeds per individual), put them into fitnessScores.text
                    python $MAINPATH/stringReplacement2.py -i $GEN_CNT $CHILD_NO $SEEDS $GAPATH/$RunName
		    
		    let GEN_CNT=$GEN_CNT+1
		    export GEN_CNT=$GEN_CNT
		done
	fi
done

cd $GAPATH

#often the runs are too long to finish, manually run this at the end of the run
#python plot_maxes_fixed.py ./$RunName/fitnessFile.txt
#python plot_all.py ./$RunName/fitnessFile.txt

printf "Roul. Xover : $A\nRoul. Mut :$B\nTour. Xover : $C\n Tour. Mut : $D\n Gen Cnt : $E\n " > $RunName/log.txt

echo "Done"_controller_job.sh.e1783513
