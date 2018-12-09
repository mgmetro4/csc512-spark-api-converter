ReduceBykey - Test Case 4 - Working

Description: Reduce by key (it wil not return in order when collected)


To Run: bash test.sh
** needs the helpers.scala file in the folder above


Example Output: 


RDD

sc.range(1,12)
    .map(i => (i%11,1))
    .reduceByKey((a:Int, b:Int) => a+b)
    .collect()

Output:  rray[(Long, Int)] = Array((4,909091), (0,909090), (8,909091), (1,909091), (9,909091), (5,909091), (6,909091), (10,909090), (2,909091), (3,909091), (7,909091))



Dataset

spark.range(1,100)
    .map(i=>(i%11,1))
    .groupByKey(_._1)
    .agg(reduceByKeyAggregator((a:Int, b:Int) => a_b))
    .collect()

Output: Array[(Long, Int)] = Array((0,909090), (7,909091), (6,909091), (9,909091), (5,909091), (1,909091), (10,909090), (3,909091), (8,909091), (2,909091), (4,909091))



Dataframe 

** Not compatible with Dataframe
