//RDD

sc.range(10, 24)
	.map( i => {val j=i%3; (I, if (j==0) i*10 else i*2) })
	.map(r => r._1+r._2)
	.collect()