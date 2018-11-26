# To use this make file, the g++ compiler must be installed.
# If g++ is not found, then update the path below to point to g++ on your system.
cc=/usr/bin/g++

all: Transformer Scanner Token Util Parser Grammar
	$(cc) -o Transformer Transformer.o Scanner.o Token.o Util.o Parser.o Grammar.o
Transformer: Transformer.cpp
	$(cc) -c Transformer.cpp
Scanner: Scanner.cpp
	$(cc) -c Scanner.cpp
Token: Token.cpp
	$(cc) -c Token.cpp
Util: Util.cpp
	$(cc) -c Util.cpp
Parser: Parser.cpp
	$(cc) -c Parser.cpp
Grammar: Grammar.cpp
	$(cc) -c Grammar.cpp
clean:
	-rm -f Transformer *.o
