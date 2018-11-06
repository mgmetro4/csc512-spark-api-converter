/* Convert text from file to lowercase */
sc.textFile(“textfile.txt”)
    .map(line => line.toLowerCase)
