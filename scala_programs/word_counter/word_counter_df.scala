spark.read.text("words.txt")
  .selectExpr("sum(size(split(value,' ')))")
  .as[Long].first
