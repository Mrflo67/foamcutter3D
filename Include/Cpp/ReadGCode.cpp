/**
*	FILE READGCODE.CPP
*	THIS FILE READ THE GCODE
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

/* C++ Libraries */
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw_gl3.h>

#include "Header/ReadGCode.h"
#include "Header/Renderer.h"

ReadFile::ReadFile(const std::string& filepath)
{
	ParseGCode(filepath);
}

ReadFile::~ReadFile()
{
}

void ReadFile::sw(const char &value)
{
	switch (value)
	{
	case 'G':
		break;
	case 'X':
		break;
	case 'Y':
		break;
	case 'U':
		break;
	case 'V':
		break;
	case 'Z':
		break;
	default:
		std::cout << "Error" << std::endl;
		break;
	}
}

/* Read file */
void ReadFile::ParseGCode(const std::string& filepath)
{
	
}