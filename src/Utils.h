#pragma once
#include <RE/N/NiSmartPointer.h>

namespace PlaneUtils
{
	struct Line
	{
		float a, b;
		float operator()(float x) const
		{
			return x * a + b;
		}
	};

	struct Plane
	{
		float a, b, c;
		float operator()(float x, float y) const
		{
			return x * a + y * b + c;
		}
	};

	struct Parab
	{
		float a, b, c;
		float operator()(float x) const
		{
			return x * x * a + x * b + c;
		}
	};
}

namespace Formulas
{
	float get_weapon_stagger(char* _attacker, char* _victim, RE::TESObjectWEAP* weap, float);
}
