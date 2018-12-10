/**
*	Source : RDDToDSGrammar.h
*	Description: Provides the grammar to be used to parse the program. Provides interface for each non-terminal in the grammar.
*				 Non-Terminal interfaces will validate the program using the terminals of the grammar.
*/
#ifndef _RDD_TO_DS_GRAMMAR_H
#define _RDD_TO_DS_GRAMMAR_H
#include <fstream>
#include "Parser.h"
#include "ASTNode.h"
#include "Grammar.h"

class RDDToDSGrammar: public Grammar
{
public:
	RDDToDSGrammar(std::ofstream *newOutFile) : Grammar(newOutFile) {}

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

	bool mapUDF(std::string &udfString);

	bool exprTupleBlock(std::string &udfString);

	bool assignThenExprTuple(std::string &udfString);

	bool assignment(std::string &udfString);

	bool exprTuple(std::string &udfString);

	bool parenExprOrTuple(std::string &udfString);

	bool tuple(std::string &udfString);

	bool tuple0(std::string &udfString);

	bool filterUDF(std::string &udfString);

	bool boolExprBlock(std::string &udfString);

	bool assignThenBoolExpr(std::string &udfString);

	bool sortByUDF(std::string &udfString);

	bool type(std::string &udfString);

	bool exprBlock(std::string &udfString);

	bool assignThenExpr(std::string &udfString);

	bool reduceUDF(std::string &udfString);

	bool rightParenExpr(std::string &udfString);

	bool noParenExpr(std::string &udfString);

	bool field(std::string &udfString);

	bool expression(std::string &udfString);

	bool opExpr(std::string &udfString);

	bool boolExpr(std::string &udfString);

	bool op(std::string &udfString);

	bool comp(std::string &udfString);

	void evaluateASTTree(ASTNode *root);

};
#endif
