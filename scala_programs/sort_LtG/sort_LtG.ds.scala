/* Dataset
 * Sort random text file of numbers from least to greatest 
 * echo "10, 1, 50, 4, 18, 35" > numbers.txt */

spark.textFile(“numbers.txt”)
    .map(line => line.split(",")
    .map(elem => elem.trim)
    // below does not work. Not sure why
    .map(row=>((_.toInt)(row), row)).orderBy("_1").map(_._2))
    .collect() //creates another array, but don't know how else to view answers on terminal window easily


//will return Array[Array[String]] = Array(Array(1, 4, 18, 35, 50, 100))
// if you add .map(_.toInt) after trim) , will return Array[Array[Int]] = Array(Array(1, 4, 18, 35, 50, 100))

// I can't get it to work with descending order