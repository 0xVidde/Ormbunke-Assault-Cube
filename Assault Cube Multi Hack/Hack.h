#pragma once
#include <Windows.h>
#include <iostream>

#include "Includes/imgui_hook.h"
#include "Includes/imgui/imgui.h"

#include "Utilities.h"
#include "Classes.h"

#include "Config.h"
#include "WeapondEnum.h"

void RenderMain()
{
    ImVec2 windowRes = Utils::GetWindowRes();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(windowRes);
    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);

    auto pDrawList = ImGui::GetWindowDrawList();

    ViewMatrix matrix = Entity::GetMatrix();

    for (int i = 1; i < Entity::GetPlayerCount(); i++)
    {
        Entity* player = Entity::GetEntity(i);

        if (Config::ESP::checkVisibility)
            if (player->IsVisible() == 0)
                continue;

        if (player->health < 1)
            continue;

        ImVec3 boxTopPos;
        ImVec3 boxTopWorldPos = ImVec3(player->headPosition.x, player->headPosition.y, player->headPosition.z + 1);
        if (!Math::WorldToScreen(boxTopWorldPos, boxTopPos, matrix.matrix, windowRes.x, windowRes.y))
            continue;

        ImVec3 boxBottomPos;
        if (!Math::WorldToScreen(player->bodyPosition, boxBottomPos, matrix.matrix, windowRes.x, windowRes.y))
            continue;

        ImVec3 headPos;
        if (!Math::WorldToScreen(player->headPosition, headPos, matrix.matrix, windowRes.x, windowRes.y))
            continue;

        ImVec3 grenadeAimbotPos;
        if (!Math::WorldToScreen(Math::GrenadePredict(Entity::GetLocal()->headPosition, player->headPosition, player->velocity, player->viewAngles), grenadeAimbotPos, matrix.matrix, windowRes.x, windowRes.y))
            continue;

        float dist = Entity::GetLocal()->GetDist(player->headPosition);

        float height = abs(boxBottomPos.y - boxTopPos.y) * 2;
        float dX = (boxBottomPos.x - boxTopPos.x);
        float width = height / 6;

        if (player->team == Entity::GetLocal()->team) {
            if (Config::ESP::headESP)
                pDrawList->AddCircle(ImVec2(headPos.x, headPos.y), width / 5, Config::ESP::friendlyColor);

            if (Config::ESP::tracerESP)
                pDrawList->AddLine(ImVec2(windowRes.x / 2, windowRes.y), ImVec2(boxBottomPos.x, boxBottomPos.y), Config::ESP::friendlyColor, 1);

            if (Config::ESP::boxESP) {
                pDrawList->AddLine(ImVec2(boxBottomPos.x + width, boxBottomPos.y), ImVec2(boxBottomPos.x - width, boxBottomPos.y), Config::ESP::friendlyColor);
                pDrawList->AddLine(ImVec2(boxBottomPos.x - width, boxBottomPos.y), ImVec2(boxBottomPos.x - width, boxTopPos.y), Config::ESP::friendlyColor);
                pDrawList->AddLine(ImVec2(boxBottomPos.x - width, boxTopPos.y), ImVec2(boxBottomPos.x + width, boxTopPos.y), Config::ESP::friendlyColor);
                pDrawList->AddLine(ImVec2(boxBottomPos.x + width, boxTopPos.y), ImVec2(boxBottomPos.x + width, boxBottomPos.y), Config::ESP::friendlyColor);
            }

            if (Config::ESP::filledBoxESP)
                pDrawList->AddQuadFilled(
                    ImVec2(boxBottomPos.x + width, boxBottomPos.y),
                    ImVec2(boxBottomPos.x + width, boxTopPos.y),
                    ImVec2(boxBottomPos.x - width, boxTopPos.y),
                    ImVec2(boxBottomPos.x - width, boxBottomPos.y),
                    Config::ESP::friendlyFilledColor
                );
        }
        else {
            if (Config::ESP::headESP)
                pDrawList->AddCircle(ImVec2(headPos.x, headPos.y), width / 5, Config::ESP::enemyColor);

            if (Config::ESP::tracerESP)
                pDrawList->AddLine(ImVec2(windowRes.x / 2, windowRes.y), ImVec2(boxBottomPos.x, boxBottomPos.y), Config::ESP::enemyColor, 1);

            if (Config::ESP::boxESP) {
                pDrawList->AddLine(ImVec2(boxBottomPos.x + width, boxBottomPos.y), ImVec2(boxBottomPos.x - width, boxBottomPos.y), Config::ESP::enemyColor);
                pDrawList->AddLine(ImVec2(boxBottomPos.x - width, boxBottomPos.y), ImVec2(boxBottomPos.x - width, boxTopPos.y), Config::ESP::enemyColor);
                pDrawList->AddLine(ImVec2(boxBottomPos.x - width, boxTopPos.y), ImVec2(boxBottomPos.x + width, boxTopPos.y), Config::ESP::enemyColor);
                pDrawList->AddLine(ImVec2(boxBottomPos.x + width, boxTopPos.y), ImVec2(boxBottomPos.x + width, boxBottomPos.y), Config::ESP::enemyColor);
            }

            if (Config::ESP::filledBoxESP)
                pDrawList->AddQuadFilled(
                    ImVec2(boxBottomPos.x + width, boxBottomPos.y),
                    ImVec2(boxBottomPos.x + width, boxTopPos.y),
                    ImVec2(boxBottomPos.x - width, boxTopPos.y),
                    ImVec2(boxBottomPos.x - width, boxBottomPos.y),
                    Config::ESP::enemyFilledColor
                );
        }

        if (Config::ESP::barESP) {
            float healthPercentage = player->health / 100.0f;
            float armorPercentage = player->armor / 100.0f;
            int healthHeight = (boxTopPos.y - boxBottomPos.y) * healthPercentage;
            int armorHeight = (boxTopPos.y - boxBottomPos.y) * armorPercentage;

            float barWidth = (100.0f / dist);

            pDrawList->AddLine(ImVec2(boxBottomPos.x - width - (barWidth), boxBottomPos.y), ImVec2(boxBottomPos.x - width - (barWidth), boxBottomPos.y + healthHeight), ImColor(0, 255, 0), barWidth);
            pDrawList->AddLine(ImVec2(boxBottomPos.x + width + (barWidth), boxBottomPos.y), ImVec2(boxBottomPos.x + width + (barWidth), boxBottomPos.y + armorHeight), ImColor(0, 0, 255), barWidth);
        }

        if (Config::ESP::nameESP)
            pDrawList->AddText(ImVec2(boxTopPos.x, boxTopPos.y - 7), ImColor(255, 255, 255), player->name);

        if (Config::ESP::rotLinesESP)
        {
            ImVec3 angles = ImVec3(player->viewAngles.y, player->viewAngles.x - 90, 0);
            ImVec3 endPoint = player->headPosition.Transform(angles, 3);

            ImVec3 endLinePoint;
            if (!Math::WorldToScreen(endPoint, endLinePoint, matrix.matrix, windowRes.x, windowRes.y))
                continue;

            pDrawList->AddLine(ImVec2(headPos.x, headPos.y), ImVec2(endLinePoint.x, endLinePoint.y), ImColor(255, 255, 255));
        }

        if (Config::Aimbot::aimbotON && Entity::GetLocal()->currentWeapond->weapondID == Weaponds::Grenade) {
            pDrawList->AddLine(ImVec2(headPos.x, headPos.y), ImVec2(grenadeAimbotPos.x, grenadeAimbotPos.y), ImColor(252, 103, 239));
            pDrawList->AddCircle(ImVec2(grenadeAimbotPos.x, grenadeAimbotPos.y), width / 4, ImColor(252, 103, 239));
        }
    }

    if (Config::Aimbot::drawFOVCircle) {
        pDrawList->AddCircle(ImVec2(windowRes.x / 2, windowRes.y / 2), Config::Aimbot::aimbotFOV, Config::Aimbot::circleColor);
        pDrawList->AddCircleFilled(ImVec2(windowRes.x / 2, windowRes.y / 2), Config::Aimbot::aimbotFOV, Config::Aimbot::circleFilledColor);
    }

    ImGui::End();
}

