#include <vector>
#include <cstring>
#include <cstdlib>
#include "RDDToDFGrammar.h"
#include "Token.h"
#include "Util.h"
#include <iostream>

/** see the Google doc for the full grammar definition */

/**
 * This is the entry point of the grammar.
 * Production:
 * <program> --> sc . <range> <mapsOrCollect>
 *               [FIRST_PLUS = { sc }]
 */
bool RDDToDFGrammar::program()
{
	// if sc is found
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "sc"))
	{
		*outFile << "spark"; // print the transformation to the output file
		// if a period is found
		if (parse->nextToken()
			&& parse->curToken()->getSymType() == Token::SYMTYPE_PERIOD)
		{
			*outFile << parse->curToken()->getTokenName(); // print it to the output file
			// if range is found
			if (parse->nextToken() && range())
			{
				// if maps or collect are found
				if (mapsOrCollect())
				{
					// make sure that all tokens have been used
					if (!parse->curToken()) // if there is no current token
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}


/**
 * Production:
 * <range> --> range ( <number> , <number> )
 *             [FIRST_PLUS = { range }]
 */
bool RDDToDFGrammar::range()
{
	// if the range keyword is found
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "range"))
	{
		*outFile << parse->curToken()->getTokenName(); // print it to the output file
		// if a left parenthesis is found
		if (parse->nextToken()
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS)
		{
			*outFile << parse->curToken()->getTokenName(); // print it to the output file
			// if a number is found
			if (parse->nextToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				*outFile << parse->curToken()->getTokenName(); // print it to the output file
				// if a comma is found
				if (parse->nextToken()
					&& parse->curToken()->getSymType() == Token::SYMTYPE_COMMA)
				{
					*outFile << parse->curToken()->getTokenName(); // print it to the output file
					// if a number is found
					if (parse->nextToken()
						&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
					{
						*outFile << parse->curToken()->getTokenName(); // print it to the output file
						// if a right parenthesis is found
						if (parse->nextToken()
							&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS)
						{
							*outFile << parse->curToken()->getTokenName(); // print it to the output file
							*outFile << ".selectExpr(\"id as _1\")" << std::endl; // print the transformation and a newline to the output file
							parse->nextToken();
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

/**
 * Production:
 * <mapsOrCollect> --> . <mapsOrCollect0>
 *                     [FIRST_PLUS = { . }]
 */
bool RDDToDFGrammar::mapsOrCollect()
{
	// if a period is found
	if (parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_PERIOD)
	{
		*outFile << parse->curToken()->getTokenName(); // print it to the output file
		parse->nextToken();
		// if mapsOrCollect0 is found
		if (mapsOrCollect0())
		{
			return true;
		}
	}
	return false;
}

/**
 * Production:
 * <mapsOrCollect0> --> map ( <UDF> ) <mapsOrCollect>
 *                      [FIRST_PLUS = { map }]
 *                    | collect ( )
 *                      [FIRST_PLUS = { collect }]
 */
bool RDDToDFGrammar::mapsOrCollect0()
{
	// if the map keyword is found
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "map"))
	{
		*outFile << "selectExpr"; // print the transformation to the output file
		// if a left parenthesis is found
		if (parse->nextToken()
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS)
		{
			*outFile << parse->curToken()->getTokenName(); // print it to the output file
			std::vector<Variable*> vars;
			// if a UDF is found
			if (parse->nextToken()
				&& UDF(vars))
			{
				// if a right parenthesis is found
				if (parse->curToken()
					&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS)
				{
					*outFile << parse->curToken()->getTokenName() << std::endl; // print it and a newline to the output file
					// if maps or collect are found
					if (parse->nextToken() && mapsOrCollect())
					{
						return true;
					}
				}
			}
		}
	}
	// else if the collect keyword is found
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "collect"))
	{
		*outFile << parse->curToken()->getTokenName(); // print it to the output file
		// if a left parenthesis is found
		if (parse->nextToken()
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS)
		{
			*outFile << parse->curToken()->getTokenName(); // print it to the output file
			// if a right parenthesis is found
			if (parse->nextToken()
				&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS)
			{
				*outFile << parse->curToken()->getTokenName() << std::endl; // print it and a newline to the output file
				parse->nextToken();
				return true;
			}
		}
	}
	return false;
}

/**
 * Production:
 * <UDF> --> <identifier> => <statementBlock>
 *           [FIRST_PLUS = { <identifier> }]
 */
bool RDDToDFGrammar::UDF(std::vector<Variable*> &vars)
{
	// if an identifier is found
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER)
	{
		// add it as the first variable with a value of "_1"
		Variable *var1 = new Variable;
		var1->name = parse->curToken()->getTokenName();
		var1->value = "_1";
		vars.push_back(var1);

		// if an arrow is found
		if (parse->nextToken()
			&& parse->curToken()->getSymType() == Token::SYMTYPE_ARROW)
		{
			// if a statement block is found
			if (parse->nextToken()
				&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
				|| parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS
				|| parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
				|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
				|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
				&& statementBlock(vars))
			{
				return true;
			}
		}
	}
	*outFile << std::endl << "ERROR in UDF";
  return false;
}

/**
 * Production:
 * <statementBlock> --> { <assignOrStmt> }
 *                      [FIRST_PLUS = { { }]
 *                    | <statement>
 *                      [FIRST_PLUS = { <identifier>, <number>, (, if }]
 */
bool RDDToDFGrammar::statementBlock(std::vector<Variable*> &vars)
{
	// if a left curly brace is found
	if (parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
	{
		// if assignments or statements are found
		if (parse->nextToken()
			&& assignOrStmt(vars))
		{
			// if a right curly brace is found
			if (parse->curToken()
				&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
			{
				parse->nextToken();
				return true;
			}
		}
	}
	// else if a statement is found
	else if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS
		|| parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
		|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
		&& statement(vars))
	{
		return true;
	}
	return false;
}

/**
 * Production:
 * <assignOrStmt> --> <assignments>
 *                    [FIRST_PLUS = { val }]
 *                  | <statement>
 *                    [FIRST_PLUS = { <identifier>, <number>, (, if }]
 */
bool RDDToDFGrammar::assignOrStmt(std::vector<Variable*> &vars)
{
	// *outFile << "<<<<<<622>>>>>>" << parse->curToken()->getTokenName();
	// *outFile << "<<<<<<622 CUR STRING>>>>>>" << udfString << std::endl;
	// if assignments are found
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "val")
		&& assignments(vars))
	{
		return true;
	}
	// else if a statement is found
	else if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS
		|| parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
		|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
		&& statement(vars))
	{
		// *outFile << "<<<<<<639>>>>>>" << parse->curToken()->getTokenName();
		// *outFile << "<<<<<<639 CUR STRING>>>>>>" << udfString << std::endl;
		return true;
	}
	return false;
}

/**
 * Production:
 * <assignments> --> <assignment> <assignOrStmt>
 *                   [FIRST_PLUS = { val }]
 */
bool RDDToDFGrammar::assignments(std::vector<Variable*> &vars)
{
	// if an assignment is found
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "val")
		&& assignment(vars))
	{
		// if assignments or statements are found
		if (parse->curToken()
			&& assignOrStmt(vars))
		{
			return true;
		}
	}
	return false;
}

