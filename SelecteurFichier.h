#pragma once
#include <string>
#include <Header\Gcode.h>

class SelecteurFichier
{
private:

	//afficher confirmation que le chargement s'est bien passé
	bool msgSucces();
	//afficher message d'erreur
	bool msgErreur(char const* const message);
	bool verifierExtension(char const* nom);

public:

	SelecteurFichier();
	~SelecteurFichier();

	//verifier puis charger le gcode contenu dans le fichier designé
	bool selectionner(Gcode &gcodeCopy);

	/*bool fermer(); inutile car tfd ferme automatiquement*/
	
};

