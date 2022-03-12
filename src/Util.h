#pragma once
#include "PCH.h"

#include <vector>

#include "RE/N/NiMath.h"



/*
enum EAxis
{
	AxisX,
	AxisY,
	AxisZ
};

struct RayCastData
{
	RayCastData(glm::vec4 startPos, glm::vec4 endPos)
	{
		StartPos = startPos;
		EndPos = endPos;
	}

	glm::vec4 StartPos;
	glm::vec4 EndPos;
};

namespace Util
{
	/*
	vector translation notes

	Euler Angles:
		x = PITCH
		y = ROLL
		z = YAW

	Position:
		x = RIGHT
		y = FORWARD
		z = UP
	

	// inserts value into a vector, keeping the vector sorted
	template <typename T, typename Pred>
	typename std::vector<T>::iterator
		insert_sorted(std::vector<T>& vec, T const& item, Pred pred)
	{
		return vec.insert(
			std::upper_bound(vec.begin(), vec.end(), item, pred),
			item);
	}

	glm::highp_mat4 GetRotationMatrix(glm::vec3 eulerAngles);

	glm::vec3 NormalizeVector(glm::vec3 p);
	glm::vec3 RotateVector(glm::quat quatIn, glm::vec3 vecIn);
	glm::vec3 RotateVector(glm::vec3 eulerIn, glm::vec3 vecIn);
	glm::vec3 GetForwardVector(glm::quat quatIn);
	glm::vec3 GetForwardVector(glm::vec3 eulerIn);
	glm::vec3 GetRightVector(glm::quat quatIn);
	glm::vec3 GetRightVector(glm::vec3 eulerIn);

	glm::vec3 ThreeAxisRotation(float r11, float r12, float r21, float r31, float r32);

	glm::vec3 RotMatrixToEuler(RE::NiMatrix3 matrixIn);

	// attempts to find the object's spine node, returns the root node if none is found
	RE::NiAVObject* GetCharacterSpine(RE::TESObjectREFR* object);
	// attempts to find the object's head node, returns the root node if none is found
	RE::NiAVObject* GetCharacterHead(RE::TESObjectREFR* object);

	bool IsRoughlyEqual(float first, float second, float maxDif);

	glm::vec3 QuatToEuler(glm::quat q);

	glm::quat EulerToQuat(glm::vec3 rotIn);

	glm::vec3 GetInverseRotation(glm::vec3 rotIn);
	glm::quat GetInverseRotation(glm::quat rotIn);

	glm::vec3 EulerRotationToVector(glm::vec3);
	glm::vec3 VectorToEulerRotation(glm::vec3);
	glm::vec3 GetCameraPos();
	glm::quat GetCameraRot();

	bool IsPosBehindPlayerCamera(glm::vec3 pos);

	// given a world coordinate and a radius, this returns a point on a rotated circle, with values of i
	// ranging between 0 and maxI to determine the point on the circle
	glm::vec3 GetPointOnRotatedCircle(glm::vec3 origin, float radius, float i, float maxI, glm::vec3 eulerAngles);

	// the Skyrim ref's position doesn't always update with its Havok mesh. This is a workaround for this bug
	// it returns the position of the havok mesh, or the vanilla position if no mesh is present
	glm::vec3 GetObjectAccuratePosition(RE::TESObjectREFR* object);

	constexpr int FOLLOW_PARENT_MAX_RECURSION = 2;

	// for distance checks toward all nearby objects, first a rough check toward the origin is performed, with an added
	// threshhold of this. If this succeeds, a more precise check will be performed against the BB center of the object
	// this avoids making BB calculations for every object in the cell(s)
	constexpr float ROUGH_DISTANCE_ADD = 250.0f;

	// values smaller / larger than this will not be counted as valid by IsValid
	constexpr float POSITIVE_INVALID_THRESHHOLD = 999999.0f;
	constexpr float NEGATIVE_INVALID_THRESHHOLD = -999999.0f;
};
*/
