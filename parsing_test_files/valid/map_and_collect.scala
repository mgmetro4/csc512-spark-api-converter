// just a simple file to test collect

sc.range(1,2000)
  .map(i => i * 2)
  .collect()
