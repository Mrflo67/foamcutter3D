/**
*	IMGUI_MENU_WINDOWS.CPP FILE
*	DEFINE ALL THE IMGUI CONFIGURATION AND WINDOWS
*	PROJECT BTS SN 2019 - FOAM CUTTER
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
#include "FileSelector.h"

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
			tinyfd_notifyPopup("Success", "Gcode file loaded", "info");

			simu.Init();
		}
	
	}
	

	return 1;
}

void ImguiMenuWindow::machineSettings(Config & cfg, Simulation & simu)
{
		static int length = cfg.Plate.length;
		static int width = cfg.Plate.width;


		static int lFoam = cfg.sFoam.length;
		static int LFoam = cfg.sFoam.width;
		static int hFoam = cfg.sFoam.height;

		static int posX = cfg.sFoam.posX;
		static int posZ = cfg.sFoam.posZ;
		static float angle = cfg.sFoam.angleY;

		ImGui::TextUnformatted("Plate (mm)");
		ImGui::DragInt("Length", &length, 1.0f, 100, 2000, "%.0f");
		ImGui::DragInt("Width", &width, 1.0f, 100, 2000, "%.0f");
		

		if (ImGui::Button("Defaut Plate", ImVec2(100,20)))
		{
			length = 800;
			width = 600;
		}
		ImGui::Separator();

		ImGui::TextUnformatted("Foam to cut (mm)");
		ImGui::DragInt("length", &lFoam, 1.0f, 10, 2000, "%.0f");
		ImGui::DragInt("width", &LFoam, 1.0f, 10, 2000, "%.0f");
		ImGui::DragInt("height", &hFoam, 1.0f, 10, 2000, "%.0f");

		ImGui::DragInt("posX", &posX, 1.0f, -2000, 2000, "%.0f");
		ImGui::DragInt("posZ", &posZ, 1.0f, -2000, 2000, "%.0f");
		ImGui::SliderFloat("angleY", &angle,-180, 180, "%.0f");

		if (ImGui::Button("Defaut Foam", ImVec2(100, 20)))
		{
			LFoam = hFoam = lFoam = 100;
			posX = posZ = 0;
			angle = 0.0f;
		}
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

		cfg.Plate.length = length;
		cfg.Plate.width = width;
		

		cfg.sFoam.length = lFoam;
		cfg.sFoam.height = hFoam;
		cfg.sFoam.width = LFoam;

		cfg.sFoam.posX = posX;
		cfg.sFoam.posZ = posZ;
		cfg.sFoam.angleY = angle;

		simu.m_cube->setRotationRad(glm::radians(angle));
		simu.m_cube->setWidth((float)LFoam);
		simu.m_cube->setLength(lFoam);
		simu.m_cube->setHeight(hFoam);
		simu.m_cube->setPosX(posX);
		simu.m_cube->setPosZ(posZ);

		simu.setBaseAngleInit(glm::radians(angle));
}

void ImguiMenuWindow::graphicSettings(Config & cfg)
{
	ImGui::TextUnformatted("V sync");
	ImGui::SameLine();
	static int e = cfg.Graphics.vsync;
	ImGui::RadioButton("OFF", &e, 0);
	ImGui::SameLine();
	ImGui::RadioButton("ON", &e, 1);
		if (e)
			cfg.Graphics.vsync = true;
		else
			cfg.Graphics.vsync = false;
}

/* Open the filedialog to select GCode */
void ImguiMenuWindow::openGcode(Simulation &simu, long int & maxLength, std::string & extensionList, std::string & validCmdList)
{
	FileSelector sf;
	std::string filename = sf.select(maxLength, extensionList, validCmdList);

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
		ImGui::Text("No gcode loaded");
		ImGui::Text("Click \"Open\" to load a gcode file");
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
				openGcode(simu, config.maxFileLength, config.extensions, config.validCmds);
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
			//ImGui::MenuItem("Informations", "Ctrl+I", &Render_Open.information);
			ImGui::MenuItem("GCode", "Ctrl+R", &Render_Open.GCode_Info);
			ImGui::EndMenu();
		}
		//if (ImGui::BeginMenu("About", "Ctrl+A")) /* Information about the program */
		//{
			ImGui::MenuItem("About", NULL, &Render_Open.about);
			//ImGui::EndMenu();
			
			
		//}
		if (ImGui::BeginMenu("Settings", "Ctrl + S"))
		{
			if (ImGui::BeginMenu("Machine"))
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
		ImGui::Text(" | %.1f ms | %.1f FPS", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::EndMainMenuBar();
	}

}

void ImguiMenuWindow::About()
{
	//std::cout << "Mutex is take by : " << __func__ << std::endl;
	ImGui::OpenPopup("About");
	if (ImGui::BeginPopupModal("About"))
	{
		ImGui::TextUnformatted("Filshow 3D Simulator\n2018-2019\nBTS SN\nLycee Heinrich Nessel Haguenau");
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
			Render_Open.about = !Render_Open.about;
		}
		ImGui::EndPopup();
	}
}

void ImguiMenuWindow::ImguiRender()
{
	//std::cout << "Mutex is take by : " << __func__ << std::endl;
	/*if (Render_Open.information)
	{
		ImGui::Information();
	}

	if (Render_Open.GCode_Info)
	{
	//	ImGui::GCodeInfo();
	}*/

	if (Render_Open.about)
	{
		About();
	}

	if (Render_Open.GCode_Info)
	{
		GCodeInfo(&Render_Open.GCode_Info);
	}

	/*if (Render_Open.show_demo_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
		ImGui::ShowDemoWindow(&Render_Open.show_demo_window);
	}*/

}

void ImGui::Information(void)
{
	ImGui::Begin("Informations");
	{
		ImGui::SetNextWindowPos(ImVec2(info.WINDOW_WIDTH * 0.1f, info.WINDOW_HEIGHT * 0.5f), ImGuiCond_FirstUseEver);
		//std::cout << "Mutex is take by : " << __func__ << std::endl;
		//ImGui::Checkbox("Show Demo Window", &Render_Open.show_demo_window);
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
	ImGui::Text("X: %.4f Y: %.4f  (mm)",axisValues[0], axisValues[1]);
	ImGui::Text("U: %.4f V: %.4f  (mm)", axisValues[2], axisValues[3]);
	ImGui::Text("");
	ImGui::Text("B: %.4f  (degrees)", rotation);

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