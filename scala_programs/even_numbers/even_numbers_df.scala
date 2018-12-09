//Dataframe

spark.range(0, 51).selectExpr("id as _1")
	.where("_1%2==0")
	.collect()