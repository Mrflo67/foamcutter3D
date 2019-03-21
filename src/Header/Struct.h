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
}; /* Set the size of the window */

struct ImguiCheckBool {
	inline static bool open_Gcode = false;
	inline static bool show_demo_window = true;
	inline static bool show_app_fixed_overlay = false;
	inline static bool GCode_Info = true;
}; /* Check if Imgui window is open */

struct FileContent {
	std::string filePath;
	std::string line;
	std::string fileName;
	std::string recentFileName;
	inline static bool check = true;
}; /* Move file information to differents functions */

/* Set all structure to extern so that we can use them in other files */
extern struct WindowInfo info;
extern struct ImguiCheckBool Render_Open;
extern struct FileContent content;