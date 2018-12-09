// just a simple file to test reduceByKey

sc.range(1,2000)
  .map(i => i * 2)
  .reduceByKey((a:Int, b:Int) => a+b)
