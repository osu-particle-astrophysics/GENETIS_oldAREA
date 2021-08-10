import matplotlib.pyplot as plt
import numpy as np
from array import *
import os

array2D = [] # Create empty array to be populated later

fitnesspath = '/users/PAS0654/fahimi/AREA/GA/Runs/20210318fahimi5run1/fitnessFile.txt' #file path to fitness file, make sure to change run name when running

# read file and write to array2D
with open(fitnesspath, "r") as f1:
	for line in f1:
		newline=line.rstrip('\n')
		newline=newline.upper()
		array2D.append(newline.split(","))

#create  empty array to be populated later
x = []
y = []
xaverage = []
yaverage = []
#average = 0

# seperate array2D into two seperate arrays
# x being generation number
# y being fitness scores
#prevx = 0
sumpoints = 0
for i in range(len(array2D)):
	print("i is ")
	print(i)
	print("\n")
	#currentx = i
	for j in range(len(array2D[i])):
		x.append(i)
 		y.append(float(array2D[i][j]))
		sumpoints += float(array2D[i][j])
		if j == (len(array2D[i])-1):
			xaverage.append(i)
			averagepoints = sumpoints/len(array2D[i])
			print(averagepoints)
			yaverage.append(averagepoints)
			sumpoints = 0
		

#plot fitness scores vs generation count
print(x)
print(y)
fig, (ax1, ax2) = plt.subplots(2)
ax1.scatter(x,y,s=150,alpha=0.5)
ax2.scatter(xaverage,yaverage,s=150)
ax1.set_title('Scatter Plot of Fitness Scores Across Each Generation')
ax1.set_ylabel('Fitness Scores')
ax2.set_xlabel('Generation Count')
ax2.set_title('Average Fitness Scores Across Each Generation')
ax1.set_ylabel('Fitness Scores')
ax2.set_xlabel('Generation Count')
plt.show()
