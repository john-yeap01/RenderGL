#include "gui/MainPanelView.h"

#include "imgui.h"

void MainPanelView::Draw(float& speed, float theta, double fps) const
{
    ImGui::Begin("Control Panel");
    ImGui::Text("OpenGL + ImGui is running");
    ImGui::SliderFloat("Speed", &speed, 0.0f, 20.0f);
    ImGui::Text("Theta: %.2f", theta);
    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    ImGui::NewLine();
    ImGui::Text("test");
    ImGui::End();
}