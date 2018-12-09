ReduceBykey - Test Case 4 - Working

Description: Reduce by key


To Run: bash test.sh
** needs the helpers.scala file in the folder above


Example Output: 


RDD

sc.range(1,12)
    .map(i => (i%11,1))
    .reduceByKey((a:Int, b:Int) => a+b)
    .collect()

Output: Aray


Dataset

spark.range(1,100)
    .map(i=>(i%11,1))
    .groupByKey(_._1)
    .agg(reduceByKeyAggregator((a:Int, b:Int) => a_b))
    .collect()


Dataframe 

** Not compatible with Dataframe