/**
 * Production:
 * <assignment> --> val <identifier> = <expression> ;
 *                  [FIRST_PLUS = { val }]
 */
bool RDDToDFGrammar::assignment(std::vector<Variable*> &vars)
{
	// if the val keyword is found
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "val"))
	{
		// if an identifier is found
		if (parse->nextToken()
			&& parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER)
		{
			// create another variable
			Variable *var = new Variable;
			var->name = parse->curToken()->getTokenName();

			// if an equal sign is found
			if (parse->nextToken()
				&& parse->curToken()->getSymType() == Token::SYMTYPE_EQUAL)
			{
				std::string exprStr;
				// if an expression is found
				if (parse->nextToken()
					&& expression(exprStr, vars))
				{
					// store the expression string as the value
					var->value = exprStr;
					vars.push_back(var);

					// if a semicolon is found
					if (parse->curToken()
						&& parse->curToken()->getSymType() == Token::SYMTYPE_SEMICOLON)
					{
						parse->nextToken();
						//*outFile << "<<<<<<LINE 697>>>>>>" << parse->curToken()->getTokenName();
						return true;
					}
				}
			}
		}
	}
	return false;
}

/**
 * Production:
 * <statement> --> <noParenExpr>
 *                 [FIRST_PLUS = { <identifier>, <number>, if }]
 *               | ( <expression> <parenExprOrTuple>
 *                 [FIRST_PLUS = { ( }]
 */
