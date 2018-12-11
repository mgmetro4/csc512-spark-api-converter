#!/bin/bash
(
echo "one two three four five" > words.txt
echo ":paste"
cat ../helpers.scala
echo -e "\04"

(cat word_counter_rdd.scala; echo) | (
	while read line
	do
		echo "$line"
	done
)


(cat word_counter_ds.scala; echo) | (
	while read line
	do
		echo "$line"
	done
)

(cat word_counter_df.scala; echo) | (
	while read line
	do
		echo "$line"
	done
)


) | spark-shell
