

RE::NiPoint3 rotate(float r, const RE::NiPoint3& rotation)
{
	RE::NiPoint3 ans;

	float gamma = -rotation.z + 3.1415926f / 2, beta = rotation.x;
	float cos_g = cos(gamma);
	float sin_g = sin(gamma);
	float cos_b = cos(beta);
	float sin_b = sin(beta);

	ans.x = r * cos_g * cos_b;
	ans.y = r * sin_g * cos_b;
	ans.z = r * -sin_b;

	return ans;
}

RE::NiPoint3 rotateZ(float r, const RE::NiPoint3& rotation)
{
	RE::NiPoint3 ans;

	float gamma = -rotation.z + 3.1415926f / 2;
	float cos_g = cos(gamma);
	float sin_g = sin(gamma);

	ans.x = r * cos_g;
	ans.y = r * sin_g;
	ans.z = 0.0f;

	return ans;
}

RE::NiPoint3* Actor__get_eye_pos(RE::Actor* me, RE::NiPoint3* ans, int mb_type)
{
	using func_t = decltype(&Actor__get_eye_pos);
	REL::Relocation<func_t> func{ REL::ID(36755) };
	return func(me, ans, mb_type);
}

float get_dist2(RE::Actor* a, RE::Actor* b)
{
	return _generic_foo<46058, float, RE::Actor*, RE::Actor*>(a, b);
}
