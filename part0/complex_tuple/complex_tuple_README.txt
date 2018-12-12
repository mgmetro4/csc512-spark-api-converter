Complex Tuple
**made for RDD to Dataframe


RDD

sc.range(0, 10)
	.map(i=>{(if(i%2==0)i else 0, i)})
	.collect()

Output: Array[(Long, Long)] = 
		Array((0,0), (0,1), (2,2), (0,3), (4,4), (0,5), (6,6), (0,7), (8,8), (0,9))



Dataframe

spark.range(0, 10).selectExpr("id as_1")
	.selectExpr("if(_1%2==0, _1, 0)", "_1 as _2")
	.collect()
	
Output: Array[org.apache.spark.sql.Row] = 
		Array([0,0], [0,1], [2,2], [0,3], [4,4], [0,5], [6,6], [0,7], [8,8], [0,9])
