#include "Gcode.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "tinyfiledialogs.h"


Gcode::Gcode(std::string filename)
{
	std::ifstream gcodeFile(filename, std::fstream::in | std::ios::beg);

	if (gcodeFile.is_open())
	{
		char c;
		std::string contenu;

		while (gcodeFile.get(c)) //copier le contenu du fichier dans l'attribut "m_commandes de l'objet Gcode
		{
			contenu += c; //On le fait caractère par caractère
		}

		if (contenu.rfind("M2") == std::string::npos)
		{
			contenu.append(" M2");
		}

		m_commandes = contenu;
		m_taille = m_commandes.size();
		m_name = filename;

		gcodeFile.close();
	}
	else
	{
		tinyfd_messageBox(
			"Erreur", // NULL or ""
			"ouverture impossible, veuillez réessayer!", // NULL or "" may contain \n \t
			"okcancel", // "ok" "okcancel" "yesno" "yesnocancel"
			"warning", // "info" "warning" "error" "question"
			1);
		// 0 for cancel/no , 1 for ok/yes , 2 for no in yesnocancel
	}
}

Gcode::Gcode() : m_name(""), m_commandes(""), m_taille(0)
{}

Gcode::~Gcode()
{}


std::string Gcode::getName()
{
	return this->m_name;
}

std::string Gcode::getlineCommand(bool reset)
{
	std::string sCmdline = std::string("");
	static unsigned int i = this->m_taille;

	if (reset)
	{
		i = 0;
		sCmdline = std::string("");
	}

	if (i >= m_taille)
		i = 0;
	if (m_commandes[i] == '\n')
		i++;

	while (m_commandes[i] != '\n')
	{
		sCmdline += m_commandes[i];
		i++;

		if (i >= m_taille)
			break;

	}

	return sCmdline;
}


std::string Gcode::getCommandes()
{
	return this->m_commandes;
}

size_t Gcode::getTaille()
{
	return this->m_taille;
}


int Gcode::isLoaded()
{

	if (this->m_taille > 0)
		return 1;

	return 0;
}
