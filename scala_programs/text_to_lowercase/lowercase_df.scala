 /* Dataframe
  * Convert text from file to lowercase. 
  * .collect() splits lines into arrays.
  * you can view original by calling .collect() on the sc directly */

spark.read.textFile(“textfile.txt”)
    .selectExpr("") 
    .collect()

    //no idea how to grab lines from Dataframe
