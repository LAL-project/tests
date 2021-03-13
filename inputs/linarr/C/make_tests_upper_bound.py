import random
from sys import argv

################################################################################

filename = argv[1]
U = int(argv[2])
N = int(argv[3])

################################################################################

new_file = []
f = open(filename, 'r')
idx = 0
for line in f:
	idx += 1
	if idx <= 5:
		new_file.append(line)
		continue
	
	if line.find(".time") == -1:
		for j in range(0,N):
			k = random.randint(0, U)
			new_file.append(line[:-1] + "\t" + str(k) + "\n")
		new_file.append("\n")
	else:
		new_file.append(line)
	
f.close()

################################################################################

f = open(filename, 'w')
for l in new_file: f.write(l)
f.close()
