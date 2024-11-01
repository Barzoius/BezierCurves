#include "Curve.h"

#include <iostream>

int Curve::BinomC(int n, int k) {
    int res = 1;
    if (k > n - k) k = n - k;
    for (int i = 0; i < k; ++i) {
        res *= (n - i);
        res /= (i + 1);
    }
    return res;
}


Curve::Curve(std::vector<glm::vec2>& points)
    :controlPoints(points)
{

    maxCurvePoints = static_cast<int>(1.0f / dt) + 1;

    AddShaderProgram(std::make_unique<ShaderSuite>(std::initializer_list<std::pair<std::string_view, Shader::ShaderType>>{
        {"Shader/vertex.glsl", Shader::ShaderType::VERTEX},
        { "Shader/fragment.glsl", Shader::ShaderType::FRAGMENT }
    }));

    controlVBO.Populate(controlPoints);
    curveVBO.AddVertexCap(maxCurvePoints);

   
}

void Curve::CreateCurve()
{
    int n = controlPoints.size();
    //std::cout << n<<" \n";

    if (t <= 1.0f)
    {
        glm::vec2 point(0.0f, 0.0f);

        for (int i = 0; i < n; i++)
        {
            float bernstinPoly = BinomC(n - 1, i) * std::pow(t, i) * std::pow(1 - t, n - 1 - i);
            point += controlPoints[i] * bernstinPoly;

            if (i + 1 < n)
            {
                glm::vec2 L = (1 - t) * controlPoints[i] + t * controlPoints[i + 1];
                lerpLines.push_back(L);
            }

        }

        curveStrip.push_back(point);

        //curveVBO.BindSub();

        glBindBuffer(GL_ARRAY_BUFFER, curveVBO.GetID());
        glBufferSubData(GL_ARRAY_BUFFER, 0, curveStrip.size() * sizeof(glm::vec2), curveStrip.data());


        t += dt;
    }
}

void Curve::CreateQudricCurve(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2)
{
    if (t <= 1.0f) {
        glm::vec2 point = (1 - t) * (1 - t) * P0 + 2 * (1 - t) * t * P1 + t * t * P2;

        glm::vec2 L01 = (1 - t) * P0 + t * P1;
        glm::vec2 L12 = (1 - t) * P1 + t * P2;

        curveStrip.push_back(point);    

        glBindBuffer(GL_ARRAY_BUFFER, curveVBO.GetID());
        glBufferSubData(GL_ARRAY_BUFFER, 0, curveStrip.size() * sizeof(glm::vec2), curveStrip.data());

        t += dt;
    }
}

void Curve::AddShaderProgram(std::unique_ptr<ShaderSuite> ss) noexcept
{
    assert("Attempting to add a second shader program" && pShaderProgram == nullptr);
    pShaderProgram = std::move(ss);
}

ShaderSuite* Curve::GetShader() const noexcept
{
    return pShaderProgram.get();
}

void Curve::AddBinds()
{
    curveVAO.Bind();
    curveVBO.Bind();

    controlVAO.Bind();
    controlVBO.Bind();

    lerpVAO.Bind();
    lerpVBO.Bind();

}

void Curve::DrawCurve()
{
    //glLineWidth(4.0);
    glBindVertexArray(curveVAO.GetID());
    glDrawArrays(GL_LINE_STRIP, 0, curveStrip.size());
}

void Curve::DrawCurveCreation()
{

    glBindVertexArray(controlVAO.GetID());
    glPointSize(10.0f);
    glDrawArrays(GL_POINTS, 0, controlPoints.size());
    glDrawArrays(GL_LINE_STRIP, 0, controlPoints.size());

}

void Curve::ClearBuffers()
{
    curveStrip.clear();
    glBindBuffer(GL_ARRAY_BUFFER, curveVBO.GetID());
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    controlPoints.clear();
    glBindBuffer(GL_ARRAY_BUFFER, controlVBO.GetID());
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
}

void Curve::UpdateControlPoints(std::vector<glm::vec2>& points)
{
    std::cout << "UPDATE\n";
    controlPoints = points;
    ClearBuffers();          
    AddBinds();
}