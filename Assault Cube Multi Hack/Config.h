#pragma once

namespace Config
{
	namespace ESP {
		inline bool checkVisibility = false;

		inline bool nameESP = false;
		inline bool boxESP = false;
		inline bool filledBoxESP = false;
		inline bool tracerESP = false;
		inline bool headESP = false;
		inline bool barESP = false;
		inline bool rotLinesESP = false;

		inline ImColor friendlyColor = ImColor(0, 0, 255);
		inline ImColor friendlyFilledColor = ImColor(0, 0, 255, 40);
		inline ImColor enemyColor = ImColor(255, 0, 0);
		inline ImColor enemyFilledColor = ImColor(255, 0, 0, 40);
	}

	namespace Aimbot
	{
		inline bool aimbotON = false;

		inline bool checkVisibility = true;

		inline bool targetTeam = true;

		inline bool drawFOVCircle = false;
		inline float aimbotFOV = 60;
		inline ImColor circleColor = ImColor(255, 255, 255);
		inline ImColor circleFilledColor = ImColor(255, 255, 255, 30);

		inline uintptr_t aimbotKey = VK_LCONTROL;
	}

	namespace TriggerBot
	{
		inline bool triggerBotON = false;
	}
}