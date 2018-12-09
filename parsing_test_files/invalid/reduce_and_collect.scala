// This should not parse because I'm calling two "done" functions

sc.range(1,2000)
  .reduce((a:Int, b:Int) => a+b)
  .collect()
