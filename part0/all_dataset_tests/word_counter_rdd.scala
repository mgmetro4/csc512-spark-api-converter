//RDD
sc.textFile("words.txt")
	.map(line => line.split(" ").size)
	.reduce( (a:Int, b:Int) => a+b )