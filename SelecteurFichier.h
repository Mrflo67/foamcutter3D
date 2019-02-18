#pragma once
#include <string>
#include <Header\Gcode.h>

class SelecteurFichier
{
private:

	bool msgSucces(char const* const message); //afficher confirmation que le chargement s'est bien passé
	bool msgErreur(char const* const message); //afficher message d'erreur
	bool verifierExtension(char const* nom);
	bool verifierLongueur(char const* nom);

public:

	SelecteurFichier();
	~SelecteurFichier();

	std::string select(); //ouvr la fenetre de sélection puis vérifie le fichier selectionné

	/*bool fermer(); inutile car tfd ferme automatiquement*/
	
};

