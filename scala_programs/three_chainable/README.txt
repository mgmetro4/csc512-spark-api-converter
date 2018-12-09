Three-Chainable-Functions - Test Case 6 - Working

Description: A program consisting of three different functions that can be chained together

To Run:  bash test.sh
** the helpers.scala file needs to be in the folder above (../)


RDD

sc.range(0,50)
  .map(a => a+3)
  .filter( a => (a%15)==0 )
  .sortBy( (a:Long) => a )
  .reduce( (a:Long, b:Long) => a+b)

Output: Long = 90



Dataset

spark.range(0,50)  
  .map(a => a+3)
  .filter( a => (a%15)==0 )
  .map(row=>(( (a:Long) => a )(row), row)).orderBy("_1").map(_._2)
  .select(reduceAggregator( (a:Long, b:Long) => a+b )).collect()

Output: Array[Long] = Array(90)



Dataframe

** Not compatible with Dataframe
