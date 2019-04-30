#pragma once

/**
*	STRUCT.H FILE
*	DEFINE ALL THE STRUCTURE THE PROGRAM USE
*	GET RID OF ARGUMENT FOR ALL THE FUNCTIONS WE USE
*/

#include <string>

struct WindowInfo {
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;
	int corner;
	float DISTANCE;
	float zoomFactor;
}; /* Set the size of the window */

struct ImguiCheckBool {
	static bool open_Gcode;
	static bool show_demo_window;
	static bool show_app_fixed_overlay;
	static bool GCode_Info;
	static bool information;
	static bool about;
	static bool show_settings;
}; /* Check if Imgui window is open */

struct FileContent {
	std::string filePath;
	std::string commands;
	std::string line;
	std::string fileName;
	std::string recentFileName;
	static bool check;
}; /* Move file information to differents functions */

struct ThreadInfo {
	static bool thread_A_End; // Wait for Thread A to finish
};

/* Set all structure to extern so that we can use them in other files */
 extern struct WindowInfo info;
 extern struct ImguiCheckBool Render_Open;
 extern struct FileContent content;
 extern struct ThreadInfo ThreadInfo;
