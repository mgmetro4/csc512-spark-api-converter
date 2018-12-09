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
		*outFile << parse->curToken()->getTokenName(); // print it to the output file
		// if a left parenthesis is found
		if (parse->nextToken()
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS)
		{
			*outFile << parse->curToken()->getTokenName(); // print it to the output file
			std::string userFunction;
			// if a UDF is found
			if (parse->nextToken()
				&& UDF(userFunction))
			{
				*outFile << userFunction; // print it to the output file
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
bool RDDToDFGrammar::UDF(std::string &udfString)
{
	// if an identifier is found
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER)
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if an arrow is found
		if (parse->nextToken()
			&& parse->curToken()->getSymType() == Token::SYMTYPE_ARROW)
		{
			udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
			// if a statement block is found
			if (parse->nextToken()
				&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
				|| parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS
				|| parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
				|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
				|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
				&& statementBlock(udfString))
			{
				return true;
			}
		}
	}
	*outFile << std::endl << "<<<<<<ERROR>>>>>>" << udfString << std::endl;
  return false;
}

/**
 * Production:
 * <statementBlock> --> { <assignOrStmt> }
 *                      [FIRST_PLUS = { { }]
 *                    | <statement>
 *                      [FIRST_PLUS = { <identifier>, <number>, (, if }]
 */
