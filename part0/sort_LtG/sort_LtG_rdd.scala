// RDD

sc.range(0,10)
    .sortBy( (a:Long) => Math.sin(a) )
    .collect()