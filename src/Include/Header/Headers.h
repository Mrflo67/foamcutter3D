#pragma once

/* Personal includes */
#ifdef _WIN32
#include "ImGui_Menu_windows.h"
#include "Renderer.h"
#include "Gcode.h"
#include "GcodeLoader.h"
#include "SceneOpenGL.h"
#include "SelecteurFichier.h"
//#include "Cube.h"
//#include "Shader.h"

#elif __linux__
/* Set Headers for linux */

#include "ImGui_Menu_windows.h"
#include "Renderer.h"
#include "Gcode.h"
#include "GcodeLoader.h"
#include "SceneOpenGL.h"
#include "SelecteurFichier.h"
//#include "Cube.h"
//#include "Shader.h"

#endif // Headers