bool RDDToDFGrammar::statement(std::vector<Variable*> &vars)
{
	std::string noParenExprStr;
	// if a no paren expression is found
	if (parse->curToken()
		&& (parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
		|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
		&& noParenExpr(noParenExprStr, vars))
	{
		*outFile << "\"" << noParenExprStr << " as " << "_1\""; // print out the expression as the sole SQL expression
		return true;
	}
	// else if a left parenthesis is found
	else if (parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS)
	{
		std::string exprStr;
		// if an expression is found
		if (parse->nextToken()
			&& expression(exprStr, vars))
		{
			// if a right paren expression or tuple is found
			if (parenExprOrTuple(exprStr, vars))
			{
				return true;
			}
		}
	}
	// *outFile << "<<<<<<FALSE 741>>>>>>" << parse->curToken()->getTokenName();
	// *outFile << "<<<<<<ERROR>>>>>>" << udfString;
	return false;
}

/**
 * Production:
 * <parenExprOrTuple> --> <rightParenExpr>
 *                        [FIRST_PLUS = { ) }]
 *                      | <tuple>
 *                        [FIRST_PLUS = { , }]
 */
bool RDDToDFGrammar::parenExprOrTuple(std::string &prevExprStr, std::vector<Variable*> &vars)
{
	// if a right paren expression is found
	if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS)
		&& rightParenExpr(prevExprStr, vars))
	{
		*outFile << "\"(" << prevExprStr << " as " << "_1\""; // print out the expression as the sole SQL expression
		return true;
	}
	// else if a tuple is found
	else if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_COMMA)
		&& tuple(prevExprStr, vars))
	{
		return true;
	}
	return false;
}

/**
 * Production:
 * <tuple> --> , <expression> <tuple0> )
 *             [FIRST_PLUS = { , }]
 */
bool RDDToDFGrammar::tuple(std::string &prevExprStr, std::vector<Variable*> &vars)
{
	// if a comma is found
	if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_COMMA))
	{
		*outFile << "\"" << prevExprStr << " as " << "_1\""; // print out the previous expression as the first SQL expression
		*outFile << ", "; // print out a comma to separate next SQL expression
		std::string exprStr;
		// if an expression is found
		if (parse->nextToken()
			&& expression(exprStr, vars))
		{
			*outFile << "\"" << exprStr << " as " << "_2\""; // print out this expression as the second SQL expression
			// if a tuple0 is found
			if (parse->curToken()
				&& tuple0(vars, 3))
			{
				// if a right parenthesis is found
				if (parse->curToken()
					&& (parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS))
				{
					parse->nextToken();
					return true;
				}
			}
		}
	}
	return false;
}

/**
 * Production:
 * <tuple0> --> , <expression> <tuple0>
 *              [FIRST_PLUS = { , }]
 *            | EPSILON
 *              [FIRST_PLUS = { ) }]
 */
bool RDDToDFGrammar::tuple0(std::vector<Variable*> &vars, int sqlExprNum)
{
	// if a comma is found
	if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_COMMA))
	{
		*outFile << ", "; // print out a comma to separate next SQL expression
		std::string exprStr;
		// if an expression is found
		if (parse->nextToken()
			&& expression(exprStr, vars))
		{
			*outFile << "\"" << exprStr << " as _" << sqlExprNum << "\""; // print out this expression as the <sqlExprNum>th SQL expression
			// if a tuple0 is found
			if (tuple0(vars, sqlExprNum + 1))
			{
				return true;
			}
		}
	}
	// else if a right parenthesis is found
	else if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS))
	{
		return true;
	}
	return false;
}

/**
 * Production:
 * <rightParenExpr> --> ) <opExpr>
 *                      [FIRST_PLUS = { ) }]
 */
bool RDDToDFGrammar::rightParenExpr(std::string &exprStr, std::vector<Variable*> &vars)
{
	// if a right parenthesis is found
	if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS))
	{
		exprStr.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if an op expression is found
		if (parse->nextToken()
			&& opExpr(exprStr, vars))
		{
			return true;
		}
	}
	return false;
}

/**
 * Production:
 * <noParenExpr> --> <identifier> <field> <opExpr>
 *                   [FIRST_PLUS = { <identifier> }]
 *                 | <number> <opExpr>
 *                   [FIRST_PLUS = { <number> }]
 *                 | if ( <boolExpr> ) <expression> else <expression>
 *                   [FIRST_PLUS = { if }]
 */
