#include "Classes.h"
#include "Utilities.h"

#include <Windows.h>
#include <iostream>

#include "Config.h"

Entity* Entity::GetLocal() {
	uintptr_t modBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");
	Entity* p = (Entity*)*(uintptr_t*)(modBase + 0x10F4F4);

	return p;
}

Entity* Entity::GetEntity(int index) {
	uintptr_t modBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");
	uintptr_t entityList = *(uintptr_t*)(modBase + 0x10F4F8);

	return (Entity*)*(uintptr_t*)(entityList + (index * 0x4));
}

int Entity::GetPlayerCount() {
	uintptr_t modBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");
	int count = *(int*)(uintptr_t*)(modBase + 0x10F500);

	return count;
}

float Entity::GetDist(ImVec3 otherPos) {
	ImVec3 delta = otherPos - this->bodyPosition;

	return delta.Length();
}

Entity* Entity::GetClosestEntity() {
    float closestDitance = 1000000;
    int closesDistanceIndex = -1;

    for (int i = 1; i < this->GetPlayerCount(); i++)
    {
        Entity* player = Entity::GetEntity(i);

        if (!player || player == NULL || player == nullptr)
            continue;

        if ((uintptr_t)player == (uintptr_t)this)
            continue;

        if (player->health < 1)
            continue;

        if (this->GetDist(player->bodyPosition) < closestDitance) {
            closestDitance = this->GetDist(player->bodyPosition);
            closesDistanceIndex = i;
        }
    }  

    if (closesDistanceIndex == -1)
        return NULL;

    return this->GetEntity(closesDistanceIndex);
}

Entity* Entity::GetClosestEnttiyToCrossHair() {
    float closestDitance = 1000000;
    int closesDistanceIndex = -1;

    ViewMatrix matrix = this->GetMatrix();
    ImVec2 windowRes = Utils::GetWindowRes();

    for (int i = 1; i < this->GetPlayerCount(); i++)
    {
        Entity* player = Entity::GetEntity(i);

        if (!player || player == NULL || player == nullptr)
            continue;

        if ((uintptr_t)player == (uintptr_t)this)
            continue;

        if (player->health < 1)
            continue;

        ImVec3 headPos;
        if (!Math::WorldToScreen(player->headPosition, headPos, matrix.matrix, windowRes.x, windowRes.y))
            continue;

        float dist = sqrt(pow(headPos.x - (windowRes.x / 2), 2) + pow(headPos.y - (windowRes.y / 2), 2));

        if (dist > Config::Aimbot::aimbotFOV)
            continue;

        if (dist < closestDitance) {
            closestDitance = dist;
            closesDistanceIndex = i;
        }
    }

    if (closesDistanceIndex == -1)
        return NULL;

    return this->GetEntity(closesDistanceIndex);
}

void Entity::AimAt(ImVec3 aimPos) {
    ImVec2 angle = Math::CalcAngle(this->headPosition, aimPos);

    this->viewAngles.x = angle.x + 180;
    this->viewAngles.y = angle.y;
}

void Entity::GrenadeAimAt(ImVec3 aimPos) {
    float dist = this->headPosition.DistTo(aimPos);
    ImVec3 pos = aimPos + ImVec3(0, 0, dist / 3.2);
    ImVec2 angle = Math::CalcAngle(this->headPosition, pos);

    this->viewAngles.x = angle.x + 180;
    this->viewAngles.y = angle.y;
}

bool Entity::IsVisible() {
    uintptr_t traceLine = 0x048A310;
    _trace_result traceresult;
    traceresult.collided = false;

    Entity* localPlayer = this->GetLocal();

    ImVec3 localHead = localPlayer->headPosition;
    ImVec3 desiredHead = this->headPosition;

    __asm
    {
        push 0
        push 0
        push localPlayer
        push desiredHead.z
        push desiredHead.y
        push desiredHead.x
        push localHead.z
        push localHead.y
        push localHead.x
        lea eax, [traceresult]
        call traceLine;
        add esp, 36
    }

    return !traceresult.collided;
}

ViewMatrix Entity::GetMatrix() {
    ViewMatrix matrix;

    for (unsigned int i = 0; i < 16; ++i) {
        matrix.matrix[i] = *(float*)(0x501AE8 + (i * 4));
    }

    return matrix;
}