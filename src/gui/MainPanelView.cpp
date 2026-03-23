#include "gui/MainPanelView.h"

#include "imgui.h"

void MainPanelView::Draw(SceneModel& sceneModel) const
{
    ImGui::Begin("Control Panel");
    ImGui::Text("OpenGL + ImGui is running");
    ImGui::SliderFloat("Speed", &sceneModel.speed, 0.0f, 20.0f);
    ImGui::Text("Theta: %.2f", sceneModel.theta);
    ImGui::Text("FPS: %.1f", sceneModel.fps);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
}