//Dataframe
//words.txt = echo “one two three” > words.txt

spark.read.text("words.txt")
	.selectExpr("sum(size(split(value,' ')))")
	.as[Long].first