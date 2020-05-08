make.qqplot <- function(tree_dist, N) {
	main.title = paste0("n=", N)
	
	qqnorm(tree_dist, main = main.title)
	qqline(tree_dist, col=2)
	
	#n = length(tree_dist)
	#cum_tree_dist = mapply(function(j) { sum(tree_dist[1:j]) }, seq(1,n)  )
	#plot(cum_tree_dist, type="o", col="red", ylab="cumul. prob.", xlab="tree", main=main.title)
}

plot.cat.prop <- function(the.prop, emp.prop) {
	num = length(emp.prop)
	n = 4:(4+num-1)
	print(n)
	
	D = abs(emp.prop-the.prop)
	plot(n, D, col = "blue", xlab = "vertices", ylab = "measured_proportion - theoretical_proportion")
	lines(n, D, col = "blue")
	abline(h = 1)
	
	plot(n, emp.prop, col = "red", xlab = "vertices", ylab = "%")
	lines(n, emp.prop, col = "red")
	lines(n, the.prop, col = "blue")
	points(n, the.prop, col = "blue")
}

get.nums <- function(data) {
  as.numeric(strsplit(data, " ")[[1]])
}

# Meaning of variables
#	D: table of two columns 'N' and 'probs'.
#		* 'N' is the number of vertices of trees.
#		* 'probs' is a vector of probabilities where the i-th value is
#		the probability that tree 'i' is generated using our method.
#		Also, the proportion of the number of times the i-th tree was
#		generated over all trees generated.
#
#	the.catN: the proportion of caterpillar trees over all trees of 'N' vertices.
#	emp.catN: the proportion of caterpillar trees that our method generates.

D = read.table('data.csv', header = TRUE, sep = '\t', stringsAsFactors=FALSE)

d4 = get.nums(D$probs[4])
the.cat4 = 1
emp.cat4 = 1

d5 = get.nums(D$probs[5])
the.cat5 = 1
emp.cat5 = 1

d6 = get.nums(D$probs[6])
the.cat6 = 1
emp.cat6 = 1

d7 = get.nums(D$probs[7])
the.cat7 = 0.90909091
emp.cat7 = 0.90874900

d8 = get.nums(D$probs[8])
the.cat8 = 0.86956522
emp.cat8 = 0.86923300

d9 = get.nums(D$probs[9])
the.cat9 = 0.76595745
emp.cat9 = 0.76579700

d10 = get.nums(D$probs[10])
the.cat10 = 0.67924528
emp.cat10 = 0.67994800

d11 = get.nums(D$probs[11])
the.cat11 = 0.57872340
emp.cat11 = 0.57806100

d12 = get.nums(D$probs[12])
the.cat12 = 0.49364791
emp.cat12 = 0.49428800

d13 = get.nums(D$probs[13])
the.cat13 = 0.40584166
emp.cat13 = 0.40574400

d14 = get.nums(D$probs[14])
the.cat14 = 0.33428300
emp.cat14 = 0.33499400

d15 = get.nums(D$probs[15])
the.cat15 = 0.26869913
emp.cat15 = 0.26805500

make.qqplot(d6, 6)
make.qqplot(d7, 7)
make.qqplot(d8, 8)
make.qqplot(d9, 9)
make.qqplot(d10, 10)
make.qqplot(d11, 11)
make.qqplot(d12, 12)
make.qqplot(d13, 13)
make.qqplot(d14, 14)
make.qqplot(d15, 15)

the.cat = c(the.cat4, the.cat5, the.cat6, the.cat7, the.cat8, the.cat9, the.cat10, the.cat11, the.cat12, the.cat13, the.cat14, the.cat15)
emp.cat = c(emp.cat4, emp.cat5, emp.cat6, emp.cat7, emp.cat8, emp.cat9, emp.cat10, emp.cat11, emp.cat12, emp.cat13, emp.cat14, emp.cat15)
plot.cat.prop(the.cat, emp.cat)
