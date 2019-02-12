#include "Header/SelecteurFichier.h"

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
	//todo
	
	int longueur = strlen(nom);
	
	if (longueur > 4)
	{
		std::string sNom = nom;
		if(sNom.rfind(EXTENSION)==std::string::npos) //recherche si l'extension voulue est dans le nom du fichier
													//dernière occurrence, donc .txt.gco valide
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

bool SelecteurFichier::afficher()
{
	char const* filtre[1] = { EXTENSIONS };
	char const* fichierAOuvrir;
	fichierAOuvrir = tinyfd_openFileDialog(
		"Choix du gcode", // NULL or ""
		"", // NULL or ""
		1, // 0
		filtre, // NULL {"*.jpg","*.png"}
		"fichiers gcode", // NULL | "image files"
		0); // 0
										  // in case of multiple files, the separator is |
										  // returns NULL on cancel
	std::cout << fichierAOuvrir << std::endl;

	if (fichierAOuvrir != NULL) //si l'utilisateur a selectionné un fichier
	{
		std::cout << fichierAOuvrir << std::endl;
		if (!selectionner(fichierAOuvrir)) //si le fichier est valide
		{
			return false;
		}
	}
	else
	{
		return false; //si l'utilisateur a annulé ou fermé la fenetre
	}
		
	return true;
}

bool SelecteurFichier::selectionner(const char* nom)
{

	if (!verifierExtension(nom))
	{
		msgErreur("Mauvaise extension !");
		return false;
	}



	std::ifstream gcodeFile(nom);
	std::cout << "ouverture de " << nom <<" en cours..."<< std::endl;

	if (gcodeFile.is_open())
	{
		gcodeFile.seekg(0, std::ios::end); //place le curseur virtuel à la fin du fichier
		
		if (gcodeFile.tellg() <= TAILLELIMITE) //verifie si le fichier a une taille inferieure à 100Mo
		{
			std::cout << "ouverture reussie ! chargement en cours !" << std::endl;
			
			gcodeFile.seekg(0, std::ios::beg); //remet le curseur au début
			char c;
			std::string s;

			while (gcodeFile.get(c))
			{
				s += c; //recupere chaque caractère et l'ajoute à s
			}

			std::cout <<s << std::endl;
			/*Gcode.setCommandes(s);*/   //affecte s à Gcode

			msgSucces();
			tinyfd_notifyPopup(
				"Simulation", // NULL or ""
				"Initialisation en cours", // NULL or "" may contain \n \t
				"info"); // "info" "warning" "error"
		}
	
		else
		{
			//std::cout << "le fichier est trop grand, veuillez reessayer !" << std::endl;
			msgErreur("le fichier est trop grand!\n Veuillez reessayer !");
		}
		

		gcodeFile.close();
	}
	else
	{
		//impossible d'ouvrir xxx
		//std::cout << "impossible d'ouvrir " << nom << " !" << std::endl;
		msgErreur("Ouverture impossible!\nVeuillez reessayer");
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

bool SelecteurFichier::fermer()
{
	return true;
}

