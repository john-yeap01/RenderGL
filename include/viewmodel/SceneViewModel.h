#ifndef RENDERGL_VIEWMODEL_SCENE_VIEW_MODEL_H
#define RENDERGL_VIEWMODEL_SCENE_VIEW_MODEL_H

#include <glm/glm.hpp>
#include <string>

#include "Camera.h"
#include "model/SceneModel.h"
#include "viewmodel/InputState.h"

struct SceneRenderState
{
    glm::mat4 viewProjection {1.0f};
    glm::mat4 pyramidModel {1.0f};
    glm::mat4 cubeModel {1.0f};
    glm::mat4 lightModel {1.0f};

    glm::vec3 lightPosition {0.0f, 0.0f, 0.0f};
    glm::vec3 lightColor {1.0f, 1.0f, 1.0f};
    glm::vec3 cubeObjectColor {1.0f, 0.5f, 0.31f};
    glm::vec3 cameraPosition {0.0f, 0.0f, 2.0f};

    float ambientStrength = 0.1f;
    float pyramidScale = 0.5f;
    float textureTime = 0.0f;
    float thetaDegrees = 0.0f;
};

class SceneViewModel
{
public:
    SceneViewModel(int framebufferWidth, int framebufferHeight);

    void ResizeViewport(int framebufferWidth, int framebufferHeight);
    void ApplyInput(const InputState& inputState, float deltaSeconds);
    void Update(float absoluteTimeSeconds);

    float& RotationSpeed();
    const SceneModel& Model() const;
    const Camera& CameraState() const;
    SceneRenderState BuildRenderState() const;
    std::string ThetaLabel() const;

private:
    SceneModel model_ {};
    Camera camera_;
};

#endif
