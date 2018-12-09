// just a simple file to test reduceByKey

sc.range(1,2000)
  .map(UDF)
  .reduceByKey(UDF)
