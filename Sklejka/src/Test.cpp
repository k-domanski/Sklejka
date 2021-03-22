#include "Test.h"
#include <stdio.h>
#include <GLFW/glfw3.h>

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

		while (!glfwWindowShouldClose(window))
		{
			glfwSwapBuffers(window);
		}

		glfwTerminate();
	}
}