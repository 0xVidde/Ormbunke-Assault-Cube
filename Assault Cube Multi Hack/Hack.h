#pragma once
#include <Windows.h>
#include <iostream>

#include "Includes/imgui_hook.h"
#include "Includes/imgui/imgui.h"

#include "Utilities.h"
#include "Classes.h"

#include "Config.h"

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

        float dist = Entity::GetLocal()->GetDist(player->headPosition);

        float height = abs(boxBottomPos.y - boxTopPos.y) * 2;
        float dX = (boxBottomPos.x - boxTopPos.x);
        float width = height / 6;

        if (player->team == Entity::GetLocal()->team) {
            if (Config::ESP::headESP)
                pDrawList->AddCircle(ImVec2(headPos.x, headPos.y), width / 5, Config::ESP::friendlyColor);

            if (Config::ESP::tracerESP)
                pDrawList->AddLine(ImVec2(windowRes.x / 2, windowRes.y), ImVec2(boxBottomPos.x, boxBottomPos.y), Config::ESP::friendlyColor, 1);

            if (Config::ESP::boxESP)
                pDrawList->AddRect(ImVec2(boxTopPos.x - width, boxTopPos.y), ImVec2(boxBottomPos.x + width, boxBottomPos.y), Config::ESP::friendlyColor, 2);

            if (Config::ESP::filledBoxESP)
                pDrawList->AddRectFilled(ImVec2(boxTopPos.x - width, boxTopPos.y), ImVec2(boxBottomPos.x + width, boxBottomPos.y), Config::ESP::friendlyFilledColor, 2);
        }
        else {
            if (Config::ESP::headESP)
                pDrawList->AddCircle(ImVec2(headPos.x, headPos.y), width / 5, Config::ESP::enemyColor);

            if (Config::ESP::tracerESP)
                pDrawList->AddLine(ImVec2(windowRes.x / 2, windowRes.y), ImVec2(boxBottomPos.x, boxBottomPos.y), Config::ESP::enemyColor, 1);

            if (Config::ESP::boxESP)
                pDrawList->AddRect(ImVec2(boxTopPos.x - width, boxTopPos.y), ImVec2(boxBottomPos.x + width, boxBottomPos.y), Config::ESP::enemyColor, 2);

            if (Config::ESP::filledBoxESP)
                pDrawList->AddRectFilled(ImVec2(boxTopPos.x - width, boxTopPos.y), ImVec2(boxBottomPos.x + width, boxBottomPos.y), Config::ESP::enemyFilledColor, 2);
        }

        if (Config::ESP::healthBarESP) {
            float healthPercentage = player->health / 100.0f;
            int healthHeight = (boxTopPos.y - boxBottomPos.y) * healthPercentage;

            pDrawList->AddLine(ImVec2(boxBottomPos.x - width, boxBottomPos.y), ImVec2(boxBottomPos.x - width, boxBottomPos.y + healthHeight), ImColor(0, 255, 0), 4);
        }

        if (Config::ESP::distanceESP) {
            char distString[4];
            snprintf(distString, sizeof(distString), "%f", trunc(dist));

            pDrawList->AddText(ImVec2(boxBottomPos.x, boxBottomPos.y), ImColor(255, 255, 255), distString);
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
    }

    if (Config::Aimbot::drawFOVCircle)
        pDrawList->AddCircle(ImVec2(windowRes.x / 2, windowRes.y / 2), Config::Aimbot::aimbotFOV, Config::Aimbot::circleColor);

    if (Config::Aimbot::drawFilledFOVCircle)
        pDrawList->AddCircleFilled(ImVec2(windowRes.x / 2, windowRes.y / 2), Config::Aimbot::aimbotFOV, Config::Aimbot::circleFilledColor);

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

void HackMain() 
{
    Entity* localPlayer = Entity::GetLocal();

    if (GetAsyncKeyState(Config::Aimbot::aimbotKey) && Config::Aimbot::aimbotON)
        Aimbot(localPlayer);

    if (GetAsyncKeyState(VK_NUMPAD0) & 0x1) {
        Config::ESP::nameESP = !Config::ESP::nameESP;
        Utils::TypewriterPrint("Toggled Name Esp.\n", 1000);
    }

    if (GetAsyncKeyState(VK_NUMPAD1) & 0x1) {
        Config::ESP::filledBoxESP = !Config::ESP::filledBoxESP;
        Utils::TypewriterPrint("Toggled Filled Box Esp.\n", 1000);
    }

    if (GetAsyncKeyState(VK_NUMPAD2) & 0x1) {
        Config::ESP::boxESP = !Config::ESP::boxESP;
        Utils::TypewriterPrint("Toggled Box Esp.\n", 1000);
    }

    if (GetAsyncKeyState(VK_NUMPAD3) & 0x1) {
        Config::ESP::tracerESP = !Config::ESP::tracerESP;
        Utils::TypewriterPrint("Toggled Tracer Esp.\n", 1000);
    }

    if (GetAsyncKeyState(VK_NUMPAD4) & 0x1) {
        Config::ESP::healthBarESP = !Config::ESP::healthBarESP;
        Utils::TypewriterPrint("Toggled HealthBar Esp.\n", 1000);
    }

    if (GetAsyncKeyState(VK_NUMPAD5) & 0x1) {
        Config::ESP::headESP = !Config::ESP::headESP;
        Utils::TypewriterPrint("Toggled Head Circle ESP.\n", 1000);
    }

    if (GetAsyncKeyState(VK_NUMPAD6) & 0x1) {
        Config::ESP::rotLinesESP = !Config::ESP::rotLinesESP;
        Utils::TypewriterPrint("Toggled Rotation Lines ESP.\n", 1000);
    }

    if (GetAsyncKeyState(VK_NUMPAD7) & 0x1) {
        Config::Aimbot::drawFOVCircle = !Config::Aimbot::drawFOVCircle;
        Utils::TypewriterPrint("Toggled FOV Circle.\n", 1000);
    }


    if (GetAsyncKeyState(VK_NUMPAD8) & 0x1) {
        Config::Aimbot::drawFilledFOVCircle = !Config::Aimbot::drawFilledFOVCircle;
        Utils::TypewriterPrint("Toggled Filled FOV Circle.\n", 1000);
    }

    if (GetAsyncKeyState(VK_NUMPAD9) & 0x1) {
        Config::Aimbot::aimbotON = !Config::Aimbot::aimbotON;
        Utils::TypewriterPrint("Toggled Aimbot.\n", 1000);
    }

    if (GetAsyncKeyState(VK_UP) & 0x1) {
        Config::ESP::checkVisibility = !Config::ESP::checkVisibility;
        Utils::TypewriterPrint("Toggled ESP Visibility.\n", 1000);
    }

    if (GetAsyncKeyState(VK_DOWN) & 0x1) {
        Config::Aimbot::checkVisibility = !Config::Aimbot::checkVisibility;
        Utils::TypewriterPrint("Toggled Aimbot Visibility.\n", 1000);
    }

    if (GetAsyncKeyState(VK_RIGHT) & 0x1) {
        Config::Aimbot::aimbotFOV += 2;
        Utils::TypewriterPrint("Increased Aimbot FOV.\n", 1000);
    }

    if (GetAsyncKeyState(VK_LEFT) & 0x1) {
        if ((Config::Aimbot::aimbotFOV - 5) > 0) {
            Config::Aimbot::aimbotFOV -= 5;
            Utils::TypewriterPrint("Decreased Aimbot FOV.\n", 1000);
        }
    }
}