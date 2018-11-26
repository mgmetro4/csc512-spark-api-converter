#include <vector>
#include <cstring>
#include <cstdlib>
#include "Grammar.h"
#include "Token.h"
#include "Util.h"
#include <iostream>

/** see the Google doc for the full grammar definition */

/**
 * This is the entry point of the grammar.
 * Production:
 * <program> --> <context> <dataset> <chainable> <done>
 */
bool Grammar::program()
{
	// if context, dataset, chainable, and done are found
	if (context()
		&& dataset()
		&& chainable()
		&& done())
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
 */
bool Grammar::context()
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
 */
bool Grammar::dataset()
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
 * <dataset0> --> range ( <int> , <int> )
 *              | textFile ( <string> )
 */
bool Grammar::dataset0()
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
					*outFile << parse->curToken()->getTokenName() << " "; // print it and a space to the output file
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
	// else if the textFile keyword is found
	else if (parse->curToken()
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
 * <chainable> --> . <chainable0>
 */
bool Grammar::chainable()
{
	// if a period is found
	if (parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_PERIOD)
	{
		*outFile << parse->curToken()->getTokenName(); // print it to the output file
		parse->nextToken();
		// if chainable0 is found
		if (chainable0())
		{
			return true;
		}
	}
	return false;
}

/**
 * Production:
 * <chainable0> --> <map> <chainable>
 *                | <filter> <chainable>
 *                | <sort> <chainable>
 *                | EPSILON
 */
bool Grammar::chainable0()
{
	// TODO fully implement
	// current state:
	// <chainable0> --> map ( UDF )
	// This should be split off into a separate map function.  Just did this for testing.
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
			// if a string is found
			if (parse->nextToken()
				&& !strcmp(parse->curToken()->getTokenName().c_str(), "UDF"))
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
 * <done> --> . <done0>
 */
bool Grammar::done()
{
	// if a period is found
	if (parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_PERIOD)
	{
		*outFile << parse->curToken()->getTokenName(); // print it to the output file
		parse->nextToken();
		// if done0 is found
		if (done0())
		{
			return true;
		}
	}
	return false;
}

/**
 * Production:
 * <done0> --> <reduce>
 *           | <collect>
 */
bool Grammar::done0()
{
	// TODO fully implement
	// current state:
	// <done0> --> reduce ( UDF )
	// This should be split off into a separate reduce function.  Just did this for testing.
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "reduce"))
	{
		*outFile << parse->curToken()->getTokenName(); // print it to the output file
		// if a left parenthesis is found
		if (parse->nextToken()
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS)
		{
			*outFile << parse->curToken()->getTokenName(); // print it to the output file
			// if a string is found
			if (parse->nextToken()
				&& !strcmp(parse->curToken()->getTokenName().c_str(), "UDF"))
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

void Grammar::instantiateParser(Parser* newParser)
{
	parse = newParser;
}
