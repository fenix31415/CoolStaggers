#pragma once

namespace Formulas
{
	float get_weapon_stagger(char* _attacker, char* _victim, RE::TESObjectWEAP* weap, float);
}

void draw_heading(RE::Actor* a);
void draw_heading_player(RE::Actor* a);
