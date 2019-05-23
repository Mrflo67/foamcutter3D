#include "Config.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <tinyfiledialogs.h>

// for convenience
using json = nlohmann::json;

Config::Config()
{
	if (this->alreadyCreated)
		throw std::logic_error("Impossible de creer\
plusieurs instances de Config !");

	this->alreadyCreated = true;

	//default parameters
	//used when the config file is unavailable
	this->theme = 1;
	this->fastSpeed = 10000;
	this->plate.longueur = 800;
	this->plate.largeur = 600;
	this->foam.longueur = 100;
	this->foam.largeur = 100;
	this->foam.hauteur = 100;
	this->foam.posX = 0;
	this->foam.posZ = 0;
	this->foam.angleY = 0.0f;
	this->graphics.vsync = true;
	this->maxFileLength = 1000000;
	this->validCmds = "G M S F P X Y U V B";
	this->extensions = "*.gco *.gcode *.g";
}

bool Config::alreadyCreated = false;

Config::~Config()
{
	this->alreadyCreated = false;
}

int Config::Write()
{
	json j;
	j["plateau"]["longueur"] = this->plate.longueur;
	j["plateau"]["largeur"] = this->plate.largeur;
	j["foam"]["longueur"] = this->foam.longueur;
	j["foam"]["largeur"] = this->foam.largeur;
	j["foam"]["hauteur"] = this->foam.hauteur;
	j["foam"]["posX"] = this->foam.posX;
	j["foam"]["posZ"] = this->foam.posZ;
	j["foam"]["angle"] = this->foam.angleY;

	j["graphics"]["vsync"] = this->graphics.vsync;

	j["fastspeed"] = this->fastSpeed;
	j["theme"] = this->theme;

	j["gcode"]["longueurMax"] = this->maxFileLength;
	j["gcode"]["commandesValides"] = this->validCmds;
	j["gcode"]["extensions"] = this->extensions;




	std::cout << "Ouverture de config.json ..." << std::endl;
	std::ofstream ofs("config.json");

	if (ofs.is_open())
	{
		ofs << std::setw(4) << j << std::endl;
		std::cout<<"La configuration a ete sauvegardee"<<std::endl;
		ofs.close();
	}
	else
	{
		std::cout << "Ouvrture de config.json impossible" << std::endl;
		std::cout << "La configuration n'a pas pu etre sauvegardee" << std::endl;
	}
	
	return 0;
}

int Config::Read()
{
	std::cout << "Ouverture de config.json ..." << std::endl;

	std::ifstream ifs("config.json");

	if (ifs.is_open())
	{
		json j;

		j = json::parse(ifs);

		this->fastSpeed = j["fastspeed"];
		this->theme = j["theme"];
		this->plate.longueur = j["plateau"]["longueur"];
		this->plate.largeur = j["plateau"]["largeur"];
		this->foam.longueur = j["foam"]["longueur"];
		this->foam.largeur = j["foam"]["largeur"];
		this->foam.hauteur = j["foam"]["hauteur"];
		this->foam.posX = j["foam"]["posX"];
		this->foam.posZ = j["foam"]["posZ"];
		this->foam.angleY = j["foam"]["angle"];
		this->graphics.vsync = j["graphics"]["vsync"];
		this->maxFileLength = j["gcode"]["longueurMax"];

		std::string validCmd = j["gcode"]["commandesValides"];
		std::string ext = j["gcode"]["extensions"];

		this->validCmds = validCmd;
		this->extensions = ext;

		std::cout << "La configuration a ete chargee" << std::endl;

		ifs.close();
	}
	else
	{
		std::cout << "Le fichier de configuration est introuvable" << std::endl;
		return 0;
	}

	return 1;
}
