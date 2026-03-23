#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CameraModel
{
public:
    glm::vec3 m_position_{0.0f, 0.0f, 2.0f};
    glm::vec3 m_forward_{0.0f, 0.0f, -1.0f};
    glm::vec3 m_up_{0.0f, 1.0f, 0.0f};

    float m_fov_{45.0f};
    float m_aspect_{4.0f / 3.0f};
    float m_nearPlane_{0.1f};
    float m_farPlane_{100.0f};

    void SetAspect(float width, float height)
    {
        if (height != 0.0f)
        {
            m_aspect_ = width / height;
        }
    }

    glm::mat4 GetView() const
    {
        return glm::lookAt(m_position_, m_position_ + m_forward_, m_up_);
    }

    glm::mat4 GetProjection() const
    {
        return glm::perspective(glm::radians(m_fov_), m_aspect_, m_nearPlane_, m_farPlane_);
    }

    glm::vec3 GetRight() const
    {
        return glm::normalize(glm::cross(m_forward_, m_up_));
    }
};