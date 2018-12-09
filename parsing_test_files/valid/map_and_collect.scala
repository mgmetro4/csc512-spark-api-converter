// just a simple file to test collect

sc.range(1,2000)
  .map(UDF)
  .collect()
