/**
*	GCODE.CPP FILE
*	READ GCODE AND RETRIEVE COMMANDS
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

#include "Gcode.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <tinyfiledialogs.h>


Gcode::Gcode(std::string filename) : m_name(""), m_commands(""), m_size(0)
{
	if (filename.length() > 0)
	{
		std::ifstream gcodeFile(filename);

		if (gcodeFile.is_open())
		{
			char c;
			std::string content;

			while (gcodeFile.get(c)) //copier le contenu du fichier dans l'attribut "m_commandes de l'objet Gcode
			{
				content += c; //On le fait caractère par caractère
			}

			if (content.rfind("M2") == std::string::npos)
			{
				content.append(" M2");
			}

			m_commands = content;
			m_size = m_commands.size();
			m_name = filename;

			gcodeFile.close();
		}
		else
		{

			tinyfd_messageBox(
				"Error", // NULL or ""
				"Can't open, try again", // NULL or "" may contain \n \t
				"okcancel", // "ok" "okcancel" "yesno" "yesnocancel"
				"warning", // "info" "warning" "error" "question"
				1);
			// 0 for cancel/no , 1 for ok/yes , 2 for no in yesnocancel
		}
	}
	
}

Gcode::~Gcode()
{}


std::string Gcode::getName()
{
	return this->m_name;
}

std::string Gcode::getlineCommand(bool reset)
{
	std::string sCmdline = std::string("");
	static unsigned int i = this->m_size;

	if (reset)
	{
		i = 0;
		sCmdline = std::string("");
	}

	if (i >= m_size)
		i = 0;
	if (m_commands[i] == '\n')
		i++;

	while (m_commands[i] != '\n')
	{
		sCmdline += m_commands[i];
		i++;

		if (i >= m_size)
			break;
	}

	return sCmdline;
}


std::string Gcode::getCommandes()
{
	return this->m_commands;
}

size_t Gcode::getSize()
{
	return this->m_size;
}


int Gcode::isLoaded()
{

	if (this->m_size > 0)
		return 1;

	return 0;
}
