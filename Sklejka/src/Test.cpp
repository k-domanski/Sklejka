#include "Test.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include "imgui_impl_glfw.h"
#include "imguiOpenGL.h"
#include "stb_image.h"
#include "assimp/Importer.hpp"
#include <assimp/scene.h>
#include <iostream>
#include <assimp/postprocess.h>
#include <enum.h>
#include <irrKlang.h>

glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

BETTER_ENUM(Word, int, Hello, World)
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

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		window = glfwCreateWindow(1280, 960, "Test Window", NULL, NULL);
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
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 150");
		stbi_set_flip_vertically_on_load(true);
		ImGui::StyleColorsDark();
		Assimp::Importer importer;
		bool test = importer.IsExtensionSupported(".fbx");
		std::cout << (+Word::Hello)._to_string() << ", "
			<< (+Word::World)._to_string() << "!"
			<< std::endl;
		irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
		
		while (!glfwWindowShouldClose(window))
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui::ShowDemoWindow();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
		}
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
		return 0;
	}
}