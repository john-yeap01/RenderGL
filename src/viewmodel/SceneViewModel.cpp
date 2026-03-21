#include "viewmodel/SceneViewModel.h"

#include <cmath>
#include <iomanip>
#include <sstream>

#include <glm/gtc/matrix_transform.hpp>

namespace
{
constexpr float kCameraFovDegrees = 45.0f;
constexpr float kCameraNearPlane = 0.1f;
constexpr float kCameraFarPlane = 100.0f;
}

SceneViewModel::SceneViewModel(int framebufferWidth, int framebufferHeight)
    : camera_(framebufferWidth, framebufferHeight, glm::vec3(0.0f, 0.0f, 2.0f))
{
    model_.light.height = 1.5f;
    model_.light.radius = 2.0f;
    model_.light.ambientStrength = 0.1f;
}

void SceneViewModel::ResizeViewport(int framebufferWidth, int framebufferHeight)
{
    camera_.Resize(framebufferWidth, framebufferHeight);
}

void SceneViewModel::ApplyInput(const InputState& inputState, float deltaSeconds)
{
    ResizeViewport(inputState.framebufferWidth, inputState.framebufferHeight);
    camera_.ApplyInput(inputState, deltaSeconds);
}

void SceneViewModel::Update(float absoluteTimeSeconds)
{
    model_.elapsedSeconds = absoluteTimeSeconds;
    model_.textureTime = absoluteTimeSeconds;
    model_.light.orbitDegrees = model_.rotationSpeedDegrees * absoluteTimeSeconds;
    model_.pyramid.rotationDegrees = model_.light.orbitDegrees;
}

float& SceneViewModel::RotationSpeed()
{
    return model_.rotationSpeedDegrees;
}

const SceneModel& SceneViewModel::Model() const
{
    return model_;
}

const Camera& SceneViewModel::CameraState() const
{
    return camera_;
}

SceneRenderState SceneViewModel::BuildRenderState() const
{
    SceneRenderState renderState {};
    renderState.viewProjection = camera_.ViewProjectionMatrix(kCameraFovDegrees, kCameraNearPlane, kCameraFarPlane);
    renderState.pyramidScale = model_.pyramidScale;
    renderState.textureTime = model_.textureTime;
    renderState.thetaDegrees = model_.light.orbitDegrees;
    renderState.ambientStrength = model_.light.ambientStrength;
    renderState.lightColor = model_.light.color;
    renderState.cubeObjectColor = model_.cubeObjectColor;
    renderState.cameraPosition = camera_.Position;

    renderState.pyramidModel = glm::mat4(1.0f);
    renderState.pyramidModel = glm::translate(renderState.pyramidModel, model_.pyramid.position);
    renderState.pyramidModel = glm::rotate(renderState.pyramidModel,
                                           glm::radians(model_.pyramid.rotationDegrees),
                                           model_.pyramid.rotationAxis);
    renderState.pyramidModel = glm::scale(renderState.pyramidModel, model_.pyramid.scale);

    renderState.cubeModel = glm::mat4(1.0f);
    renderState.cubeModel = glm::translate(renderState.cubeModel, model_.cube.position);
    renderState.cubeModel = glm::rotate(renderState.cubeModel,
                                        glm::radians(model_.cube.rotationDegrees),
                                        model_.cube.rotationAxis);
    renderState.cubeModel = glm::scale(renderState.cubeModel, model_.cube.scale);

    renderState.lightPosition = glm::vec3(
        model_.light.radius * std::cos(glm::radians(model_.light.orbitDegrees)),
        model_.light.height,
        model_.light.radius * std::sin(glm::radians(model_.light.orbitDegrees))
    );

    renderState.lightModel = glm::mat4(1.0f);
    renderState.lightModel = glm::translate(renderState.lightModel, renderState.lightPosition);
    renderState.lightModel = glm::scale(renderState.lightModel, glm::vec3(0.3f));

    return renderState;
}

std::string SceneViewModel::ThetaLabel() const
{
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << model_.light.orbitDegrees;
    return stream.str();
}
