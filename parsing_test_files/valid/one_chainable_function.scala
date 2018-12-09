// one chainable function

sc.range(1,2000)
  .map(UDF)
  .reduce(UDF)
