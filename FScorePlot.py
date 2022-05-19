import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import argparse
import csv
import matplotlib.cm as cm

## Run as folloows:
## python FScoreplot.py <source> <destination> <NPOP> <numGens>
## Ex:
## python FScoreplot.py . . 50 31

## For AREA, do this
## python FScoreplot.py ~/AREA/GA/Runs/20211104fahimi5run2 ~/AREA/GA/Runs/20211104fahimi5run2 50 31

 
#---------GLOBAL VARIABLES----------GLOBAL VARIABLES----------GLOBAL VARIABLES----------GLOBAL VARIABLES

# We need to grab the three arguments from the bash script or user. These arguments in order are [the name of the source folder of the fitness scores], [the name of the destination folder for the plots], and [the number of generations]
parser = argparse.ArgumentParser()
parser.add_argument("source", help="Name of source folder from home directory", type=str)
parser.add_argument("destination", help="Name of destination folder from home directory (no end dash)", type=str)
parser.add_argument("NPOP", help="Number of individuals per generation", type=int)
parser.add_argument("numGens", help="Number of generations the code is running for (no end dash)", type=int)
g = parser.parse_args()

# The name of the plot that will be put into the destination folder, g.destination
Plot2DName = "/FScorePlot2D.png"
#Plot3DName = "/FScorePlot3D.png"

#----------STARTS HERE----------STARTS HERE----------STARTS HERE----------STARTS HERE
fileReadTemp = []
fScoresGen = []
fScoresInd = []
"""
#We may want to only have g.numGens and not g.numGens+1
for gen in range(g.numGens+1):
    filename = "/{}_fitnessScores.csv".format(gen)
    #fileReadTemp = np.genfromtxt(g.source + filename, delimiter=',')
    #The above comment was here orignally but the delimiter is no longer a comma so the next line should work with new line
    #fileReadTemp = (line.rstrip('\n') for line in open(g.source + filename))
    #fScoresGen.append(fileReadTemp[2:])
    with open(g.source + filename, "r") as f:
        fileReadTemp = [line.strip() for line in f]
    fScoresGen.append(fileReadTemp[2:])
fScoresInd = np.transpose(fScoresGen)
NPOP = len(fScoresInd)
"""
file = open(g.source + "/" + "fitnessFile.txt", "r")

tempFitnesses = []
FitnessesArray = []
FlipFitnessesArray = []

## Pasted from plot_all.py    
#linenum = 1

for line in file:
    line = line.rstrip('\n').split(",")

    for num in range(len(line)):
        #create plot for all Veff vs. generation count
        fitness = float(line[num])
        tempFitnesses.append(fitness)
    FlipFitnessesArray.append(tempFitnesses)
    #print(FitnessesArray)
    tempFitnesses = []
    #plt.plot(linenum,float(line[num]),'o',color = 'blue')
    #increment line number  
    #linenum += 1
file.close()
##

#print(FlipFitnessesArray[0])
#print(len(FlipFitnessesArray[0]))

#switch array
for k in range(len(FlipFitnessesArray[0])):
    for j in range(len(FlipFitnessesArray)):
        fitness = FlipFitnessesArray[j][k]
        tempFitnesses.append(fitness)
        #j += 1
    FitnessesArray.append(tempFitnesses)
    tempFitnesses = []
    #k += 1
#print(FitnessesArray)
#print(len(FitnessesArray))
#print(len(FitnessesArray[0]))

genAxis = np.linspace(0,g.numGens-1,g.numGens,endpoint=True)

Veff_ARA = []
Err_plus_ARA = []
Err_minus_ARA = []
Veff_ARA_Ref = []

filenameActual = "/AraOut_ActualBicone.txt"
fpActual = open("/users/PAS0654/machtay1" + filenameActual)

for line in fpActual:
    if "test Veff(ice) : " in line:
        Veff_ARA = float(line.split()[5]) #changed from 3 to 5 for switching to km^3 from m^3
        #print(Veff_ARA)
    elif "And Veff(water eq.) error plus :" in line:
        Err_plus_ARA = float(line.split()[6])
        Err_minus_ARA = float(line.split()[11])
#    line = fpActual.readline()
    #print(line)
fpActual.close()

## Adding line of average fitness score
MeanFitness = []
FlippedFitness = np.transpose(FitnessesArray)
#print(FlippedFitness)
for gen in range(g.numGens):	
	mean = sum(FlippedFitness[gen])/g.NPOP
	MeanFitness.append(mean)	
print(np.shape(FlippedFitness))
## Adding line of median fitness score
MedianFitness = []
for gen in range(g.numGens):
	median = np.median(FlippedFitness[gen])
	MedianFitness.append(median)

#Veff_ARA_Ref = Veff_ARA * np.ones(len(genAxis))

