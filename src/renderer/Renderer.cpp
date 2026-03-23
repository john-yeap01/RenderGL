#include "renderer/Renderer.h"

#include <cmath>

#include "shaderClass.h"
#include "VAO.h"
#include "Texture.h"
#include "model/CameraModel.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Renderer::BeginFrame() const
{
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderScene(
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
) const
{
    glm::mat4 camMatrix = BuildCameraMatrix(cameraModel);
    glm::vec3 lightPos = ComputeLightPosition(theta);

    DrawPyramid(
        camMatrix,
        shaderProgram,
        VAO1,
        texture,
        uniID,
        pyramidIndexCount,
        theta,
        timeNow
    );

    DrawLightCube(
        camMatrix,
        lightProgram,
        VAO2,
        lightPos
    );

    DrawLitCube(
        cameraModel,
        camMatrix,
        cubeProgram,
        VAO3,
        lightPos
    );
}

glm::mat4 Renderer::BuildCameraMatrix(const CameraModel& cameraModel) const
{
    glm::mat4 view = cameraModel.GetView();
    glm::mat4 proj = cameraModel.GetProjection();
    return proj * view;
}

glm::vec3 Renderer::ComputeLightPosition(float theta) const
{
    float radius = 2.0f;

    return glm::vec3(
        radius * std::cos(glm::radians(theta)),
        1.5f,
        radius * std::sin(glm::radians(theta))
    );
}

void Renderer::DrawPyramid(
    const glm::mat4& camMatrix,
    Shader& shaderProgram,
    VAO& VAO1,
    Texture& texture,
    GLint uniID,
    GLsizei pyramidIndexCount,
    float theta,
    float timeNow
) const
{
    shaderProgram.Activate();

    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram.ID, "camMatrix"),
        1,
        GL_FALSE,
        glm::value_ptr(camMatrix)
    );

    glUniform1f(uniID, 0.5f);
    texture.Bind();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(theta), glm::vec3(0.0f, 1.0f, 0.0f));
    shaderProgram.SetMat4("model", model);

    GLint timeLoc = glGetUniformLocation(shaderProgram.ID, "time");
    glUniform1f(timeLoc, timeNow);

    VAO1.Bind();
    glDrawElements(GL_TRIANGLES, pyramidIndexCount, GL_UNSIGNED_INT, 0);
    VAO1.Unbind();
}

void Renderer::DrawLightCube(
    const glm::mat4& camMatrix,
    Shader& lightProgram,
    VAO& VAO2,
    const glm::vec3& lightPos
) const
{
    lightProgram.Activate();

    glUniformMatrix4fv(
        glGetUniformLocation(lightProgram.ID, "camMatrix"),
        1,
        GL_FALSE,
        glm::value_ptr(camMatrix)
    );

    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.3f));
    lightProgram.SetMat4("model", lightModel);

    VAO2.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    VAO2.Unbind();
}

void Renderer::DrawLitCube(
    const CameraModel& cameraModel,
    const glm::mat4& camMatrix,
    Shader& cubeProgram,
    VAO& VAO3,
    const glm::vec3& lightPos
) const
{
    cubeProgram.Activate();

    glUniformMatrix4fv(
        glGetUniformLocation(cubeProgram.ID, "camMatrix"),
        1,
        GL_FALSE,
        glm::value_ptr(camMatrix)
    );

    glm::mat4 cubeModel = glm::mat4(1.0f);
    cubeModel = glm::translate(cubeModel, glm::vec3(1.0f, 0.5f, 0.5f));
    cubeProgram.SetMat4("model", cubeModel);

    cubeProgram.SetVec3("viewPos", cameraModel.m_position_);

    cubeProgram.SetFloat("ambientStrength", 0.1f);
    cubeProgram.SetVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    cubeProgram.SetVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
    cubeProgram.SetVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    cubeProgram.SetFloat("material.shininess", 32.0f);

    cubeProgram.SetVec3("light.position", lightPos);
    cubeProgram.SetVec3("light.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
    cubeProgram.SetVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    cubeProgram.SetVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    VAO3.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    VAO3.Unbind();
}