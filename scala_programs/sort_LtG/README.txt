Sort Least to Gratest - Test Case 3 - Working

Description: Sort numbers from least to greatest by their sine values


To Run: bash test.sh



Example Output: 


RDD

sc.range(0,10)
    .sortBy( (a:Long) => Math.sin(a) )
    .collect()

Output: Array[Long] = Array(5, 4, 6, 0, 3, 9, 7, 1, 2, 8)


Dataset

spark.range(0,10)
    .map(row => (( ((a:Long) => Math.sin(a))) (row),row)).orderBy("_1").map(_._2)
    .collect()

Output: Array[Long] = Array(5, 4, 6, 0, 3, 9, 7, 1, 2, 8)


Dataframe - the _1 is the Ints that are in order by sin value in _2

spark.range(0,10).selectExpr("id as _1")
    .selectExpr("_1 as _1", "if(_1==_1,SIN(_1),SIN(_1)) as _2")
    .orderBy("_2")
    .collect()

 Output: Array[org.apache.spark.sql.Row] = Array(
    [5,-0.9589242746631385], [4,-0.7568024953079282], [6,-0.27941549819892586], [0,0.0], [3,0.1411200080598672], [9,0.4121184852417566], [7,0.6569865987187891], [1,0.8414709848078965], [2,0.9092974268256817], [8,0.9893582466233818])
