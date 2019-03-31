#pragma once

#include "Simulation.h"

class ImguiMenuWindow {
public:

	ImguiMenuWindow();
	~ImguiMenuWindow();

	void getScreenResolution(void);
	void AppMainMenuBar(Simulation & simu);
	void AppFixedOverlay(bool* p_open);
	void openFileDialog(Simulation & simu);
	void ImguiRender(void);
	void GCodeInfo(bool* p_open);
	void readGcode(void);
	void savePath(void);
	void recentPath(void);
	void retrievePath(void);
	void axisPos(Simulation & simu);
	int  openRecent(Simulation & simu);
	
};

/* using namespace ImGui */
namespace ImGui
{
	void Information(void);
	void Transformation(void);
}