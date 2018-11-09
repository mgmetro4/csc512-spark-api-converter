// RDD
// Filter out and collect all even numbers in the range

sc.range(0, 101)
    .filter(i => i%2 == 0)
    .collect()
