#include <vector>
#include <cstring>
#include <cstdlib>
#include "RDDToDSGrammar.h"
#include "Token.h"
#include "Util.h"
#include <iostream>

/** see the Google doc for the full grammar definition */

/**
 * This is the entry point of the grammar.
 * Production:
 * <program> --> <context> <dataset> <chainordone>
 *               [FIRST_PLUS = { sc }]
 *             | EOF
 *               [FIRST_PLUS = { EOF }]
 */
bool RDDToDSGrammar::program()
{
	// if context, dataset, chainable, and done are found
	if (context()
		&& dataset()
		&& chainordone())
	{
		// make sure that all tokens have been used
		if (!parse->curToken()) // if there is no current token
		{
			return true;
		}
	}
	return false;
}

/**
 * Production:
 * <context> --> sc
 *               [FIRST_PLUS = { sc }]
 *
 * This transforms "sc" to "spark" in the output file.
 */
bool RDDToDSGrammar::context()
{
	// if sc is found
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "sc"))
	{
		*outFile << "spark"; // print the transformation to the output file
		parse->nextToken();
		return true;
	}
	return false;
}

/**
 * Production:
 * <dataset> --> . <dataset0>
 *               [FIRST_PLUS = { . }]
 */
bool RDDToDSGrammar::dataset()
{
	// if a period is found
	if (parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_PERIOD)
	{
		*outFile << parse->curToken()->getTokenName(); // print it to the output file
		parse->nextToken();
		// if dataset0 is found
		if (dataset0())
		{
			return true;
		}
	}
	return false;
}

/**
 * Production:
 * <dataset0> --> <range>
 *                [FIRST_PLUS = { range }]
 *              | <textFile>
 *                [FIRST_PLUS = { textFile }]
 */
bool RDDToDSGrammar::dataset0()
{
	// if the range keyword is found
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "range"))
	{
		if (range())
		{
			return true;
		}
	}
	// else if the textFile keyword is found
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "textFile"))
	{
		if (textFile())
		{
			return true;
		}
	}
	return false;
}

/**
 * Production:
 * <range> --> range ( <number> , <number> )
 *             [FIRST_PLUS = { range }]
 */
bool RDDToDSGrammar::range()
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
							*outFile << parse->curToken()->getTokenName() << std::endl; // print it and a newline to the output file
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
 * <textFile> --> textFile ( <string> )
 *                [FIRST_PLUS = { textFile }]
 *
 * This transforms "textFile" to "read.textFile" in the output file.
 */
bool RDDToDSGrammar::textFile()
{
	// if the textFile keyword is found
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "textFile"))
	{
		*outFile << "read.textFile"; // print the transformation to the output file
		// if a left parenthesis is found
		if (parse->nextToken()
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS)
		{
			*outFile << parse->curToken()->getTokenName(); // print it to the output file
			// if a string is found
			if (parse->nextToken()
				&& parse->curToken()->getID() == Token::IDTYPE_STRING)
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
	}
	return false;
}

/**
 * Production:
 * <chainordone> --> . <chainordone0>
 *                   [FIRST_PLUS = { . }]
 */
bool RDDToDSGrammar::chainordone()
{
	// if a period is found
	if (parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_PERIOD)
	{
		*outFile << parse->curToken()->getTokenName(); // print it to the output file
		parse->nextToken();
		// if chainordone0 is found
		if (chainordone0())
		{
			return true;
		}
	}
	return false;
}

/**
 * Production:
 * <chainordone0> --> <chainable>
 *                    [FIRST_PLUS = { map, filter, sortBy }]
 *                  | <done>
 *                    [FIRST_PLUS = { reduce, reduceByKey, collect }]
 */
bool RDDToDSGrammar::chainordone0()
{
	// if the map, filter, or sortBy keywords are found
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& (!strcmp(parse->curToken()->getTokenName().c_str(), "map")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "filter")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "sortBy")))
	{
		if (chainable())
		{
			return true;
		}
	}
	// else if the reduce, reduceByKey, or collect keywords are found
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& (!strcmp(parse->curToken()->getTokenName().c_str(), "reduce")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "reduceByKey")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "collect")))
	{
		if (done())
		{
			return true;
		}
	}
	return false;
}

