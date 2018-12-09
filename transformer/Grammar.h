/**
*	Source : Grammar.h
*	Description: Provides the grammar to be used to parse the program. Provides interface for each non-terminal in the grammar.
*				 Non-Terminal interfaces will validate the program using the terminals of the grammar.
*/
#ifndef _GRAMMAR_H
#define _GRAMMAR_H
#include <fstream>
#include "Parser.h"
#include "ASTNode.h"

class Parser;

class Grammar
{
public:
	/*Interfaces for each of the non-terminal in the grammar*/
	bool program();

	bool context();

	bool dataset();

	bool dataset0();

	bool range();

	bool textFile();

	bool chainordone();

	bool chainordone0();

	bool chainable();

	bool map();

	bool filter();

	bool sort();

	bool done();

	bool reduce();

	bool collect();

	bool UDF(std::string &udfString);

	bool statementBlock(std::string &udfString);

	bool assignOrStmt(std::string &udfString);

	bool assignments(std::string &udfString);

	bool assignment(std::string &udfString);

	bool statement(std::string &udfString);

	bool parenExprOrTuple(std::string &udfString);

	bool tuple(std::string &udfString);

	bool tuple0(std::string &udfString);

	bool rightParenExpr(std::string &udfString);

	bool noParenExpr(std::string &udfString);

	bool field(std::string &udfString);

	bool expression(std::string &udfString);

	bool opExpr(std::string &udfString);

	bool boolExpr(std::string &udfString);

	bool op(std::string &udfString);

	bool comp(std::string &udfString);

	void evaluateASTTree(ASTNode *root);

	void instantiateParser(Parser* newParser);

private:
	Parser *parse;

public:
		std::ofstream *outFile;

		Grammar(std::ofstream *newOutFile)
		{
			outFile = newOutFile;
		}

};
#endif
