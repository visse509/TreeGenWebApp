#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <fstream>

using namespace std;
class FileReader
{
public:
	FileReader(std::string inFile);
	int size;
	std::string text;
	const char * FileReader::getChars();
	~FileReader(void);
};

