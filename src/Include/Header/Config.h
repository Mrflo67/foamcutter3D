/**
*	CONFIG.H FILE
*	CONFIG CLASS DECLARATION
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/


#pragma once
#include <string>

struct Plate {
	unsigned int length;
	unsigned int width;
};

struct sFoam {
	unsigned int length;
	unsigned int width;
	unsigned int height;
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

	Plate Plate;
	sFoam sFoam;
	Graphics Graphics;
	
	unsigned int fastSpeed;

	long int maxFileLength;
	std::string validCmds;
	std::string extensions;

private:
	static bool alreadyCreated;
};



