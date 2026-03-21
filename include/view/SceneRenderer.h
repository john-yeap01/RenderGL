#ifndef RENDERGL_VIEW_SCENE_RENDERER_H
#define RENDERGL_VIEW_SCENE_RENDERER_H

#include <filesystem>
#include <memory>

#include "EBO.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "shaderClass.h"
#include "viewmodel/SceneViewModel.h"

class SceneRenderer
{
public:
    explicit SceneRenderer(const std::filesystem::path& resourceRoot);
    ~SceneRenderer();

    void Render(const SceneRenderState& renderState);

private:
    struct MeshBuffers
    {
        VAO vao;
        std::unique_ptr<VBO> vbo;
        std::unique_ptr<EBO> ebo;
        GLsizei drawCount = 0;
        bool indexed = false;
    };

    std::filesystem::path resourceRoot_;
    Shader pyramidShader_;
    Shader lightShader_;
    Shader cubeShader_;
    Texture brickTexture_;

    MeshBuffers pyramidMesh_;
    MeshBuffers lightMesh_;
    MeshBuffers cubeMesh_;

    GLint pyramidScaleUniform_ = -1;
    GLint pyramidTimeUniform_ = -1;

    static MeshBuffers CreatePyramidMesh();
    static MeshBuffers CreateLightMesh();
    static MeshBuffers CreateCubeMesh();
};

#endif
