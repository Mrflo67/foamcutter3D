/**
*	IMGUI_MENU_WINDOWS.H FILE
*	CLASS IMGUIMENUWINDOW DECLARATION
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

#pragma once

#include "Simulation.h"
#include "Config.h"

class ImguiMenuWindow {
public:

	ImguiMenuWindow();
	~ImguiMenuWindow();

	void getScreenResolution(void);
	void AppMainMenuBar(Simulation & simu, Config & config);
	void AppFixedOverlay(bool* p_open);
	void openGcode(Simulation & simu, long int & maxLength, std::string & extensionList, std::string & validCmdList);
	void ImguiRender(void);
	void GCodeInfo(bool* p_open);
	void readGcode(void);
	void savePath(void);
	void recentPath(void);
	void retrievePath(void);
	void axisPos(Simulation & simu);
	int  openRecent(Simulation & simu);
	void machineSettings(Config & cfg, Simulation & simu);
	void graphicSettings(Config & cfg);
	void About(void);
};

/* using namespace ImGui */
namespace ImGui
{
	void Information(void);
	void Transformation(void);
}