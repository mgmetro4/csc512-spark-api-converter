//RDD

sc.range(0,50)
  .map(a => a*10)
  .filter( a => (a%15)==0 )
  .sortBy( (a:Long) => a )
  .reduce( (a:Long, b:Long) => a-b)
