Word Counter - Test Case 1 - Working

Description: Count the number of words

To Run: bash test.sh
** helpers.scala must be in the scala_programs folder (above all the spark program folders)


Example Output:


RDD
words.txt = echo “one two three four five” > words.txt

sc.textFile("words.txt")
.map(line => line.split(" ").size)
.reduce( (a:Int, b:Int) => a+b )

Output: Int = 5 (cannot .collect() this)


Dataset
spark.read.textFile("words.txt")
.map(l => l.split(" ").size)
.select(reduceAggregator( (a:Int, b:Int) => a+b )).collect() 

Output: Array[Int] = Array(5)


Dataframe
spark.read.text("words.txt")
.selectExpr("sum(size(split(value,' ')))")
.as[Long].first

Output: Long = 5

** add .toInt at the end to get Int = 5
