//Dataframe

spark.range(10, 24).selectExpr(“id as _1”)
	.selectExpr(“_1 as _1”, “if (_1%3==0), _1*10, _1*2) as _2”)
	.selectExpr(“_1+_2 as _1”)
	.collect()