#ifndef GLFW_CONTEXT_HPP
#define GLFW_CONTEXT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

class GLFWContext
{
    public:
    GLFWContext();
    ~GLFWContext();

    void createWindow(unsigned int width, unsigned int height, std::string name);

    void setCurrent();

    void initGLAD();

    void swapBuffers();

    GLFWwindow* getWindow();

    private:

    static void framebufferCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* window;
};

#endif