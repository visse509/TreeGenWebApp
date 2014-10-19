#include "stringGenerator.h"
#include "rapidxml.hpp"
#include "FileReader.h"
using namespace rapidxml;
stringGenerator::stringGenerator(std::string lsXml){


	ifstream ifile(lsXml);
	FileReader *file = new FileReader(lsXml);
	if (ifile) {
		file = new FileReader(lsXml);
	}
	char * text = const_cast<char*>(file->getChars());

	//Parse xmlfile
	xml_document<> doc;
	doc.parse<0>(text);

	//nbranches
	xml_node<> *root = doc.first_node()->first_node();
	xml_node<> *node = root->next_sibling();

	for (xml_node<> *child = root->first_node(); child; child = child->next_sibling())
	{
		xml_node<> *axiomNode = child;
		char* str = axiomNode->first_attribute()->value();
		axiom.push_back(str[0]);
	}
	xml_node<>* rulesNode = node->next_sibling();
	for (xml_node<> *child = node->first_node(); child; child = child->next_sibling())
	{

		xml_node<> *rulesNode = child;
		char* kStr = rulesNode->first_attribute()->value();
		char* vStr = rulesNode->value();
		std::string string(vStr);
		pRules.insert(std::pair<char,std::string>(kStr[0],vStr));

	}


	alphabet.push_back('f');
	alphabet.push_back('x');
	alphabet.push_back('y');
	alphabet.push_back('r');
	alphabet.push_back('l');
	alphabet.push_back('[');
	alphabet.push_back(']');
	alphabet.push_back('q');

	currentString = axiom;

	expandString();
	expandString();
	expandString();
	//expandString();
	//expandString();
	//expandString();
	//expandString();
}

void stringGenerator::expandString(){
	newString.clear();
	for(int i = 0; i < currentString.size(); i++){
		char tChar = currentString[i];
		std::vector<char> tVec = getProductionRule(tChar);
		newString.insert( newString.end(), tVec.begin(), tVec.end() );
	}
	currentString = newString;
}

std::vector<char> stringGenerator::getProductionRule(char inWord){
	std::vector<char> res;
	std::string tString = pRules.find(inWord)->second;
	for(int i = 0; i < tString.size();i++){
		res.push_back(tString[i]);
	}
	return res;
}

std::vector<char> stringGenerator::getString(){
	return newString;
}