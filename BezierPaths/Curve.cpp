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
    std::cout << controlPoints.size()<<" controlP\n";
    std::cout << points.size() << " parmeter\n";

    maxCurvePoints = static_cast<int>(1.0f / dt) + 1;

    AddShaderProgram(std::make_unique<ShaderSuite>(std::initializer_list<std::pair<std::string_view, Shader::ShaderType>>{
        {"Shader/vertex.glsl", Shader::ShaderType::VERTEX},
        { "Shader/fragment.glsl", Shader::ShaderType::FRAGMENT }
    }));

    curveVBO.AddVertexCap(maxCurvePoints);

   
}

void Curve::CreateCurve()
{
    int n = controlPoints.size();

    if (t <= 1.0f)
    {
        glm::vec2 point(0.0f, 0.0f);

        for (int i = 0; i < n; i++)
        {
            float bernstinPoly = BinomC(n - 1, i) * std::pow(t, i) * std::pow(1 - t, n - 1 - i);
            point += controlPoints[i] * bernstinPoly;
        }

        curveStrip.push_back(point);

        //curveVBO.BindSub();

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
}

void Curve::DrawCurve()
{
    glBindVertexArray(curveVAO.GetID());
    glDrawArrays(GL_LINE_STRIP, 0, curveStrip.size());
}

void Curve::DrawCurveCreation()
{

}