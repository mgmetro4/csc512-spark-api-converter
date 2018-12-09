// This should not parse because I'm calling collect (using a "done" function)
// and then trying to call a chainable function

sc.range(1,2000)
  .collect()
  .reduce(UDF)
