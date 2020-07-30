#!/bin/bash

# This script runs and controls the GA and looping

# Input arguments for this controller script are:
# $1 = Roulette cross-over
# $2 = Roulette mutation
# $3 = Tournament cross-over
# $4 = Tournament mutation
# $5 = number of generations to run

# There will be $1 + $2 + $3 + $4 chromosomes per generation

#permissions variable
#USER=$whoami

#Path Variables
GAPATH='/users/PAS0654/bensipe01/GA1/1.0/GA'
ARAPATH='/users/PAS0654/bensipe01/GA1/1.0/AraSim'

# Read input GA parameter arguments
let CHILD_NO=$1+$2+$3+$4						# number of individuals are in each generation
let TOT=$CHILD_NO-1									# probably for counting from 0?
GEN_CNT=0														# start the generation number from 0

export TOT=$TOT											# exporting this makes it a variable any bash script can see
export GEN_CNT=$GEN_CNT

# Navigate to GA directory
cd $GAPATH

# Make a new folder for generation 0
mkdir ./gen_$GEN_CNT

# Run the GA withGA parameters passed to this script
# $1 = Roulette cross-over
# $2 = Roulette mutation
# $3 = Tournament cross-over
# $4 = Tournament mutation
# The GA only runs one generation and the makes children but to run the next generation the loop below is needed, the far right number which is 0 in this case is the current generation and the last input when running this script is just how many times the GA program is run
./GA -gp $1 $2 $3 $4 -p 0						# -p represents the number of input parents (0 in gen 0)
																		# -gp represents the number of children for each algorithm
																			# hence, we have four numbers following -gp

# Move all the children to current generation folder
mv child_* $GAPATH/gen_$GEN_CNT

#change permissions
#chown $USER gen* temp* child*

# Navigate up a directory to run AraSim (EDITED)
#cd $ARAPATH

# Begin simulating each chromosome in this generation
# Run AraSim with setup.txt, once, with each current child, and pipe the Veff to a temp.txt file
# Placed outside loop to simulate all children at once

#edited out the -j16
#make -f GA_make.mk temp

#Loop for submitting multiple AraSim jobs to the osc
cd ..
for i in $(seq 0 $TOT)
do
    qsub -v 1=$i,2=$GEN_CNT oscRun.sh
done
cd $ARAPATH

for ind in $(seq 0 $TOT)
do
    # Replace the dummy Veff score in the child with the correct score found by AraSim 
    GREPOUT=$(grep "test Veff" $GAPATH/gen_$GEN_CNT/temp_$ind.txt)
    grep "Veff" $GAPATH/gen_$GEN_CNT/child_$ind.txt | xargs -I OutPutiFromGrep sed -e "s/^.*OutPutFromGrep.*$/$GREPOUT/" $GAPATH/gen_$GEN_CNT/child_$ind.txt | tee $GAPATH/gen_$GEN_CNT/childUpdated.txt &> out.log

    #change permissions
 #   chown $USER gen* temp* child*
    
    # Rename this updated child to the current child name
    mv $GAPATH/gen_$GEN_CNT/childUpdated.txt $GAPATH/gen_$GEN_CNT/child_$ind.txt
done

mv temp* $GAPATH/gen_$GEN_CNT

echo "Finished simulating Gen : $GEN_CNT"
let GEN_CNT=$GEN_CNT+1
export GEN_CNT=$GEN_CNT
# Repeat above process for each generation until $5 is reached
while [ $GEN_CNT -lt $5 ]
do
    echo "Beginning Gen : $GEN_CNT"


    #change permissions
  #  chown $USER gen* temp* child*
    
    # Navigate to GA (EDITED)
    cd $GAPATH
    mkdir ./gen_$GEN_CNT

    let LAST_CNT=$GEN_CNT-1

    # Run the GA with all chromosomes from last generation as potential parents
    ./GA -gp $1 $2 $3 $4 -p $CHILD_NO ./gen_$LAST_CNT/child* 

    # Take new children and move them to current generation folder
    mv child_* $GAPATH/gen_$GEN_CNT
	
    # Navigate up a directory to run AraSim (EDITED)
    cd $ARAPATH

    # Simulate each child in the current generation(outside for loop)
    #make -f GA_make.mk temp

    for ind in $(seq 0 $TOT)
    do
	#take the Veff line from each child, xargs takes the output of grep and replaces OutPutFromGrep with that output, then sed takes that output and replaces it with the Veff score stored in temp_number.txt the tee command acts will send the standard output of sed to the terminal and also to 

    cd ..
    qsub -v 1=$ind,2=$GEN_CNT oscRun.sh
    cd $ARAPATH
    GREPOUT=$(grep "test Veff" $GAPATH/gen_$GEN_CNT/temp_${ind}.txt)
    grep "Veff" $GAPATH/gen_$GEN_CNT/child_$ind.txt | xargs -I OutPutiFromGrep sed -e "s/^.*OutPutFromGrep.*$/$GREPOUT/" $GAPATH/gen_$GEN_CNT/child_$ind.txt | tee $GAPATH/gen_$GEN_CNT/childUpdated.txt &> out.log
        #grep "Veff" $GAPATH/gen_$GEN_CNT/child_$ind.txt | xargs -I OutPutFromGrep sed -e "s/^.*OutPutFromGrep.*$/$(cat temp_$ind.txt)/" $GAPATH/gen_$GEN_CNT/child_$ind.txt | tee $GAPATH/gen_$GEN_CNT/childUpdated.txt &> out.log

        mv $GAPATH/gen_$GEN_CNT/childUpdated.txt $GAPATH/gen_$GEN_CNT/child_$ind.txt
    done
	
    mv temp* $GAPATH/gen_$GEN_CNT
    
    let GEN_CNT=$GEN_CNT+1
    export GEN_CNT=$GEN_CNT
done

#(EDITED)
cd $GAPATH
#(EDITED) remove the previous run_x directory before making a new one
rm -r run_x

mkdir run_x
mv gen* ./run_x
mv fitnessFile.txt ./run_x

python plot_maxes.py /run_x/fitnessFile.txt

mv *Veff.png ./run_x

cd $ARAPATH

printf "Roul. Xover : $1\nRoul. Mut :$2\nTour. Xover : $3\n Tour. Mut : $4\n Gen Cnt : $5\n " > ./run_x/log.txt

echo "Done"
