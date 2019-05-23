#pragma once
#include <string>

class SelecteurFichier
{
private:

	bool msgSucces(char const* const message); //afficher confirmation que le chargement s'est bien passé
	bool msgErreur(char const* const message); //afficher message d'erreur
	bool verifierExtension(char const* nom, std::string extensionList);
	bool verifierLongueur(char const* nom, long int maxLength=1000000);
	bool verifierContenu(char const* nom, std::string & validCmdList);

public:

	SelecteurFichier();
	~SelecteurFichier();

	//ouvre la fenetre de sélection puis vérifie le fichier selectionné	
	std::string select(long int maxLength, std::string extensionList, std::string & validCmdList); 
};

