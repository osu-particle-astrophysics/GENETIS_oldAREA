#PBS -A PAS0654
#PBS -l walltime=4:15:00
#PBS -l nodes=1:ppn=8
#PBS -o /users/PAS0654/bensipe01/GA1/1.0/termoutput/
#PBS -e /users/PAS0654/bensipe01/GA1/1.0/termoutput/



# This script runs and controls the GA and looping

# Input arguments for this controller script are:
# $1 = Roulette cross-over
# $2 = Roulette mutation
# $3 = Tournament cross-over
# $4 = Tournament mutation
# $5 = number of generations to run

# There will be $1 + $2 + $3 + $4 chromosomes per generation

# To run this job, do the following (giving 1 to each algorithm and for 1 generation):
## qsub -v A=1,B=1,C=1,D=1,E=1 GA_controller_job.sh

#permissions variable
#USER=$whoami

#**
# make a variable called RunName
RunName='Runs/87349Bensipe01'

#Path Variables
GAPATH='/users/PAS0654/bensipe01/GA1/1.0/GA'
ARAPATH='/users/PAS0654/bensipe01/GA1/1.0/AraSim'


# Read input GA parameter arguments
let CHILD_NO=$(($A+$B+$C+$D))						# number of individuals are in each generation
let TOT=$CHILD_NO-1									# probably for counting from 0?
GEN_CNT=0														# start the generation number from 0

export TOT=$TOT											# exporting this makes it a variable any bash script can see
export GEN_CNT=$GEN_CNT

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

# Make a new folder for generation 0
mkdir ./gen_$GEN_CNT

#**
#navigate to GA directory
cd $GAPATH

# Run the GA withGA parameters passed to this script
# $1 = Roulette cross-over
# $2 = Roulette mutation
# $3 = Tournament cross-over
# $4 = Tournament mutation
# The GA only runs one generation and the makes children but to run the next generation the loop below is needed, the far right number which is 0 in this case is the current generation and the last input when running this script is just how many times the GA program is run
./GA -gp $A $B $C $D -p 0						# -p represents the number of input parents (0 in gen 0)
																		# -gp represents the number of children for each algorithm
																			# hence, we have four numbers following -gp

#**
# Move all the children to current generation folder
mv child_* $GAPATH/$RunName/gen_$GEN_CNT

#**
# Move back to RunName Directory
cd ./$RunName

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

mkdir -m775 Flag_Files
flag_files=0

#**
cd $GAPATH
cd ..
for i in $(seq 0 $TOT)
do
    qsub -v KIDS=$i,GENER=$GEN_CNT,RUNPATH=$RunName oscRun.sh
done

# this is where we want to put in a wait function
# don't continue until number of files in flag_directory = CHILD_NO

#**
cd $GAPATH/$RunName/Flag_Files

while [[ $flag_files -lt $CHILD_NO ]]
do
	echo "Waiting for the first batch of jobs to complete"
	sleep 60
	flag_files=$(ls -l --file-type | grep -v '/$' | wc -l)
done

echo "After the while loop!"


# At the end of the looping
cd .. 
rm -r Flag_Files

cd $ARAPATH

for ind in $(seq 0 $TOT)
do
    # Replace the dummy Veff score in the child with the correct score found by AraSim 
    #**
    GREPOUT="$(grep "Veff(ice)" $GAPATH/$RunName/gen_$GEN_CNT/temp_${ind}.txt)"
    #**
    #sed -i "160149s/.*/$GREPOUT/" $GAPATH/$RunName/gen_$GEN_CNT/child_${ind}.txt
    sed -i "s/test Veff : 0/$GREPOUT/" $GAPATH/$RunName/gen_$GEN_CNT/child_${ind}.txt
    #change permissions
    #chown $USER gen* temp* child*
    
    # Rename this updated child to the current child name
    #mv $GAPATH/gen_$GEN_CNT/childUpdated.txt $GAPATH/gen_$GEN_CNT/child_$ind.txt
done

#**
#mv temp* $GAPATH/$RunName/gen_$GEN_CNT