plt.figure(figsize=(10, 8))
#plt.plot(genAxis, Veff_ARA_Ref, label = "ARA Reference", linestyle= '--', color = 'k')
plt.axhline(y=Veff_ARA, linestyle = '--', color = 'k', label = "ARA Bicone")
#plt.axes([-1, g.numGens+1, -1, 6])
#plt.xlabel('Generation', size = 22)
#plt.ylabel('Fitness Score', size = 22)
#plt.title("Fitness Score over Generations (0 - {})".format(int(g.numGens)), size = 25)
colors = cm.rainbow(np.linspace(0, 1, g.NPOP))
#plt.axis([-1, g.numGens+1, -0.5, np.max(FitnessesArray) + np.max(Err_plus_ARA) + 0.5])
plt.axis([-1, g.numGens+1, np.min(FitnessesArray) - np.max(Err_plus_ARA), np.max(FitnessesArray) + np.max(Err_plus_ARA)])


#for ind in range(g.numGens):
		#LabelName = "Individual {}".format(ind+1)
		#plt.plot(genAxis[ind], FitnessesArray[ind], marker = 'o', color = colors[ind], linestyle='', alpha = 0.4, markersize = 11)
		#plt.plot(genAxis, FitnessesArray[ind], label = LabelName, marker = 'x', color = 'purple', linestyle='', alpha = 0.8, markersize = 9)
		#plt.plot(genAxis, MeanFitness, label = "Mean", linestyle='-', alpha = 1, linewidth = 2, color = 'r')
		#plt.plot(genAxis, MedianFitness, label = "Median", linestyle = '--', alpha = 1, linewidth = 2, color = 'b')
#plt.plot(genAxis, MeanFitness, label = "Mean", linestyle='-', alpha = 1, linewidth = 2, color = 'r')
#plt.plot(genAxis, MedianFitness, label = "Median", linestyle = '--', alpha = 1, linewidth = 2, color = 'b')
#plt.xlabel('Generation', size = 26)
#plt.ylabel('Fitness Score (km$^3$str)', size = 26)
#plt.title("Fitness Score over Generations (0 - {})".format(int(g.numGens)), size = 30)

#plt.legend(loc = 'lower right', fontsize = 'x-large')
#plt.savefig(g.destination + Plot2DName)

#for x in range (len(fScoresInd[1])):
    #print fScoresInd[1][x]

#plt.show()
# was commented out to prevent graph from popping up and block=False replaced it along with plt.pause
# the pause functions for how many seconds to wait until it closes graph
#plt.show(block=False)
#plt.pause(1)


## Fuck it, boxplot time
fig = plt.figure(figsize = (12, 8))
ax = fig.add_subplot(111)

ax.axis([-1, g.numGens+1, np.min(FitnessesArray) - 3*np.max(Err_plus_ARA), np.max(FitnessesArray) + np.max(Err_plus_ARA)])

print(len(FlippedFitness),genAxis)

ax.violinplot(list(FlippedFitness), genAxis, showmeans = False, showextrema = False, showmedians = False)
ax.plot(genAxis, MeanFitness, linestyle = '-', color = 'r', label = "Mean", linewidth = 2)
ax.plot(genAxis, MedianFitness, linestyle = '--', color = 'g', label = "Median", linewidth = 2)
ax.axhline(y=Veff_ARA, linestyle = '--', color = 'k', label = "ARA Bicone", linewidth = 2)
for i in range(0, g.numGens):
	ax.vlines(i, ymin = np.min(FlippedFitness[i]), ymax = np.max(FlippedFitness[i]))

#quartile1, medians, quartile3 = np.percentile(FitnessesArray, [25, 50, 75], axis=1)
#inds = np.arange(1, len(medians)+1)
#ax.vlines(inds, whiskersmin, whiskersmax, color = 'k', linestyle = '--', lw = 1)

ax.legend(loc = 'lower right', prop={'size': 26}, framealpha=0.5)
ax.set_xticks(np.arange(0, g.numGens + 1, 5.0))
plt.xticks(fontsize = 15)
plt.yticks(fontsize = 15)
plt.xlabel('Generation', size = 26)
plt.ylabel('Fitness Score (V$_e$$_f$$_f$ (km$^3$str))', size = 26)#'Fitness Score (km$^3$str)', size = 26)
plt.title("Fitness Score over Generations (0 - {})".format(int(g.numGens)), size = 30)
	
plt.savefig(g.destination + "/Violin_Plot.png")


# I just want to print the indices of the highest individuals in each generation

max_scores = []
individual_index = []
sorted_array = []
for i in range(g.numGens):
	sorted_array = list(FlippedFitness[i])
	sorted_array.sort()
	#print(sorted_array)
	print("Generation {} highest score: ".format(i) +  str(sorted_array[-1]))
	max_scores.append(sorted_array[-1])
	individual_index.append(list(FlippedFitness[i]).index(np.max(list(FlippedFitness[i])))+1)
print(max_scores)
print(individual_index)
#print(sorted_array)
#print(FlippedFitness[-1])
#print(FlippedFitness)
