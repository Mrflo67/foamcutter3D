#pragma once
#include <string>

class SelecteurFichier
{
private:

	//afficher confirmation que le chargement s'est bien pass�
	bool msgSucces();
	//afficher message d'erreur
	bool msgErreur(char const* const message);
	bool verifierExtension(char const* nom);

public:

	SelecteurFichier();
	~SelecteurFichier();

	bool afficher();

	//verifier puis charger le gcode contenu dans le fichier design�
	bool selectionner(const char* nom);

	bool fermer();
	
};

