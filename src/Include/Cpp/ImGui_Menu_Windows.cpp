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

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <tinyfiledialogs.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Struct.h"
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
		while (getline(myFile, content.line)) /* Get every line of the file selected */
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
			content.fileName = content.filePath;

			savePath();

			content.check = true;

			simu.Init();
		}
	
	}
	

	return 1;
}

void ImguiMenuWindow::machineSettings(Config & cfg, Simulation & simu)
{
		static int length = cfg.plate.longueur;
		static int width = cfg.plate.largeur;
		//static int height = cfg.hauteurMaxFil;

		static int lFoam = cfg.foam.longueur;
		static int LFoam = cfg.foam.largeur;
		static int hFoam = cfg.foam.hauteur;

		static int posX = cfg.foam.posX;
		static int posZ = cfg.foam.posZ;
		static float angle = cfg.foam.angleY;

		ImGui::TextUnformatted("Taille plateau");
		ImGui::DragInt("Longueur", &length, 1.0f, 100, 2000, "%.0f");
		ImGui::DragInt("Largeur", &width, 1.0f, 100, 2000, "%.0f");
		//ImGui::TextUnformatted("Fil");
		//ImGui::DragInt("Hauteur max", &height, 1.0f, 100, 2000, "%.0f");

		if (ImGui::Button("Defaut", ImVec2(100,20)))
		{
			length = 800;
			width = 600;
			//height = 500;
			LFoam = hFoam = lFoam = 100;
			posX = posZ = 0;
			angle = 0.0f;
		}
		ImGui::Separator();

		ImGui::TextUnformatted("Piece a decouper");
		ImGui::DragInt("longueur", &lFoam, 1.0f, 10, 2000, "%.0f");
		ImGui::DragInt("largeur", &LFoam, 1.0f, 10, 2000, "%.0f");
		ImGui::DragInt("hauteur", &hFoam, 1.0f, 10, 2000, "%.0f");

		ImGui::DragInt("posX", &posX, 1.0f, -2000, 2000, "%.0f");
		ImGui::DragInt("posZ", &posZ, 1.0f, -2000, 2000, "%.0f");
		ImGui::SliderFloat("angleY", &angle,-180, 180, "%.0f");

		static int limitX1, limitX2, limitZ1, limitZ2;

		if (length < lFoam)
			lFoam = length;
		if (width < LFoam)
			LFoam = width;

		limitX1 = -length / 2 + lFoam /2;
		limitX2 = length / 2 - lFoam /2;
		limitZ1 = -width / 2 + LFoam /2;
		limitZ2 = width / 2 - LFoam/ 2;

		if (posX > limitX2)
			posX = limitX2;
		if (posX < limitX1)
			posX = limitX1;
		if (posZ > limitZ2)
			posZ = limitZ2;
		if (posZ < limitZ1)
			posZ = limitZ1;

		cfg.plate.longueur = length;
		cfg.plate.largeur = width;
		//cfg.hauteurMaxFil = height;

		cfg.foam.longueur = lFoam;
		cfg.foam.hauteur = hFoam;
		cfg.foam.largeur = LFoam;

		cfg.foam.posX = posX;
		cfg.foam.posZ = posZ;
		cfg.foam.angleY = angle;

		simu.m_cube->setLargeur((float)LFoam);
		simu.m_cube->setLongueur(lFoam);
		simu.m_cube->setHauteur(hFoam);
		simu.m_cube->setPosX(posX);
		simu.m_cube->setPosZ(posZ);
}

void ImguiMenuWindow::graphicSettings(Config & cfg)
{
	ImGui::TextUnformatted("Options graphiques");
	ImGui::TextUnformatted("V sync");
	ImGui::SameLine();
	static int e = 1;
	ImGui::RadioButton("OFF", &e, 0); ImGui::SameLine();
	ImGui::RadioButton("ON", &e, 1);
	if (ImGui::Button("Valider"))
	{
		if (e)
			cfg.graphics.vsync = true;
		else
			cfg.graphics.vsync = false;
	}
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
void ImguiMenuWindow::AppMainMenuBar(Simulation & simu, Config & config) 
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
		if (ImGui::BeginMenu("Display"))
		{
			ImGui::MenuItem("Informations", "Ctrl+I", &Render_Open.information);
			ImGui::MenuItem("Open GCode", "Ctrl+R", &Render_Open.GCode_Info);
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
		if (ImGui::BeginMenu("Settings", "Ctrl + S"))
		{
			if (ImGui::BeginMenu("Reglages machine"))
			{
				machineSettings(config, simu);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Graphics"))
			{
				graphicSettings(config);
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

}

void ImguiMenuWindow::ImguiRender()
{
	//std::cout << "Mutex is take by : " << __func__ << std::endl;
	if (Render_Open.information)
	{
		ImGui::Information();
	}

	if (Render_Open.GCode_Info)
	{
	//	ImGui::GCodeInfo();
	}

	if (Render_Open.about)
	{

	}

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
	ImGui::Begin("Informations");
	{
		ImGui::SetNextWindowPos(ImVec2(info.WINDOW_WIDTH * 0.1f, info.WINDOW_HEIGHT * 0.5f), ImGuiCond_FirstUseEver);
		//std::cout << "Mutex is take by : " << __func__ << std::endl;
		ImGui::Checkbox("Show Demo Window", &Render_Open.show_demo_window);
		//ImGui::Checkbox("Show GCode", &Render_Open.GCode_Info);
		ImGui::Text("Application average %.1f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); /* Display the actual framerate */
		 /* Display the path of the file selected */

	}ImGui::End();
}

void ImguiMenuWindow::axisPos(Simulation &simu)
{
	float axisValues[4];
	simu.m_fil->getCurrentPos(axisValues);
	float rotation = simu.m_cube->getRotationDeg();//todo getcurrentRotation
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