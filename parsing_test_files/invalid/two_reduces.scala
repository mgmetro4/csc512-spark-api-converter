// This should not parse because I'm calling reduce twice

sc.range(1,2000)
  .reduce((a:Int, b:Int) => a+b)
  .reduce((a:Int, b:Int) => a+b)
