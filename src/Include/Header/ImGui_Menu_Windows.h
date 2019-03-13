#pragma once

void getScreenResolution(void);
void AppMainMenuBar(void);
void AppFixedOverlay(bool* p_open);
void openFileDialog(void);
void ImguiRender(void);
void GCodeInfo(bool* p_open);
void readGcode(void);
void savePath(void);
void recentPath(void);
void retrievePath(void);
int openRecent(void);