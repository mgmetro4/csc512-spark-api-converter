//RDD

sc.range(0, 10)
	.map(i=>{(if(i%2==0)i else 0, i)})
	.collect()
