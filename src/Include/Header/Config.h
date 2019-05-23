#pragma once
#include <string>

struct Plate {
	unsigned int longueur;
	unsigned int largeur;
};

struct sFoam {
	unsigned int longueur;
	unsigned int largeur;
	unsigned int hauteur;
	int posX;
	int posZ;
	float angleY;
};

struct Graphics {
	bool vsync;
};

typedef struct Plate Plate;
typedef struct sFoam sFoam;
typedef struct Graphics Graphics;

class Config
{
public:
	Config();
	~Config();

	int Write();
	int Read();

	Plate plate;
	sFoam foam;
	Graphics graphics;
	
	unsigned int fastSpeed;
	int theme;

	long int maxFileLength;
	std::string validCmds;
	std::string extensions;

private:
	static bool alreadyCreated;
};



