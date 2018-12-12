//Dataframe

spark.range(1, 11).selectExpr("id as _1")
	.selectExpr("_1%11 as _1", "1 as _2")
	.collect()