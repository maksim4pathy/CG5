#define GLEW_DLL
#define GLFW_DLL

#include <iostream>
#include <cmath>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Shader.h"
#include "Model.h"

#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

float yaw = -90.0f;
float pitch = 0.0f;

float lastX = 256.0f;
float lastY = 256.0f;

bool firstMouse = true;

float sensitivity = 0.1f;

glm::vec3 cameraPos =
glm::vec3(0.0f, 0.0f, 3.0f);

glm::vec3 cameraFront =
glm::vec3(0.0f, 0.0f, -1.0f);

glm::vec3 cameraUp =
glm::vec3(0.0f, 1.0f, 0.0f);

void mouse_callback(
    GLFWwindow* window,
    double xpos,
    double ypos)
{
    if (firstMouse)
    {
        lastX = (float)xpos;
        lastY = (float)ypos;

        firstMouse = false;
    }

    float xoffset =
        (float)xpos - lastX;

    float yoffset =
        lastY - (float)ypos;

    lastX = (float)xpos;
    lastY = (float)ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;

    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;

    front.x =
        cos(glm::radians(yaw)) *
        cos(glm::radians(pitch));

    front.y =
        sin(glm::radians(pitch));

    front.z =
        sin(glm::radians(yaw)) *
        cos(glm::radians(pitch));

    cameraFront =
        glm::normalize(front);
}

int main()
{
    GLFWwindow* window;

    if (!glfwInit())
    {
        fprintf(stderr,
            "error: not running GLFW3.\n");

        return 1;
    }

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MAJOR,
        4);

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MINOR,
        6);

    glfwWindowHint(
        GLFW_OPENGL_FORWARD_COMPAT,
        GL_TRUE);

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(
        512,
        512,
        "Mainwindow",
        NULL,
        NULL
    );

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetCursorPosCallback(
        window,
        mouse_callback
    );

    glfwSetInputMode(
        window,
        GLFW_CURSOR,
        GLFW_CURSOR_DISABLED
    );

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    GLenum ret = glewInit();

    if (GLEW_OK != ret)
    {
        fprintf(stderr,
            "error: %s\n",
            glewGetErrorString(ret));

        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    // шейдер
    Shader ourShader(
        "vert.glsl",
        "frag.glsl"
    );

    // модель
    Model ourModel(
        "model_Sobolivsky.obj"
    );

    glm::mat4 model =
        glm::mat4(1.0f);

    model = glm::scale(
        model,
        glm::vec3(0.5f)
    );

    glm::mat4 view;
    glm::mat4 projection;

    float cameraSpeed = 0.05f;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(
            0.1f,
            0.1f,
            0.1f,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );

        ourShader.use();

        view = glm::lookAt(
            cameraPos,
            cameraPos + cameraFront,
            cameraUp
        );

        projection =
            glm::perspective(
                glm::radians(45.0f),
                512.0f / 512.0f,
                0.1f,
                100.0f
            );

        ourShader.setMat4(
            "model",
            model
        );

        ourShader.setMat4(
            "view",
            view
        );

        ourShader.setMat4(
            "projection",
            projection
        );

        ourShader.setVec3(
            "lightColor",
            0.8f,
            0.2f,
            0.7f
        );

        // движение камеры

        if (glfwGetKey(
            window,
            GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPos +=
                cameraSpeed *
                cameraFront;
        }

        if (glfwGetKey(
            window,
            GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPos -=
                cameraSpeed *
                cameraFront;
        }

        if (glfwGetKey(
            window,
            GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraPos -=
                glm::normalize(
                    glm::cross(
                        cameraFront,
                        cameraUp
                    )
                ) * cameraSpeed;
        }

        if (glfwGetKey(
            window,
            GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPos +=
                glm::normalize(
                    glm::cross(
                        cameraFront,
                        cameraUp
                    )
                ) * cameraSpeed;
        }

        // рендер модели
        ourModel.Draw(ourShader);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}