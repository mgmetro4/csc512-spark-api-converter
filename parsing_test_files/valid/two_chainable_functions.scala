// two chainable functions

sc.range(1,2000)
  .map(UDF)
  .filter(UDF)
  .reduce(UDF)
