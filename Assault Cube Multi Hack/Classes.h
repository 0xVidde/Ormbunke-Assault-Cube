#pragma once
#include <cstdint>

#include "Includes/imgui/imgui.h"
#include "Includes/imgui/ImVec3.h"

#include "Math/Math.h"

struct _trace_result
{
	ImVec3 end;
	bool collided;
};

class Entity
{
public:
	char pad_0000[4]; //0x0000
	ImVec3 headPosition; //0x0004
	ImVec3 velocity; //0x0010
	char pad_001C[24]; //0x001C
	ImVec3 bodyPosition; //0x0034
	ImVec2 viewAngles; //0x0040
	char pad_0048[176]; //0x0048
	int32_t health; //0x00F8
	int32_t armor; //0x00FC
	char pad_0100[293]; //0x0100
	char name[16]; //0x0225
	char pad_0235[247]; //0x0235
	int32_t team; //0x032C
	char pad_0330[3449]; //0x0330



public:
	float GetDist(ImVec3 otherPos);
	Entity* GetClosestEntity();
	Entity* GetClosestEnttiyToCrossHair();
	void AimAt(ImVec3 pos);
	void GrenadeAimAt(ImVec3 pos);
	bool IsVisible();

public:
	static Entity* GetLocal();
	static Entity* GetEntity(int index);
	static int GetPlayerCount();
	static ViewMatrix GetMatrix();
}; //Size: 0x1054