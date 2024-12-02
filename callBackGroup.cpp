#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)//���ݴ��ڵ����ӿڴ�С
{
	glViewport(0, 0, width, height);
}

void error_callback(int, const char* err_str) {
	std::cout << "ERROR: " << err_str << std::endl;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

