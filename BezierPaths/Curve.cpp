#include "Curve.hpp"

#include <iostream>

#include <chrono>
#include <cmath>
#include <random> 

int Curve::BinomC(int n, int k) {
    int res = 1;
    if (k > n - k) k = n - k;
    for (int i = 0; i < k; ++i) {
        res *= (n - i);
        res /= (i + 1);
    }
    return res;
}


glm::vec3 generateRandomColor() 
{


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    float x = dis(gen);
    float y = dis(gen);
    float z = dis(gen);

    glm::vec3 color = glm::vec3(x, y, z);

    return color;
}


Curve::Curve(std::vector<glm::vec2>& points)
    :controlPoints(points)
{
    rect = new Rectangle(1.0f);

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

        rect->SetPosition(point);



        
        if (curveStrip.size() >= 3)
        {
            glm::vec2 P0 = curveStrip[curveStrip.size() - 3];
            glm::vec2 P1 = curveStrip[curveStrip.size() - 2];
            glm::vec2 P2 = curveStrip[curveStrip.size() - 1];

            glm::vec2 L1 = P1 - P0;
            glm::vec2 L2 = P2 - P1;

            float angle = glm::dot(L1, L2) / (glm::length(L1) * glm::length(L2));

            angle = glm::clamp(angle, -1.0f, 1.0f);

            float angleRad = std::acos(angle);

            float angleDeg = glm::degrees(angleRad);

            glm::vec2 tangent = glm::normalize(P2 - P1);

            angleRad = std::atan2(tangent.y, tangent.x);

            rect->SetRotation(angleRad);

        }


        std::vector<glm::vec2> currentControlPoly = controlPoints;

        // Continue interpolating until we get down to a single point
        while (currentControlPoly.size() > 1)
        {
            std::vector<glm::vec2> nextControlPoly;  
            std::vector<glm::vec2> currentLerpLine; 

            for (int i = 0; i < currentControlPoly.size() - 1; i++)
            {
                glm::vec2 L = (1 - t) * currentControlPoly[i] + t * currentControlPoly[i + 1];
                nextControlPoly.push_back(L);
                currentLerpLine.push_back(L);  
            }

            controlPolygons.push_back(currentLerpLine);
            stripsColors.push_back(generateRandomColor());


            currentControlPoly = nextControlPoly;
        }

        //if (!currentControlPoly.empty())
        //{
        //    curveStrip.push_back(currentControlPoly[0]); // add 
        //}


        glBindBuffer(GL_ARRAY_BUFFER, curveVBO.GetID());
        glBufferSubData(GL_ARRAY_BUFFER, 0, curveStrip.size() * sizeof(glm::vec2), curveStrip.data());


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

    glm::mat4 model = glm::mat4(1.0f);


    this->rect->GetShader()->use();
    this->rect->GetShader()->setVec3("color", glm::vec3(0.5, 1.0, 0.0));
    rect->DrawRectangle();

    this->GetShader()->use();
    this->GetShader()->setMat4("model", model);
    this->GetShader()->setVec3("color", glm::vec3(0.5, 1.0, 0.0));

    glLineWidth(4.0);
    glBindVertexArray(curveVAO.GetID());
    glDrawArrays(GL_LINE_STRIP, 0, curveStrip.size());


    int i = 0;

    for (const auto& points : controlPolygons)
    {
       



        if (!points.empty())
        {
            glBindBuffer(GL_ARRAY_BUFFER, lerpVBO.GetID());
            glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec2), points.data(), GL_DYNAMIC_DRAW);

            glBindVertexArray(lerpVAO.GetID());
       
            glPointSize(10.0f);

            this->GetShader()->use();

            //this->GetShader()->setVec3("color", stripsColors[i]);
            this->GetShader()->setVec3("color", glm::vec3(0.5, 1.0, 0.0));

            glDrawArrays(GL_POINTS, 0, points.size());
            glDrawArrays(GL_LINE_STRIP, 0, points.size());
        }

        i++;
    }

    i = 0;


}

void Curve::DrawCurveCreation()
{

    glBindVertexArray(controlVAO.GetID());
    glPointSize(10.0f);

    if (bViz == true)
    {
        this->GetShader()->use();

        this->GetShader()->setVec3("color", glm::vec3(0.7, 0.7, 0.7));
        glDrawArrays(GL_POINTS, 0, controlPoints.size());
        glDrawArrays(GL_LINE_STRIP, 0, controlPoints.size());
    }

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