bool RDDToDFGrammar::noParenExpr(std::string &exprStr, std::vector<Variable*> &vars)
{
	// *outFile << "<<<<<<874>>>>>>" << parse->curToken()->getTokenName();
	// *outFile << "<<<<<<874 CUR STRING>>>>>>" << udfString << std::endl;
	// if an identifier is found
	if (parse->curToken()
		&& (parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER))
	{
		std::string idStr(parse->curToken()->getTokenName());
		// search through the vars (excluding the first var (the param of the UDF)) to see if any match this id
		bool foundMatch = false;
		for (unsigned i = 1; i < vars.size(); i++)
		{
			Variable *curVar = vars.at(i);
			// check if the current variable's name matches the id
			if (!strcmp(idStr.c_str(), curVar->name.c_str()))
			{
				exprStr.append("(");
				exprStr.append(curVar->value); // add the value of the variable to the expression string
				exprStr.append(")");
				foundMatch = true;
				break;
			}
		}

		std::string fieldStr;
		// if a valid field configuration is found
		if (parse->nextToken()
			&& field(fieldStr))
		{
			// if a field was found
			if (!fieldStr.empty())
			{
				// if a match was previously found with one of the vars
				if (foundMatch) {
					return false; // there shouldn't be any field calls after a declared var
				}
				// if the idStr matches the first variable (the param of the UDF)
				Variable *udfParam = vars.at(0);
				if (!strcmp(idStr.c_str(), udfParam->name.c_str()))
				{
					// add the actual field without a period
					exprStr.append(fieldStr);
				}
				// else just add the id and the field with the period
				else
				{
					exprStr.append(idStr); // add the actual id to the string that's being constructed
					exprStr.append(".");
					exprStr.append(fieldStr);
				}
			}
			// else a field wasn't found
			else
			{
				// if there wasn't a match found when searching through the 1-N vars
				if (!foundMatch) {
					// if the idStr matches the first variable (the param of the UDF)
					Variable *udfParam = vars.at(0);
					if (!strcmp(idStr.c_str(), udfParam->name.c_str()))
					{
						// add the value of the udf param (_1)
						exprStr.append(udfParam->value);
					}
					// else just add the id
					else
					{
						exprStr.append(idStr); // add the actual id to the string that's being constructed
					}
				}
			}


			// *outFile << "<<<<<<889>>>>>>" << parse->curToken()->getTokenName();
			// *outFile << "<<<<<<889 CUR STRING>>>>>>" << udfString << std::endl;
			// if an op expression is found
			if (opExpr(exprStr, vars))
			{
				return true;
			}
		}
	}
	// else if a number is found
	else if (parse->curToken()
		&& (parse->curToken()->getID() == Token::IDTYPE_NUMBER))
	{
		exprStr.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if an op expression is found
		if (parse->nextToken()
			&& opExpr(exprStr, vars))
		{
			return true;
		}
	}
	// else if the if keyword is found
	else if (parse->curToken()
		&& (!strcmp(parse->curToken()->getTokenName().c_str(), "if")))
	{
		exprStr.append(" "); // add a space to the string that's being constructed
		exprStr.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		exprStr.append(" "); // add a space to the string that's being constructed
		// if a left parenthesis is found
		if (parse->nextToken()
			&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS))
		{
			exprStr.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
			// if a bool expression is found
			if (parse->nextToken()
				&& boolExpr(exprStr, vars))
			{
				// if a right parenthesis is found
				if (parse->curToken()
					&& (parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS))
				{
					//exprStr.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
					exprStr.append(","); // add a comma to the expression string
					// if an expression is found
					if (parse->nextToken()
						&& expression(exprStr, vars))
					{
						// if the else keyword is found
						if (parse->curToken()
							&& (!strcmp(parse->curToken()->getTokenName().c_str(), "else")))
						{
							exprStr.append(","); // add a comma to the expression string
							// if an expression is found
							if (parse->nextToken()
								&& expression(exprStr, vars))
							{
								exprStr.append(")"); // add a right parenthesis to the expression string
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

/**
 * Production:
 * <field> --> . <identifier>
 *             [FIRST_PLUS = { . }]
 *           | EPSILON
 *             [FIRST_PLUS = { +, -, *, %, ), }, ;, ,, else, ==, <, >, !=, <=, >= }]
 */
bool RDDToDFGrammar::field(std::string &fieldStr)
{
	// if a period is found
	if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_PERIOD))
	{
		// if an identifier is found
		if (parse->nextToken()
			&& (parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER))
		{
			fieldStr.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
			parse->nextToken();
			return true;
		}
	}
	// else if any op, any comparator, a right parenthesis, a right brace, a semicolon, or a comma is found
	else if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_PLUS
		|| parse->curToken()->getSymType() == Token::SYMTYPE_MINUS
		|| parse->curToken()->getSymType() == Token::SYMTYPE_STAR
		|| parse->curToken()->getSymType() == Token::SYMTYPE_PERCENT
		|| parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS
		|| parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE
		|| parse->curToken()->getSymType() == Token::SYMTYPE_SEMICOLON
		|| parse->curToken()->getSymType() == Token::SYMTYPE_COMMA
		|| parse->curToken()->getSymType() == Token::SYMTYPE_DOUBLE_EQUAL
		|| parse->curToken()->getSymType() == Token::SYMTYPE_LT
		|| parse->curToken()->getSymType() == Token::SYMTYPE_GT
		|| parse->curToken()->getSymType() == Token::SYMTYPE_NT_EQUAL
		|| parse->curToken()->getSymType() == Token::SYMTYPE_LT_EQUAL
		|| parse->curToken()->getSymType() == Token::SYMTYPE_GT_EQUAL
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "else")))
	{
		return true;
	}
	return false;
}

/**
 * Production:
 * <expression> --> <noParenExpr>
 *                  [FIRST_PLUS = { <identifier>, <number>, if }]
 *                | ( <expression> <rightParenExpr>
 *                  [FIRST_PLUS = { ( }]
 */
bool RDDToDFGrammar::expression(std::string &exprStr, std::vector<Variable*> &vars)
{
	// if a no paren expression is found
	if (parse->curToken()
		&& (parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
		|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
		&& noParenExpr(exprStr, vars))
	{
		return true;
	}
	// else if a left parenthesis is found
	else if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS))
	{
		exprStr.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if an expression is found
		if (parse->nextToken()
			&& expression(exprStr, vars))
		{
			// if a right paren expression is found
			if (rightParenExpr(exprStr, vars))
			{
				return true;
			}
		}
	}
	return false;
}

/**
 * Production:
 * <opExpr> --> <op> <expression> <opExpr>
 *              [FIRST_PLUS = { +, -, *, % }]
 *            | EPSILON
 *              [FIRST_PLUS = { ), }, ;, ,, else, ==, <, >, !=, <=, >= }]
 */
bool RDDToDFGrammar::opExpr(std::string &exprStr, std::vector<Variable*> &vars)
{
	// if an op is found
	if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_PLUS
		|| parse->curToken()->getSymType() == Token::SYMTYPE_MINUS
		|| parse->curToken()->getSymType() == Token::SYMTYPE_STAR
		|| parse->curToken()->getSymType() == Token::SYMTYPE_PERCENT)
		&& op(exprStr))
	{
		// if an expression is found
		if (expression(exprStr, vars))
		{
			// if an op expression is found
			if (opExpr(exprStr, vars))
			{
				return true;
			}
		}
	}
	// else if a right parenthesis, right curly brace, semicolon, comma, else keyword,
	// double equal sign, less than sign, greater than sign, not equal to sign,
	// less than or equal to sign, or greater than or equal to sign is found
	else if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS
		|| parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE
		|| parse->curToken()->getSymType() == Token::SYMTYPE_SEMICOLON
		|| parse->curToken()->getSymType() == Token::SYMTYPE_COMMA
		|| parse->curToken()->getSymType() == Token::SYMTYPE_DOUBLE_EQUAL
		|| parse->curToken()->getSymType() == Token::SYMTYPE_LT
		|| parse->curToken()->getSymType() == Token::SYMTYPE_GT
		|| parse->curToken()->getSymType() == Token::SYMTYPE_NT_EQUAL
		|| parse->curToken()->getSymType() == Token::SYMTYPE_LT_EQUAL
		|| parse->curToken()->getSymType() == Token::SYMTYPE_GT_EQUAL
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "else")))
	{
		return true;
	}
	return false;
}

