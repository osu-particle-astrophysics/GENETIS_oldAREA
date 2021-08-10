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
avg_Veff = []

for line in file:
	line = line.rstrip('\n').split(",")
	array_veff=[]

	for num in range(len(line)):	
		#array_veff.append(float(line[num]))
		array_veff.append(float(line[num]))

	#print(array_veff)
	avg_Veff.append(np.mean(array_veff))

file.close()
	
# Plot max. Veff vs. generation count
plt.plot(range(len(avg_Veff)),avg_Veff)
plt.xlabel("Generation Count")
plt.ylabel("Fitness Score")
#this may give an error "unable to parse pattern" this is known by redhat as of 2018, and shouldnt cause problems.
plt.title("Average Fitness Score Per Generation")
plt.grid()
plt.savefig("avgVeff.png")
