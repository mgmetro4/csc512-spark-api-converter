# csc512-spark-api-converter
This repository contains the code for a program which can convert Scala programs which have been written with the 3 Apache Spark APIs (RDD, Dataset, Dataframe) from one API to another.

The repo follows the structure below:
- scala_programs - This contains several simple Scala programs which utilize the 3 Spark APIs.  The RDD versions of each program can be used as inputs to the Transformer.
- transformer - This contains the Spark API converter program (Transformer).

## Compilation and Usage of the Spark API Converter
*Perform these actions while inside the transformer directory.*

Use the ```make``` command to compile the program.  Alternatively, you can manually follow the steps outlined in the Makefile.

Run the executable named Main with the input filename as an argument.
Eg: ```./Transformer [INPUT_FILE_NAME]```

The program will generate an output file with the name [INPUT_FILE_NAME_WITHOUT_FILE_EXTENSION]_gen.scala.  "Error" will be printed to standard out if the input file is not accepted by the Scanner.


## Description of the Spark API Converter
### Transformer.cpp
Transformer.cpp is the interface between the user and the scanner program. It
makes use of the Scanner API to identify each token and print it to standard
out. Parser functionality will be incorporated later.

### Token.cpp
Token.cpp contains the class for a token. It holds the properties of a token
and provides methods to access the token and populate it. The properties of the
token identifies the type of the token and if it is a symbol, it contains the
symbol type.

### Util.cpp
Util.cpp provides methods to check the type of a character as well as a few
helper methods.

### Scanner.cpp
Scanner.cpp provides methods to check whether there is a next token, get the next
token, and populate a token. The method to populate a token contains the logic
for identifying a token and populating Token objects with token details.

### Implementation
1. If a token is an identifier then the token value is compared against the
list of reserved words to mark the token as a reserved word.
2. '#' and '//' are treated as symbols and at later stage the character is compared
against '#' or '//' to mark the token as a MetaStatement.
3. If a " is found, then the following characters are considered a part of a
string until another " is reached. If no ending " is found in the same line
then the token is marked as invalid.
4. If a ' is found, then it, the following character, and another closing ' are
considered a char.  If there is not EXACTLY one character between two single
quotes, then the token is marked as invalid.
5. Any token starting with a digit is treated as a number and it is populated
until no new digits are found.
6. Any token starting with a letter or underscore is treated as an identifier
and it is populated until no new letter, underscore, or digit is found.
7. Any token starting with '#' or '//' is treated as a MetaStatement and it is
populated until the end of line is reached.

## Grammar Description for Valid Input Files

### RDD to Dataset

#### Transforming Rules
RDD | Dataset
------------ | ------------
sc                    | spark
range                 | range
textFile              | read.textFile
map                   | map
filter                | filter
reduce(<func>)        | select(reduceAggregator(<func>)).collect()
reduceByKey(<func>)   | groupByKey(_._1).agg(reduceByKeyAggregator(<func>))
sortBy(<func>)        | map(row=>((<func>)(row), row)).orderBy("_1").map(_._2)
collect               | collect

#### Grammar


### RDD to Dataframe 

#### Transforming Rules
RDD | Dataframe
------------ | ------------
sc           | spark
range(m,n)   | range(m,n).selectExpr("id as _1")
map(UDF)     | selectExpr(SQL)
collect      | collect

#### Grammar
