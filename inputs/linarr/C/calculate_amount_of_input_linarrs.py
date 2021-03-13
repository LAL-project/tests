import random
from sys import argv

################################################################################

filename = argv[1]

################################################################################

new_file = []
f = open(filename, 'r')
idx = 0
nlinarrsinput = 0
for line in f:
	idx += 1
	if idx <= 5:
		new_file.append(line)
		continue
	
	if line.find(".time") != -1:
		continue
	
	if line != "\t\n" and line != "\n":
		nlinarrsinput += 1
	
f.close()

print(nlinarrsinput)
################################################################################
