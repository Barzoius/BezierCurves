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
    lerpVBO.AddVertexCap(maxCurvePoints * 10);
    creationVBO.AddVertexCap(maxCurvePoints * 10);

   
}

void Curve::CreateCurve()
{
    int n = controlPoints.size();

    // Clear previous frame data
    lerpLines.clear();
    creationLines.clear();
    controlPolygons.clear();

    if (t <= 1.0f)
    {

        glm::vec2 point(0.0f, 0.0f);

        for (int i = 0; i < n; i++)
        {
            float bernsteinPoly = BinomC(n - 1, i) * std::pow(t, i) * std::pow(1 - t, n - 1 - i);
            point += controlPoints[i] * bernsteinPoly;

            //glm::vec2 L = (1 - t) * controlPoints[i] + t * controlPoints[i + 1];
        }
        curveStrip.push_back(point);



        std::vector<glm::vec2> currentControlPoly = controlPoints;

        // Continue interpolating until we get down to a single point
        while (currentControlPoly.size() > 1)
        {
            std::vector<glm::vec2> nextControlPoly;  // To store the next level's points
            std::vector<glm::vec2> currentLerpLine;  // Store points for the current interpolation line

            // Interpolate between each consecutive pair of points in the current level
            for (int i = 0; i < currentControlPoly.size() - 1; i++)
            {
                glm::vec2 L = (1 - t) * currentControlPoly[i] + t * currentControlPoly[i + 1];
                nextControlPoly.push_back(L);
                currentLerpLine.push_back(L);  // Add to the current line's level
            }

            // Store the current level's lerp points for separate rendering
            controlPolygons.push_back(currentLerpLine);

            // Move to the next level
            currentControlPoly = nextControlPoly;
        }

        glBindBuffer(GL_ARRAY_BUFFER, curveVBO.GetID());
        glBufferSubData(GL_ARRAY_BUFFER, 0, curveStrip.size() * sizeof(glm::vec2), curveStrip.data());

        //glBindBuffer(GL_ARRAY_BUFFER, lerpVBO.GetID());
        //glBufferSubData(GL_ARRAY_BUFFER, 0, lerpLines.size() * sizeof(glm::vec2), lerpLines.data());

        t += dt;  // t forward for the next frame
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

    creationVAO.Bind();
    creationVBO.Bind();

}

std::vector<glm::vec2> Curve::GetCurvePoints() const
{
    return curveStrip;
}


void Curve::DrawCurve()
{
    //glLineWidth(4.0);
    glBindVertexArray(curveVAO.GetID());
    glDrawArrays(GL_LINE_STRIP, 0, curveStrip.size());

    glBindVertexArray(lerpVAO.GetID());
    glPointSize(10.0f);
    glDrawArrays(GL_POINTS, 0, lerpLines.size());
    glDrawArrays(GL_LINE_STRIP, 0, lerpLines.size());

    glBindVertexArray(creationVAO.GetID());
    glPointSize(10.0f);
    glDrawArrays(GL_POINTS, 0, creationLines.size());
    glDrawArrays(GL_LINE_STRIP, 0, creationLines.size());


    for (const auto& points : controlPolygons)
    {
        if (!points.empty())
        {
            glBindBuffer(GL_ARRAY_BUFFER, lerpVBO.GetID());
            glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec2), points.data(), GL_DYNAMIC_DRAW);

            glBindVertexArray(lerpVAO.GetID());
            glPointSize(10.0f);
            glDrawArrays(GL_POINTS, 0, points.size());
            glDrawArrays(GL_LINE_STRIP, 0, points.size());
        }
    }
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