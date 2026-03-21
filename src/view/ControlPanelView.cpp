#include "view/ControlPanelView.h"

#ifdef RENDERGL_HAS_IMGUI
#include "imgui.h"
#endif

void ControlPanelView::Draw(SceneViewModel& sceneViewModel)
{
#ifdef RENDERGL_HAS_IMGUI
    ImGui::Begin("Control Panel");
    ImGui::Text("OpenGL + MVVM");
    ImGui::SliderFloat("Speed", &sceneViewModel.RotationSpeed(), 0.0f, 20.0f);
    ImGui::Text("Theta: %s", sceneViewModel.ThetaLabel().c_str());
    ImGui::Text("Camera: (%.2f, %.2f, %.2f)",
                sceneViewModel.CameraState().Position.x,
                sceneViewModel.CameraState().Position.y,
                sceneViewModel.CameraState().Position.z);
    ImGui::End();
#else
    (void)sceneViewModel;
#endif
}
