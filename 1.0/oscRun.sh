#PBS -A PAS0654
#PBS -l walltime=2:15:00
#PBS -l nodes=1:ppn=8
#PBS -o /users/PAS0654/bensipe01/GA1/1.0/termoutput/
#PBS -e /users/PAS0654/bensipe01/GA1/1.0/termoutput/




GAPATH='/users/PAS0654/bensipe01/GA1/1.0/GA'
ARAPATH='/users/PAS0654/bensipe01/GA1/1.0/AraSim'

cd $ARAPATH

echo "Running AraSim for generation $GENER and child $KIDS"
#**
./AraSim setup.txt 1 ./outputs $GAPATH/$RUNPATH/gen_$GENER/child_${KIDS}.txt | tee $GAPATH/$RUNPATH/gen_$GENER/temp_${KIDS}.txt

#**
cd $GAPATH/$RUNPATH/Flag_Files
echo "Creating Flag File for child $KIDS"
touch ${KIDS}.txt
