// RDD

sc.range(0, 51)
	.filter( i => i%2 == 0 )
	.collect()
