#include "FileSelector.h"

#include <tinyfiledialogs.h>

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <array>
#include <cstddef>



FileSelector::FileSelector()
{}

FileSelector::~FileSelector()
{}

//returns true if all gcode commands are known
//returns false if gcode contains unknown command(s)
//doesn't check numeric values
bool FileSelector::verifierContenu(char const* nom, std::string & validCmdList)
{
	std::ifstream fichier(nom);
	std::string commandeLue;
	
	if (fichier.is_open())
	{
		//std::cout << "Debut de l'analyse des commandes contenues dans le fichier" << std::endl;
		while (fichier >> commandeLue)
		{
			//std::cout << commandeLue << std::endl;

			//find() returns npos if the string has not been found
			if (validCmdList.find(commandeLue[0]) == std::string::npos)
				return false;

			/* DEBUG*/
			//std::cout << "commandeLue :    " << commandeLue << std::endl;
			//std::cout << "CommandeLue[0] : " << commandeLue[0]<<std::endl;
		}
		fichier.close();
	} 
	else
	{
		std::cout << "File opening failed !" << std::endl;
		msgErreur("Can't open the file\nPlease try again");
		return false;
	}
	
	//msgSucces("file loaded successfully !");
	tinyfd_notifyPopup("Success", "Gcode file loaded !", "info");

	std::cout << "File loaded successfully" << std::endl;
	return true;
}


bool FileSelector::verifierExtension(char const* nom, std::string extensionList)
{
	if (strlen(nom) > 4)
	{
		std::string sNom = nom;
		size_t dotPos = sNom.rfind(".");
		sNom.erase(0, dotPos);
		sNom.push_back(' ');

		extensionList.push_back(' ');

		if(extensionList.find(sNom)==std::string::npos) //recherche la dernière occurence de l'extension dans le nom
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


bool FileSelector::verifierLongueur(char const* nom, long int maxLength)
{
	std::cout << "Opening of " << nom << " ..." << std::endl; //debug
	std::ifstream gcodeFile(nom, std::ios::binary | std::ios::ate);//ouvre fichier avec curseur à la fi
	
	if (gcodeFile.is_open())
	{
		long int longueur = gcodeFile.tellg(); //recupère la longueur du fichier

		if (longueur == 0 || longueur > maxLength)
		{
			gcodeFile.close();
			return false;
		}
	}
	else
	{
		msgErreur("Cannot open the file !\nPlease try again !");
		return false;
	}gcodeFile.close();

	return true;
}




std::string FileSelector::select(long int maxLength, std::string extensionList,
	std::string & validCmdList)
{
	std::array<std::string, 3> arr;
	size_t j = 0;
	std::string sFilterText("");

	for (size_t i = 0; i < arr.size(); ++i)
	{
		std::string ext("");
		
		while (extensionList[j] != ' ' && j < extensionList.length())
		{
			ext.push_back(extensionList[j]);
			j++;
		}
		j++;
		arr[i] = ext;
		sFilterText += ext + ", ";
	}
	sFilterText.erase(sFilterText.length()-2);


	char const* filters[3] = { arr[0].c_str(), arr[1].c_str(), arr[2].c_str() };

	char const* filterText = sFilterText.c_str();

	std::string nomFichierGcode("");

	while (nomFichierGcode.length() == 0)
	{
		char const* fichierChoisi=NULL;

		fichierChoisi = tinyfd_openFileDialog(
			"Select gcode", // NULL or ""
			"./gcodes", // NULL or ""
			3, // 0
			filters, // NULL {"*.jpg","*.png"}
			filterText, // NULL | "image files"
			0); // 0
				// in case of multiple files, the separator is |
				// returns NULL on cancel

		if (fichierChoisi == NULL)
			break;
		
		if(!verifierExtension(fichierChoisi, extensionList))
		{
			msgErreur("Wrong file extension !\nPlease select a valid Gcode file");
		}
		else
		{
			if (!verifierLongueur(fichierChoisi, maxLength))
			{
				msgErreur("The selected file is empty or too large !\nPlease try again !");
			}
			else
			{
				if (!verifierContenu(fichierChoisi, validCmdList))
				{
					msgErreur("Unsupported or unknown commands detected");
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

bool FileSelector::msgSucces(char const* const message)
{
	tinyfd_messageBox(
		"Info", // NULL or ""
		message, // NULL or "" may contain \n \t
		"ok", // "ok" "okcancel" "yesno" "yesnocancel"
		"Info", // "info" "warning" "error" "question"
		1);

	return true;
}

bool FileSelector::msgErreur(char const* const message)
{
	tinyfd_messageBox(
		"Error", // NULL or ""
		message, // NULL or "" may contain \n \t
		"okcancel", // "ok" "okcancel" "yesno" "yesnocancel"
		"warning", // "info" "warning" "error" "question"
		1);
	// 0 for cancel/no , 1 for ok/yes , 2 for no in yesnocancel

	return true;
}

/*bool FileSelector::fermer()
{
	return true; //peut être utile selon la bibliothèque graphique utilisée
}*/