// This should not parse because there is code after a "done" function

sc.range(1,2000)
  .map(UDF)
  .collect()
  .somefunction()
