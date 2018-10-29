#ifndef _UTIL_H
#define _UTIL_H

#include <string>
#include <queue>
#include <fstream>

class Util
{
public:
	static std::pair<bool, int> isSymbol(std::string::iterator pos);

	static std::pair<bool, int> isDoubleSymbol(std::string::iterator pos);

	static bool isLetter(std::string::iterator pos);

	static bool isDigit(std::string::iterator pos);

	static bool isReservedWord(std::string curString);

	static bool isMetaChar(std::string::iterator pos);

	static std::string to_string(const int& num);
};
#endif
