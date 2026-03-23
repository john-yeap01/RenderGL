#pragma once

#include <cstddef>
#include <glad/glad.h>

struct SceneModel
{
    float theta = 0.0f;
    float speed = 5.0f;
    double fps = 0.0;
};

extern GLfloat vertices[];
extern GLfloat light_vertices[];
extern GLfloat cube_vertices[];
extern GLuint indices[];

extern const std::size_t vertices_size_bytes;
extern const std::size_t light_vertices_size_bytes;
extern const std::size_t cube_vertices_size_bytes;
extern const std::size_t indices_size_bytes;
extern const GLsizei indices_count;