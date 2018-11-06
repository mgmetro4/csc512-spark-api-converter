/* Dataframe
 * Filter out and collect all even numbers in the range */

spark.range(0, 101).selectExpr("id as _1")
    .selectExpr("if(_1%2==0) _i) as _i" ) //cannot get this to work. filter i%2==0
    .collect()