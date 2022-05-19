## File name: polar_plotter.py
## Author: Alex Machtay (machtay.1@osu.edu)
## Date: 07/19/21
## Purpose:
##	This script is designed to read in the results from XF and plot the gain patterns.
##	It will plot each of the antennas given (in a range) at a specific (given) frequency.
##	These will show the gain at zenih angles, since the patterns are azimuthally symmetric
#
#
#
## Instructions:
## 		To run, give the following arguments
##			source directory, destination directory, frequency number, initial antenna, final antenna 
#
## 		Example:
##			python polar_plotter.py Length_Tweak Length_Tweak 60 118 127
## 				This will plot the antennas for the length tweak from number 118-127
##				(which is all of them) at the highest frequency number (1067 MHz) and
##				will place the image inside the Length_Tweak directory.
#		
#		For AREA:
#			python polar_plotter.py ~/AREA/GA/Runs/$RUNNAME/#GEN_CNT ~/AREA/GA/Runs/$RUNNAME/$GEN_CNT FREQNUM(1-60) 0 24
## Imports
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import argparse
import csv
import matplotlib.cm as cm
#
#
#
## Arguments
parser = argparse.ArgumentParser()
parser.add_argument("source1", help="Name of source folder from home directory", type=str)
#parser.add_argument("source2", help="Name of source folder from home directory", type=str)
parser.add_argument("destination", help="Name of destination folder from home directory", type=str)
parser.add_argument("freq_num", help="Frequency number (1-60) to plot", type=int)
parser.add_argument("initial_antenna", help="First antenna number to start reading (ex: 65)", type=int)
parser.add_argument("final_antenna", help="Last antenna number to start reading (ex: 69)", type=int)
g=parser.parse_args()
#
#
#
## Loop over files
# Declare list for each file's gain list
gains = []
realized_gains = []
for i in range(g.initial_antenna, g.final_antenna+1):
	# Gains
	## Open the file to read
	with open(g.source1 + "/" + "child" + "_" + str(i) + ".txt", "r") as f:
		## Read in the file
		uan_read = csv.reader(f, delimiter = ' ')
		## Declare list to hold azimuth gains
		azimuth_gain = []
		for m, row in enumerate(uan_read):
			## skip the header
			if m >= 5 and m <= 39:
				## Let's just add the azimuthal angles
				print(row[4])
				azimuth_gain.append(float(row[4]))
	f.close()
	### Declare temporary zenith gains
	#zenith_gains = []
	### Loop over the zenith angles
	#for j in range(1, 36):
	#	## Declare temporary azimuth means
	#	azimuth_mean = []
	#	## Loop over the azimuth angles
	#	k = j*73
	#	while k < ((j+1)*73):
	#		azimuth_mean.append(azimuth_gain[k])
	#		## Remember to increment!
	#		k += 1
	#	zenith_gains.append(np.mean(azimuth_mean))
		## Remember to increment!
	## Append the list of mean azimuth gains at each zenith to the gains list
	gains.append(azimuth_gain)

	# Realized gains

	#with open(g.source2 + "/" + "/" + str(i) + "_" + str(g.freq_num) + ".uan", "r") as f:
	#	## Read in the file
	#	uan_read = csv.reader(f, delimiter = ' ')
	#	## Declare list to hold azimuth gains
	#	azimuth_gain = []
	#	for m, row in enumerate(uan_read):
	#		## skip the header
	#		if m >= 17:
	#			## Let's just add the azimuthal angles
	#			azimuth_gain.append(float(row[2]))
	#f.close()
	# Declare temporary zenith gains
	#zenith_gains = []
	## Loop over the zenith angles
	#for j in range(1, 36):
	#	## Declare temporary azimuth means
	#	azimuth_mean = []
	#	## Loop over the azimuth angles
	#	k = j*73
	#	while k < ((j+1)*73):
	#		azimuth_mean.append(azimuth_gain[k])
	#		## Remember to increment!
	#		k += 1
	#	zenith_gains.append(np.mean(azimuth_mean))
	#	## Remember to increment!
	## Append the list of mean azimuth gains at each zenith to the gains list
	#realized_gains.append(zenith_gains)


print(len(azimuth_gain))
print(len(gains))
#print(len(gains[0]))
#
#
#
## Plotting
# Make a list for the zenith angles
zenith_angles = []
flipped_zenith_angles = [] ## For making a version with realized gains on the other side
for i in range(1, 36):
	zenith_angles.append(i*5*np.pi/180)
	flipped_zenith_angles.append(-i*5*np.pi/180)
#
#
#
## Declare a figure
colors = cm.rainbow(np.linspace(0, 1, g.final_antenna - g.initial_antenna + 1))

print(len(zenith_angles))

## Let's make a figure where the lines are overlayed
fig, ax = plt.subplots(subplot_kw={'projection': 'polar'}, figsize = (10, 8))
ax.set_theta_zero_location("N")
ax.set_rlabel_position(225)
for i in range(0, g.final_antenna - g.initial_antenna + 1):
	LabelName1 = "{} Gain 1".format(i+g.initial_antenna)
	LabelName2 = "{} Gain 2".format(i+g.initial_antenna)
	ax.plot(zenith_angles, gains[i], color = colors[i], linestyle = '--', alpha = 0.4, label = LabelName1, linewidth = 2)
	#ax.plot(zenith_angles, realized_gains[i], color = colors[g.final_antenna - g.initial_antenna - i], linestyle = ':', alpha = 0.4, label = LabelName2, linewidth = 2)
angle=np.deg2rad(0)
ax.legend(loc = 'lower left', bbox_to_anchor=(0.4 + np.cos(angle)/2, 0.25 + np.sin(angle)/2))
plt.title("Antennas at Frequency number {} MHz".format(round(83.33 + 16.67*(g.freq_num-1), 6)), fontsize = 16)
plt.savefig(g.destination + "polar_plot_" + str(round(83.33 + 16.67 * (g.freq_num-1), 3)) + ".png")

'''
## Let's make a figure where the realized gains are on the other side
fig, ax = plt.subplots(subplot_kw={'projection': 'polar'}, figsize = (10, 8))
ax.set_theta_zero_location("N")
ax.set_rlabel_position(225)
for i in range(0, g.final_antenna - g.initial_antenna + 1):
	LabelName1 = "{} Gain 1".format(i+g.initial_antenna)
	LabelName2 = "{} Gain 2".format(i+g.initial_antenna)
	ax.plot(zenith_angles, gains[i], color = colors[i], linestyle = '--', alpha = 0.4, label = LabelName1, linewidth = 2)
	ax.plot(flipped_zenith_angles, realized_gains[i], color = colors[g.final_antenna - g.initial_antenna - i], linestyle = ':', alpha = 0.4, linewidth = 2)
angle=np.deg2rad(45)
ax.legend(loc = 'upper right', bbox_to_anchor=(1.25 , 0.4 + np.sin(angle)/2))
plt.title("Antennas at Frequency number {} MHz".format(round(83.33 + 16.67*(g.freq_num-1), 6)), fontsize = 16)
plt.savefig(g.destination + "polar_plot_" + str(round(83.33 + 16.67 * (g.freq_num-1), 6)) + "_flipped.png")
'''


