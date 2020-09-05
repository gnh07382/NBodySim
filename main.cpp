#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "model.h"

#include "Space.h"

#pragma warning(disable: c26451)

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 10.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

double deltaTime = 0.0f;
double lastFrame = 0.0f;

int timestep;
int stepsize = 16000;

/*
template <typename... M>
void SaveMass(M... Mass)
{
    masslist.push_back(Mass);//error
}

template <typename... D>
void SaveDist(D... Dist)
{
    distlist.push_back(Dist);
}
*/

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Kepler3", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    Shader Shader("planetvertex.glsl", "planetfrag.frag");

    Model Model[5] = 
    {
        {"Celestials/Jupiter.obj"},
        {"Celestials/Io.obj"},
        {"Celestials/Europa.obj"},
        {"Celestials/Ganymede.obj"},
        {"Celestials/Callisto.obj"},
    };
 
    //SI unit 쓰기(kg, m)
    //순서: 이름 질량 자전속도(rad/s) AxialTilt 지름 위치 속도 
    Planet planet[5] = 
    {
        {"Jupiter", 1.89813e+27, 0.00017585, glm::dvec3(0.0, 0.0, 0.0), 71492000.0,
        glm::dvec3(1.339363988310993E+05,  9.092405264331063E+04,  5.073445722988616E+03),
        glm::dvec3(-1.217066061792022E-00,  4.484495409373950E-01, -2.501334739871315E-04)},
        
        {"Io", 8.919e+22, 0.0, glm::dvec3(0.0, 0.0, 0.0), 1821300.0, 
        glm::dvec3(1.783588628428087E+08, -3.813132022198399E+08, -1.122969170963552E+07), 
        glm::dvec3(1.575622234571306E+04,  7.274824411696751E+03,  5.017042541746966E+02)},
        
        {"Europa", 4.799e+22, 0.0, glm::dvec3(0.0, 0.0, 0.0), 1565000.0, 
         glm::dvec3(1.090756855262995E+08,  6.563612022517135E+08,  2.839224074616516E+07),
         glm::dvec3(-1.364656326617821E+04,  2.353365332781587E+03, -2.039139274185915E+02)},
        
        {"Ganymede", 1.482e+23, 0.0, glm::dvec3(0.0, 0.0, 0.0), 2634000.0,
         glm::dvec3(-4.970886905251951E+08, -9.460581334912166E+08, -4.271837978299236E+07),
         glm::dvec3(9.645972005561218E+03, -5.046466898601443E+03, -6.028417363952232E+01)},
        
        {"Callisto", 1.076e+23, 0.0, glm::dvec3(0.0, 0.0, 0.0), 2403000.0,
         glm::dvec3(-1.875550445329819E+09, -2.777401160541600E+08, -3.403549534719028E+07),
         glm::dvec3(1.199591768065817E+03, -8.051860690784368E+03, -2.380699073063166E+02)}
    };

    std::vector<double> masslist;
    std::vector<glm::dvec3> distlist;

    Space space(60);//60sec step size
    bool calculated = false;

    while (!glfwWindowShouldClose(window))
    {
        double currentFrame = glfwGetTime(); 
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Shader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        Shader.setMat4("projection", projection);
        Shader.setMat4("view", view);

        glm::dmat4 model[5];
        for (int i = 0; i < 5; i++)
        {
            model[i] = glm::dmat4(1.0f);
            model[i] = glm::translate(model[i], planet[i].PositionPredict.at(timestep));
            model[i] = glm::scale(model[i], glm::dvec3(planet[i].Radius, planet[i].Radius, planet[i].Radius));
            Shader.setMat4("model", model[i]);
            Model[i].Draw(Shader);
            /*
            CDE방식으로 움직일 꺼니까 좀 더 생각해보고 구현하기,
            예측선 함수 만들기
            move 클래스 수정, ephemeris 코드 수정(전체 데이터 받느 부분이 이상함)
            */
        }
        if (!calculated)
        {
            for (int j = 0; j < 5; j++)
                space.Ephemeris(planet[j], stepsize, planet, 5);
            
            calculated = true;
        }

        for (int i = 0; i < 5; i++)//draw line
        {
            planet[i].PositionPredict;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}