bool RDDToDFGrammar::statementBlock(std::string &udfString)
{
	// if a left curly brace is found
	if (parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if assignments or statements are found
		if (parse->nextToken()
			&& assignOrStmt(udfString))
		{
			// if a right curly brace is found
			if (parse->curToken()
				&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
			{
				udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
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
		&& statement(udfString))
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
bool RDDToDFGrammar::assignOrStmt(std::string &udfString)
{
	// *outFile << "<<<<<<622>>>>>>" << parse->curToken()->getTokenName();
	// *outFile << "<<<<<<622 CUR STRING>>>>>>" << udfString << std::endl;
	// if assignments are found
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "val")
		&& assignments(udfString))
	{
		return true;
	}
	// else if a statement is found
	else if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS
		|| parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
		|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
		&& statement(udfString))
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
bool RDDToDFGrammar::assignments(std::string &udfString)
{
	// if an assignment is found
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "val")
		&& assignment(udfString))
	{
		// if assignments or statements are found
		if (parse->curToken()
			&& assignOrStmt(udfString))
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
bool RDDToDFGrammar::assignment(std::string &udfString)
{
	// if the val keyword is found
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "val"))
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if an identifier is found
		if (parse->nextToken()
			&& parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER)
		{
			udfString.append(" "); // add a space to the string that's being constructed
			udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
			// if an equal sign is found
			if (parse->nextToken()
				&& parse->curToken()->getSymType() == Token::SYMTYPE_EQUAL)
			{
				udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
				// if an expression is found
				if (parse->nextToken()
					&& expression(udfString))
				{
					// if a semicolon is found
					if (parse->curToken()
						&& parse->curToken()->getSymType() == Token::SYMTYPE_SEMICOLON)
					{
						udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
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
bool RDDToDFGrammar::statement(std::string &udfString)
{
	// if a no paren expression is found
	if (parse->curToken()
		&& (parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
		|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
		&& noParenExpr(udfString))
	{
		return true;
	}
	// else if a left parenthesis is found
	else if (parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS)
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if an expression is found
		if (parse->nextToken()
			&& expression(udfString))
		{
			// if a right paren expression or tuple is found
			if (parenExprOrTuple(udfString))
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
bool RDDToDFGrammar::parenExprOrTuple(std::string &udfString)
{
	// if a right paren expression is found
	if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS)
		&& rightParenExpr(udfString))
	{
		return true;
	}
	// else if a tuple is found
	else if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_COMMA)
		&& tuple(udfString))
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
bool RDDToDFGrammar::tuple(std::string &udfString)
{
	// if a comma is found
	if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_COMMA))
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if an expression is found
		if (parse->nextToken()
			&& expression(udfString))
		{
			// if a tuple0 is found
			if (parse->curToken()
				&& tuple0(udfString))
			{
				// if a right parenthesis is found
				if (parse->curToken()
					&& (parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS))
				{
					udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
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
bool RDDToDFGrammar::tuple0(std::string &udfString)
{
	// if a comma is found
	if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_COMMA))
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if an expression is found
		if (parse->nextToken()
			&& expression(udfString))
		{
			// if a tuple0 is found
			if (tuple0(udfString))
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
bool RDDToDFGrammar::rightParenExpr(std::string &udfString)
{
	// if a right parenthesis is found
	if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS))
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if an op expression is found
		if (parse->nextToken()
			&& opExpr(udfString))
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
bool RDDToDFGrammar::noParenExpr(std::string &udfString)
{
	// *outFile << "<<<<<<874>>>>>>" << parse->curToken()->getTokenName();
	// *outFile << "<<<<<<874 CUR STRING>>>>>>" << udfString << std::endl;
	// if an identifier is found
	if (parse->curToken()
		&& (parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER))
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if a field is found
		if (parse->nextToken()
			&& field(udfString))
		{
			// *outFile << "<<<<<<889>>>>>>" << parse->curToken()->getTokenName();
			// *outFile << "<<<<<<889 CUR STRING>>>>>>" << udfString << std::endl;
			// if an op expression is found
			if (opExpr(udfString))
			{
				return true;
			}
		}
	}
	// else if a number is found
	else if (parse->curToken()
		&& (parse->curToken()->getID() == Token::IDTYPE_NUMBER))
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if an op expression is found
		if (parse->nextToken()
			&& opExpr(udfString))
		{
			return true;
		}
	}
	// else if the if keyword is found
	else if (parse->curToken()
		&& (!strcmp(parse->curToken()->getTokenName().c_str(), "if")))
	{
		udfString.append(" "); // add a space to the string that's being constructed
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		udfString.append(" "); // add a space to the string that's being constructed
		// if a left parenthesis is found
		if (parse->nextToken()
			&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS))
		{
			udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
			// if a bool expression is found
			if (parse->nextToken()
				&& boolExpr(udfString))
			{
				// if a right parenthesis is found
				if (parse->curToken()
					&& (parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS))
				{
					udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
					// if an expression is found
					if (parse->nextToken()
						&& expression(udfString))
					{
						// if the else keyword is found
						if (parse->curToken()
							&& (!strcmp(parse->curToken()->getTokenName().c_str(), "else")))
						{
							udfString.append(" "); // add a space to the string that's being constructed
							udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
							udfString.append(" "); // add a space to the string that's being constructed
							// if an expression is found
							if (parse->nextToken()
								&& expression(udfString))
							{
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
bool RDDToDFGrammar::field(std::string &udfString)
{
	// if a period is found
	if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_PERIOD))
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if an identifier is found
		if (parse->nextToken()
			&& (parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER))
		{
			udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
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
bool RDDToDFGrammar::expression(std::string &udfString)
{
	// if a no paren expression is found
	if (parse->curToken()
		&& (parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
		|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
		&& noParenExpr(udfString))
	{
		return true;
	}
	// else if a left parenthesis is found
	else if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS))
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if an expression is found
		if (parse->nextToken()
			&& expression(udfString))
		{
			// if a right paren expression is found
			if (rightParenExpr(udfString))
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
bool RDDToDFGrammar::opExpr(std::string &udfString)
{
	// if an op is found
	if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_PLUS
		|| parse->curToken()->getSymType() == Token::SYMTYPE_MINUS
		|| parse->curToken()->getSymType() == Token::SYMTYPE_STAR
		|| parse->curToken()->getSymType() == Token::SYMTYPE_PERCENT)
		&& op(udfString))
	{
		// if an expression is found
		if (expression(udfString))
		{
			// if an op expression is found
			if (opExpr(udfString))
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
bool RDDToDFGrammar::boolExpr(std::string &udfString)
{
	// if an expression is found
	if (parse->curToken()
		&& (parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
		|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
		|| parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
		&& expression(udfString))
	{
		// if a comparator is found
		if (comp(udfString))
		{
			// if an expression is found
			if (expression(udfString))
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
bool RDDToDFGrammar::op(std::string &udfString)
{
	// if an op is found
	if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_PLUS
		|| parse->curToken()->getSymType() == Token::SYMTYPE_MINUS
		|| parse->curToken()->getSymType() == Token::SYMTYPE_STAR
		|| parse->curToken()->getSymType() == Token::SYMTYPE_PERCENT))
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
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
bool RDDToDFGrammar::comp(std::string &udfString)
{
	if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_DOUBLE_EQUAL
		|| parse->curToken()->getSymType() == Token::SYMTYPE_LT
		|| parse->curToken()->getSymType() == Token::SYMTYPE_GT
		|| parse->curToken()->getSymType() == Token::SYMTYPE_NT_EQUAL
		|| parse->curToken()->getSymType() == Token::SYMTYPE_LT_EQUAL
		|| parse->curToken()->getSymType() == Token::SYMTYPE_GT_EQUAL))
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
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
