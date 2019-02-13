#include "Header/SelecteurFichier.h"
#include "Header/Gcode.h"

#include "tfd\tinyfiledialogs.h"
#include <iostream>
#include <string>
#include <cstddef>
#include <string.h>
#include <fstream> 

#define EXTENSIONS  "*.gco" 
#define EXTENSION ".gco"
#define TAILLELIMITE 1024*1024*100

SelecteurFichier::SelecteurFichier()
{}

SelecteurFichier::~SelecteurFichier()
{}

bool SelecteurFichier::verifierExtension(char const* nom)
{
	if (strlen(nom) > 4)
	{
		std::string sNom = nom;
		if(sNom.rfind(EXTENSION)==std::string::npos) //recherche la dernière occurence de l'extension dans le nom
		{ 
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}



bool SelecteurFichier::selectionner(Gcode &gcodeCopy)
{
	char const* filtre[1] = { EXTENSIONS };
	char const* fichierAOuvrir;

	fichierAOuvrir = tinyfd_openFileDialog(
		"Choix du gcode", // NULL or ""
		"", // NULL or ""
		1, // 0
		filtre, // NULL {"*.jpg","*.png"}
		"Fichiers gcode", // NULL | "image files"
		0); // 0
			// in case of multiple files, the separator is |
			// returns NULL on cancel

	if (fichierAOuvrir != NULL) //si l'utilisateur a selectionné un fichier
	{

		if (!verifierExtension(fichierAOuvrir))
		{
			msgErreur("Mauvaise extension !");
			return false;
		}

		std::ifstream gcodeFile(fichierAOuvrir);
		std::cout << "ouverture de " << fichierAOuvrir << " en cours..." << std::endl; //debug

		if (gcodeFile.is_open())
		{
			gcodeFile.seekg(0, std::ios::end); //place le curseur virtuel à la fin du fichier

			if (gcodeFile.tellg() <= TAILLELIMITE) //verifie si le fichier a une taille inferieure à 100Mo
			{
				std::cout << "ouverture reussie ! chargement en cours !" << std::endl; //debug

				gcodeFile.seekg(0, std::ios::beg); //remet le curseur au début
				char c;
				std::string s;

				while (gcodeFile.get(c))
				{
					s += c; //recupere chaque caractère et l'ajoute à s
				}

				std::cout << s << std::endl; //debug
				gcodeCopy.setCommandes(s);//affecte s à Gcode

				msgSucces();
				tinyfd_notifyPopup(
					"Simulation", // NULL or ""
					"Initialisation en cours", // NULL or "" may contain \n \t
					"info"); // "info" "warning" "error"
			}
			else
			{
				msgErreur("le fichier est trop grand!\n Veuillez reessayer !");
			}


			gcodeFile.close();
		}
		else
		{
			msgErreur("Ouverture impossible!\nVeuillez reessayer");
		}
	}
	else
	{
		return false; //si l'utilisateur a annulé ou fermé la fenetre
	}
	return true;
}

bool SelecteurFichier::msgSucces()
{
	tinyfd_messageBox(
		"Gcode", // NULL or ""
		"Chargement réussi", // NULL or "" may contain \n \t
		"ok", // "ok" "okcancel" "yesno" "yesnocancel"
		"info", // "info" "warning" "error" "question"
		1);
	// 0 for cancel/no , 1 for ok/yes , 2 for no in yesnocancel

	return true;
}

bool SelecteurFichier::msgErreur(char const* const message)
{
	tinyfd_messageBox(
		"Erreur", // NULL or ""
		message, // NULL or "" may contain \n \t
		"okcancel", // "ok" "okcancel" "yesno" "yesnocancel"
		"warning", // "info" "warning" "error" "question"
		1);
	// 0 for cancel/no , 1 for ok/yes , 2 for no in yesnocancel

	return true;
}

/*bool SelecteurFichier::fermer()
{
	return true;
}*/

