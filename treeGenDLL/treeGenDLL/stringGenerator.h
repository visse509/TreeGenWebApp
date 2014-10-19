#pragma once
#include <vector>
#include <map>
class stringGenerator
{
public:
	stringGenerator(std::string lsXml);
	std::vector<char> getString();

private:
	std::map<char,std::string> pRules;
	std::vector<char> axiom;
	std::vector<char> alphabet;
	std::vector<char> currentString;
	std::vector<char> newString;

	void expandString();
	std::vector<char> getProductionRule(char inWord);
};

