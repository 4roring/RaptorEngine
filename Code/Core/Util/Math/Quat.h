#pragma once

struct Quat
{
	union
	{
		struct  
		{
			float x, y, z, w;
		};
		float v[4];
	};
};




