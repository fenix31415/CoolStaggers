#pragma once
#include "Util.h"
#include "DebugAPI.h"
#include "RE/RTTI.h"

/*
glm::highp_mat4 Util::GetRotationMatrix(glm::vec3 eulerAngles)
{
	return glm::eulerAngleXYZ(-(eulerAngles.x), -(eulerAngles.y), -(eulerAngles.z));
}

glm::vec3 Util::NormalizeVector(glm::vec3 p)
{
	return glm::normalize(p);
}

glm::vec3 Util::RotateVector(glm::quat quatIn, glm::vec3 vecIn)
{
	float num = quatIn.x * 2.0f;
	float num2 = quatIn.y * 2.0f;
	float num3 = quatIn.z * 2.0f;
	float num4 = quatIn.x * num;
	float num5 = quatIn.y * num2;
	float num6 = quatIn.z * num3;
	float num7 = quatIn.x * num2;
	float num8 = quatIn.x * num3;
	float num9 = quatIn.y * num3;
	float num10 = quatIn.w * num;
	float num11 = quatIn.w * num2;
	float num12 = quatIn.w * num3;
	glm::vec3 result;
	result.x = (1.0f - (num5 + num6)) * vecIn.x + (num7 - num12) * vecIn.y + (num8 + num11) * vecIn.z;
	result.y = (num7 + num12) * vecIn.x + (1.0f - (num4 + num6)) * vecIn.y + (num9 - num10) * vecIn.z;
	result.z = (num8 - num11) * vecIn.x + (num9 + num10) * vecIn.y + (1.0f - (num4 + num5)) * vecIn.z;
	return result;
}

glm::vec3 Util::RotateVector(glm::vec3 eulerIn, glm::vec3 vecIn)
{
	glm::vec3 glmVecIn(vecIn.x, vecIn.y, vecIn.z);
	glm::mat3 rotationMatrix = glm::eulerAngleXYZ(eulerIn.x, eulerIn.y, eulerIn.z);

	return rotationMatrix * glmVecIn;
}

glm::vec3 Util::GetForwardVector(glm::quat quatIn)
{
	// rotate Skyrim's base forward vector (positive Y forward) by quaternion
	return RotateVector(quatIn, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Util::GetForwardVector(glm::vec3 eulerIn)
{
	float pitch = eulerIn.x;
	float yaw = eulerIn.z;

	return glm::vec3(
		sin(yaw) * cos(pitch),
		cos(yaw) * cos(pitch),
		sin(pitch));
}

glm::vec3 Util::GetRightVector(glm::quat quatIn)
{
	// rotate Skyrim's base right vector (positive X forward) by quaternion
	return RotateVector(quatIn, glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 Util::GetRightVector(glm::vec3 eulerIn)
{
	float pitch = eulerIn.x;
	float yaw = eulerIn.z + glm::half_pi<float>();

	return glm::vec3(
		sin(yaw) * cos(pitch),
		cos(yaw) * cos(pitch),
		sin(pitch));
}

glm::vec3 Util::ThreeAxisRotation(float r11, float r12, float r21, float r31, float r32)
{
	return glm::vec3(
		asin(r21),
		atan2(r11, r12),
		atan2(-r31, r32));
}

glm::vec3 Util::RotMatrixToEuler(RE::NiMatrix3 matrixIn)
{
	auto ent = matrixIn.entry;
	auto rotMat = glm::mat4(
		{
			ent[0][0], ent[1][0], ent[2][0],
			ent[0][1], ent[1][1], ent[2][1],
			ent[0][2], ent[1][2], ent[2][2]
		}
	);

	glm::vec3 rotOut;
	glm::extractEulerAngleXYZ(rotMat, rotOut.x, rotOut.y, rotOut.z);

	return rotOut;
}

constexpr int FIND_COLLISION_MAX_RECURSION = 2;

RE::NiAVObject* Util::GetCharacterSpine(RE::TESObjectREFR* object)
{
	auto characterObject = object->GetObjectReference()->As<RE::TESNPC>();
	auto mesh = object->GetCurrent3D();

	if (characterObject && mesh)
	{
		auto spineNode = mesh->GetObjectByName("NPC Spine [Spn0]");
		if (spineNode)
			return spineNode;
	}

	return mesh;
}

RE::NiAVObject* Util::GetCharacterHead(RE::TESObjectREFR* object)
{
	auto characterObject = object->GetObjectReference()->As<RE::TESNPC>();
	auto mesh = object->GetCurrent3D();

	if (characterObject && mesh)
	{
		auto spineNode = mesh->GetObjectByName("NPC Head [Head]");
		if (spineNode)
			return spineNode;
	}

	return mesh;
}

bool Util::IsRoughlyEqual(float first, float second, float maxDif)
{
	return abs(first - second) <= maxDif;
}

glm::vec3 Util::QuatToEuler(glm::quat q)
{
	auto matrix = glm::toMat4(q);

	glm::vec3 rotOut;
	glm::extractEulerAngleXYZ(matrix, rotOut.x, rotOut.y, rotOut.z);

	return rotOut;
}

glm::quat Util::EulerToQuat(glm::vec3 rotIn)
{
	auto matrix = glm::eulerAngleXYZ(rotIn.x, rotIn.y, rotIn.z);
	return glm::toQuat(matrix);
}

glm::vec3 Util::GetInverseRotation(glm::vec3 rotIn)
{
	auto matrix = glm::eulerAngleXYZ(rotIn.y, rotIn.x, rotIn.z);
	auto inverseMatrix = glm::inverse(matrix);

	glm::vec3 rotOut;
	glm::extractEulerAngleYXZ(inverseMatrix, rotOut.x, rotOut.y, rotOut.z);
	return rotOut;
}

glm::quat Util::GetInverseRotation(glm::quat rotIn)
{
	return glm::inverse(rotIn);
}

glm::vec3 Util::EulerRotationToVector(glm::vec3 rotIn)
{
	return glm::vec3(
		cos(rotIn.y) * cos(rotIn.x),
		sin(rotIn.y) * cos(rotIn.x),
		sin(rotIn.x));
}

glm::vec3 Util::VectorToEulerRotation(glm::vec3 vecIn)
{
	float yaw = atan2(vecIn.x, vecIn.y);
	float pitch = atan2(vecIn.z, sqrt((vecIn.x * vecIn.x) + (vecIn.y * vecIn.y)));

	return glm::vec3(pitch, 0.0f, yaw);
}

glm::vec3 Util::GetCameraPos()
{
	auto playerCam = RE::PlayerCamera::GetSingleton();
	return glm::vec3(playerCam->pos.x, playerCam->pos.y, playerCam->pos.z);
}

glm::quat Util::GetCameraRot()
{
	auto playerCam = RE::PlayerCamera::GetSingleton();

	auto cameraState = playerCam->currentState.get();
	if (!cameraState)
		return glm::quat();

	RE::NiQuaternion niRotation;
	cameraState->GetRotation(niRotation);

	return glm::quat(niRotation.w, niRotation.x, niRotation.y, niRotation.z);
}

bool Util::IsPosBehindPlayerCamera(glm::vec3 pos)
{
	auto cameraPos = GetCameraPos();
	auto cameraRot = GetCameraRot();

	auto toTarget = Util::NormalizeVector(pos - cameraPos);
	auto cameraForward = Util::NormalizeVector(Util::GetForwardVector(cameraRot));

	auto angleDif = abs( glm::length(toTarget - cameraForward) );

	// root_two is the diagonal length of a 1x1 square. When comparing normalized forward
	// vectors, this accepts an angle of 90 degrees in all directions
	return angleDif > glm::root_two<float>();
}

glm::vec3 Util::GetPointOnRotatedCircle(glm::vec3 origin, float radius, float i, float maxI, glm::vec3 eulerAngles)
{
	float currAngle = (i / maxI) * glm::two_pi<float>();

	glm::vec3 targetPos(
		(radius * cos(currAngle)),
		(radius * sin(currAngle)),
		0.0f
	);

	auto targetPosRotated = Util::RotateVector(eulerAngles, targetPos);

	return glm::vec3(targetPosRotated.x + origin.x, targetPosRotated.y + origin.y , targetPosRotated.z + origin.z);
}

glm::vec3 Util::GetObjectAccuratePosition(RE::TESObjectREFR* object)
{
	auto mesh = object->GetCurrent3D();

	// backup, if no mesh is found
	if (!mesh)
	{
		auto niPos = object->GetPosition();
		return glm::vec3(niPos.x, niPos.y, niPos.z);
	}

	auto niPos = mesh->world.translate;
	return glm::vec3(niPos.x, niPos.y, niPos.z);
}
*/