echo "Finished simulating Gen : $GEN_CNT"
let GEN_CNT=$GEN_CNT+1
export GEN_CNT=$GEN_CNT
# Repeat above process for each generation until $5 is reached
while [ $GEN_CNT -lt $E ]
do
    echo "Beginning Gen : $GEN_CNT"


    #change permissions
    #chown $USER gen* temp* child*
    
    # Navigate to GA (EDITED)
    #**
    cd $GAPATH/$RunName
    mkdir ./gen_$GEN_CNT

    let LAST_CNT=$GEN_CNT-1

    #**
    #Navigate to GA
    cd $GAPATH
    
    #**
    # Run the GA with all chromosomes from last generation as potential parents
    ./GA -gp $A $B $C $D -p $CHILD_NO ./$RunName/gen_$LAST_CNT/child* 

    #**
    # Take new children and move them to current generation folder
    mv child_* $GAPATH/$RunName/gen_$GEN_CNT
	
    # Navigate up a directory to run AraSim (EDITED)
    cd $ARAPATH

    # Simulate each child in the current generation(outside for loop)
    #make -f GA_make.mk temp
    #mkdir -m775 Flag_Files

    #-----------------------------------------------------------------------------
    flag_files=0

    cd ..
    for i in $(seq 0 $TOT)
    do
        qsub -v KIDS=$i,GENER=$GEN_CNT,RUNPATH=$RunName oscRun.sh
    done

    # this is where we want to put in a wait function
    # don't continue until number of files in flag_directory = CHILD_NO
    #**
    mkdir $GAPATH/$RunName/Flag_Files
    cd $GAPATH/$RunName/Flag_Files

    while [[ $flag_files -lt $CHILD_NO ]]
    do
        echo "Waiting for the first batch of jobs to complete"
        sleep 60
        flag_files=$(ls -l --file-type | grep -v '/$' | wc -l)
    done

    echo "After the while loop!"


    # At the end of the looping 
    cd ..
    rm -r Flag_Files
    #---------------------------------------------------------------------------------^

    for ind in $(seq 0 $TOT)
    do
	#take the Veff line from each child, xargs takes the output of grep and replaces OutPutFromGrep with that output, then sed takes that output and replaces it with the Veff score stored in temp_number.txt the tee command acts will send the standard output of sed to the terminal and also to 
    #**
    GREPOUT="$(grep "Veff(ice)" $GAPATH/$RunName/gen_$GEN_CNT/temp_${ind}.txt)"
    #**
    #sed -i "160149s/.*/$GREPOUT/" $GAPATH/$RunName/gen_$GEN_CNT/child_${ind}.txt
    sed -i "s/test Veff : 0/$GREPOUT/" $GAPATH/$RunName/gen_$GEN_CNT/child_${ind}.txt
 
   #igrep "Veff" $GAPATH/gen_$GEN_CNT/child_$ind.txt | xargs -I OutPutFromGrep sed -e "s/^.*OutPutFromGrep.*$/$(cat temp_$ind.txt)/" $GAPATH/gen_$GEN_CNT/child_$ind.txt | tee $GAPATH/gen_$GEN_CNT/childUpdated.txt &> out.log

        #mv $GAPATH/gen_$GEN_CNT/childUpdated.txt $GAPATH/gen_$GEN_CNT/child_$ind.txt
    done
    #**
    #mv temp* $GAPATH/$RunName/gen_$GEN_CNT
    
    let GEN_CNT=$GEN_CNT+1
    export GEN_CNT=$GEN_CNT
done

#**
#(EDITED)
cd $GAPATH/$RunName
#(EDITED) remove the previous run_x directory before making a new one


#**
#Move up to GAPATH
cd $GAPATH

#**
mv gen* ./$RunName
#**
#This loop adds the last generation's fitness scores to the end of the fitness file
#for ind in $(seq 0 $TOT)
#do
#	if [ $ind == 0]
#	then
#		sed -n 160149p $GAPATH/$RunName/gen_$GEN_CNT/child_${ind}.txt | awk '{print substr($0,18,7)}' >> $GAPATH/fitnessFile.txt
#	else
#		VEFFVAL="$(sed -n 160149p $GAPATH/$RunName/gen_$GEN_CNT/child_${ind}.txt | awk '{print substr($0,18,7)}')"
#		sed $s/$/,$VEFFVAL/ $GAPATH/fitnessFile.txt
#
#	fi
#done
FINAL_GEN=$(($E - 1))
echo "The last generation is $FINAL_GEN"
declare -a fitnessArray
for ind in $(seq 0 $TOT)
do
	VEFFVAL="$(sed -n 160149p $GAPATH/$RunName/gen_$FINAL_GEN/child_${ind}.txt | awk '{print $4}')"
	fitnessArray[ind]=$VEFFVAL
done
printf -v joined '%s,' "${fitnessArray[@]}"
echo "${joined%,}" >> fitnessFile.txt


mv fitnessFile.txt ./$RunName

#**
python plot_maxes.py /$RunName/fitnessFile.txt

#**
mv *Veff.png ./$RunName

cd $ARAPATH

#**
printf "Roul. Xover : $A\nRoul. Mut :$B\nTour. Xover : $C\n Tour. Mut : $D\n Gen Cnt : $E\n " > ./$RunName/log.txt

echo "Done"_controller_job.sh.e1783513
