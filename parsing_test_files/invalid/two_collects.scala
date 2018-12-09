// This should not parse because I'm calling collect twice

sc.range(1,2000)
  .collect()
  .collect()
