/**
*	FileSelector.H FILE
*	FileSelector CLASS DECLARATION
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

#pragma once
#include <string>

#define MAX_LENGTH 1000000

class FileSelector
{
private:

	bool msgSucces(char const* const message); //afficher confirmation que le chargement s'est bien passé
	bool msgErreur(char const* const message); //afficher message d'erreur
	bool verifierExtension(char const* nom, std::string extensionList);
	bool verifierLongueur(char const* nom, long int maxLength=MAX_LENGTH);
	bool verifierContenu(char const* nom, std::string & validCmdList);

public:

	FileSelector();
	~FileSelector();

	//ouvre la fenetre de sélection puis vérifie le fichier selectionné	
	std::string select(long int maxLength, std::string extensionList, std::string & validCmdList); 
};

