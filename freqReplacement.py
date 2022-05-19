import numpy as np
import os
import argparse
# to run, input
# python freqReplacement.py -i Gtot Ntot S
# Gtot = total number of generations
# Ntot = total number of individuals per generation
# S = source 
parser = argparse.ArgumentParser()
parser.add_argument('-i', '--input', help='Input values', nargs='+', required=True)

args = parser.parse_args()

#bash input for current generation
gen=args.input[0]

#bash input for number of individuals
individuals=int(args.input[1])

# bash input for directory containing the child files
source=args.input[2]

# Define important variables
startpattern = "freq : 30"
endpattern = "freq : 31"
Flag = 0
FreqGainPattern = []
# Find and copy gain pattern at desired freq
for ind in range(0, individuals):
	f = open(source + "/" + "gen_{}".format(gen) + "/child_{}.txt".format(ind), 'rt')
	for line in f:
		if startpattern in line:
			Flag = 1
		elif endpattern in line:
                        Flag = 0
		elif Flag == 1:
			FreqGainPattern.append(line)
	f.close()
f.close()
print(FreqGainPattern)
# Replace gain patterns with copied pattern at all freqs
#Read in the child file to a variable and replace the dummyVeff with Veff
#f2 = open(childpath, 'rt')
#data = f2.read()
#data = data.replace(dummyVeff,Veff)
#f2.close()
#
#f3 = open(childpath, 'wt')
#f3.write(data)
#f3.close()
 
