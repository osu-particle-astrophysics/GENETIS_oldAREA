import numpy as np
import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-i', '--input', help='Input values', nargs='+', required=True)

args = parser.parse_args()

#bash input for temp path
gen=args.input[0]

#bash input for number of individuals
individuals=int(args.input[1])

# bash input for number of jobs per individual
jobs=int(args.input[2])

# bash input for directory containing the temp files
source=args.input[3]

new_file_content=""
pattern = "test Veff(ice) : "
pattern2 = "test"

mean_Veff_array = []

for ind in range(0, individuals):

        Veff_array = []
        for i in range(1, jobs+1):

                f = open(source + "/" + "gen_{}".format(gen) + "/temp_{}_{}.txt".format(ind, i), 'rt')
                #print()
                for line in f:
                        if pattern in line:
                                Veff = float(line.split()[5])
                f.close()
                print(Veff)
                Veff_array.append(Veff)

        print(Veff_array)

        mean_Veff = np.mean(Veff_array)
        mean_Veff_array.append(str(mean_Veff))

        print("mean Veff across all seeds: ", mean_Veff)

        #Write correct Veff from temp files to child_*.txt files
        f_childIn = open(source + "/" + "gen_{}".format(gen) + "/child_{}.txt".format(ind), 'rt')
        childData = f_childIn.readlines()
        for i,line in enumerate(childData):
                if "test Veff :" in line:
                        childData[i] = "test Veff : " + str(mean_Veff) + "\n"
                        print("writing this to child_", ind, ": ", childData[i])
        f_childIn.close()
        
        #write new child file with correct Veff
        f_childOut = open(source + "/" + "gen_{}".format(gen) + "/child_{}.txt".format(ind), 'wt')
        f_childOut.writelines(childData)
        f_childOut.close()
        

# I unindented the next 3 lines out of the for loop to fix how it displays the fitness scores

f2 = open(source + "/" + "gen_{}".format(gen) + "/fitnessFile_gen_{}".format(gen) + ".txt", 'a')
f2.write(','.join(mean_Veff_array) + "\n")
f2.close()

print("SR2: wrote mean Veff values to ", source + "/" + "gen_{}".format(gen) + "/fitnessFile_gen_{}".format(gen) + ".txt")

print(mean_Veff_array)
#Check for the Dummy Veff line
numlines=0
f1 = open(source + "/fitnessFile.txt", 'a')
f1.write(','.join(mean_Veff_array) + "\n")
f1.close()

print ("SR2: wrote mean Veff values to ", source + "/fitnessFile.txt")


 
