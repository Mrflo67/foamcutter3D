#pragma once

#include "Header\Gcode.h"
class GcodeLoader
{
public:
	GcodeLoader();
	~GcodeLoader();

	void load(Gcode gc);
};

