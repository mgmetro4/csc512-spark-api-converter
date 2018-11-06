/* reduceByKey Example Dataset */
spark.range(1,10000000)
	.map(i=>(i%11, 1))
	.groupByKey(_._1)
	.agg(reduceByKeyAggregator((a:Int, b:Int) => a+b))
	.collect()