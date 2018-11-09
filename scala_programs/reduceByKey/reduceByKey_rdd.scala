// reduceByKey Example RDD
sc.range(1,10000000)
	.map(i=>(i%11, 1))
	.reduceByKey((a:Int, b:Int) => a+b)
	.collect()
