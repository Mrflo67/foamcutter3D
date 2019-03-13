/**
*	IMGUI_MENU.CPP FILE
*	DEFINE ALL THE IMGUI CONFIGURATION
*/


#ifdef _WIN32
#include <windows.h> //debug, should not be used in the final code
#elif __linux__
#include <X11/Xlib.h>
#endif


#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>

#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Struct.h"
#include "tinyfiledialogs.h"
#include "ImGui_Menu_Windows.h"

struct WindowInfo info;
struct ImguiCheckBool Render_Open;
struct FileContent content;


#ifdef _WIN32 // note the underscore: without it, it's not msdn official!

void getScreenResolution()
{
	info.WINDOW_WIDTH = (int)GetSystemMetrics(SM_CXSCREEN);
	info.WINDOW_HEIGHT = (int)GetSystemMetrics(SM_CYSCREEN);
}

#elif __unix__ // all unices

void getScreenResolution()
{
	Display* d = XOpenDisplay(NULL);
	Screen*  s = DefaultScreenOfDisplay(d);

	info.WINDOW_WIDTH = s->width;
	info.WINDOW_HEIGHT = s->height;
}

#elif __APPLE__
// Mac OS, not sure if this is covered by __posix__ and/or __unix__ though...
#endif




/* Retreive screen resolution in pixel */


/* Save the FilePath to a .txt file */
void savePath()
{
	std::ofstream outfile;
	outfile.open("SavePath.txt", std::ios::out); // Open the file in read mode

	outfile << content.filePath << std::endl;

	outfile.close();
}

void recentPath()
{
	retrievePath();
	content.recentFileName.clear();

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

void retrievePath()
{
	std::ifstream myFile("SavePath.txt");

	if (myFile.is_open())
	{
		while (getline(myFile, (content.line))) /* Get every line of the file selected */
		{
			content.fileName = content.line;
		}
		myFile.close(); /* Close the file */
	}
}

int openRecent()
{
	content.filePath = content.fileName;
	content.fileName.clear();

	if (content.filePath.c_str() == NULL)
	{
		int error = tinyfd_messageBox("Error", "The file is missing", "okcancel", "error", 1);
		return 0;
	}
	readGcode();
}

/* Open the filedialog to select GCode */
void openFileDialog()
{
	char const * lTheOpenFileName;
	char const * lFilterPatterns[2] = { "*.gco", "*.gcode" };

retry:
	lTheOpenFileName = tinyfd_openFileDialog("Select the GCode File", "", 2, lFilterPatterns, NULL, 0);

	if (!lTheOpenFileName)
	{
		int error = tinyfd_messageBox("Error", "-The file selected can't be open \n-No file selected", "okcancel", "error", 1);

		/* Set display for error */
		switch (error)
		{
		case 0:
			ImGui::Text("Unable to open File");
			break;
		case 1:
			goto retry;
			break;
		default:
			ImGui::Text("Unable to open File");
		}
	}

	if (lTheOpenFileName)
	{
		/* Patch space */
		/* Convert char to std::string */
		std::stringstream ss; /* Set a stringstream variable */
		ss.str(lTheOpenFileName); /* convert char to std::string */
		content.filePath = ss.str(); /* Save the filepath */
		savePath();
		recentPath();
	}
}

/* Read the GCode file selected */
void readGcode()
{
	std::ifstream myFile(content.filePath); /* Open the file */

	if (&content.check) /* Check if we already read the file / If not read the file */
	{
		if (myFile.is_open())
		{
			while (getline(myFile, (content.line))) /* Get every line of the file selected */
			{
				ImGui::Text(content.line.c_str()); /* Display the line */
			}
			myFile.close(); /* Close the file */
			content.check = false; /* Set the file to already read */
		}
		else
		{
			ImGui::Text("Unable to open File");
			ImGui::Text("Check if the file format is the right one !");
		}
	}
	else
	{
		ImGui::Text("Select a File");
		ImGui::Text("You can select the file by press the open button");
	}
}

void GCodeInfo(bool* p_open)
{
	/* Every data for the window is in pixel */

	info.DISTANCE = 30.0f; /* Set the distance from the border of the main window and the GCodeInfo window */
	info.corner = 1; /* Corner : top right */

	/* Set the position of the window */
	ImVec2 window_pos = ImVec2((info.corner & 1) ? ImGui::GetIO().DisplaySize.x - info.DISTANCE : info.DISTANCE, (info.corner & 2) ? ImGui::GetIO().DisplaySize.y - info.DISTANCE : info.DISTANCE);
	/* Set the GCode window to be fixed */
	ImVec2 window_pos_pivot = ImVec2((info.corner & 1) ? 1.0f : 0.0f, (info.corner & 2) ? 1.0f : 0.0f);
	/* Set the GCode window size */
	ImVec2 window_size = ImVec2(info.WINDOW_WIDTH * 0.2, info.WINDOW_HEIGHT * 0.7);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetWindowSize(window_size);

	/* Display the window if we open it */
	if (ImGui::Begin("GCode Contents", p_open))
	{
		ImGui::Text("Hello this is the contents of the GCode : ");
		ImGui::Separator();
		readGcode(); /* Function that read the GCode file */
		ImGui::Separator();
		ImGui::Text("This is the end of the GCode");
		ImGui::End();
	}
}

/* Main menu bar fix at the top of the main window */
void AppMainMenuBar()
{
	/* Shortcut doesn't work now */
	if (ImGui::BeginMainMenuBar()) /* Start the main menu bar */
	{
		if (ImGui::BeginMenu("Open", "Ctrl+O")) /* Display a list */
		{
			if (ImGui::MenuItem("New GCode", NULL)) /* Button open */
			{
				openFileDialog();
			}
			if (ImGui::BeginMenu("Recent", "Ctrl+R")) /* Button recent */
			{
				if (ImGui::MenuItem(content.recentFileName.c_str(), NULL))
				{
					openRecent();
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

void AppFixedOverlay(bool* p_open)
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

void ImguiRender()
{
	ImGui::Begin("Informations");
	{
		ImGui::Checkbox("Open GCode", &Render_Open.open_Gcode); /* Make checkbox */
		ImGui::Checkbox("Show Demo Window", &Render_Open.show_demo_window);
		ImGui::Checkbox("Show GCode", &Render_Open.GCode_Info);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); /* Display the actual framerate */
		ImGui::Separator();
		ImGui::Text("This is the file you selected : ");
		ImGui::SameLine();
		ImGui::Text(content.filePath.c_str()); /* Display the path of the file selected */
		
	}
	ImGui::End(); // END Test

	if (Render_Open.open_Gcode)
	{
		ImGui::Begin("Open the GCode");
		ImGui::Text("This is the GCode");
		ImGui::Separator();
		ImGui::Checkbox("Show App Fixed Overlay", &Render_Open.show_app_fixed_overlay);
		if (ImGui::Button("Close GCode"))
		{
			Render_Open.open_Gcode = !Render_Open.open_Gcode;
			Render_Open.show_app_fixed_overlay = !Render_Open.show_app_fixed_overlay;
		}
		if (Render_Open.show_app_fixed_overlay)
			AppFixedOverlay(&Render_Open.show_app_fixed_overlay);
		ImGui::End();
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