//RDD

sc.range(1, 11)
	.map(i=>(i%11, 1))
	.collect()