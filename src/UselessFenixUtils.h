#pragma once

template <int id, typename T, typename... Args>
T _generic_foo(Args... args)
{
	using func_t = T(Args...);
	REL::Relocation<func_t> func{ REL::ID(id) };
	return func(std::forward<Args>(args)...);
}

template <int ID, int offset = 0x0>
void writebytes(const std::string_view& data)
{
	REL::safe_write(REL::ID(ID).address() + offset, data.data(), data.size());
}

/// <summary>
/// rotates vector V to rotation vector
/// </summary>
/// <param name="r">V = (r, 0, 0)</param>
/// <param name="rotation">vert (pi/2..-pi/2), 0, horiz (0..2pi, clock, 0 in OY)</param>
RE::NiPoint3 rotate(float r, const RE::NiPoint3& rotation);

/// <summary>
/// rotates vector V to rotation vector (uses only Z)
/// </summary>
RE::NiPoint3 rotateZ(float r, const RE::NiPoint3& rotation);

RE::NiPoint3* Actor__get_eye_pos(RE::Actor* me, RE::NiPoint3* ans, int mb_type);

float get_dist2(RE::Actor* a, RE::Actor* b);
