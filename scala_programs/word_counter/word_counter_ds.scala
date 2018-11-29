//Dataset
//words.txt = echo “one two three” > words.txt

spark.read.textFile("words.txt")
	.map(l => l.split(" ").size)
	.select(reduceAggregator( (a:Int, b:Int) => a+b )).collect() 