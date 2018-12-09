#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "Scanner.h"
#include "Parser.h"
#include "Util.h"
#include "Grammar.h"
#include "RDDToDSGrammar.h"
#include "RDDToDFGrammar.h"

/**
 * Creates a string representing an output file name based on the input file name;
 *
 * @param input the input file name as a char array
 * @param output the string mem loc in which to place the output file name
 */
void genOutFileName(char *input, std::string &output)
{
	bool printed = false;
	while (*input != '\0')
	{
		if (*input == '.' && !printed)
		{
			output.push_back('_');
			output.push_back('g');
			output.push_back('e');
			output.push_back('n');
			output.push_back('.');
			printed = true;
			/*std::cout << " I [" << input << "] output [" << output << "]\n";*/
		}
		else
		{
			output.push_back(*input);
			/*std::cout << " I [" << input << "] output [" << output << "]\n";*/
		}
		input++;
	}
}

/**
 *
 *
 * @param argc the number of arguments given on the command line (includes this program)
 * @param argv the array of command line arguments (includes this program's name)
 */
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Invalid number of arguments." << std::endl << "Usage:\t./Transformer -ds|-df <InputFileName>" << std::endl;
		return EXIT_FAILURE;
	}

	if (strcmp(argv[1], "-ds") && strcmp(argv[1], "-df"))
	{
		std::cout << "Invalid conversion option." << std::endl << "You must specify if you want to convert to dataset (-ds) or dataframe (-df)." << std::endl;
		return EXIT_FAILURE;
	}

	// initialize a scanner for the input file
	Scanner scan(argv[2]); // object memory is on stack

	// create stream to output file
	std::string outFileName;
	genOutFileName(argv[2], outFileName);
	std::ofstream outFile(outFileName.c_str(), std::ifstream::out);

	Grammar *gram = NULL;
	if (!strcmp(argv[1], "-ds"))
	{
		gram = new RDDToDSGrammar(&outFile); // object memory is on heap
	}
	else
	{
		gram = new RDDToDFGrammar(&outFile); // object memory is on heap
	}

	Parser parse(gram); // object memory is on stack

	// scan through the file while loading tokens into the parser
	while (scan.hasNextToken()) // while there are more tokens
	{
		Token *t = scan.getNextToken();
		if (t->getID() == Token::IDTYPE_INVALID)
		{
			std::cout << "Error while scanning" << std::endl;
			std::cout << *t << std::endl;
			delete t;
			return EXIT_FAILURE;
		}
		// print out the token
		std::cout << *t << std::endl;

		// load the token into the parser's token list
		parse.addToken(t);
	}

	// parse the program based on the tokens loaded in the parser
	if (!parse.parseProgram()) // if the program cannot be parsed
	{
		std::cout << "Error while parsing" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << std::endl << "------------------------------------------------------------------" << std::endl;
	std::cout << "The input file \"" << argv[1] << "\" was successfully transformed." << std::endl;
	std::cout << "The output file is \"" << outFileName << "\"." << std::endl;
}


/**
 * Operator overloading for << between std::ofstream and Function;
 * Defines the interaction between an ofstream and Function using the << operator;
 * This basically describes how to print a Function to an ofstream when used with
 * the << operator.
 *
 * Ex:
 * ofstream << Function
 */
// std::ofstream& operator<<(std::ofstream& outFile, Function& printFunc)
// {
// 	outFile << std::endl << printFunc.datatype << " " << printFunc.name << " (";
//
// 	if (printFunc.arguments.size() == 0)
// 	{
// 		outFile << "void";
// 	}
// 	else
// 	{
// 		for (std::vector< std::pair<std::string, Variable *> >::iterator it = printFunc.arguments.begin(); it != printFunc.arguments.end();)
// 		{
// 			outFile << it->second->datatype << " " << it->second->name;
// 			if (++it != printFunc.arguments.end())
// 			{
// 				outFile << ", ";
// 			}
// 		}
// 	}
// 	outFile << ")";
//
// 	if (printFunc.isDeclaration)
// 	{
// 		outFile << ";" << std::endl;
// 	}
// 	else
// 	{
// 		outFile << std::endl << "{" << std::endl;
// 		if (printFunc.localVarCnt != 0)
// 		{
// 			outFile << "\t" << "int local[" << printFunc.localVarCnt << "];" << std::endl;
// 		}
//
// 		while (!printFunc.funcStats.empty())
// 		{
// 			outFile << "\t" << printFunc.funcStats.front() << std::endl;
// 			printFunc.funcStats.pop();
// 		}
// 		outFile << "}" << std::endl;
// 	}
// 	return outFile;
// }

/**
 * Operator overloading for << between std::ostream and Token;
 * Defines the interaction between an ostream and Token using the << operator;
 * This basically describes how to print a Token to an ostream when used with
 * the << operator.
 *
 * Ex:
 * ostream << Token
 */
std::ostream& operator<<(std::ostream& stream, Token& token)
{
	stream << token.getID() << " : " << token.getTokenName();
	return stream;
}
