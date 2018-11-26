#include <vector>
#include "Parser.h"
#include "Util.h"
/*#include <iostream>*/

// move the parser so it's looking at the next token in its list and return it
Token* Parser::nextToken()
{
	// increment the iterator so it looks at the next token in the list
	++tokenIT;

	// run through all whitespace and metastatements while printing metastatements
	while ((tokenIT != tokenVector->end())
		&& ((*tokenIT)->getID() == Token::IDTYPE_TABSPACE
		|| (*tokenIT)->getID() == Token::IDTYPE_METASTATEMENT
		|| (*tokenIT)->getID() == Token::IDTYPE_EMPTY)) // while there are more tokens AND the next token is whitespace, a metastatement, or empty
	{
		if ((*tokenIT)->getID() == Token::IDTYPE_METASTATEMENT) // if the token is a metastatment
		{
			// print the metastatement to the output file
			(*myGrammar->outFile) << (*tokenIT)->getTokenName() << std::endl;
		}
		++tokenIT;
	}

	if (tokenIT == tokenVector->end()) // if there is no next token
	{
		return NULL;
	}

	/*std::cout << "Token : " << (*tokenIT)->getTokenName() << std::endl;*/
	return *tokenIT;
}

// get the current token that the parser is looking at
Token* Parser::curToken()
{
	if (tokenIT == tokenVector->end()) // if there is no next token
	{
		return NULL;
	}

	return *tokenIT;
}

// load the given token into the list of tokens that the parser will use
void Parser::addToken(Token *newToken)
{
	tokenVector->push_back(newToken);
}

// run through the loaded tokens, matching them according to the grammar
bool Parser::parseProgram()
{
	tokenIT = tokenVector->begin();

	// run through all initial whitespace and metastatements while printing metastatements
	while ((tokenIT != tokenVector->end())
		&& ((*tokenIT)->getID() == Token::IDTYPE_TABSPACE
		|| (*tokenIT)->getID() == Token::IDTYPE_METASTATEMENT
		|| (*tokenIT)->getID() == Token::IDTYPE_EMPTY)) // while there are more tokens AND the next token is whitespace, a metastatement, or empty
	{
		if ((*tokenIT)->getID() == Token::IDTYPE_METASTATEMENT) // if the token is a metastatment
		{
			// print the metastatement to the output file
			(*myGrammar->outFile) << (*tokenIT)->getTokenName() << std::endl;
		}
		++tokenIT;
	}

	// parse the rest of the program based on the grammar
	if (tokenIT != tokenVector->end()) // if there are more tokens
	{
		bool temp = myGrammar->program();
		// flush all buffered data to the output file
		(*myGrammar->outFile) << std::flush;
		/*std::cout << std::endl;
		while (tokenIT != tokenVector->end())
		{
			std::cout << (*tokenIT)->getTokenName();
			++tokenIT;
		}*/
		return temp;
	}

	return true;
}

void Parser::incrementVariableCount()
{
		numVar++;
}

void Parser::decrementVariableCount()
{
		numVar--;
}

void Parser::incrementFunctionCount()
{
	numFunc++;
}

void Parser::incrementStmtCount()
{
	numStmt++;
}

int Parser::getNumVar()
{
	return numVar;
}

int Parser::getNumFunc()
{
	return numFunc;
}

int Parser::getNumStmt()
{
	return numStmt;
}

Parser::Parser(Grammar *newGrammar)
				: myGrammar(newGrammar)
				, numVar(0)
				, numFunc(0)
				, numStmt(0)
				, isDeclaration(false)
				, tokenVector(new std::vector<Token*>())
{
	newGrammar->instantiateParser(this);
}

Parser::~Parser()
{
	for (std::vector<Token *>::iterator it = tokenVector->begin(); it != tokenVector->end(); ++it)
	{
		delete *it;
	}

	delete tokenVector;
}
