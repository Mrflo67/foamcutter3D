/**
*	FILE READGCODE.CPP
*	THIS FILE READ THE GCODE
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

/* C++ Libraries */
#include "Header/ReadGCode.h"

ReadFile::ReadFile(const std::string& filePath)
{
	ParseGCode(filePath);
}

ReadFile::~ReadFile()
{
}

void ReadFile::sw(const char &value)
{
	switch (value)
	{
	case 'G':
		break;
	case 'X':
		break;
	case 'Y':
		break;
	case 'U':
		break;
	case 'V':
		break;
	case 'Z':
		break;
	default:
		std::cout << "Error" << std::endl;
		break;
	}
}

/* Read file */
void ReadFile::ParseGCode(const std::string& filePath)
{
	/*std::ifstream myFile(filePath);

	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			std::cout << line << std::endl;
		}
		myFile.close();
	}
	else
	{
		std::cout << "Unable to open file" << std::endl;
	}*/
}