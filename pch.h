/**
*	PCH.H FILE
*	PRECOMPILE HEADERS TO SPEED UP THE COMPILATION
*/

#pragma once

/* OpenGL Libraries */
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* IMGUI Include */
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw_gl3.h>


/*MATH Libraries*/
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>


/* CPP Includes */
#include <iostream>
#include <thread>
#include <mutex>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <vector>
#include <algorithm>

#include "Header/tinyfiledialogs.h"