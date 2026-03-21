#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glm/glm.hpp>

#include "viewmodel/InputState.h"

class Camera
{
public:
    Camera(int width, int height, glm::vec3 position);

    void Resize(int width, int height);
    void ApplyInput(const InputState& inputState, float deltaSeconds);

    glm::mat4 ViewMatrix() const;
    glm::mat4 ProjectionMatrix(float fovDegrees, float nearPlane, float farPlane) const;
    glm::mat4 ViewProjectionMatrix(float fovDegrees, float nearPlane, float farPlane) const;

    glm::vec3 Position;
    glm::vec3 Orientation {0.0f, 0.0f, -1.0f};
    glm::vec3 Up {0.0f, 1.0f, 0.0f};

private:
    int width_;
    int height_;
    bool firstLookFrame_ = true;
    float baseSpeed_ = 2.0f;
    float boostMultiplier_ = 4.0f;
    float sensitivity_ = 50.0f;
};

#endif
