//Dataset

spark.range(0,50)  
  .map(a => a*10)
  .filter( a => (a%15)==0 )
  .map(row=>(( (a:Long) => a )(row), row)).orderBy("_1").map(_._2)
  .select(reduceAggregator( (a:Long, b:Long) => a-b )).collect()
