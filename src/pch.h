/**
*	PCH.H FILE
*	PRECOMPILE HEADERS TO SPEED UP THE COMPILATION
*/

#pragma once

/* OpenGL Libraries */




/*MATH Libraries*/



/* CPP Includes */
#include <iostream>
#include <thread>
#include <mutex>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <vector>
#include <algorithm>

#ifdef _WIN32
#include <windows.h> //debug, should not be used in the final code
#elif __linux__
#include <X11/Xlib.h>
#endif

//tfd
#include "tinyfiledialogs.h"

