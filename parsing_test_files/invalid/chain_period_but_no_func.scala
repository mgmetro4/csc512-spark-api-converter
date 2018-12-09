// This should not parse because there is an extra period as if a chainable
// function is going to be called

sc.range(1,2000)
  .
  .reduce(UDF)