/**
 * Production:
 * <chainable> --> <map> <chainordone>
 *                 [FIRST_PLUS = { map }]
 *               | <filter> <chainordone>
 *                 [FIRST_PLUS = { filter }]
 *               | <sort> <chainordone>
 *                 [FIRST_PLUS = { sortBy }]
 */
bool RDDToDSGrammar::chainable()
{
	// if the map keyword is found
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "map"))
	{
		if (map()
			&& chainordone())
		{
			return true;
		}
	}
	// else if the filter keyword is found
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "filter"))
	{
		if (filter()
			&& chainordone())
		{
			return true;
		}
	}
	// else if the sortBy keyword is found
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "sortBy"))
	{
		if (sort()
			&& chainordone())
		{
			return true;
		}
	}
	return false;
}

/**
 * Production:
 * <map> --> map ( <mapUDF> )
 *           [FIRST_PLUS = { map }]
 */
bool RDDToDSGrammar::map()
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
			// if a mapUDF is found
			if (parse->nextToken()
				&& mapUDF(userFunction))
			{
				*outFile << userFunction; // print it to the output file
				// if a right parenthesis is found
				if (parse->curToken()
					&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS)
				{
					*outFile << parse->curToken()->getTokenName() << std::endl; // print it and a newline to the output file
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
 * <filter> --> filter ( <filterUDF> )
 *              [FIRST_PLUS = { filter }]
 */
bool RDDToDSGrammar::filter()
{
	// if the filter keyword is found
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "filter"))
	{
		*outFile << parse->curToken()->getTokenName(); // print it to the output file
		// if a left parenthesis is found
		if (parse->nextToken()
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS)
		{
			*outFile << parse->curToken()->getTokenName(); // print it to the output file
			std::string userFunction;
			// if a filter UDF is found
			if (parse->nextToken()
				&& filterUDF(userFunction))
			{
				*outFile << userFunction; // print it to the output file
				// if a right parenthesis is found
				if (parse->curToken()
					&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS)
				{
					*outFile << parse->curToken()->getTokenName() << std::endl; // print it and a newline to the output file
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
 * <sort> --> sortBy ( <sortByUDF> )
 *           [FIRST_PLUS = { sortBy }]
 *
 * This transforms "sortBy(<func>)" to "map(row=>((<func>)(row), row)).orderBy("_1").map(_._2)" in the output file.
 */
bool RDDToDSGrammar::sort()
{
	// if the sortBy keyword is found
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "sortBy"))
	{
		// if a left parenthesis is found
		if (parse->nextToken()
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS)
		{
			std::string userFunction;
			// if a sortBy UDF is found
			if (parse->nextToken()
				&& sortByUDF(userFunction))
			{
				// if a right parenthesis is found
				if (parse->curToken()
					&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS)
				{
					*outFile << "map(row=>((" << userFunction << ")(row), row)).orderBy(\"_1\").map(_._2)" << std::endl; // print the transformation to the output file
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
 * <done> --> <reduce>
 *            [FIRST_PLUS = { reduce, reduceByKey }]
 *          | <collect>
 *            [FIRST_PLUS = { collect }]
 */
bool RDDToDSGrammar::done()
{
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& (!strcmp(parse->curToken()->getTokenName().c_str(), "reduce")
	  || !strcmp(parse->curToken()->getTokenName().c_str(), "reduceByKey")))
	{
		if (reduce())
		{
			return true;
		}
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "collect"))
	{
		if (collect())
		{
			return true;
		}
	}
	return false;
}

/**
 * Production:
 * <reduce> --> reduce ( <reduceUDF> )
 *              [FIRST_PLUS = { reduce }]
 *            | reduceByKey ( <reduceUDF> )
 *              [FIRST_PLUS = { reduceByKey }]
 * This transforms "reduce(<func>)" to "select(reduceAggregator(<func>)).collect()" in the output file.
 * This transforms "reduceByKey(<func>)" to "groupByKey(_._1).agg(reduceByKeyAggregator(<func>))" in the output file.
 */
bool RDDToDSGrammar::reduce()
{
	// if the reduce keyword is found
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "reduce"))
	{
		// if a left parenthesis is found
		if (parse->nextToken()
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS)
		{
			std::string userFunction;
			// if a reduce UDF is found
			if (parse->nextToken()
				&& reduceUDF(userFunction))
			{
				// if a right parenthesis is found
				if (parse->curToken()
					&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS)
				{
					*outFile << "select(reduceAggregator(" << userFunction << ")).collect()" << std::endl; // print the transformation to the output file
					parse->nextToken();
					return true;
				}
			}
		}
	}
	// else if the reduceByKey keyword is found
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "reduceByKey"))
	{
		// if a left parenthesis is found
		if (parse->nextToken()
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS)
		{
			std::string userFunction;
			// if a reduce UDF is found
			if (parse->nextToken()
				&& reduceUDF(userFunction))
			{
				// if a right parenthesis is found
				if (parse->curToken()
					&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS)
				{
					*outFile << "groupByKey(_._1).agg(reduceByKeyAggregator(" << userFunction << "))" << std::endl; // print the transformation to the output file
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
 * <collect> --> collect ( )
 *              [FIRST_PLUS = { collect }]
 */
bool RDDToDSGrammar::collect()
{
	if (parse->curToken()
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
 * <mapUDF> --> <id> => <exprTupleBlock>
 *              [FIRST_PLUS = { <id> }]
 */
bool RDDToDSGrammar::mapUDF(std::string &udfString)
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
			// if an expression tuple block is found
			if (parse->nextToken()
				&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
				|| parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS
				|| parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
				|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
				|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
				&& exprTupleBlock(udfString))
			{
				return true;
			}
		}
	}
	*outFile << std::endl << "<<<<<<ERROR>>>>>>" << std::endl << udfString << std::endl;
  return false;
}

/**
 * Production:
 * <exprTupleBlock> --> { <assignThenExprTuple> }
 *                      [FIRST_PLUS = { { }]
 *                    | <exprTuple>
 *                      [FIRST_PLUS = { <id>, <number>, (, if }]
 */
bool RDDToDSGrammar::exprTupleBlock(std::string &udfString)
{
	// if a left curly brace is found
	if (parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if assignments or statements are found
		if (parse->nextToken()
			&& assignThenExprTuple(udfString))
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
	// else if an expression or tuple is found
	else if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS
		|| parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
		|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
		&& exprTuple(udfString))
	{
		return true;
	}
	return false;
}

/**
 * Production:
 * <assignThenExprTuple> --> <assignment> <assignThenExprTuple>
 *                           [FIRST_PLUS = { val }]
 *                         | <exprTuple>
 *                           [FIRST_PLUS = { <id>, <number>, (, if }]
 */
bool RDDToDSGrammar::assignThenExprTuple(std::string &udfString)
{
	// *outFile << "<<<<<<622>>>>>>" << parse->curToken()->getTokenName();
	// *outFile << "<<<<<<622 CUR STRING>>>>>>" << udfString << std::endl;
	// if an assignment is found
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "val")
		&& assignment(udfString))
	{
		// if assignments or expression or tuple are found
		if (parse->curToken()
			&& assignThenExprTuple(udfString))
		{
			return true;
		}
	}
	// else if an expression or tuple is found
	else if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS
		|| parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
		|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
		&& exprTuple(udfString))
	{
		// *outFile << "<<<<<<639>>>>>>" << parse->curToken()->getTokenName();
		// *outFile << "<<<<<<639 CUR STRING>>>>>>" << udfString << std::endl;
		return true;
	}
	return false;
}

/**
 * Production:
 * <assignment> --> val <identifier> = <expression> ;
 *                  [FIRST_PLUS = { val }]
 */
bool RDDToDSGrammar::assignment(std::string &udfString)
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
 * <exprTuple> --> <noParenExpr>
 *                 [FIRST_PLUS = { <identifier>, <number>, if }]
 *               | ( <expression> <parenExprOrTuple>
 *                 [FIRST_PLUS = { ( }]
 */
bool RDDToDSGrammar::exprTuple(std::string &udfString)
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
bool RDDToDSGrammar::parenExprOrTuple(std::string &udfString)
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
bool RDDToDSGrammar::tuple(std::string &udfString)
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
bool RDDToDSGrammar::tuple0(std::string &udfString)
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
 * <filterUDF> --> <id> => <boolExprBlock>
 *                 [FIRST_PLUS = { <id> }]
 */
bool RDDToDSGrammar::filterUDF(std::string &udfString)
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
			// if a boolean expression block is found
			if (parse->nextToken()
				&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
				|| parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS
				|| parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
				|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
				|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
				&& boolExprBlock(udfString))
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
 * <boolExprBlock> --> { <assignThenBoolExpr> }
 *                     [FIRST_PLUS = { { }]
 *                   | <boolExpr>
 *                     [FIRST_PLUS = { <id>, <number>, (, if }]
 */
bool RDDToDSGrammar::boolExprBlock(std::string &udfString)
{
	// if a left curly brace is found
	if (parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if assignments or a boolean expression are found
		if (parse->nextToken()
			&& assignThenBoolExpr(udfString))
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
	// else if a boolean expression is found
	else if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS
		|| parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
		|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
		&& boolExpr(udfString))
	{
		return true;
	}
	return false;
}

/**
 * Production:
 * <assignThenBoolExpr> --> <assignment> <assignThenBoolExpr>
 *                         [FIRST_PLUS = { val }]
 *                       | <boolExpr>
 *                         [FIRST_PLUS = { <id>, <number>, (, if }]
 */
bool RDDToDSGrammar::assignThenBoolExpr(std::string &udfString)
{
	// *outFile << "<<<<<<622>>>>>>" << parse->curToken()->getTokenName();
	// *outFile << "<<<<<<622 CUR STRING>>>>>>" << udfString << std::endl;
	// if an assignment is found
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "val")
		&& assignment(udfString))
	{
		// if assignments or a boolean expression are found
		if (parse->curToken()
			&& assignThenBoolExpr(udfString))
		{
			return true;
		}
	}
	// else if a boolean expression is found
	else if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS
		|| parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
		|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
		&& boolExpr(udfString))
	{
		// *outFile << "<<<<<<639>>>>>>" << parse->curToken()->getTokenName();
		// *outFile << "<<<<<<639 CUR STRING>>>>>>" << udfString << std::endl;
		return true;
	}
	return false;
}

/**
 * Production:
 * <sortByUDF> --> ( <id> : <type> ) => <exprBlock>
 *                 [FIRST_PLUS = { ( }]
 */
bool RDDToDSGrammar::sortByUDF(std::string &udfString)
{
	// if a left parenthesis is found
	if (parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS)
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if an identifier is found
		if (parse->nextToken()
			&& parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER)
		{
			udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
			// if a colon is found
			if (parse->nextToken()
				&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
			{
				udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
				// if a type is found
				if (parse->nextToken()
					&& type(udfString))
				{
					// if a right parenthesis is found
					if (parse->curToken()
						&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS)
					{
						udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
						// if an arrow is found
						if (parse->nextToken()
							&& parse->curToken()->getSymType() == Token::SYMTYPE_ARROW)
						{
							udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
							// if an expression block is found
							if (parse->nextToken()
								&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
								|| parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS
								|| parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
								|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
								|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
								&& exprBlock(udfString))
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
	*outFile << std::endl << "<<<<<<ERROR>>>>>>" << udfString << std::endl;
  return false;
}

/**
 * Production:
 * <type> --> Long
 *            [FIRST_PLUS = { Long }]
 *          | Int
 *            [FIRST_PLUS = { Int }]
 */
bool RDDToDSGrammar::type(std::string &udfString)
{
	// if Long or Int are found
	if (parse->curToken()
		&& (!strcmp(parse->curToken()->getTokenName().c_str(), "Long")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "Int")))
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		parse->nextToken();
		return true;
	}
	return false;
}

/**
 * Production:
 * <exprBlock> --> { <assignThenExpr> }
 *                 [FIRST_PLUS = { { }]
 *               | <expression>
 *                 [FIRST_PLUS = { <id>, <number>, (, if }]
 */
bool RDDToDSGrammar::exprBlock(std::string &udfString)
{
	// if a left curly brace is found
	if (parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if assignments or an expression are found
		if (parse->nextToken()
			&& assignThenExpr(udfString))
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
	// else if an expression is found
	else if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS
		|| parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
		|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
		&& expression(udfString))
	{
		return true;
	}
	return false;
}

/**
 * Production:
 * <assignThenExpr> --> <assignment> <assignThenExpr>
 *                      [FIRST_PLUS = { val }]
 *                    | <expression>
 *                      [FIRST_PLUS = { <id>, <number>, (, if }]
 */
bool RDDToDSGrammar::assignThenExpr(std::string &udfString)
{
	// *outFile << "<<<<<<622>>>>>>" << parse->curToken()->getTokenName();
	// *outFile << "<<<<<<622 CUR STRING>>>>>>" << udfString << std::endl;
	// if an assignment is found
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "val")
		&& assignment(udfString))
	{
		// if assignments or an expression are found
		if (parse->curToken()
			&& assignThenExpr(udfString))
		{
			return true;
		}
	}
	// else if an expression is found
	else if (parse->curToken()
		&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS
		|| parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
		|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
		&& expression(udfString))
	{
		// *outFile << "<<<<<<639>>>>>>" << parse->curToken()->getTokenName();
		// *outFile << "<<<<<<639 CUR STRING>>>>>>" << udfString << std::endl;
		return true;
	}
	return false;
}

/**
 * Production:
 * <reduceUDF> --> ( <id> : <type> , <id> : <type> ) => <exprBlock>
 *                 [FIRST_PLUS = { ( }]
 */
bool RDDToDSGrammar::reduceUDF(std::string &udfString)
{
	// if a left parenthesis is found
	if (parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS)
	{
		udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
		// if an identifier is found
		if (parse->nextToken()
			&& parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER)
		{
			udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
			// if a colon is found
			if (parse->nextToken()
				&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
			{
				udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
				// if a type is found
				if (parse->nextToken()
					&& type(udfString))
				{
					// if a comma is found
					if (parse->curToken()
						&& parse->curToken()->getSymType() == Token::SYMTYPE_COMMA)
					{
						udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
						// if an identifier is found
						if (parse->nextToken()
							&& parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER)
						{
							udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
							// if a colon is found
							if (parse->nextToken()
								&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
							{
								udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
								// if a type is found
								if (parse->nextToken()
									&& type(udfString))
								{
									// if a right parenthesis is found
									if (parse->curToken()
										&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_PARENTHESIS)
									{
										udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
										// if an arrow is found
										if (parse->nextToken()
											&& parse->curToken()->getSymType() == Token::SYMTYPE_ARROW)
										{
											udfString.append(parse->curToken()->getTokenName()); // add it to the string that's being constructed
											// if an expression block is found
											if (parse->nextToken()
												&& (parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
												|| parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS
												|| parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
												|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
												|| !strcmp(parse->curToken()->getTokenName().c_str(), "if"))
												&& exprBlock(udfString))
											{
												return true;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	*outFile << std::endl << "<<<<<<ERROR>>>>>>" << udfString << std::endl;
  return false;
}

/**
 * Production:
 * <rightParenExpr> --> ) <opExpr>
 *                      [FIRST_PLUS = { ) }]
 */
bool RDDToDSGrammar::rightParenExpr(std::string &udfString)
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
bool RDDToDSGrammar::noParenExpr(std::string &udfString)
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
bool RDDToDSGrammar::field(std::string &udfString)
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
bool RDDToDSGrammar::expression(std::string &udfString)
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
bool RDDToDSGrammar::opExpr(std::string &udfString)
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
bool RDDToDSGrammar::boolExpr(std::string &udfString)
{
	// if an expression is found
	if (parse->curToken()
		&& (parse->curToken()->getID() == Token::IDTYPE_IDENTIFIER
		|| parse->curToken()->getID() == Token::IDTYPE_NUMBER
		|| parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS
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
bool RDDToDSGrammar::op(std::string &udfString)
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
bool RDDToDSGrammar::comp(std::string &udfString)
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

void RDDToDSGrammar::evaluateASTTree(ASTNode *root)
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
