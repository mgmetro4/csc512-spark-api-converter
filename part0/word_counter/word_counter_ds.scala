//Dataset
spark.read.textFile("words.txt")
	.map(l => l.split(" ").size)
	.select(reduceAggregator( (a:Int, b:Int) => a+b )).collect() 