/**
 * Production:
 * <boolExpr> --> <expression> <comp> <expression>
 *                [FIRST_PLUS = { <identifier>, <number>, (, if }]
 */
bool RDDToDFGrammar::boolExpr(std::string &exprStr, std::vector<Variable*> &vars)
{
	// if an expression is found
	if (parse->curToken()
		&& (parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
		|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
		|| parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
		&& expression(exprStr, vars))
	{
		// if a comparator is found
		if (comp(exprStr))
		{
			// if an expression is found
			if (expression(exprStr, vars))
			{
				return true;
			}
		}
	}
	return false;
}

/**
 * Production:
 * <op> --> +
 *          [FIRST_PLUS = { + }]
 *        | -
 *          [FIRST_PLUS = { - }]
 *        | *
 *          [FIRST_PLUS = { * }]
 *        | %
 *          [FIRST_PLUS = { % }]
 */
bool RDDToDFGrammar::op(std::string &exprStr)
{
	// if an op is found
	if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_PLUS
		|| parse->curToken()->getSymType() == Token::SYMTYPE_MINUS
		|| parse->curToken()->getSymType() == Token::SYMTYPE_STAR
		|| parse->curToken()->getSymType() == Token::SYMTYPE_PERCENT))
	{
		exprStr.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		parse->nextToken();
		return true;
	}
	return false;
}

