#include "../Includes/imgui/ImVec3.h"

struct ViewMatrix
{
	float matrix[16];
};

class Math
{
public:
	static ImVec2 CalcAngle(ImVec3 src, ImVec3 dst);
	static bool WorldToScreen(ImVec3 pos, ImVec3& screen, float matrix[16], int windowWidth, int windowHeight);
	static ImVec3 GrenadePredict(ImVec3 from, ImVec3 to, ImVec3 vel, ImVec2 angle);
};