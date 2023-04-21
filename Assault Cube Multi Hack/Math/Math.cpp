#include "Math.h"

ImVec2 Math::CalcAngle(ImVec3 src, ImVec3 dst) {
	ImVec2 angle;
	angle.x = -atan2f(dst.x - src.x, dst.y - src.y) / (float)3.141 * 180;
	angle.y = asinf((dst.z - src.z) / src.DistTo(dst)) * 180 / (float)3.141;

	return angle;
}

bool Math::WorldToScreen(ImVec3 pos, ImVec3& screen, float matrix[16], int windowWidth, int windowHeight)
{
    ImVec4 clipCoords;
    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
    clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
    clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
    clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

    if (clipCoords.w < 0.1f)
        return false;

    ImVec3 NDC;
    NDC.x = clipCoords.x / clipCoords.w;
    NDC.y = clipCoords.y / clipCoords.w;
    NDC.z = clipCoords.z / clipCoords.w;

    screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
    screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
    return true;
}

ImVec3 Math::GrenadePredict(ImVec3 from, ImVec3 to, ImVec3 vel, ImVec2 angle) {
    float grenadeSpeed = 4;

    float dist = from.DistTo(to);

    ImVec3 angles = ImVec3(angle.y, angle.x - 90, 0);
    ImVec3 endPoint = to.Transform(angles, dist / 5);

    ImVec3 pos = ImVec3(endPoint.x + (vel.x * 3), endPoint.y + (vel.y * 3), endPoint.z + dist / 1.4 / grenadeSpeed);

    return pos;
}