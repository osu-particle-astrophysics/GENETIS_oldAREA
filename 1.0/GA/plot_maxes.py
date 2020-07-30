import numpy as np
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
from sys import *

def main():
	# Try opening file given on command-line
	try:
		file = open(argv[1], "r")

	# Throw error prompt and exit if cannot open file
	except:
		print("Problem reading file\n")
		exit()

	# Find max Veff score in each generation
	max_Veff = []

	for line in file:
		line = line.split(",")
		for num in range(len(line)):
			print(line[num])
			line[num] = line[num].replace('\n','')
			line[num] = (float(line[num]))

		print(line)
		max_Veff.append(max(line))
	
	# Plot max. Veff vs. generation count
	plt.plot(range(len(max_Veff)),max_Veff)
	plt.xlabel("Generation Count")
	plt.ylabel("Fitness Score")
	plt.title("Maximum Fitness Score Per Generation")
	plt.grid()
	plt.savefig("maxVeff.png")

if __name__ == '__main__':
	main()
