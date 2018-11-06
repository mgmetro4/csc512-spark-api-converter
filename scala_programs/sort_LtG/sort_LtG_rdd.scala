/* Sort random text file of numbers from least to greatest */

sc.textFile(“textfile.txt”)
	.map(line => line.split(",")
	.map(elem => elem.trim))
    .sortBy(x => x(1), true)