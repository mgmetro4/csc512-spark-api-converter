// two chainable functions

sc.range(1,2000)
  .map(i => i * 2)
  .filter(i => i%2 == 0)
  .reduce((a:Int, b:Int) => a+b)
