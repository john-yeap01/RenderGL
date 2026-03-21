#include "view/SceneRenderer.h"

#include <array>
#include <filesystem>
#include <stdexcept>
#include <vector>

namespace
{
using VertexList = std::vector<GLfloat>;
using IndexList = std::vector<GLuint>;

constexpr const char* kShaderDir = "shaders";
constexpr const char* kAssetDir = "assets";

VertexList PyramidVertices()
{
    return {
         0.5f,  0.0f,  0.5f,     1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         0.5f,  0.0f, -0.5f,     0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f,  0.0f, -0.5f,     0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        -0.5f,  0.0f,  0.5f,     1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
         0.0f,  0.8f,  0.0f,     1.0f, 0.0f, 1.0f,  0.5f, 0.5f
    };
}

IndexList PyramidIndices()
{
    return {
        0, 1, 2,
        2, 3, 0,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };
}

VertexList LightVertices()
{
    return {
        -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,-0.5f,  0.5f, -0.5f,-0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f, 0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,-0.5f,  0.5f,  0.5f,-0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,-0.5f,  0.5f, -0.5f,-0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,-0.5f, -0.5f,  0.5f,-0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f, 0.5f,  0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f, 0.5f, -0.5f,  0.5f, 0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,-0.5f, -0.5f,  0.5f,-0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f, 0.5f,  0.5f, -0.5f, 0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,-0.5f,  0.5f,  0.5f,-0.5f,  0.5f, -0.5f
    };
}

VertexList CubeVertices()
{
    return {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
}
}

SceneRenderer::SceneRenderer(const std::filesystem::path& resourceRoot)
    : resourceRoot_(resourceRoot),
      pyramidShader_((resourceRoot_ / kShaderDir / "default.vert").string().c_str(),
                     (resourceRoot_ / kShaderDir / "default.frag").string().c_str()),
      lightShader_((resourceRoot_ / kShaderDir / "light.vert").string().c_str(),
                   (resourceRoot_ / kShaderDir / "light.frag").string().c_str()),
      cubeShader_((resourceRoot_ / kShaderDir / "cube.vert").string().c_str(),
                  (resourceRoot_ / kShaderDir / "cube.frag").string().c_str()),
      brickTexture_((resourceRoot_ / kAssetDir / "brick.png").string().c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE),
      pyramidMesh_(CreatePyramidMesh()),
      lightMesh_(CreateLightMesh()),
      cubeMesh_(CreateCubeMesh())
{
    pyramidScaleUniform_ = glGetUniformLocation(pyramidShader_.ID, "scale");
    pyramidTimeUniform_ = glGetUniformLocation(pyramidShader_.ID, "time");
    brickTexture_.texUnit(pyramidShader_, "tex0", 0);
}

SceneRenderer::~SceneRenderer()
{
    pyramidMesh_.vao.Delete();
    if (pyramidMesh_.vbo) pyramidMesh_.vbo->Delete();
    if (pyramidMesh_.ebo) pyramidMesh_.ebo->Delete();

    lightMesh_.vao.Delete();
    if (lightMesh_.vbo) lightMesh_.vbo->Delete();

    cubeMesh_.vao.Delete();
    if (cubeMesh_.vbo) cubeMesh_.vbo->Delete();

    brickTexture_.Delete();
    pyramidShader_.Delete();
    lightShader_.Delete();
    cubeShader_.Delete();
}

void SceneRenderer::Render(const SceneRenderState& renderState)
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    pyramidShader_.Activate();
    pyramidShader_.SetMat4("camMatrix", renderState.viewProjection);
    pyramidShader_.SetMat4("model", renderState.pyramidModel);
    glUniform1f(pyramidScaleUniform_, renderState.pyramidScale);
    glUniform1f(pyramidTimeUniform_, renderState.textureTime);
    brickTexture_.Bind();
    pyramidMesh_.vao.Bind();
    glDrawElements(GL_TRIANGLES, pyramidMesh_.drawCount, GL_UNSIGNED_INT, nullptr);
    pyramidMesh_.vao.Unbind();

    lightShader_.Activate();
    lightShader_.SetMat4("camMatrix", renderState.viewProjection);
    lightShader_.SetMat4("model", renderState.lightModel);
    lightMesh_.vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, lightMesh_.drawCount);
    lightMesh_.vao.Unbind();

    cubeShader_.Activate();
    cubeShader_.SetMat4("camMatrix", renderState.viewProjection);
    cubeShader_.SetMat4("model", renderState.cubeModel);
    cubeShader_.SetFloat("ambientStrength", renderState.ambientStrength);
    cubeShader_.SetVec3("lightPos", renderState.lightPosition);
    cubeShader_.SetVec3("objectColor", renderState.cubeObjectColor);
    cubeShader_.SetVec3("lightColor", renderState.lightColor);
    cubeShader_.SetVec3("viewPos", renderState.cameraPosition);
    cubeMesh_.vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, cubeMesh_.drawCount);
    cubeMesh_.vao.Unbind();
}

SceneRenderer::MeshBuffers SceneRenderer::CreatePyramidMesh()
{
    MeshBuffers mesh;
    const VertexList vertices = PyramidVertices();
    const IndexList indices = PyramidIndices();

    mesh.vao.Bind();
    mesh.vbo = std::make_unique<VBO>(const_cast<GLfloat*>(vertices.data()), static_cast<GLsizeiptr>(vertices.size() * sizeof(GLfloat)));
    mesh.ebo = std::make_unique<EBO>(const_cast<GLuint*>(indices.data()), static_cast<GLsizeiptr>(indices.size() * sizeof(GLuint)));
    mesh.vao.LinkAttrib(*mesh.vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), reinterpret_cast<void*>(0));
    mesh.vao.LinkAttrib(*mesh.vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    mesh.vao.LinkAttrib(*mesh.vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
    mesh.vao.Unbind();
    mesh.vbo->Unbind();
    mesh.ebo->Unbind();
    mesh.drawCount = static_cast<GLsizei>(indices.size());
    mesh.indexed = true;
    return mesh;
}

SceneRenderer::MeshBuffers SceneRenderer::CreateLightMesh()
{
    MeshBuffers mesh;
    const VertexList vertices = LightVertices();

    mesh.vao.Bind();
    mesh.vbo = std::make_unique<VBO>(const_cast<GLfloat*>(vertices.data()), static_cast<GLsizeiptr>(vertices.size() * sizeof(GLfloat)));
    mesh.vao.LinkAttrib(*mesh.vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), reinterpret_cast<void*>(0));
    mesh.vao.Unbind();
    mesh.vbo->Unbind();
    mesh.drawCount = static_cast<GLsizei>(vertices.size() / 3);
    return mesh;
}

SceneRenderer::MeshBuffers SceneRenderer::CreateCubeMesh()
{
    MeshBuffers mesh;
    const VertexList vertices = CubeVertices();

    mesh.vao.Bind();
    mesh.vbo = std::make_unique<VBO>(const_cast<GLfloat*>(vertices.data()), static_cast<GLsizeiptr>(vertices.size() * sizeof(GLfloat)));
    mesh.vao.LinkAttrib(*mesh.vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), reinterpret_cast<void*>(0));
    mesh.vao.LinkAttrib(*mesh.vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    mesh.vao.Unbind();
    mesh.vbo->Unbind();
    mesh.drawCount = static_cast<GLsizei>(vertices.size() / 6);
    return mesh;
}
