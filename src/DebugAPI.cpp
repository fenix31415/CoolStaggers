#include "DebugAPI.h"
#include <windows.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace DebugAPI_Utils
{
	glm::vec3 RotateVector(glm::quat quatIn, glm::vec3 vecIn)
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

	glm::vec3 RotateVector(glm::vec3 eulerIn, glm::vec3 vecIn)
	{
		glm::vec3 glmVecIn(vecIn.x, vecIn.y, vecIn.z);
		glm::mat3 rotationMatrix = glm::eulerAngleXYZ(eulerIn.x, eulerIn.y, eulerIn.z);

		return rotationMatrix * glmVecIn;
	}

	glm::vec3 GetPointOnRotatedCircle(glm::vec3 origin, float radius, float i, float maxI, glm::vec3 eulerAngles)
	{
		float currAngle = (i / maxI) * glm::two_pi<float>();

		glm::vec3 targetPos(
			(radius * cos(currAngle)),
			(radius * sin(currAngle)),
			0.0f);

		auto targetPosRotated = RotateVector(eulerAngles, targetPos);

		return glm::vec3(targetPosRotated.x + origin.x, targetPosRotated.y + origin.y, targetPosRotated.z + origin.z);
	}

	bool IsRoughlyEqual(float first, float second, float maxDif)
	{
		return abs(first - second) <= maxDif;
	}

	glm::vec3 GetCameraPos()
	{
		auto playerCam = RE::PlayerCamera::GetSingleton();
		return glm::vec3(playerCam->pos.x, playerCam->pos.y, playerCam->pos.z);
	}

	glm::quat GetCameraRot()
	{
		auto playerCam = RE::PlayerCamera::GetSingleton();

		auto cameraState = playerCam->currentState.get();
		if (!cameraState)
			return glm::quat();

		RE::NiQuaternion niRotation;
		cameraState->GetRotation(niRotation);

		return glm::quat(niRotation.w, niRotation.x, niRotation.y, niRotation.z);
	}

	glm::vec3 NormalizeVector(glm::vec3 p)
	{
		return glm::normalize(p);
	}

	glm::vec3 GetForwardVector(glm::quat quatIn)
	{
		// rotate Skyrim's base forward vector (positive Y forward) by quaternion
		return RotateVector(quatIn, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	bool IsPosBehindPlayerCamera(glm::vec3 pos)
	{
		auto cameraPos = GetCameraPos();
		auto cameraRot = GetCameraRot();

		auto toTarget = NormalizeVector(pos - cameraPos);
		auto cameraForward = NormalizeVector(GetForwardVector(cameraRot));

		auto angleDif = abs(glm::length(toTarget - cameraForward));

		// root_two is the diagonal length of a 1x1 square. When comparing normalized forward
		// vectors, this accepts an angle of 90 degrees in all directions
		return angleDif > glm::root_two<float>();
	}
}

std::string to_string(const RE::NiPoint3& p)
{
	std::ostringstream ss;
	ss << "(" << p.x << ", " << p.y << ", " << p.z << ")";
	return ss.str();
}

std::vector<DebugAPILine*> DebugAPI::LinesToDraw;

bool DebugAPI::CachedMenuData;

float DebugAPI::ScreenResX;
float DebugAPI::ScreenResY;

DebugAPILine::DebugAPILine(glm::vec3 from, glm::vec3 to, glm::vec4 color, float lineThickness, unsigned __int64 destroyTickCount)
{
	From = from;
	To = to;
	Color = color;
	fColor = DebugAPI::RGBToHex(color);
	Alpha = color.a * 100.0f;
	LineThickness = lineThickness;
	DestroyTickCount = destroyTickCount;
}

void DebugAPI::DrawLineForMS(const glm::vec3& from, const glm::vec3& to, int liftetimeMS, const glm::vec4& color, float lineThickness)
{
	DebugAPILine* oldLine = GetExistingLine(from, to, color, lineThickness);
	if (oldLine) {
		oldLine->From = from;
		oldLine->To = to;
		oldLine->DestroyTickCount = GetTickCount64() + liftetimeMS;
		oldLine->LineThickness = lineThickness;
		return;
	}

	DebugAPILine* newLine = new DebugAPILine(from, to, color, lineThickness, GetTickCount64() + liftetimeMS);
	LinesToDraw.push_back(newLine);
}

void DebugAPI::Update()
{
	auto hud = GetHUD();
	if (!hud || !hud->uiMovie)
		return;

	CacheMenuData();
	ClearLines2D(hud->uiMovie);

	for (int i = 0; i < LinesToDraw.size(); i++) {
		DebugAPILine* line = LinesToDraw[i];

		DrawLine3D(hud->uiMovie, line->From, line->To, line->fColor, line->LineThickness, line->Alpha);

		if (GetTickCount64() > line->DestroyTickCount) {
			LinesToDraw.erase(LinesToDraw.begin() + i);
			delete line;

			i--;
			continue;
		}
	}
}

void DebugAPI::DrawSphere(glm::vec3 origin, float radius, int liftetimeMS, const glm::vec4& color, float lineThickness)
{
	DrawCircle(origin, radius, glm::vec3(0.0f, 0.0f, 0.0f), liftetimeMS, color, lineThickness);
	DrawCircle(origin, radius, glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f), liftetimeMS, color, lineThickness);
}

void DebugAPI::DrawCircle(glm::vec3 origin, float radius, glm::vec3 eulerAngles, int liftetimeMS, const glm::vec4& color, float lineThickness)
{
	glm::vec3 lastEndPos = DebugAPI_Utils::GetPointOnRotatedCircle(origin, radius, CIRCLE_NUM_SEGMENTS, (float)(CIRCLE_NUM_SEGMENTS - 1), eulerAngles);

	for (int i = 0; i <= CIRCLE_NUM_SEGMENTS; i++) {
		glm::vec3 currEndPos = DebugAPI_Utils::GetPointOnRotatedCircle(origin, radius, (float)i, (float)(CIRCLE_NUM_SEGMENTS - 1), eulerAngles);

		DrawLineForMS(
			lastEndPos,
			currEndPos,
			liftetimeMS,
			color,
			lineThickness);

		lastEndPos = currEndPos;
	}
}

DebugAPILine* DebugAPI::GetExistingLine(const glm::vec3& from, const glm::vec3& to, const glm::vec4& color, float lineThickness)
{
	for (int i = 0; i < LinesToDraw.size(); i++) {
		DebugAPILine* line = LinesToDraw[i];

		if (
			DebugAPI_Utils::IsRoughlyEqual(from.x, line->From.x, DRAW_LOC_MAX_DIF) &&
			DebugAPI_Utils::IsRoughlyEqual(from.y, line->From.y, DRAW_LOC_MAX_DIF) &&
			DebugAPI_Utils::IsRoughlyEqual(from.z, line->From.z, DRAW_LOC_MAX_DIF) &&
			DebugAPI_Utils::IsRoughlyEqual(to.x, line->To.x, DRAW_LOC_MAX_DIF) &&
			DebugAPI_Utils::IsRoughlyEqual(to.y, line->To.y, DRAW_LOC_MAX_DIF) &&
			DebugAPI_Utils::IsRoughlyEqual(to.z, line->To.z, DRAW_LOC_MAX_DIF) &&
			DebugAPI_Utils::IsRoughlyEqual(lineThickness, line->LineThickness, DRAW_LOC_MAX_DIF) &&
			color == line->Color) {
			return line;
		}
	}

	return nullptr;
}

void DebugAPI::DrawLine3D(RE::GPtr<RE::GFxMovieView> movie, glm::vec3 from, glm::vec3 to, float color, float lineThickness, float alpha)
{
	if (DebugAPI_Utils::IsPosBehindPlayerCamera(from) && DebugAPI_Utils::IsPosBehindPlayerCamera(to))
		return;

	glm::vec2 screenLocFrom = WorldToScreenLoc(movie, from);
	glm::vec2 screenLocTo = WorldToScreenLoc(movie, to);
	DrawLine2D(movie, screenLocFrom, screenLocTo, color, lineThickness, alpha);
}

void DebugAPI::DrawLine3D(RE::GPtr<RE::GFxMovieView> movie, glm::vec3 from, glm::vec3 to, glm::vec4 color, float lineThickness)
{
	DrawLine3D(movie, from, to, RGBToHex(glm::vec3(color.r, color.g, color.b)), lineThickness, color.a * 100.0f);
}

void DebugAPI::DrawLine2D(RE::GPtr<RE::GFxMovieView> movie, glm::vec2 from, glm::vec2 to, float color, float lineThickness, float alpha)
{
	// all parts of the line are off screen - don't need to draw them
	if (!IsOnScreen(from, to))
		return;

	FastClampToScreen(from);
	FastClampToScreen(to);

	// lineStyle(thickness:Number = NaN, color : uint = 0, alpha : Number = 1.0, pixelHinting : Boolean = false,
	// scaleMode : String = "normal", caps : String = null, joints : String = null, miterLimit : Number = 3) :void
	//
	// CapsStyle values: 'NONE', 'ROUND', 'SQUARE'
	// const char* capsStyle = "NONE";

	RE::GFxValue argsLineStyle[3]{ lineThickness, color, alpha };
	movie->Invoke("lineStyle", nullptr, argsLineStyle, 3);

	RE::GFxValue argsStartPos[2]{ from.x, from.y };
	movie->Invoke("moveTo", nullptr, argsStartPos, 2);

	RE::GFxValue argsEndPos[2]{ to.x, to.y };
	movie->Invoke("lineTo", nullptr, argsEndPos, 2);

	movie->Invoke("endFill", nullptr, nullptr, 0);
}

void DebugAPI::DrawLine2D(RE::GPtr<RE::GFxMovieView> movie, glm::vec2 from, glm::vec2 to, glm::vec4 color, float lineThickness)
{
	DrawLine2D(movie, from, to, RGBToHex(glm::vec3(color.r, color.g, color.b)), lineThickness, color.a * 100.0f);
}

void DebugAPI::ClearLines2D(RE::GPtr<RE::GFxMovieView> movie)
{
	movie->Invoke("clear", nullptr, nullptr, 0);
}

RE::GPtr<RE::IMenu> DebugAPI::GetHUD()
{
	RE::GPtr<RE::IMenu> hud = RE::UI::GetSingleton()->GetMenu(DebugOverlayMenu::MENU_NAME);
	return hud;
}

float DebugAPI::RGBToHex(glm::vec3 rgb)
{
	return ConvertComponentR(rgb.r * 255) + ConvertComponentG(rgb.g * 255) + ConvertComponentB(rgb.b * 255);
}

// if drawing outside the screen rect, at some point Scaleform seems to start resizing the rect internally, without
// increasing resolution. This will cause all line draws to become more and more pixelated and increase in thickness
// the farther off screen even one line draw goes. I'm allowing some leeway, then I just clamp the
// coordinates to the screen rect.
//
// this is inaccurate. A more accurate solution would require finding the sub vector that overshoots the screen rect between
// two points and scale the vector accordingly. Might implement that at some point, but the inaccuracy is barely noticeable
const float CLAMP_MAX_OVERSHOOT = 10000.0f;
void DebugAPI::FastClampToScreen(glm::vec2& point)
{
	if (point.x < 0.0) {
		float overshootX = abs(point.x);
		if (overshootX > CLAMP_MAX_OVERSHOOT)
			point.x += overshootX - CLAMP_MAX_OVERSHOOT;
	} else if (point.x > ScreenResX) {
		float overshootX = point.x - ScreenResX;
		if (overshootX > CLAMP_MAX_OVERSHOOT)
			point.x -= overshootX - CLAMP_MAX_OVERSHOOT;
	}

	if (point.y < 0.0) {
		float overshootY = abs(point.y);
		if (overshootY > CLAMP_MAX_OVERSHOOT)
			point.y += overshootY - CLAMP_MAX_OVERSHOOT;
	} else if (point.y > ScreenResY) {
		float overshootY = point.y - ScreenResY;
		if (overshootY > CLAMP_MAX_OVERSHOOT)
			point.y -= overshootY - CLAMP_MAX_OVERSHOOT;
	}
}

float DebugAPI::ConvertComponentR(float value)
{
	return (value * 0xffff) + value;
}

float DebugAPI::ConvertComponentG(float value)
{
	return (value * 0xff) + value;
}

float DebugAPI::ConvertComponentB(float value)
{
	return value;
}

glm::vec2 DebugAPI::WorldToScreenLoc(RE::GPtr<RE::GFxMovieView> movie, glm::vec3 worldLoc)
{
	glm::vec2 screenLocOut;
	RE::NiPoint3 niWorldLoc(worldLoc.x, worldLoc.y, worldLoc.z);

	float zVal;

	RE::NiCamera::WorldPtToScreenPt3((float(*)[4])(REL::ID(519579).address()), *((RE::NiRect<float>*)REL::ID(519618).address()), niWorldLoc, screenLocOut.x, screenLocOut.y, zVal, 1e-5f);
	RE::GRectF rect = movie->GetVisibleFrameRect();

	screenLocOut.x = rect.left + (rect.right - rect.left) * screenLocOut.x;
	screenLocOut.y = 1.0f - screenLocOut.y;  // Flip y for Flash coordinate system
	screenLocOut.y = rect.top + (rect.bottom - rect.top) * screenLocOut.y;

	return screenLocOut;
}

DebugOverlayMenu::DebugOverlayMenu()
{
	auto scaleformManager = RE::BSScaleformManager::GetSingleton();

	inputContext = Context::kNone;
	depthPriority = 127;

	menuFlags.set(RE::UI_MENU_FLAGS::kRequiresUpdate);
	menuFlags.set(RE::UI_MENU_FLAGS::kAllowSaving);
	menuFlags.set(RE::UI_MENU_FLAGS::kCustomRendering);

	scaleformManager->LoadMovieEx(this, MENU_PATH, [](RE::GFxMovieDef* a_def) -> void {
		a_def->SetState(RE::GFxState::StateType::kLog,
			RE::make_gptr<Logger>().get());
	});
}

void DebugOverlayMenu::Register()
{
	auto ui = RE::UI::GetSingleton();
	if (ui) {
		ui->Register(MENU_NAME, Creator);

		DebugOverlayMenu::Show();
	}
}

void DebugOverlayMenu::Show()
{
	auto msgQ = RE::UIMessageQueue::GetSingleton();
	if (msgQ) {
		msgQ->AddMessage(DebugOverlayMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
	}
}

void DebugOverlayMenu::Hide()
{
	auto msgQ = RE::UIMessageQueue::GetSingleton();
	if (msgQ) {
		msgQ->AddMessage(DebugOverlayMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
	}
}

void DebugAPI::CacheMenuData()
{
	if (CachedMenuData)
		return;

	RE::GPtr<RE::IMenu> menu = RE::UI::GetSingleton()->GetMenu(DebugOverlayMenu::MENU_NAME);
	if (!menu || !menu->uiMovie)
		return;

	RE::GRectF rect = menu->uiMovie->GetVisibleFrameRect();

	ScreenResX = abs(rect.left - rect.right);
	ScreenResY = abs(rect.top - rect.bottom);

	CachedMenuData = true;
}

bool DebugAPI::IsOnScreen(glm::vec2 from, glm::vec2 to)
{
	return IsOnScreen(from) || IsOnScreen(to);
}

bool DebugAPI::IsOnScreen(glm::vec2 point)
{
	return (point.x <= ScreenResX && point.x >= 0.0 && point.y <= ScreenResY && point.y >= 0.0);
}

void DebugOverlayMenu::AdvanceMovie(float a_interval, std::uint32_t a_currentTime)
{
	RE::IMenu::AdvanceMovie(a_interval, a_currentTime);

	DebugAPI::Update();
}

RE::NiPoint3 TransformVectorByMatrix(const RE::NiPoint3& a_vector, const RE::NiMatrix3& a_matrix)
{
	return RE::NiPoint3(a_matrix.entry[0][0] * a_vector.x + a_matrix.entry[0][1] * a_vector.y + a_matrix.entry[0][2] * a_vector.z,
		a_matrix.entry[1][0] * a_vector.x + a_matrix.entry[1][1] * a_vector.y + a_matrix.entry[1][2] * a_vector.z,
		a_matrix.entry[2][0] * a_vector.x + a_matrix.entry[2][1] * a_vector.y + a_matrix.entry[2][2] * a_vector.z);
}
