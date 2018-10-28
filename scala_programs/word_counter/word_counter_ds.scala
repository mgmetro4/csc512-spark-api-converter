spark.read.textFile("words.txt")
  .map(l => l.split(" ").size)
  .reduce((a, b) => a+b)
