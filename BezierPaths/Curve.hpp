#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BindableObjects.hpp"

#include <vector>

#include <memory>

#include "Shader.hpp"

#include "Rectangle.hpp"

class Curve
{
public:
    Curve(std::vector<glm::vec2>& points);

    void DrawCurve();
    void DrawCurveCreation();

    void AddBinds();

    void CreateCurve();

    void CreateQudricCurve(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2);

    void ClearBuffers();

    void AddShaderProgram(std::unique_ptr<ShaderSuite> ss) noexcept;
    ShaderSuite* GetShader() const noexcept;


    std::vector<glm::vec2> GetCurvePoints() const;


    Rectangle* rect;

    bool bViz = true;

private:

    int BinomC(int n, int k);


private:

    float t = 0.0f;
    float dt = 0.01f;

  

    int maxCurvePoints;
    std::vector<std::vector<glm::vec2>> controlPolygons;
    std::vector<glm::vec3> stripsColors;

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