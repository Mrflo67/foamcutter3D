#pragma once
#include "Gcode.h"

class GcodeLoader
{
public:
	GcodeLoader();
	~GcodeLoader();

	void load(Gcode &gc);
};

