#include <algorithm>
#include <string>
#include <sstream>
#include "Util.h"
#include "Token.h"

std::pair<bool, int> Util::isSymbol(std::string::iterator pos)
{
	switch (*pos)
	{
	case '(':
		return std::pair<bool, int>(true, Token::SYMTYPE_LEFT_PARENTHESIS);
	case ')':
		return std::pair<bool, int>(true, Token::SYMTYPE_RIGHT_PARENTHESIS);
	case '{':
		return std::pair<bool, int>(true, Token::SYMTYPE_LEFT_BRACE);
	case '}':
		return std::pair<bool, int>(true, Token::SYMTYPE_RIGHT_BRACE);
	case '[':
		return std::pair<bool, int>(true, Token::SYMTYPE_LEFT_BRACKET);
	case ']':
		return std::pair<bool, int>(true, Token::SYMTYPE_RIGHT_BRACKET);
	case '.':
		return std::pair<bool, int>(true, Token::SYMTYPE_PERIOD);
	case ',':
		return std::pair<bool, int>(true, Token::SYMTYPE_COMMA);
	case ';':
		return std::pair<bool, int>(true, Token::SYMTYPE_SEMICOLON);
	case '+':
		return std::pair<bool, int>(true, Token::SYMTYPE_PLUS);
	case '-':
		return std::pair<bool, int>(true, Token::SYMTYPE_MINUS);
	case '*':
		return std::pair<bool, int>(true, Token::SYMTYPE_STAR);
	case '/':
		return std::pair<bool, int>(true, Token::SYMTYPE_FORWARD_SLASH);
	case '%':
		return std::pair<bool, int>(true, Token::SYMTYPE_PERCENT);
	case '#':				/**
							 * # is treated as symbol and later a call to isMetaChar
							 * will differentiate it from the actual set of symbols
							 */
		return std::pair<bool, int>(true, Token::SYMTYPE_INVALID);
	case '>':
		return std::pair<bool, int>(true, Token::SYMTYPE_GT);
	case '<':
		return std::pair<bool, int>(true, Token::SYMTYPE_LT);
	case '=':
		return std::pair<bool, int>(true, Token::SYMTYPE_EQUAL);
	default:
		return std::pair<bool, int>(false, Token::SYMTYPE_INVALID);
	}
}

std::pair<bool, int> Util::isDoubleSymbol(std::string::iterator pos)
{
	switch (*pos)
	{
	case '!':
		if (*(pos + 1) != '=')
		{
			return std::pair<bool, int>(false, Token::SYMTYPE_INVALID);
		}
		return std::pair<bool, int>(true, Token::SYMTYPE_NT_EQUAL);
	case '&':
		if (*(pos + 1) != '&')
		{
			return std::pair<bool, int>(false, Token::SYMTYPE_INVALID);
		}
		return std::pair<bool, int>(true, Token::SYMTYPE_DOUBLE_AND);
	case '|':
		if (*(pos + 1) != '|')
		{
			return std::pair<bool, int>(false, Token::SYMTYPE_INVALID);
		}
		return std::pair<bool, int>(true, Token::SYMTYPE_DOUBLE_OR);
	case '=':
		if (*(pos + 1) == '=')
		{
			return std::pair<bool, int>(true, Token::SYMTYPE_DOUBLE_EQUAL);
		}
		else if (*(pos + 1) == '>')
		{
			return std::pair<bool, int>(true, Token::SYMTYPE_ARROW);
		}
		return std::pair<bool, int>(false, Token::SYMTYPE_INVALID);
	case '<':
		if (*(pos + 1) != '=')
		{
			return std::pair<bool, int>(false, Token::SYMTYPE_INVALID);
		}
		return std::pair<bool, int>(true, Token::SYMTYPE_LT_EQUAL);
	case '>':
		if (*(pos + 1) != '=')
		{
			return std::pair<bool, int>(false, Token::SYMTYPE_INVALID);
		}
		return std::pair<bool, int>(true, Token::SYMTYPE_GT_EQUAL);
	default:
		return std::pair<bool, int>(false, Token::SYMTYPE_INVALID);
	}

}

bool Util::isLetter(std::string::iterator pos)
{
	if ((*pos >= 'a' && *pos <= 'z')
		|| (*pos >= 'A' && *pos <= 'Z')
		|| (*pos == '_'))
	{
		return true;
	}
	return false;
}

bool Util::isDigit(std::string::iterator pos)
{
	if (*pos >= '0' && *pos <= '9')
	{
		return true;
	}
	return false;
}

bool Util::isReservedWord(std::string curString)
{
	if (!curString.compare("sc")
		|| !curString.compare("range")
		|| !curString.compare("textFile")
		|| !curString.compare("map")
		|| !curString.compare("filter")
		|| !curString.compare("reduce")
		|| !curString.compare("reduceByKey")
		|| !curString.compare("sortBy")
		|| !curString.compare("collect")
		|| !curString.compare("if")
		|| !curString.compare("else")
		|| !curString.compare("val"))
	{
		return true;
	}

	return false;
}

bool Util::isMetaChar(std::string::iterator pos)
{
	/*std::cout << "Checking for meta\n";*/
	if (*pos == '/' && *(pos + 1) == '/')
	{
		/*std::cout << "It is double //\n";*/
		return true;
	}
	else if (*pos == '#')
	{
		return true;
	}
	return false;
}

std::string Util::to_string(const int& num)
{
	std::ostringstream outString;
        outString << num;
        return outString.str();
}
