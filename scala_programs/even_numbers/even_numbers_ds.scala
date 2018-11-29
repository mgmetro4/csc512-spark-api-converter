//Dataset
 
spark.range(0, 101)
	.filter( i => (i%2 == 0) )
	.collect()