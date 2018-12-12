/**
*	Source : Grammar.h
*	Description: Provides the grammar to be used to parse the program. Provides interface for each non-terminal in the grammar.
*				 Non-Terminal interfaces will validate the program using the terminals of the grammar.
*/
#ifndef _GRAMMAR_H
#define _GRAMMAR_H
#include <fstream>
#include "Parser.h"

class Parser;

class Grammar
{
public:
	Parser *parse;

	std::ofstream *outFile;

	/*Interfaces for each of the non-terminal in the grammar*/
	virtual bool program() = 0;

	void instantiateParser(Parser* newParser)
	{
		parse = newParser;
	}

	Grammar(std::ofstream *newOutFile)
	{
		outFile = newOutFile;
	}

};
#endif
