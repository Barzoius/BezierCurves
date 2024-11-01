
#include "App.hpp"

#include <vector>

#include <thread>  
#include <chrono>
#include <cmath>
#include <random> 

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if (app->draw == false)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            int wndWidth, wndHeight;
            glfwGetWindowSize(window, &wndWidth, &wndHeight);

            float NDC_X = (xpos / wndWidth) * 2.0f - 1.0f;
            float NDC_Y = 1.0f - (ypos / wndHeight) * 2.0f;

            std::cout << "xpos: " << NDC_X << " ypos: " << ypos << "\n";
            app->selectedPoints.push_back(glm::vec2(NDC_X, NDC_Y));
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        std::cout << "TRUE_DRAW\n";
        std::cout << "selectedPoints: " << app->selectedPoints.size() << "\n";
        app->draw = true;
    }
}


Application::Application()
{
    mWindow = std::make_unique<Window>(800, 600, "TOY_GFX");



    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD FAILED";
    }

    SetCallBacks();

    glEnable(GL_DEPTH_TEST);
}

void Application::SetCallBacks()
{
    glfwSetWindowUserPointer(mWindow->GetWindow(), this);
    glfwSetMouseButtonCallback(mWindow->GetWindow(), mouse_button_callback);
}

Application::~Application()
{

}

void Application::OnEvent()
{
    if (glfwGetKey(mWindow->GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(mWindow->GetWindow(), true);
    }

    if (glfwGetKey(mWindow->GetWindow(), GLFW_KEY_P) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if (glfwGetKey(mWindow->GetWindow(), GLFW_KEY_E) == GLFW_PRESS)
    {
        //glfwSetWindowShouldClose(mWindow->GetWindow(), true);
        std::cout << "E";
    }

}

void Application::updateCurvePoints(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2)
{
    if (t <= 1.0f) {
        glm::vec2 point = (1 - t) * (1 - t) * P0 + 2 * (1 - t) * t * P1 + t * t * P2;

        glm::vec2 L01 = (1 - t) * P0 + t * P1;
        glm::vec2 L12 = (1 - t) * P1 + t * P2;

        curveVertices.push_back(point);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, curveVertices.size() * sizeof(glm::vec2), curveVertices.data());

        t += dt;
    }
}

std::vector<glm::vec2> generateRandomVec2Vector(size_t count) {
    std::vector<glm::vec2> vec;
    vec.reserve(count);


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    for (size_t i = 0; i < count; ++i) {
        float x = dis(gen);
        float y = dis(gen);
        vec.emplace_back(x, y);
    }

    return vec;
}


void Application::renderCurve()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, curveVertices.size());
}


// 0.5, 1.0, 0.0 - nice green color 

void Application::Run()
{

    ShaderSuite ss = ShaderSuite(std::initializer_list<std::pair<std::string_view, Shader::ShaderType>>{
        {"Shader/vertex.glsl", Shader::ShaderType::VERTEX},
        { "Shader/fragment.glsl", Shader::ShaderType::FRAGMENT }
    });

    int maxCurvePoints = static_cast<int>(1.0f / dt) + 1;

    std::vector<glm::vec2> points = generateRandomVec2Vector(20);


    glm::vec2 P0(0.32f, 0.0f);
    glm::vec2 P1(-1.0f, 0.25f);
    glm::vec2 P2(0.23f, -0.80f);

    orthoMatrix = glm::ortho(xMin, xMax, yMin, yMax);

    
    bool deleted = false;

    while (!glfwWindowShouldClose(mWindow->GetWindow()))
    {
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        mWindow->ProcessInput();

        

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (mWindow->erase == true)
        {
            selectedPoints.clear();
            //bezierCurve->ClearBuffers();
            delete bezierCurve;
            bezierCurve = nullptr;
            mWindow->erase = false;
            deleted = true;
            draw = false;

        }

        if (draw == true)
        {
           

            if (bezierCurve == nullptr)
            {
                bezierCurve = new Curve(selectedPoints);
                bezierCurve->AddBinds();
            }


           bezierCurve->CreateCurve();
           bezierCurve->GetShader()->use();


           bezierCurve->DrawCurve();
           bezierCurve->DrawCurveCreation();

        }

        glBindVertexArray(0); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        mWindow->OnUpdate();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

}

bool Application::OnWindowClose()
{
    mIsRunning = false;
    return true;
}