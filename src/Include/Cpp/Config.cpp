/**
*	CONFIG.CPP FILE
*	RETRIEVE THE CONFIGURATION OF THE PROJECT
*	PROJECT BTS SN 2019 - Foam CUTTER
*/


#include "Config.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <tinyfiledialogs.h>

// for convenience
using json = nlohmann::json;

const std::string configFileName = "config.json";

Config::Config()
{
	if (this->alreadyCreated)
		throw std::logic_error("Can't create multiple instances of config file !");

	this->alreadyCreated = true;

	//default parameters
	//used when the config file is unavailable

	this->fastSpeed = 10000;
	this->Plate.length = 800;
	this->Plate.width = 600;
	this->sFoam.length = 100;
	this->sFoam.width = 100;
	this->sFoam.height = 100;
	this->sFoam.posX = 0;
	this->sFoam.posZ = 0;
	this->sFoam.angleY = 0.0f;
	this->Graphics.vsync = true;
	this->maxFileLength = 1000000;
	this->validCmds = "G M S F P X Y U V B";
	this->extensions = "*.gco *.gcode *.g";
}

bool Config::alreadyCreated = false;

Config::~Config()
{
	this->alreadyCreated = false;
}

// Write in config file
int Config::Write()
{
	json j;
	j["plate"]["length"] = this->Plate.length;
	j["plate"]["width"] = this->Plate.width;
	j["foam"]["length"] = this->sFoam.length;
	j["foam"]["width"] = this->sFoam.width;
	j["foam"]["height"] = this->sFoam.height;
	j["foam"]["posX"] = this->sFoam.posX;
	j["foam"]["posZ"] = this->sFoam.posZ;
	j["foam"]["angle"] = this->sFoam.angleY;

	j["graphics"]["vsync"] = this->Graphics.vsync;

	j["fastspeed"] = this->fastSpeed;

	j["gcode"]["longueurMax"] = this->maxFileLength;
	j["gcode"]["commandesValides"] = this->validCmds;
	j["gcode"]["extensions"] = this->extensions;

	std::cout << "Opening " + configFileName + " ..." << std::endl;
	std::ofstream ofs(configFileName);

	if (ofs.is_open())
	{
		ofs << std::setw(4) << j << std::endl;
		std::cout << "The configuration has been saved." << std::endl;
		ofs.close();
	}
	else
	{
		std::cout << "Opening " + configFileName + " impossible" << std::endl;
		std::cout << "Configuration save failed" << std::endl;
	}
	
	return 0;
}


// Read config file
int Config::Read()
{
	std::cout << "Opening " + configFileName + " ..." << std::endl;

	std::ifstream ifs(configFileName);

	if (ifs.is_open())
	{
		json j;

		j = json::parse(ifs);

		this->fastSpeed = j["fastspeed"];
		this->Plate.length = j["plate"]["length"];
		this->Plate.width = j["plate"]["width"];
		this->sFoam.length = j["foam"]["length"];
		this->sFoam.width = j["foam"]["width"];
		this->sFoam.height = j["foam"]["height"];
		this->sFoam.posX = j["foam"]["posX"];
		this->sFoam.posZ = j["foam"]["posZ"];
		this->sFoam.angleY = j["foam"]["angle"];
		this->Graphics.vsync = j["graphics"]["vsync"];
		this->maxFileLength = j["gcode"]["longueurMax"];
		
		std::string validCmd = j["gcode"]["commandesValides"];
		std::string ext = j["gcode"]["extensions"];
		
		this->validCmds = validCmd;
		this->extensions = ext;
		
		std::cout << "Configuration loaded" << std::endl;
		ifs.close();
	}
	else
	{
		std::cout << "The configuration file is not available" << std::endl;
		return 0;
	}

	return 1;
}
