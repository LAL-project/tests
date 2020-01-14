make.qqplot <- function(tree_dist) {
	qqnorm(tree_dist)
	qqline(tree_dist, col=2)
}


dist_jd = c(49838, 50043, 49903, 49979, 50392, 50272, 49952, 49832, 50225, 49860, 49842, 49893, 49756, 49882, 50237, 50196, 49942, 50112, 49966, 49878)

dist_dj = c(50044, 50028, 49905, 49573, 49947, 50305, 50134, 49980, 50054, 49976, 50141, 49481, 49901, 50386, 50269, 49636, 50044, 49986, 49931, 50279)

make.qqplot(dist_jd/10^6)
make.qqplot(dist_dj/10^6)

n = length(dist_jd)
cum_dist_jd = mapply(function(j) { sum(dist_jd[1:j]/10^6) }, seq(1,n)  )
cum_dist_dj = mapply(function(j) { sum(dist_dj[1:j]/10^6) }, seq(1,n)  )

plot(  cum_dist_jd, type="o", col="red", ylab="prob", xlab="tree", ptype=1)
points(cum_dist_dj, lty = 2, type="o", col="blue")
