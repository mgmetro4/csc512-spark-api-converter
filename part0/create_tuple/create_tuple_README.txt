Create Tuple Test Case **Created just for dataframe


RDD

sc.range(1, 11)
	.map(i=>(i%11, 1))
	.collect()
	
Output: Array[(Long, Int)] = 
		Array((1,1), (2,1), (3,1), (4,1), (5,1), (6,1), (7,1), (8,1), (9,1), (10,1))




Dataframe

spark.range(1, 11).selectExpr("id as _1")
	.selectExpr("_1%11 as _1", "1 as _2")
	.collect()

Output: Array[org.apache.spark.sql.Row] = 
		Array([1,1], [2,1], [3,1], [4,1], [5,1], [6,1], [7,1], [8,1], [9,1], [10,1])
