#pragma once
#include <cstdint>

#include "Includes/imgui/imgui.h"
#include "Includes/imgui/ImVec3.h"

#include "Math/Math.h"

struct TraceResult
{
	ImVec3 point;
	bool collided;
};

class Weapond
{
public:
	char pad_0000[4]; //0x0000
	int32_t weapondID; //0x0004
	char pad_0008[8]; //0x0008
	class WeapondObject* weapondObject; //0x0010
	char pad_0014[48]; //0x0014
}; //Size: 0x0044

class WeapondObject
{
public:
	int32_t ammoReserve; //0x0000
	char pad_0004[36]; //0x0004
	int32_t ammoClip; //0x0028
	char pad_002C[24]; //0x002C
}; //Size: 0x0044


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
	char pad_0100[292]; //0x0100
	int8_t isAttacking; //0x0224
	char name[16]; //0x0225
	char pad_0235[247]; //0x0235
	int8_t team; //0x032C
	char pad_032D[71]; //0x032D
	class Weapond* currentWeapond; //0x0374
	char pad_0378[3362]; //0x0378




public:
	float GetDist(ImVec3 otherPos);
	Entity* GetClosestEntity();
	Entity* GetClosestEnttiyToCrossHair();
	Entity* GetEntityUnderCrossHair();
	void AimAt(ImVec3 pos);
	void GrenadeAimAt(Entity* target);
	bool IsVisible();

public:
	static Entity* GetLocal();
	static Entity* GetEntity(int index);
	static int GetPlayerCount();
	static ViewMatrix GetMatrix();
}; //Size: 0x1054