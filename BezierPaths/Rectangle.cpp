#include "Rectangle.hpp"


#include <string>



Rectangle::Rectangle(float size)
{
    rectangleVerticies = {
        glm::vec2(-length / 2, -width / 2),  // Bottom-left
        glm::vec2(length / 2, -width / 2),   // Bottom-right
        glm::vec2(length / 2,  width / 2),   // Top-right
        glm::vec2(-length / 2,  width / 2)   // Top-left
    };


    AddShaderProgram(std::make_unique<ShaderSuite>(std::initializer_list<std::pair<std::string_view, Shader::ShaderType>>{
        {"Shader/vertex.glsl", Shader::ShaderType::VERTEX},
        { "Shader/fragment.glsl", Shader::ShaderType::FRAGMENT }
    }));

    VBO.Populate(rectangleVerticies);
}


void Rectangle::AddShaderProgram(std::unique_ptr<ShaderSuite> ss) noexcept
{
    assert("Attempting to add a second shader program" && pShaderProgram == nullptr);
    pShaderProgram = std::move(ss);
}

ShaderSuite* Rectangle::GetShader() const noexcept
{
    return pShaderProgram.get();
}


void Rectangle::AddBinds()
{
    VAO.Bind();
    VBO.Bind();

}


void Rectangle::DrawRectangle()
{

    if (pShaderProgram) {
        pShaderProgram->use();
    }

    glm::mat4 model = this->GetTransformMatrix();

    this->GetShader()->setMat4("model", model);

    //AddBinds();
    //glLineWidth(4.0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, rectangleVerticies.size());
    glBindVertexArray(0);
}


void Rectangle::SetPosition(glm::vec3 pos) noexcept
{
    mPos.x = pos.x;
    mPos.y = pos.y;
    mPos.z = pos.z;

}

glm::mat4x4 Rectangle::GetTransformMatrix() const noexcept
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 0.0f, 1.0f)) *    // Yaw (Z-axis)
                         glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(0.0f, 1.0f, 0.0f)) *  // Pitch (Y-axis)
                         glm::rotate(glm::mat4(1.0f), roll, glm::vec3(1.0f, 0.0f, 0.0f));    // Roll (X-axis)

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), mPos);

    return translation * rotation;
}

