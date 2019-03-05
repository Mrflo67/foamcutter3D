#include "pch.h"
#include "Header/SelecteurFichier.h"
#include "Header/Gcode.h"
#include "Header/tinyfiledialogs.h"

#define EXTENSIONS  "*.gco" 
#define EXTENSION ".gco"
#define TAILLELIMITE 1024*1024*100


#define COMMANDES_G "G0 G1 G4 G28 G90 G91 G92"
#define COMMANDES_M "M0 M1 M2 M3 M5"
#define COMMANDES_CONNUES "G M S F P\
X Y U V B"

SelecteurFichier::SelecteurFichier()
{}

SelecteurFichier::~SelecteurFichier()
{}


bool SelecteurFichier::verifierContenu(char const* nom)
{
	std::ifstream fichier(nom);
	std::string commandeLue;

	std::string commandesConnues(COMMANDES_CONNUES);
	std::string commandes_M(COMMANDES_M);
	std::string commandes_G(COMMANDES_G);

	//todo
	/*
	while (fichier >> commandeLue)
	{
		if (commandesConnues.find(commandeLue[0]) != std::string::npos)
		{
			if (commandeLue[0] == 'G' || commandeLue[0] == 'M')
			{
				if (commandes_M.find(commandeLue) != std::string::npos)
				{
					
				}
				else
				{
					if (commandes_G.find(commandeLue) != std::string::npos)
					{

					}
				} else
				{
					return false;
				}
			}

		} else
		{
			return false;
		}

	}

	*/


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
			msgErreur("Choix du gcode annulé, fermeture du programme...");
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
				msgErreur("le fichier est trop grand!\n Veuillez reessayer !");
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
					msgSucces("Fichier selectionné ! Chargement en cours...");
				}
				
			}
		}	
	}
	
	
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