#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace tema2
{
    class Camera
    {
     public:
        Camera()
        {
            position    = glm::vec3(0, 2, 5);
            forward     = glm::vec3(0, 0, -1);
            up          = glm::vec3(0, 1, 0);
            right       = glm::vec3(1, 0, 0);
            distanceToTarget = 2;
        }

        Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            Set(position, center, up, 2);
        }

        ~Camera()
        { }

        // Update camera
        void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up, float dist)
        {
            this->position = position;
            forward     = glm::normalize(center - position);
            right       = glm::cross(forward, up);
            this->up    = glm::cross(right, forward);
            distanceToTarget = dist;
        }

        void MoveForward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, forward.y, forward.z));
            position += dir * distance;
        }

        void TranslateUpward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(up.x, up.y, up.z));
            position += dir * distance;
        }

        void TranslateRight(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
            position += dir * distance;
        }

        void RotateFirstPerson_OX(float angle)
        {
            forward = glm::normalize(glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 0));
            up = glm::normalize(glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(up, 0));
        }

        void RotateFirstPerson_OY(float angle)
        {
            forward = glm::normalize(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0));
            right = glm::normalize(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0));
            up = glm::normalize(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(up, 0));
        }

        void RotateFirstPerson_OZ(float angle)
        {
            right = glm::normalize(glm::rotate(glm::mat4(1.0f), angle, forward) * glm::vec4(right, 0));
            up = glm::normalize(glm::rotate(glm::mat4(1.0f), angle, forward) * glm::vec4(up, 0));
        }

        void RotateThirdPerson_OX(float angle)
        {
            position += distanceToTarget* forward;
            RotateFirstPerson_OX(-angle);
            position -= distanceToTarget * forward;
        }

        void RotateThirdPerson_OY(float angle)
        {
            position += distanceToTarget * forward;
            RotateFirstPerson_OY(-angle);
            position -= distanceToTarget * forward;
        }

        void RotateThirdPerson_OZ(float angle)
        {
            position += distanceToTarget * forward;
            RotateFirstPerson_OZ(-angle);
            position -= distanceToTarget * forward;
        }

        glm::mat4 GetViewMatrix()
        {
            // Returns the view matrix
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

     public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}   // namespace implemented
