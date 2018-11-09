// RDD
// Convert text from file to lowercase.
// .collect() splits lines into arrays.
// you can view original by calling .collect() on the sc directly

sc.textFile("textfile.txt")
  .map(line => line.toLowerCase)
  .collect()
