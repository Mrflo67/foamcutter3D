#include <fstream>
#include "Header\GcodeLoader.h"
#include <tfd\tinyfiledialogs.h>

GcodeLoader::GcodeLoader()
{
}


GcodeLoader::~GcodeLoader()
{
}

void GcodeLoader::load(Gcode gc)
{
	std::ifstream gcodeFile(gc.getName());

	if (gcodeFile.is_open())
	{
		char c;
		std::string contenu;

		while (gcodeFile.get(c)) //copier le contenu du fichier dans l'attribut "commandes de l'objet Gcode
		{
			contenu += c; //On le fait caractère par caractère
		}

		gc.setCommandes(contenu);


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
	
	return;
}
