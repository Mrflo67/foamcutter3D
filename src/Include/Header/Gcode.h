/**
*	GCODE.H FILE
*	GCODE CLASS DECLARATION
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

#pragma once

#include <string>

class Gcode
{
private:
	std::string m_name;
	std::string m_commands;
	size_t m_size;

public:
	Gcode(std::string n);
	~Gcode();

	int isLoaded();
	size_t getSize();
	std::string getName();
	std::string getlineCommand(bool reset);
	std::string getCommandes();

};




