// RDD

// words.txt = echo “one two three” > words.txt

sc.textFile("words.txt")
	.map(line => line.split(" ").size)
	.reduce( (a:Int, b:Int) => a+b )