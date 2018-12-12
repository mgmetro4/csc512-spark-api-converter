/**
*	Source : RDDToDFGrammar.h
*	Description: Provides the grammar to be used to parse the program. Provides interface for each non-terminal in the grammar.
*				 Non-Terminal interfaces will validate the program using the terminals of the grammar.
*/
#ifndef _RDD_TO_DF_GRAMMAR_H
#define _RDD_TO_DF_GRAMMAR_H
#include <fstream>
#include <vector>
#include "Parser.h"
#include "ASTNode.h"
#include "Grammar.h"

struct Variable {
	std::string name;
	std::string value;
};

class RDDToDFGrammar: public Grammar
{
public:
	RDDToDFGrammar(std::ofstream *newOutFile) : Grammar(newOutFile) {}

	bool program();

	bool range();

	bool mapsOrCollect();

	bool mapsOrCollect0();

	bool UDF(std::vector<Variable*> &vars);

	bool statementBlock(std::vector<Variable*> &vars);

	bool assignOrStmt(std::vector<Variable*> &vars);

	bool assignments(std::vector<Variable*> &vars);

	bool assignment(std::vector<Variable*> &vars);

	bool statement(std::vector<Variable*> &vars);

	bool parenExprOrTuple(std::string &prevExprStr, std::vector<Variable*> &vars);

	bool tuple(std::string &prevExprStr, std::vector<Variable*> &vars);

	bool tuple0(std::vector<Variable*> &vars, int sqlExprNum);

	bool rightParenExpr(std::string &exprStr, std::vector<Variable*> &vars);

	bool noParenExpr(std::string &exprStr, std::vector<Variable*> &vars);

	bool field(std::string &fieldStr);

	bool expression(std::string &exprStr, std::vector<Variable*> &vars);

	bool opExpr(std::string &exprStr, std::vector<Variable*> &vars);

	bool boolExpr(std::string &exprStr, std::vector<Variable*> &vars);

	bool op(std::string &exprStr);

	bool comp(std::string &exprStr);

	void evaluateASTTree(ASTNode *root);

};
#endif
