#include "Utils.h"

namespace Formulas
{
	using Plane = PlaneUtils::Plane;
	using Line = PlaneUtils::Line;
	using Parab = PlaneUtils::Parab;

	constexpr Plane ARMO_ALPHA = { -90.0f, 90.0f, 360.0f + 90.0f };
	constexpr Plane WEAP_MIN = { 0.2f, 0.2f, -0.2f };
	constexpr Plane WEAP_MAX = { 0.1f, 0.2f, 0.2f };

	constexpr Parab WEAP_WEIGHT = { 0.0025f, 0.075f, 0.0f };
	constexpr Parab ARMO_WEIGHT = { -0.002587991718f, 0.1609730849f, 0.0f };

	static float lerp(float x, float minX, float maxX, float minY, float maxY)
	{
		if (maxX == minX)
			return 0.0f;

		return (x - minX) / (maxX - minX) * (maxY - minY) + minY;
	}

	template <int id, typename T, typename... Args>
	T _generic_foo(Args... args)
	{
		using func_t = T(Args...);
		REL::Relocation<func_t> func{ REL::ID(id) };
		return func(std::forward<Args>(args)...);
	}

	static RE::BGSAttackData* get_attackData(RE::Actor* a)
	{
		auto ptr = _generic_foo<37625, RE::NiPointer<RE::BGSAttackData>*, RE::Actor*>(a);
		if (ptr)
			return ptr->get();
		else
			return nullptr;
	}

	static float get_alpha(float victim_mass, float attack)
	{
		float alpha = ARMO_ALPHA(victim_mass, attack);
		if (alpha < 0.0f)
			alpha = 0.0f;
		if (alpha > 360.0f)
			alpha = 360.0f;
		return alpha;
	}

	static float get_stagger(float angle, float victim_mass, float attacker_mass, float attack)
	{
		float alpha = get_alpha(victim_mass, attack) / 2.0f;
		float min = WEAP_MIN(attacker_mass, attack);
		float max = WEAP_MAX(attacker_mass, attack);

		float ans = lerp(angle, 0.0f, alpha, max, min);
		if (ans < min || ans < 0.0f)
			ans = 0.0f;

		logger::info("victim_mass={}, attacker_mass={}, alpha={}, min={}, max={}, angle={}, ans={}", victim_mass, attacker_mass, alpha, min, max, angle, ans);

		return ans;
	}

	template <typename T>
	static float get_mass(RE::Actor* a, T* item, const Parab& p)
	{
		auto ans = a->GetActorValue(RE::ActorValue::kMass);
		if (item)
			ans += p(item->GetWeight());
		if (ans < 0.1f)
			ans = 0.1f;
		return ans;
	}

	float get_weapon_stagger(char* _attacker, char* _victim, RE::TESObjectWEAP* weap, float default_mult)
	{
		// yes i can stagger mamonth by spell
		if (default_mult != 0.0f)
			return default_mult;

		auto attacker = reinterpret_cast<RE::Actor*>(_attacker - 0xB0);
		auto victim = reinterpret_cast<RE::Actor*>(_victim - 0xB0);

		auto PI = 3.1415926f;
		auto angle = abs(attacker->GetHeading(true) - victim->GetHeading(true));

		if (angle > PI)
			angle = 2 * PI - angle;

		angle = angle * 180.0f / PI;

		auto victim_mass = get_mass(victim, victim->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kBody), ARMO_WEIGHT);
		auto attacker_mass = get_mass(attacker, weap, WEAP_WEIGHT);

		auto attack = 0.0f;
		auto attackdata = get_attackData(attacker);
		if (attackdata)
			attack = attackdata->data.staggerOffset;

		return get_stagger(angle, victim_mass, attacker_mass, attack);
	}
}
