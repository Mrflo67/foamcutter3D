/**
*	IMGUI_MENU.CPP FILE
*	DEFINE ALL THE IMGUI CONFIGURATION
*/

#ifdef _WIN32
#include <windows.h> 
#elif __linux__
#include <X11/Xlib.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>


#include "Struct.h"
#include "tinyfiledialogs.h"
#include "ImGui_Menu_Windows.h"
#include "SelecteurFichier.h"



struct WindowInfo info;
struct ImguiCheckBool Render_Open;
struct FileContent content;

ImguiMenuWindow::ImguiMenuWindow()
{
}

ImguiMenuWindow::~ImguiMenuWindow()
{
}

/* Get screen resolution in pixels */
void ImguiMenuWindow::getScreenResolution()
{
	//std::cout << "Mutex is take by : " << __func__ << std::endl;

#ifdef _WIN32

	info.WINDOW_WIDTH = (int)GetSystemMetrics(SM_CXSCREEN);
	info.WINDOW_HEIGHT = (int)GetSystemMetrics(SM_CYSCREEN);

#elif __unix__

	Display* d = XOpenDisplay(NULL);
	Screen*  s = DefaultScreenOfDisplay(d);

	info.WINDOW_WIDTH = s->width;
	info.WINDOW_HEIGHT = s->height;

#elif __APPLE__

	info.WINDOW_WIDTH = 800;
	info.WINDOW_HEIGHT = 600;

#endif
}


/* Save the most recent FilePath to a .txt file */
void ImguiMenuWindow::savePath()
{
	//std::cout << "Mutex is take by : " << __func__ << std::endl;
	std::ofstream outfile;
	outfile.open("SavePath.txt", std::ios::out); // Open the file in read mode

	outfile << content.filePath << std::endl;

	outfile.close();
}

void ImguiMenuWindow::recentPath()
{
	//std::cout << "Mutex is take by : " << __func__ << std::endl;
	retrievePath();
	content.recentFileName.clear(); // Reset the content of recentFileName

	int i = 0;
	int save = -1;

	while (content.fileName[i] != '\0')
	{
		if (content.fileName[i] == '\\')
		{
			save = i;
		}
		i++;
	}

	switch (save)
	{
	case -1:
		/* Set an error */
		break;
	default:
		break;
	}

	save++;
	while (content.fileName[save] != '\0')
	{
		content.recentFileName = content.recentFileName + content.fileName[save];
		save++;
	}
}

void ImguiMenuWindow::retrievePath()
{
	//std::cout << "Mutex is take by : " << __func__ << std::endl;
	std::ifstream myFile("SavePath.txt");

	if (myFile.is_open())
	{
		while (getline(myFile, (content.line))) /* Get every line of the file selected */
		{
			content.fileName = content.filePath = content.line;
		}
		myFile.close(); /* Close the file */
	}
}

int ImguiMenuWindow::openRecent(Simulation & simu)
{
	//std::cout << "Mutex is take by : " << __func__ << std::endl;
	content.filePath = content.fileName;
	content.fileName.clear();

	if (content.filePath.c_str() == NULL)
	{
		int error = tinyfd_messageBox("Error", "The file is missing", "okcancel", "error", 1);
		return 0;
	}
	else
	{
		if (simu.ChargerGcode(content.filePath))
		{
			savePath();

			content.check = true;

			simu.Init();
		}
		recentPath();
	}
	
	return 1;
}

/* Open the filedialog to select GCode */
void ImguiMenuWindow::openFileDialog(Simulation &simu)
{
	SelecteurFichier sf;
	std::string filename = sf.select();

	if (simu.ChargerGcode(filename))
	{
		savePath();
		content.check = true;
		simu.Init();
	}
	recentPath();
}

/* Read the GCode file selected */
void ImguiMenuWindow::readGcode()
{
	//std::cout << "Mutex is take by : " << __func__ << std::endl;

	if (content.check)
	{
		ImGui::TextUnformatted(content.recentFileName.c_str());
		ImGui::Separator();
		ImGui::Text(content.commands.c_str()); /* Display the line */
	}
	else
	{
		ImGui::Text("Aucun fichier choisi");
		ImGui::Text("Vous pouvez charger un fichier en cliquant sur \"Open\"");
	}
}

