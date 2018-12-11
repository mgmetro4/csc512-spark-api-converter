#!/bin/bash

(

echo ":paste"
cat ../helpers.scala
echo -e "\04"

(cat reduceByKey_rdd.scala; echo) | (
    while read line
    do
        echo "$line"
    done
)


(cat  reduceByKey_ds.scala; echo) | (
    while read line
    do
        echo "$line"
    done
)


) | spark-shell
