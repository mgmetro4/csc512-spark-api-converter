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
 * <program> --> <context> <dataset> <chainordone>
 *               [FIRST_PLUS = { sc }]
 *             | EOF
 *               [FIRST_PLUS = { EOF }]
 */
bool Grammar::program()
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
 *               [FIRST_PLUS = { . }]
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
 * <dataset0> --> <range>
 *                [FIRST_PLUS = { range }]
 *              | <textFile>
 *                [FIRST_PLUS = { textFile }]
 */
bool Grammar::dataset0()
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
bool Grammar::range()
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
bool Grammar::textFile()
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
bool Grammar::chainordone()
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
bool Grammar::chainordone0()
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
bool Grammar::chainable()
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
 * <map> --> map ( <UDF> )
 *           [FIRST_PLUS = { map }]
 */
bool Grammar::map()
{
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
 * <filter> --> filter ( <UDF> )
 *              [FIRST_PLUS = { filter }]
 */
bool Grammar::filter()
{
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
 * <sort> --> sortBy ( <UDF> )
 *           [FIRST_PLUS = { sortBy }]
 *
 * This transforms "sortBy(<func>)" to "map(row=>((<func>)(row), row)).orderBy("_1").map(_._2)" in the output file.
 */
bool Grammar::sort()
{
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_RESERVEDWORD
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "sortBy"))
	{
		// if a left parenthesis is found
		if (parse->nextToken()
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_PARENTHESIS)
		{
			std::string userFunction;
			// if a UDF is found
			if (parse->nextToken()
				&& UDF(userFunction))
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
bool Grammar::done()
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
 * <reduce> --> reduce ( <UDF> )
 *              [FIRST_PLUS = { reduce }]
 *            | reduceByKey ( <UDF> )
 *              [FIRST_PLUS = { reduceByKey }]
 * This transforms "reduce(<func>)" to "select(reduceAggregator(<func>)).collect()" in the output file.
 * This transforms "reduceByKey(<func>)" to "groupByKey(_._1).agg(reduceByKeyAggregator(<func>))" in the output file.
 */
bool Grammar::reduce()
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
			// if a UDF is found
			if (parse->nextToken()
				&& UDF(userFunction))
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
			// if a UDF is found
			if (parse->nextToken()
				&& UDF(userFunction))
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
bool Grammar::collect()
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
 * <UDF> --> <identifier> => <Expression>
 */
bool Grammar::UDF(std::string &datatype)
{
	// TODO implement the real production

	// if a string is found
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "UDF"))
	{
		datatype.append(parse->curToken()->getTokenName());
		parse->nextToken();
		return true;
	}
  return false;
}



void Grammar::instantiateParser(Parser* newParser)
{
	parse = newParser;
}
