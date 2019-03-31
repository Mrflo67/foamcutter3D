#include "tinyfiledialogs.h"

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <cstddef>

#include "SelecteurFichier.h"


#define EXTENSIONS  "*.gco" 
#define EXTENSION ".gco"
#define TAILLELIMITE 1024*1024*100

#define COMMANDES_G "G0 G1 G4 G17 G21 G28 G90 G91 G92"
#define COMMANDES_M "M0 M1 M2 M3 M5"
#define COMMANDES_CONNUES "G M S F P X Y U V B"

SelecteurFichier::SelecteurFichier()
{}

SelecteurFichier::~SelecteurFichier()
{}

//returns true if all gcode commands are known
//returns false if gcode contains unknown command(s)
//doesn't check numeric values
bool SelecteurFichier::verifierContenu(char const* nom)
{
	std::ifstream fichier(nom);
	std::string commandeLue;

	std::string commandesConnues(COMMANDES_CONNUES);
	std::string commandes_M(COMMANDES_M);
	std::string commandes_G(COMMANDES_G);

	if (fichier.is_open())
	{
		std::cout << "Debut de l'analyse des commandes contenues dans le fichier" << std::endl;
		while (fichier >> commandeLue)
		{
			std::cout << commandeLue << std::endl;

			//find() returns npos if the string has not been found
			if (commandesConnues.find(commandeLue[0]) == std::string::npos)
				return false;

			if (commandeLue[0] == 'G' || commandeLue[0] == 'M')
			{
				if (commandes_G.find(commandeLue) == std::string::npos)
				{
					if (commandes_M.find(commandeLue) == std::string::npos)
					{
						return false;
					}
				}
			}
			

			/* DEBUG*/
			//std::cout << "commandeLue :    " << commandeLue << std::endl;
			//std::cout << "CommandeLue[0] : " << commandeLue[0]<<std::endl;
		}
		fichier.close();
	} 
	else
	{
		std::cout << "Ouverture du fichier impossible" << std::endl;
		msgErreur("Ouverture impossible!\nVeuillez reessayer");
		return false;
	}
	
	std::cout << "Contenu valide, chargement du fichier..." << std::endl;
	return true;
}


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


bool SelecteurFichier::verifierLongueur(char const* nom)
{
	std::ifstream gcodeFile(nom);
	std::cout << "ouverture de " << nom << " en cours..." << std::endl; //debug

	if (gcodeFile.is_open())
	{
		gcodeFile.seekg(0, std::ios::end); //place le curseur virtuel à la fin du fichier

		size_t longueur = gcodeFile.tellg(); //recupère la longueur du fichier
		gcodeFile.seekg(0, std::ios::beg); //remet le curseur au début

		if (longueur == 0 || longueur > TAILLELIMITE)
		{
			gcodeFile.close();
			return false;
		}
		
	}
	else
	{
		msgErreur("Ouverture impossible!\nVeuillez reessayer");
		return false;
	}gcodeFile.close();
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
			break;
		}

		if(!verifierExtension(fichierChoisi))
		{
			msgErreur("Mauvaise extension !");
		}
		else
		{
			if (!verifierLongueur(fichierChoisi))
			{
				msgErreur("Le fichier est vide ou trop grand!\n Veuillez reessayer !");
			}
			else
			{
				if (!verifierContenu(fichierChoisi))
				{
					msgErreur("Des commandes inconnues ont été détectées dans le fichier");
				}
				else
				{
					nomFichierGcode = fichierChoisi;
					//msgSucces("Selection reussie");
				}
				
			}
		}	
	}
	
	
	return nomFichierGcode;
}

bool SelecteurFichier::msgSucces(char const* const message)
{
	tinyfd_messageBox(
		"Info", // NULL or ""
		message, // NULL or "" may contain \n \t
		"ok", // "ok" "okcancel" "yesno" "yesnocancel"
		"Info", // "info" "warning" "error" "question"
		1);


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