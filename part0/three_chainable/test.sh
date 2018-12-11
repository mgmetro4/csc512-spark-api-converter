#!/bin/bash
(

echo ":paste"
cat ../helpers.scala
echo -e "\04"

(cat three_chainable_rdd.scala; echo) | (
	while read line
	do
		echo "$line"
	done
)


(cat three_chainable_ds.scala; echo) | (
	while read line
	do
		echo "$line"
	done
)

(cat three_chainable_df.scala; echo) | (
    while read line
    do
        echo "$line"
    done
)



) | spark-shell
