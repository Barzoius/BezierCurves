#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BindableObjects.h"

#include <vector>

#include <memory>

#include "Shader.hpp"

class Curve
{
public:
    Curve(std::vector<glm::vec2>& points);

    void DrawCurve();
    void DrawCurveCreation();

    void AddBinds();

    void CreateCurve();

    void AddShaderProgram(std::unique_ptr<ShaderSuite> ss) noexcept;
    ShaderSuite* GetShader() const noexcept;


private:

    int BinomC(int n, int k);

private:

    float t = 0.0f;
    float dt = 0.01f;

    int maxCurvePoints;

    std::unique_ptr<class ShaderSuite> pShaderProgram;

    VertexArray curveVAO;
    VertexBuffer<glm::vec2> curveVBO;
    std::vector<glm::vec2> curveStrip;

    VertexArray controlVAO;
    VertexBuffer<glm::vec2> controlVBO;
    std::vector<glm::vec2> controlPoints;

    VertexArray lerpVAO;
    VertexBuffer<glm::vec2> lerpVBO;
    std::vector<glm::vec2> lerpLines;

    VertexArray creationVAO;
    VertexBuffer<glm::vec2> creationVBO;
    std::vector<glm::vec2> creationLines;


};