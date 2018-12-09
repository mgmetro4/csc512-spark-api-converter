/**
*	Source : RDDToDFGrammar.h
*	Description: Provides the grammar to be used to parse the program. Provides interface for each non-terminal in the grammar.
*				 Non-Terminal interfaces will validate the program using the terminals of the grammar.
*/
#ifndef _RDD_TO_DF_GRAMMAR_H
#define _RDD_TO_DF_GRAMMAR_H
#include <fstream>
#include "Parser.h"
#include "ASTNode.h"
#include "Grammar.h"

class RDDToDFGrammar: public Grammar
{
public:
	RDDToDFGrammar(std::ofstream *newOutFile) : Grammar(newOutFile) {}

	bool program();

	bool range();

	bool mapsOrCollect();

	bool mapsOrCollect0();

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

};
#endif
