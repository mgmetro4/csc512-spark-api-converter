// one chainable function

sc.range(1,2000)
  .map(i => i * 2)
  .reduce((a:Int, b:Int) => a+b)
