#pragma once
#include "PCH.h"

struct Settings
{
	using ISetting = AutoTOML::ISetting;
	using bSetting = AutoTOML::bSetting;
	using fSetting = AutoTOML::fSetting;

	static void load()
	{
		try {
			const auto table = toml::parse_file("Data/SKSE/Plugins/CoolStaggers.toml"s);
			for (const auto& setting : ISetting::get_settings()) {
				setting->load(table);
			}
		} catch (const toml::parse_error& e) {
			std::ostringstream ss;
			ss
				<< "Error parsing file \'" << *e.source().path << "\':\n"
				<< '\t' << e.description() << '\n'
				<< "\t\t(" << e.source().begin << ')';
			logger::error(ss.str());
			throw std::runtime_error("failed to load settings"s);
		}
	}

	static inline fSetting RegenDelayMult{ "Regen"s, "RegenDelayMult"s, 0.05f };  // k
	static inline fSetting RegenDelayBase{ "Regen"s, "RegenDelayBase"s, 0.2f };   // b

	static inline fSetting SkillMult{ "Skill"s, "SkillMult"s, -0.35f };
	static inline fSetting SkillBase{ "Skill"s, "SkillBase"s, 1.1f };
	
	static inline bSetting RegenPlayer{ "Regen"s, "Player"s, true };             // Change vanilla formula for player
	static inline bSetting RegenNPC{ "Regen"s, "NPC"s, true };                   // Change vanilla formula for NPC
	static inline bSetting RegenNPCState{ "Regen"s, "StateNPC"s, false };        // Mult formula by this consts for NPC
	static inline fSetting RegenStay{ "Regen"s, "Stay"s, 1.0f };                 // Stay regen mult
	static inline fSetting RegenWalk{ "Regen"s, "Walk"s, 0.8f };                 // Walk regen mult
	static inline fSetting RegenRun{ "Regen"s, "Run"s, 0.5f };                   // Run regen mult
	static inline fSetting RegenWeightMult{ "Regen"s, "WeightMult"s, -0.005f };
	static inline fSetting RegenCarryMult{ "Regen"s, "CarryMult"s, -0.5f };
	static inline fSetting RegenCarryBase{ "Regen"s, "CarryBase"s, 1.5f };

	static inline fSetting RegenHP_Ay{ "Regen"s, "HP_Ay"s, 0.2f };
	static inline fSetting RegenHP_Bx{ "Regen"s, "HP_Bx"s, 0.4f };
	static inline fSetting RegenHP_By{ "Regen"s, "HP_By"s, 0.9f };
	static inline fSetting RegenHP_Cx{ "Regen"s, "HP_Cx"s, 0.95f };
	static inline fSetting RegenHP_Cy{ "Regen"s, "HP_Cy"s, 1.0f };
	static inline fSetting RegenHP_Dy{ "Regen"s, "HP_Dy"s, 1.3f };
	static inline fSetting RegenSP_Ay{ "Regen"s, "SP_Ay"s, 0.5f };
	static inline fSetting RegenSP_Bx{ "Regen"s, "SP_Bx"s, 0.2f };
	static inline fSetting RegenSP_By{ "Regen"s, "SP_By"s, 0.9f };
	static inline fSetting RegenSP_Cx{ "Regen"s, "SP_Cx"s, 0.9f };
	static inline fSetting RegenSP_Cy{ "Regen"s, "SP_Cy"s, 1.0f };
	static inline fSetting RegenSP_Dy{ "Regen"s, "SP_Dy"s, 1.3f };


	static inline bSetting meleeNPC{ "Melee"s, "DenyNPC"s, true };              // Deny NPC attacks with low stamina
	static inline bSetting meleePlayer{ "Melee"s, "DenyPlayer"s, true };        // Deny Player attacks with low stamina
	static inline bSetting meleeCostPlayer{ "Melee"s, "DamagePlayer"s, true };  // Damage Player stamina every melee attack
	static inline bSetting meleeCostNPC{ "Melee"s, "DamageNPC"s, true };        // Damage NPC stamina every melee attack
	static inline fSetting meleeWeightMult{ "Melee"s, "WeightMult"s, 0.9f };
	static inline fSetting meleeStaminaMult{ "Melee"s, "StaminaMult"s, 0.05f };
	static inline fSetting meleeBase{ "Melee"s, "Base"s, 6.8f };

