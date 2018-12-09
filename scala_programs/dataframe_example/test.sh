#!/bin/bash
(
echo "one two three four five" > words.txt
echo ":paste"
cat ../helpers.scala
echo -e "\04"

(cat dataframe_example_rdd.scala; echo) | (
	while read line
	do
		echo "$line"
	done
)


(cat dataframe_example_ds.scala; echo) | (
	while read line
	do
		echo "$line"
	done
)

(cat dataframe_example_df.scala; echo) | (
	while read line
	do
		echo "$line"
	done
)


) | spark-shell
