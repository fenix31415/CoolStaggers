#pragma once

namespace Hooks
{
	class MainUpdateHook
	{
	public:
		static void Hook()
		{
			REL::Relocation<uintptr_t> hook{ REL::ID(35551) };

			auto& trampoline = SKSE::GetTrampoline();
			_Update = trampoline.write_call<5>(hook.address() + 0x11F, Update);  // SkyrimSE.exe+5AF4EF
		}

	private:
		static void Update(RE::Main* a_this, float a2);
		static inline REL::Relocation<decltype(Update)> _Update;
	};

	class PlayerCharacterHook
	{
	public:
		static void Hook()
		{
			REL::Relocation<std::uintptr_t> PlayerCharacter_Vtbl{ RE::Offset::PlayerCharacter::Vtbl };
			_Update = PlayerCharacter_Vtbl.write_vfunc(0xAD, Update);
		}

	private:
		static void Update(RE::PlayerCharacter* a, float delta);

		static inline REL::Relocation<decltype(Update)> _Update;
	};

	class CharacterHook
	{
	public:
		static void Hook()
		{
			REL::Relocation<std::uintptr_t> Character_Vtbl{ REL::ID(261397) };
			_Update = Character_Vtbl.write_vfunc(0xAD, Update);
		}

	private:
		static void Update(RE::Character* a, float delta);

		static inline REL::Relocation<decltype(Update)> _Update;
	};

	void apply_hooks();
}
