/**
*	IMGUI_MENU.CPP FILE
*	DEFINE ALL THE IMGUI CONFIGURATION
*/

//#include "pch.h"
#include "Header/Headers.h"
#include "Header/Struct.h"

struct WindowInfo info;
struct ImguiCheckBool Render_Open;
struct FileContent content;

/* Retreive screen resolution in pixel */
void getScreenResolution()
{
	info.WINDOW_WIDTH = (int)GetSystemMetrics(SM_CXSCREEN);
	info.WINDOW_HEIGTH = (int)GetSystemMetrics(SM_CYSCREEN);
}

/* Open the filedialog to select GCode */
void openFileDialog()
{
	char filename[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "GCODE\0*.gco\0GCODE\0*.gcode\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a File";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn))
	{
		/* Patch des espaces */
		std::cout << "You chose the file \"" << filename << "\"\n";
		/* Convert char to std::string */
		std::stringstream ss;
		ss.str(filename);
		content.filePath = ss.str(); /* Save the filepath */
	}
	else
	{
		/* DON'T DELETE THE DATA BELOW */
		bool error = true;
		// All this stuff below is to tell you exactly how you messed up above. 
		// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default: std::cout << "You cancelled.\n";
		}
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
	ImVec2 window_size = ImVec2(info.WINDOW_WIDTH * 0.2, info.WINDOW_HEIGTH * 0.7);
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
			if (ImGui::MenuItem("Recent", NULL)) /* Button recent */
			{
				/* Need to save the path in a file */
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