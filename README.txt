README

--------------------------------------------------
How To Compile
--------------------------------------------------

1. cd into /transformer directory
2. make
3. Run RDD to Dataset via ./Transformer -ds test1.scala
   or to Dataframe via ./Transformer -df test1.scala
   where test1.scala is the RDD code you want to transform.
   This file can be named anything as long as it ends in .scala

4. Output will be named test1_gen.scala


--------------------------------------------------
How To Test Dataset
--------------------------------------------------

All the RDD test cases that can be transformed to Dataset
are located in /csc512-spark-api-converter/part0/all_dataset_tests

Exptected Dataset Output (proven to work in spark-shell) are located
in /csc512-spark-api-converter/ds_tests_expected_output.txt

This text file contains all the dataset representations found in the /part0
test cases for easy viewing.


--------------------------------------------------
How To Test Dataframe
--------------------------------------------------
All the RDD test cases that can be transformed to Dataframe
are located in /csc512-spark-api-converter/part0/all_dataframe_tests

