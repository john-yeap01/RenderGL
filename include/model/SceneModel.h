#ifndef RENDERGL_MODEL_SCENE_MODEL_H
#define RENDERGL_MODEL_SCENE_MODEL_H

#include <glm/glm.hpp>

struct TransformModel
{
    glm::vec3 position {0.0f, 0.0f, 0.0f};
    glm::vec3 rotationAxis {0.0f, 1.0f, 0.0f};
    float rotationDegrees = 0.0f;
    glm::vec3 scale {1.0f, 1.0f, 1.0f};
};

struct OrbitLightModel
{
    glm::vec3 color {1.0f, 1.0f, 1.0f};
    float ambientStrength = 0.1f;
    float radius = 2.0f;
    float height = 1.5f;
    float orbitDegrees = 0.0f;
};

struct SceneModel
{
    TransformModel pyramid {};
    TransformModel cube {
        glm::vec3(1.0f, 0.5f, 0.5f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        0.0f,
        glm::vec3(1.0f, 1.0f, 1.0f)
    };
    OrbitLightModel light {};

    glm::vec3 cubeObjectColor {1.0f, 0.5f, 0.31f};
    float pyramidScale = 0.5f;
    float textureTime = 0.0f;
    float elapsedSeconds = 0.0f;
    float rotationSpeedDegrees = 5.0f;
};

#endif
