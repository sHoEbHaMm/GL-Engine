#pragma once
#include<iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

class GLFWindow
{
public:
    static GLFWwindow* CreateWindow(const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT);
    static void processInput(GLFWwindow* window);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};