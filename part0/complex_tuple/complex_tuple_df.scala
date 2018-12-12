//Dataframe

spark.range(0, 10).selectExpr("id as_1")
	.selectExpr("if(_1%2==0, _1, 0)", "_1 as _2")
	.collect()