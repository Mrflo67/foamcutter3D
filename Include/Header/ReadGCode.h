/**
*	FILE READGCODE.H
*	THIS FILE DEFINE THE CLASS READGCODE.CPP
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

#pragma once

#include <string>

#include "glm/glm.hpp"

class ReadFile
{
public:
	ReadFile(const std::string& filepath);
	void sw(const char &value);
	~ReadFile();
private:
	void ParseGCode(const std::string& filepath);
};