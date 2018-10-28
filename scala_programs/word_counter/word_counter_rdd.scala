sc.textFile("words.txt")
  .map(line => line.split(" ").size)
  .reduce((a, b) => a+b)
