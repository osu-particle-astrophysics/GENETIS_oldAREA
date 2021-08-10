import numpy as np
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
from sys import *

# Try opening file given on command-line
print(argv[1])
try:
	file = open(argv[1], "r")

# Throw error prompt and exit if cannot open file
except:
	print("Problem reading file\n")
	exit()

# Find max Veff score in each generation
max_Veff = []
linenum = 1

for line in file:
	line = line.rstrip('\n').split(",")
	array_veff=[]

	for num in range(len(line)):	
		#create plot for all Veff vs. generation count
		plt.plot(linenum,float(line[num]),'o',color = 'blue')
	#increment line number	
	linenum += 1
file.close()
	
#make labels for Plot of all Veff vs. generation count
plt.xlabel("Generation Count")
plt.ylabel("Fitness Scores (km3sr)")
#this may give an error "unable to parse pattern" this is known by redhat as of 2018, and shouldnt cause problems.
plt.title(f"Fitness Scores of {len(line)} Indivduals Per Generation, for {linenum-1} Generations")
plt.grid()
plt.savefig("allVeff.png")
