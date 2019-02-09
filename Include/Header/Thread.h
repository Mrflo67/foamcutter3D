#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <windows.h>

#include "IMGUI/imgui.h"

extern std::string filePath;
extern bool waitSelectPath;
std::string line;
std::string contenu;
//char L = 'a';

void openGCode()
{
	std::ifstream myFile(filePath);

	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			
			std::cout << line;
		}

	
		
		
		


		myFile.close();
	}
	else
	{
		//ImGui::Text("Unable to open file");
		std::cout << "Unable to open file" << std::endl;
	}
}

inline void readGCode()
{

}
