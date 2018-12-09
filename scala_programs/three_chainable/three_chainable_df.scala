//Dataframe

spark.range(0,50).selectExpr("id as _1")
  .selectExpr("_1+3 as _1")
  .where("_1%15==0")
  .orderBy("_1")
  .selectExpr("sum(_1)")
  .as[Long].first