void Aimbot(Entity* localPlayer) {
    Entity* closestEntity = localPlayer->GetClosestEnttiyToCrossHair();

    if (closestEntity) {
        if (Config::Aimbot::checkVisibility && closestEntity->IsVisible() == 0)
            return;

        localPlayer->AimAt(closestEntity->headPosition);
    }
}

void GrenadeAimbot(Entity* localPlayer) {
    Entity* closestEntity = localPlayer->GetClosestEntity();

    if (closestEntity) {
        if (Config::Aimbot::checkVisibility && closestEntity->IsVisible() == 0)
            return;

        localPlayer->GrenadeAimAt(closestEntity);
    }
}

void TriggerBot(Entity* localPlayer) {
    Entity* triggerEnt = localPlayer->GetEntityUnderCrossHair();

    if (triggerEnt) {
        localPlayer->isAttacking = 1;
        Sleep(1);
        localPlayer->isAttacking = 0;
    }
}

void HackMain() 
{
    Entity* localPlayer = Entity::GetLocal();

    if (GetAsyncKeyState(Config::Aimbot::aimbotKey) && Config::Aimbot::aimbotON) {
        if (localPlayer->currentWeapond->weapondID == Weaponds::Grenade)
            GrenadeAimbot(localPlayer);
        else
            Aimbot(localPlayer);
    }

    if (Config::TriggerBot::triggerBotON)
        TriggerBot(localPlayer);

    localPlayer->currentWeapond->weapondObject->ammoClip = 1337;

    if (GetAsyncKeyState(VK_NUMPAD0) & 0x1) {
        Config::ESP::nameESP = !Config::ESP::nameESP;
        Utils::TypewriterPrint("Toggled Name Esp.\n", 100000);
    }

    if (GetAsyncKeyState(VK_NUMPAD1) & 0x1) {
        Config::ESP::filledBoxESP = !Config::ESP::filledBoxESP;
        Utils::TypewriterPrint("Toggled Filled Box Esp.\n", 100000);
    }

    if (GetAsyncKeyState(VK_NUMPAD2) & 0x1) {
        Config::ESP::boxESP = !Config::ESP::boxESP;
        Utils::TypewriterPrint("Toggled Box Esp.\n", 100000);
    }

    if (GetAsyncKeyState(VK_NUMPAD3) & 0x1) {
        Config::ESP::tracerESP = !Config::ESP::tracerESP;
        Utils::TypewriterPrint("Toggled Tracer Esp.\n", 1100000000);
    }

    if (GetAsyncKeyState(VK_NUMPAD4) & 0x1) {
        Config::ESP::barESP = !Config::ESP::barESP;
        Utils::TypewriterPrint("Toggled HealthBar Esp.\n", 100000);
    }

    if (GetAsyncKeyState(VK_NUMPAD5) & 0x1) {
        Config::ESP::headESP = !Config::ESP::headESP;
        Utils::TypewriterPrint("Toggled Head Circle ESP.\n", 100000);
    }

    if (GetAsyncKeyState(VK_NUMPAD6) & 0x1) {
        Config::ESP::rotLinesESP = !Config::ESP::rotLinesESP;
        Utils::TypewriterPrint("Toggled Rotation Lines ESP.\n", 100000);
    }

    if (GetAsyncKeyState(VK_NUMPAD7) & 0x1) {
        Config::Aimbot::drawFOVCircle = !Config::Aimbot::drawFOVCircle;
        Utils::TypewriterPrint("Toggled FOV Circle.\n", 100000);
    }

    if (GetAsyncKeyState(VK_NUMPAD8) & 0x1) {
        Config::TriggerBot::triggerBotON = !Config::TriggerBot::triggerBotON;
        Utils::TypewriterPrint("Toggled TriggerBot.\n", 100000);
    }

    if (GetAsyncKeyState(VK_NUMPAD9) & 0x1) {
        Config::Aimbot::aimbotON = !Config::Aimbot::aimbotON;
        Utils::TypewriterPrint("Toggled Aimbot.\n", 100000);
    }

    if (GetAsyncKeyState(VK_RIGHT)) {
        Config::Aimbot::aimbotFOV += 1;
        Utils::TypewriterPrint("Increased Aimbot FOV.\n", 100000);
    }

    if (GetAsyncKeyState(VK_LEFT)) {
        if ((Config::Aimbot::aimbotFOV - 1) > 0) {
            Config::Aimbot::aimbotFOV -= 1;
            Utils::TypewriterPrint("Decreased Aimbot FOV.\n", 100000);
        }
    }
}