void ImguiMenuWindow::GCodeInfo(bool* p_open)
{
	//std::cout << "Mutex is take by : " << __func__ << std::endl;
	/* Set the GCode window size */
	ImVec2 window_size = ImVec2(info.WINDOW_WIDTH * 0.2f, info.WINDOW_HEIGHT * 0.7f);
	ImGui::SetNextWindowPos(ImVec2(info.WINDOW_WIDTH * 0.5f, info.WINDOW_HEIGHT * 0.1f), ImGuiCond_FirstUseEver);
	ImGui::SetWindowSize(window_size);

	/* Display the window if we open it */
	if (ImGui::Begin("Gcode", p_open))
	{
		readGcode(); /* Function that read the GCode file */
		ImGui::End();
	}
}

/* Main menu bar fix at the top of the main window */
void ImguiMenuWindow::AppMainMenuBar(Simulation &simu) 
{ 
	//std::cout << "Mutex is take by : " << __func__ << std::endl;
	/* Shortcut doesn't work now */
	if (ImGui::BeginMainMenuBar()) /* Start the main menu bar */
	{
		if (ImGui::BeginMenu("Open", "Ctrl+O")) /* Display a list */
		{
			if (ImGui::MenuItem("New GCode", NULL)) /* Button open */
			{
				openFileDialog(simu);
			}
			if (ImGui::BeginMenu("Recent", "Ctrl+R")) /* Button recent */
			{
				if (ImGui::MenuItem(content.recentFileName.c_str(), NULL))
				{
					openRecent(simu);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("About", "Ctrl+A")) /* Information about the program */
		{
			if (ImGui::MenuItem("Foam Cutter", NULL))
			{
				/* Retreive data from .txt file ?? */
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Quit", "Alt+F4"))
		{
			exit(0); /* Quit the program */
		}
		ImGui::EndMainMenuBar();
	}

}

void ImguiMenuWindow::ImguiRender()
{
	//std::cout << "Mutex is take by : " << __func__ << std::endl;
	ImGui::Begin("Informations");
	{
		ImGui::SetNextWindowPos(ImVec2(info.WINDOW_WIDTH * 0.1f, info.WINDOW_HEIGHT * 0.5f), ImGuiCond_FirstUseEver);
		ImGui::Information();
	}
	ImGui::End();

	if (Render_Open.GCode_Info)
	{
		GCodeInfo(&Render_Open.GCode_Info);
	}

	if (Render_Open.show_demo_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
		ImGui::ShowDemoWindow(&Render_Open.show_demo_window);
	}

}

void ImGui::Information(void)
{
	//std::cout << "Mutex is take by : " << __func__ << std::endl;
	ImGui::SetNextWindowPos(ImVec2(10, 20), ImGuiCond_FirstUseEver);
	ImGui::Checkbox("Show Demo Window", &Render_Open.show_demo_window);
	ImGui::Checkbox("Show GCode", &Render_Open.GCode_Info);
	ImGui::Text("Application average %.1f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); /* Display the actual framerate */
	 /* Display the path of the file selected */
}

void ImguiMenuWindow::axisPos(Simulation &simu)
{
	float axisValues[4];
	simu.m_fil->getCurrentPos(axisValues);
	float rotation = simu.m_cube->getRotationAngle();//todo getcurrentRotation
	ImGui::SetNextWindowBgAlpha(0.4f); // Transparent background

	ImGui::Begin("Axis Values");
	ImGui::Text("X: %.4f Y: %.4f",axisValues[0], axisValues[1]);
	ImGui::Text("U: %.4f V: %.4f", axisValues[2], axisValues[3]);
	ImGui::Text("");
	ImGui::Text("B: %.4f", rotation);

	ImGui::End();
}

void ImguiMenuWindow::AppFixedOverlay(bool* p_open)
{
	/* Same as the top one */

	info.DISTANCE = 30.0;
	info.corner = 2;

	ImVec2 window_pos = ImVec2((info.corner & 1) ? ImGui::GetIO().DisplaySize.x - info.DISTANCE : info.DISTANCE, (info.corner & 2) ? ImGui::GetIO().DisplaySize.y - info.DISTANCE : info.DISTANCE);
	ImVec2 window_pos_pivot = ImVec2((info.corner & 1) ? 1.0f : 0.0f, (info.corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background

	if (ImGui::Begin("Example: Fixed Overlay", p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		ImGui::Text("Simple overlay\nin the corner of the screen.");
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Text("Foam Cutter Simulation");
		ImGui::End();
	}
}


void ImGui::Transformation(void)
{
	
}