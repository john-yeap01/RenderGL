#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader;
class VAO;
class Texture;
class CameraModel;

class Renderer
{
public:
    void BeginFrame() const;

    void RenderScene(
        const CameraModel& cameraModel,
        Shader& shaderProgram,
        Shader& lightProgram,
        Shader& cubeProgram,
        VAO& VAO1,
        VAO& VAO2,
        VAO& VAO3,
        Texture& texture,
        GLint uniID,
        GLsizei pyramidIndexCount,
        float theta,
        float timeNow
    ) const;

private:
    glm::mat4 BuildCameraMatrix(const CameraModel& cameraModel) const;
    glm::vec3 ComputeLightPosition(float theta) const;

    void DrawPyramid(
        const glm::mat4& camMatrix,
        Shader& shaderProgram,
        VAO& VAO1,
        Texture& texture,
        GLint uniID,
        GLsizei pyramidIndexCount,
        float theta,
        float timeNow
    ) const;

    void DrawLightCube(
        const glm::mat4& camMatrix,
        Shader& lightProgram,
        VAO& VAO2,
        const glm::vec3& lightPos
    ) const;

    void DrawLitCube(
        const CameraModel& cameraModel,
        const glm::mat4& camMatrix,
        Shader& cubeProgram,
        VAO& VAO3,
        const glm::vec3& lightPos
    ) const;
};