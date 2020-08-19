#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "model.h"

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

template <typename T>
T Kepler1(T r, T D_Theta, T GM)
{
    return (r * D_Theta * D_Theta) - (GM / (r * r));
};

template <typename U>
U Kepler2(U r, U D_Theta, U D_R)
{
    return -2.0 * D_Theta * D_R / r;
};

template<typename V>
V Euler(V currentval, V deltaT, V deriv)
{
    return currentval + deltaT * deriv;
};

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

    Shader Shader("shader.vs", "shader.fs");

    Model Model1("Earth/Earth.obj");
    Model Model2("Moon/Moon.obj");
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    double distance = 10.0;
    double distance_speed = 0;
    double distance_accel;

    double angle = 3.14159265 / 6.0;
    double angle_speed = 1.0;
    double angle_accel;

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

        // render the loaded model
        glm::mat4 model1 = glm::mat4(1.0f);
        model1 = glm::translate(model1, glm::vec3(0.0f, 0.0f, 0.0f)); 
        model1 = glm::scale(model1, glm::vec3(0.5f, 0.5f, 0.5f));	
        //model = glm::rotate(model, (float)glm::radians(180.0), glm::vec3(0.0, 0.0, 1.0));
        Shader.setMat4("model", model1);
        Model1.Draw(Shader);

        distance_accel = Kepler1(distance, distance_speed, 5.0);
        angle_accel = Kepler2(distance, distance_speed, angle_speed);
        distance_speed = Euler(distance_speed, deltaTime, distance_accel);
        angle_speed = Euler(angle_speed, deltaTime, angle_accel);
        distance = Euler(distance, deltaTime, distance_speed);
        angle = Euler(angle, deltaTime, angle_speed);
        
        double x = distance * cos(angle);
        double z = distance * sin(angle);

        glm::mat4 model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(x, 0.0, z));
        model2 = glm::scale(model2, glm::vec3(0.2f, 0.2f, 0.2f));
        Shader.setMat4("model", model2);
        Model2.Draw(Shader);
        
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
