#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BindableObjects.h"
#include "Shader.hpp"

class Rectangle
{
public:
    Rectangle(float size);

    void SetPosition(glm::vec2 pos) noexcept;
    void SetRotation(float) noexcept;

    glm::mat4x4 GetTransformMatrix() const noexcept ;

    void AddBinds();

    void AddShaderProgram(std::unique_ptr<ShaderSuite> ss) noexcept;
    ShaderSuite* GetShader() const noexcept;

    void DrawRectangle();

private:

    VertexArray VAO;
    VertexBuffer<glm::vec2> VBO;

    std::unique_ptr<class ShaderSuite> pShaderProgram;

    std::vector<glm::vec2> rectangleVerticies;

    glm::vec2 mPos = glm::vec2(0.0f, 0.0f);

    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;

    float length = 0.15f;
    float width = 0.1f;


};