/**
 * Production:
 * <comp> --> ==
 *            [FIRST_PLUS = { == }]
 *          | <
 *            [FIRST_PLUS = { < }]
 *          | >
 *            [FIRST_PLUS = { > }]
 *          | !=
 *            [FIRST_PLUS = { != }]
 *          | >=
 *            [FIRST_PLUS = { >= }]
 *          | <=
 *            [FIRST_PLUS = { <= }]
 */
bool RDDToDFGrammar::comp(std::string &exprStr)
{
	if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_DOUBLE_EQUAL
		|| parse->curToken()->getSymType() == Token::SYMTYPE_LT
		|| parse->curToken()->getSymType() == Token::SYMTYPE_GT
		|| parse->curToken()->getSymType() == Token::SYMTYPE_NT_EQUAL
		|| parse->curToken()->getSymType() == Token::SYMTYPE_LT_EQUAL
		|| parse->curToken()->getSymType() == Token::SYMTYPE_GT_EQUAL))
	{
		exprStr.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		parse->nextToken();
		return true;
	}
	return false;
}



void RDDToDFGrammar::evaluateASTTree(ASTNode *root)
{
	// if (root == NULL)
	// 	return;
	//
	// std::string newValue;
	//
	// switch (root->type)
	// {
	// case ASTNode::NUMBER:
	// 	newValue.append("local[");
	// 	newValue.append(Util::to_string(mySymTab->curFunction->localVarCnt++));
	// 	newValue.push_back(']');
	//
	// 	std::string constAssign(newValue);
	// 	constAssign.append(" = ");
	// 	constAssign.append(root->value);
	// 	constAssign.push_back(';');
	// 	mySymTab->curFunction->funcStats.push(constAssign);
	//
	// 	root->value.clear();
	// 	root->value.append(newValue);
	// 	root->type = ASTNode::OPERAND;
	// 	break;
	// case ASTNode::OPERAND:
	// 	break;
	// case ASTNode::FUNCCALL:
	// 	newValue.append("local[");
	// 	newValue.append(Util::to_string(mySymTab->curFunction->localVarCnt++));
	// 	newValue.push_back(']');
	//
	// 	std::string funcCallStmt(newValue);
	// 	funcCallStmt.append(" = ");
	// 	funcCallStmt.append(root->value);
	// 	funcCallStmt.push_back(';');
	// 	mySymTab->curFunction->funcStats.push(funcCallStmt);
	//
	// 	root->value.clear();
	// 	root->value.append(newValue);
	// 	root->type = ASTNode::OPERAND;
	// 	break;
	// case ASTNode::OPERATION:
	// 	evaluateASTTree(root->left, false);
	// 	evaluateASTTree(root->right, false);
	//
	// 	newValue.append("local[");
	// 	newValue.append(Util::to_string(mySymTab->curFunction->localVarCnt++));
	// 	newValue.push_back(']');
	//
	// 	std::string oprStmt(newValue);
	// 	oprStmt.append(" = ");
	// 	oprStmt.append(root->left->value);
	// 	oprStmt.push_back(' ');
	// 	oprStmt.append(root->value);
	// 	oprStmt.push_back(' ');
	// 	oprStmt.append(root->right->value);
	// 	oprStmt.push_back(';');
	//
	// 	mySymTab->curFunction->funcStats.push(oprStmt);
	// 	root->value.clear();
	// 	root->value.append(newValue);
	// 	root->type = ASTNode::OPERAND;
	// 	delete root->left;
	// 	root->left = NULL;
	// 	delete root->right;
	// 	root->right = NULL;
	// }
}
