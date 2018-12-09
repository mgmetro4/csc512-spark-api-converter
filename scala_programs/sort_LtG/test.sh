#!/bin/bash
(


(cat sort_LtG_rdd.scala; echo) | (
    while read line
    do
        echo "$line"
    done
)


(cat  sort_LtG_ds.scala; echo) | (
    while read line
    do
        echo "$line"
    done
)

(cat  sort_LtG_df.scala; echo) | (
    while read line
    do
        echo "$line"
    done
)


) | spark-shell
