README

Github located at: https://github.ncsu.edu/stparham/csc512-spark-api-converter

--------------------------------------------------
How To Compile
--------------------------------------------------

1. cd into /transformer directory
2. make
3. move the RDD API .scala file into the /transformer directory
4. Run RDD to Dataset via ./Transformer -ds test1.scala
   or to Dataframe via ./Transformer -df test1.scala
   where test1.scala is the RDD file you want to transform.
   This file can be named anything as long as it ends in .scala

5. Output will be named test1_gen.scala


--------------------------------------------------
How To Test Dataset
--------------------------------------------------
 ./Transformer -ds test1.scala

All the RDD test cases that can be transformed to Dataset
are located in /csc512-spark-api-converter/part0/all_dataset_tests

Exptected Dataset Output (proven to work in spark-shell) are located
at /csc512-spark-api-converter/ds_tests_expected_output.txt

This text file contains all the dataset representations found in the /part0
test cases for easy viewing.


--------------------------------------------------
How To Test Dataframe
--------------------------------------------------
 ./Transformer -df test1.scala
 
All the RDD test cases that can be transformed to Dataframe
are located in /csc512-spark-api-converter/part0/all_dataframe_tests

Expected Dataframe Output (proven to work in spark-shell) are located
at /csc512-spark-api-converter/df_tests_expected_output.txt

This text file contains all the dataframe representations found in the /part0
test cases for easy viewing. (dataframe example, create tuple, complex tuple)