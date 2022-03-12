#include "Settings.h"
#include "Utils.h"

#include "Hooks.h"
#include "DebugAPI.h"

namespace Hooks
{
	void MainUpdateHook::Update(RE::Main* a_this, float a2)
	{
		_Update(a_this, a2);

		DebugAPI::Update();
	}

	void PlayerCharacterHook::Update(RE::PlayerCharacter* a, float delta)
	{
		_Update(a, delta);

		draw_heading_player(a);
	}

	void CharacterHook::Update(RE::Character* a, float delta)
	{
		_Update(a, delta);

		draw_heading(a);
	}

	void apply_hooks() {
		SKSE::GetTrampoline().write_call<5>(REL::ID(42839).address() + 0x81, Formulas::get_weapon_stagger);

		writebytes<37633, 0x799>("\xEB");

#ifdef DEBUG
		MainUpdateHook::Hook();

		PlayerCharacterHook::Hook();
		CharacterHook::Hook();
#endif  // DEBUG

	}
}
