#include "FileReader.h"
#pragma unmanaged

FileReader::FileReader(std::string inFile)
{
	std::string line;
	std::ifstream in(inFile);
	while(std::getline(in, line))
	{
		text += line + "\n";
	}
	size = text.size();
}

const char * FileReader::getChars(){
	return text.c_str();
}

FileReader::~FileReader(void)
{
	delete [] &text;
}
