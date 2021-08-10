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
		for line in f:
			if pattern in line:
				Veff = float(line.split()[5])
		f.close()
		print(Veff)
		Veff_array.append(Veff)

	print(Veff_array)

	mean_Veff = np.mean(Veff_array)
	mean_Veff_array.append(str(mean_Veff))

print(mean_Veff_array)
#Check for the Dummy Veff line
numlines=0
f1 = open(source + "/fitnessFile.txt", 'a')
f1.write(','.join(mean_Veff_array) + "\n")
f1.close()


 
