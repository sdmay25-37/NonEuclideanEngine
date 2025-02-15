#include "GLFWContext.hpp"

GLFWContext::GLFWContext()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

GLFWContext::~GLFWContext()
{

}

void GLFWContext::initAll(unsigned int width, unsigned int height, std::string window_name)
{
    createWindow(width, height, window_name);
    setCurrent();
    initGLAD();
}

void GLFWContext::createWindow(unsigned int width, unsigned int height, std::string window_name)
{
    window = glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

}

void GLFWContext::setCurrent()
{
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferCallback);
}

void GLFWContext::initGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}

void GLFWContext::swapBuffers()
{
    glfwSwapBuffers(window);
}

GLFWwindow* GLFWContext::getWindow()
{
    return window;
}

void GLFWContext::framebufferCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}