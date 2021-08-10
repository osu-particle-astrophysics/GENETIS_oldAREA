import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-i', '--input', help='Input values', nargs='+', required=True)

args = parser.parse_args()

#bash input for child path
path1=args.input[0]

#bash input for temp path
path2=args.input[1]

#combine the run path and input strings
childpath = path1
temppath = path2 

print childpath
print temppath

new_file_content=""
pattern = "Veff(ice)"
pattern2 = "test"
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

#new addition 2/18/21
Veff = "Hello World!"

f = open(temppath, 'rt')
for line in f:
	if pattern in line:
		Veff = line
f.close()
print(Veff)

#Check for the Dummy Veff line
numlines=0
f1 = open(childpath, 'rt')
for line in f1:
	numlines += 1
	if pattern2 in line:
		print("hi")
		dummyVeff = line
f1.close()
print(numlines)

#Read in the child file to a variable and replace the dummyVeff with Veff
f2 = open(childpath, 'rt')
data = f2.read()
data = data.replace(dummyVeff,Veff)
f2.close()

f3 = open(childpath, 'wt')
f3.write(data)
f3.close()

 