	static inline bSetting bashNPC{ "Bash"s, "DenyNPC"s, true };              // Deny NPC bash with low stamina
	static inline bSetting bashPlayer{ "Bash"s, "DenyPlayer"s, true };        // Deny Player bash with low stamina
	static inline bSetting bashCostNPC{ "Bash"s, "DamageNPC"s, true };        // Damage NPC stamina every bash
	static inline bSetting bashCostPlayer{ "Bash"s, "DamagePlayer"s, true };  // Damage Player stamina every bash
	static inline fSetting bashWeightMult{ "Bash"s, "WeightMult"s, 0.45f };
	static inline fSetting bashStaminaMult{ "Bash"s, "StaminaMult"s, 0.025f };
	static inline fSetting bashBase{ "Bash"s, "Base"s, 3.4f };

	static inline fSetting attackTypeMult_powerattack{ "AttackTypeMults"s, "PowerAttack"s, 1.5f };
	static inline fSetting attackTypeMult_powerbash{ "AttackTypeMults"s, "PowerBash"s, 2.0f };


	static inline bSetting blockNPC{ "Block"s, "DenyNPC"s, true };              // Deny NPC block with low stamina
	static inline bSetting blockPlayer{ "Block"s, "DenyPlayer"s, true };        // Deny Player block with low stamina
	static inline bSetting blockCostNPC{ "Block"s, "DamageNPC"s, true };        // Damage NPC stamina every successful block
	static inline bSetting blockCostPlayer{ "Block"s, "DamagePlayer"s, true };  // Damage Player stamina every successful block
	static inline fSetting blockWeightMult{ "Block"s, "WeightMult"s, 1.0f };
	static inline fSetting blockStaminaMult{ "Block"s, "StaminaMult"s, 0.01f };
	static inline fSetting blockBase{ "Block"s, "Base"s, 1.0f };

	static inline bSetting rangedNPC{ "Bow"s, "DenyNPC"s, true };              // Deny NPC bow shot with low stamina
	static inline bSetting rangedPlayer{ "Bow"s, "DenyPlayer"s, true };        // Deny Player bow shot with low stamina
	static inline bSetting rangedCostNPC{ "Bow"s, "DamageNPC"s, true };        // Damage NPC stamina every bow shot
	static inline bSetting rangedCostPlayer{ "Bow"s, "DamagePlayer"s, true };  // Damage Player stamina every bow shot
	static inline fSetting rangedWeightMult{ "Bow"s, "WeightMult"s, 1.0f };
	static inline fSetting rangedStaminaMult{ "Bow"s, "StaminaMult"s, 0.01f };
	static inline fSetting rangedBase{ "Bow"s, "Base"s, 1.0f };
	static inline bSetting rangedKeepDamage{ "Bow"s, "KeepDamage"s, true };          // Damage stamina when the bow has a taut string
	static inline fSetting rangedKeepDamageRate{ "Bow"s, "KeepDamageRate"s, 8.0f };  // How much damage stamina when the bow has a taut string

	static inline bSetting jumpPlayer{ "Jumps"s, "Player"s, true };  // Deny Player jump with low stamina
	static inline bSetting jumpCost{ "Jumps"s, "Damage"s, true };    // Damage Player stamina every jump
	static inline fSetting jumpWeightMult{ "Jumps"s, "WeightMult"s, 1.0f };
	static inline fSetting jumpStaminaMult{ "Jumps"s, "StaminaMult"s, 0.01f };
	static inline fSetting jumpBase{ "Jumps"s, "Base"s, 1.0f };


	static inline bSetting runspeedNPC{ "Speed"s, "NPC"s, true };  // decrease movement speed
};

