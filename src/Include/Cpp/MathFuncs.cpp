/**
*	MATHFUNCS.CPP FILE
*	DO THE MATHS
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

#include "MathFuncs.h"

float interpolate2L(glm::vec2 Adata, glm::vec2 Bdata, float x)
{
	if (Adata.x != Bdata.x)
	{
		if (Adata.y == Bdata.y)
			return x;
		float xa = Adata.x;
		float xb = Bdata.x;
		float ya = Adata.y;
		float yb = Bdata.y;
		float y = ((ya - yb) / (xa - xb)) * x + (xa * yb - xb * ya) / (xa - xb);

		return y;
	}
	else
	{
		return x;
	}
}