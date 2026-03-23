#pragma once

#include <GLFW/glfw3.h>
#include "model/CameraModel.h"

#include <iostream>
#include <cmath>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

class CameraController
{
public:
    float m_moveSpeed_{0.01f};
    float m_sprintSpeed_{0.04f};
    float m_sensitivity_{100.0f};

    bool m_firstClick_{true};
    int m_ignoreLookFrames_{0};

    void HandleInput(GLFWwindow* window, CameraModel& camera)
    {
        float currentSpeed = m_moveSpeed_;

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            currentSpeed = m_sprintSpeed_;
        }

        // -------- Movement --------
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera.m_position_ += currentSpeed * camera.m_forward_;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera.m_position_ -= currentSpeed * camera.m_forward_;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera.m_position_ -= currentSpeed * camera.GetRight();
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera.m_position_ += currentSpeed * camera.GetRight();
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            camera.m_position_ += currentSpeed * camera.m_up_;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            camera.m_position_ -= currentSpeed * camera.m_up_;
        }

        // -------- Mouse look --------
        int winW = 0;
        int winH = 0;
        glfwGetWindowSize(window, &winW, &winH);

        if (winW <= 0 || winH <= 0)
            return;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            if (m_firstClick_)
            {
                glfwSetCursorPos(window, winW / 2.0, winH / 2.0);
                m_firstClick_ = false;
                m_ignoreLookFrames_ = 1;
                return;
            }

            if (m_ignoreLookFrames_ > 0)
            {
                glfwSetCursorPos(window, winW / 2.0, winH / 2.0);
                --m_ignoreLookFrames_;
                return;
            }

            double mouseX = 0.0;
            double mouseY = 0.0;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            float rotX = m_sensitivity_ * static_cast<float>(mouseY - (winH / 2.0)) / static_cast<float>(winH);
            float rotY = m_sensitivity_ * static_cast<float>(mouseX - (winW / 2.0)) / static_cast<float>(winW);

            glm::vec3 right = glm::normalize(glm::cross(camera.m_forward_, camera.m_up_));

            glm::vec3 newForward =
                glm::rotate(camera.m_forward_,
                            glm::radians(-rotX),
                            right);

            // Prevent flipping too far up/down
            if (std::abs(glm::angle(newForward, camera.m_up_) - glm::radians(90.0f)) <= glm::radians(85.0f))
            {
                camera.m_forward_ = newForward;
            }

            camera.m_forward_ =
                glm::rotate(camera.m_forward_,
                            glm::radians(-rotY),
                            camera.m_up_);

            camera.m_forward_ = glm::normalize(camera.m_forward_);

            glfwSetCursorPos(window, winW / 2.0, winH / 2.0);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_firstClick_ = true;
            m_ignoreLookFrames_ = 0;
        }
    }
};