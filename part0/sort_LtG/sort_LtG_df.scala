//Dataframe

spark.range(0,10).selectExpr("id as _1")
    .selectExpr("_1 as _1", "if(_1==_1,SIN(_1),SIN(_1)) as _2")
    .orderBy("_2")
    .collect()