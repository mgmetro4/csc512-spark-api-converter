Dataframe Example - Test Case 2 - Working

Description: Test Case for Dataframe taken off project PDF. 
             Uses an if-else condition to determine value


To Run: bash test.sh



Example Output: 


RDD

sc.range(10, 24)
	.map( i => {val j=i%3; (i, if (j==0) i*10 else i*2) })
	.map(r => r._1+r._2)
	.collect()

Output: Array[Long] = Array(30, 33, 132, 39, 42, 165, 48, 51 198, 57, 60, 231, 66, 69)


Dataset

spark.range(10, 24)
	.map( i => {val j=i%3; (i, if (j==0) i*10 else i*2) })
	.map(r => r._1+r._2)
	.collect()

Output: Array[Long] = Array(30, 33, 132, 39, 42, 165, 48, 51 198, 57, 60, 231, 66, 69)


Dataframe

spark.range(10, 24).selectExpr("id as _1")
	.selectExpr("_1 as _1", "if (_1%3==0), _1*10, _1*2) as _2")
	.selectExpr("_1+_2 as _1")
	.collect()

Output: Array[org.apache.spark.sql.Row] = Array([30], [33], [132], [39], [42], [165], [48], [51], [198], [57], [60], [231], [66], [69])
