import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-i', '--input', help='Input values', nargs='+', required=True)

args = parser.parse_args()

#bash input for child path
##path1=args.input[0]

#bash input for temp path
temppath=args.input[0] 

print(temppath)

new_file_content=""
pattern = "weight"
##pattern2 = "test"
#with open(temppath,'r') as f:
#	with open(childpath, 'r+') as f1:
#		data = f1.read()
#		for line in f:
#			if pattern in line:
#				Veff = line
#				print Veff
#		for line in f1:
#			if pattern2 in line:
#				dummyVeff = line
#				print line

Weight = "If you see this text there is a problem! >:D"

f = open(temppath, 'rt')
for line in f:
	if pattern in line:
		Weight = line
		WeightOnly = Weight[-12:]
		ExpoW = WeightOnly[-4:]
		NumberW = WeightOnly.rstrip(ExpoW)
f.close()
print(NumberW)
print(ExpoW)

#Check for the Dummy Veff line
#numlines=0
#f1 = open(childpath, 'rt')
#for line in f1:
#	numlines += 1
#	if pattern2 in line:
#		print("hi")
#		dummyVeff = line
#f1.close()
#print(numlines)
#
##Read in the child file to a variable and replace the dummyVeff with Veff
#f2 = open(childpath, 'rt')
#data = f2.read()
#data = data.replace(dummyVeff,Veff)
#f2.close()
#
#f3 = open(childpath, 'wt')
#f3.write(data)
#f3.close()

 
