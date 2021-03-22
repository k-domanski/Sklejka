#include "Test.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Sklejka
{
	void Print()
	{
		printf("Welcome to Sklejka Engine!\n");
	}

	int TestWindow()
	{
		GLFWwindow* window;
		if (!glfwInit())
			return -1;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		window = glfwCreateWindow(640, 480, "Test Window", NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			return -1;
		}
		unsigned int id;
		glGenBuffers(1, &id);
		while (!glfwWindowShouldClose(window))
		{
			glfwSwapBuffers(window);
		}

		glfwTerminate();
	}
}