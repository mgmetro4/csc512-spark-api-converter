// just a simple file to test collect

sc.range(1,2000)
  .map(i => { val j = i % 3; (i, if (j == 0) i * 10 else i * 2 )})
  .collect()
