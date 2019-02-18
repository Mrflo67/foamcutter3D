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



std::string SelecteurFichier::select()
{
	char const* filtre[1] = { EXTENSIONS };
	std::string nomFichierGcode = "";

	while (nomFichierGcode == "")
	{
		char const* fichierChoisi=NULL;

		fichierChoisi = tinyfd_openFileDialog(
			"Choix du gcode", // NULL or ""
			"", // NULL or ""
			1, // 0
			filtre, // NULL {"*.jpg","*.png"}
			"Fichiers gcode", // NULL | "image files"
			0); // 0
				// in case of multiple files, the separator is |
				// returns NULL on cancel

		if (fichierChoisi == NULL)
		{
			msgErreur("Choix du gcode annulé, fermeture du programme...");
			break;
		}
			

		if (!verifierExtension(fichierChoisi))
		{
			msgErreur("Mauvaise extension !");
		}
		else
		{
			std::ifstream gcodeFile(fichierChoisi);
			std::cout << "ouverture de " << fichierChoisi << " en cours..." << std::endl; //debug

			if (gcodeFile.is_open())
			{
				gcodeFile.seekg(0, std::ios::end); //place le curseur virtuel à la fin du fichier

				size_t longueur = gcodeFile.tellg(); //recupère la longueur du fichier
				gcodeFile.seekg(0, std::ios::beg); //remet le curseur au début

				if (longueur > 0 && longueur <= TAILLELIMITE)
				{
					char c;
					while (gcodeFile.get(c))
					{
						nomFichierGcode += c;
					}
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
	}
	
	msgSucces("Fichier selectionné ! Chargement en cours...");
	return nomFichierGcode;
}

bool SelecteurFichier::msgSucces(char const* const message)
{
	tinyfd_messageBox(
		"Succès", // NULL or ""
		message, // NULL or "" may contain \n \t
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
	return true; //peut être utile selon la bibliothèque graphique utilisée
}*/

