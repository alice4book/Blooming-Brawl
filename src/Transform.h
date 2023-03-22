#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <glad/glad.h>

class Transform
{
protected:
 
    //Local space information
    glm::vec3 m_pos = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_eulerRot = { 0.0f, 0.0f, 0.0f }; //In degrees
    glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };

    //Global space information concatenate in matrix
    glm::mat4 m_modelMatrix = glm::mat4(1.0f);

    //Dirty flag
    bool m_isDirty = true;

protected:
    glm::mat4 getLocalModelMatrix()
    {
        const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
            glm::radians(m_eulerRot.x),
            glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
            glm::radians(m_eulerRot.y),
            glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
            glm::radians(m_eulerRot.z),
            glm::vec3(0.0f, 0.0f, 1.0f));

        // Y * X * Z
        const glm::mat4 roationMatrix = transformY * transformX * transformZ;

        // translation * rotation * scale (also know as TRS matrix)
        return glm::translate(glm::mat4(1.0f), m_pos) *
            roationMatrix *
            glm::scale(glm::mat4(1.0f), m_scale);
    }
public:

    void computeModelMatrix()
    {
        m_modelMatrix = getLocalModelMatrix();
    }

    void computeModelMatrix(const glm::mat4& parentGlobalModelMatrix)
    {
        m_modelMatrix = parentGlobalModelMatrix * getLocalModelMatrix();
    }

    void setLocalPosition(const glm::vec3& newPosition)
    {
        m_pos = newPosition;
        m_isDirty = true;
    }

    void addToLocalPosition(const glm::vec3& newPosition)
    {
        m_pos += newPosition;
        m_isDirty = true;
    }

    void setLocalRotation(const glm::vec3& newEulerRot)
    {
        m_eulerRot = newEulerRot;
        m_isDirty = true;
    }

    void rotateLocal(const glm::vec3& newEulerRot)
    {
        m_eulerRot += newEulerRot;
        m_isDirty = true;
    }

    void scaleEntity(const glm::vec3& newScale)
    {
        m_scale = newScale;
        m_isDirty = true;
    }

    const glm::vec3& getLocalPosition()
    {
        return m_pos;
    }

    const glm::mat4& getModelMatrix()
    {
        return m_modelMatrix;
    }

    bool isDirty()
    {
        return m_isDirty;
    }
};