#!/bin/bash

(


(cat even_numbers_rdd.scala; echo) | (
    while read line
    do
        echo "$line"
    done
)


(cat  even_numbers_ds.scala; echo) | (
    while read line
    do
        echo "$line"
    done
)

(cat  even_numbers_df.scala; echo) | (
    while read line
    do
        echo "$line"
    done
)


) | spark-shell
