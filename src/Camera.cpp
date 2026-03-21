#include "Camera.h"

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

Camera::Camera(int width, int height, glm::vec3 position)
    : Position(position),
      width_(width),
      height_(height)
{
}

void Camera::Resize(int width, int height)
{
    width_ = std::max(1, width);
    height_ = std::max(1, height);
}

void Camera::ApplyInput(const InputState& inputState, float deltaSeconds)
{
    const float movementSpeed = baseSpeed_ * (inputState.boost ? boostMultiplier_ : 1.0f) * deltaSeconds;
    const glm::vec3 right = glm::normalize(glm::cross(Orientation, Up));

    if (inputState.moveForward)
    {
        Position += movementSpeed * Orientation;
    }
    if (inputState.moveBackward)
    {
        Position -= movementSpeed * Orientation;
    }
    if (inputState.moveLeft)
    {
        Position -= movementSpeed * right;
    }
    if (inputState.moveRight)
    {
        Position += movementSpeed * right;
    }
    if (inputState.moveUp)
    {
        Position += movementSpeed * Up;
    }
    if (inputState.moveDown)
    {
        Position -= movementSpeed * Up;
    }

    if (!inputState.lookActive || !inputState.captureCameraInput)
    {
        firstLookFrame_ = true;
        return;
    }

    const float windowWidth = static_cast<float>(std::max(1, inputState.windowWidth));
    const float windowHeight = static_cast<float>(std::max(1, inputState.windowHeight));
    const float centerX = windowWidth * 0.5f;
    const float centerY = windowHeight * 0.5f;

    if (firstLookFrame_)
    {
        firstLookFrame_ = false;
        return;
    }

    const float rotX = sensitivity_ * static_cast<float>(inputState.mouseY - centerY) / windowHeight;
    const float rotY = sensitivity_ * static_cast<float>(inputState.mouseX - centerX) / windowWidth;

    const glm::vec3 rotationAxis = glm::normalize(glm::cross(Orientation, Up));
    glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), rotationAxis);

    if (std::abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
    {
        Orientation = newOrientation;
    }

    Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
    Orientation = glm::normalize(Orientation);
}

glm::mat4 Camera::ViewMatrix() const
{
    return glm::lookAt(Position, Position + Orientation, Up);
}

glm::mat4 Camera::ProjectionMatrix(float fovDegrees, float nearPlane, float farPlane) const
{
    return glm::perspective(glm::radians(fovDegrees), static_cast<float>(width_) / static_cast<float>(height_), nearPlane, farPlane);
}

glm::mat4 Camera::ViewProjectionMatrix(float fovDegrees, float nearPlane, float farPlane) const
{
    return ProjectionMatrix(fovDegrees, nearPlane, farPlane) * ViewMatrix();
}
