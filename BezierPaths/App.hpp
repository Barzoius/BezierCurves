#include "Window.hpp"
//#include "Camera.hpp"
#include "Shader.hpp"

#include "memory"

#include "Curve.hpp"
#include "Rectangle.hpp"

class Application
{
public:
    Application();
    ~Application();

    void Run();
    void OnEvent();

    bool OnWindowClose();

    void SetCallBacks();

    inline Window* GetWindow() { return mWindow.get(); }
    
    void updateCurvePoints(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2);
    void renderCurve();


    GLuint VAO, VBO;

    GLuint VAO2, VBO2;
    
    std::vector<glm::vec2> curveVertices;
    std::vector<glm::vec2> selectedPoints;

    std::vector<glm::vec2> segment;


    float t = 0.0f;
    float dt = 0.01f;

    bool draw = false;
    Curve* bezierCurve;

    Rectangle* rect;

    std::vector<glm::vec2> constructedCurve;

private:

    std::unique_ptr<Window> mWindow;
    bool mIsRunning = true;

};