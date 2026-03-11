#include "LoadingState.h"
#include "InGameReadyState.h"
#include "UManager.h"
#include "ImGui/imgui.h"

void LoadingState::OnEnter(UManager* manager)
{
    m_currentTimer = 0.0f;
    uiManager = new UIManager();


}

IState* LoadingState::Update(float deltaTime, UManager* manager)
{
    m_currentTimer += deltaTime;
    if (m_currentTimer >= TOTAL_LOADING_TIME)
    {
        return new InGameReadyState();
    }

    return this;
}

void LoadingState::Render(URenderer* renderer, UManager* manager)
{
    if (uiManager)
    {
        //ImGui::SetNextWindowPos(ImVec2(412, 462));
        //ImGui::Begin("Loading", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);

        //float progress = m_currentTimer / TOTAL_LOADING_TIME;
        //ImGui::Text("Loading Stage...");
        //ImGui::ProgressBar(progress, ImVec2(200, 30), "Please Wait");

        //ImGui::End();

        uiManager->Render();
    }
}

void LoadingState::OnExit(UManager* manager)
{
    if (uiManager)
    {
        delete uiManager;
        uiManager = nullptr;
    }
}