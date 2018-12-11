//Dataset

spark.range(0,10)
    .map(row => ( ((a:Long) => Math.sin(a)) (row),row)).orderBy("_1").map(_._2)
    .collect()