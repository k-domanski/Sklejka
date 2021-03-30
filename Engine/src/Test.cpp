#include "pch.h"
#include "Test.h"
#include <stdio.h>
#include <glm/vec3.hpp>                   // glm::vec3
#include <glm/vec4.hpp>                   // glm::vec4
#include <glm/mat4x4.hpp>                 // glm::mat4
#include <glm/ext/matrix_transform.hpp>   // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp>  // glm::perspective
#include <glm/ext/scalar_constants.hpp>   // glm::pi
#include "imgui_impl_glfw.h"
#include "imguiOpenGL.h"
#include "stb_image.h"
#include "assimp/Importer.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <irrKlang.h>
#include <rttr/type>
// tmp
#include <GL/GLCore.h>
#include "App/Window.h"
#include <App/Log.h>
#include <Renderer/Mesh.h>
#include <Utility/Utility.h>

glm::mat4 camera(float Translate, glm::vec2 const& Rotate) {
  glm::mat4 Projection = glm::perspective(glm::pi< float >() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
  glm::mat4 View       = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
  View                 = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
  View                 = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 Model      = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
  return Projection * View * Model;
}

template< typename T >
using ptr_t = std::shared_ptr< T >;

BETTER_ENUM(Word, int, Hello, World)
namespace Engine {
  // Remove
  void Print() {
    printf("Welcome to Sklejka Engine!\n");
  }

  int TestWindow() {
    Log::Init();

    // TODO: Window as singleton
    std::unique_ptr< Engine::Window > window = Engine::Window::Create(Engine::WindowData());

    // TODO: Spearate ImGUI calls
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window->GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 430");
    ImGui::StyleColorsDark();

    /* OpenGL calls for initialization */
    GL::Context::Initialize();
    GL::Context::SetClearBufferMask(GL::BufferBit::Color);
    GL::Context::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});

    stbi_set_flip_vertically_on_load(true);

    /* Example triangle */
    std::vector< glm::vec3 > points = {{-0.5f, -0.5f, 0.0f},
                                       {0.5f, -0.5f, 0.0f},
                                       {0.0f, 0.5f, 0.0f}};
    std::vector< GLuint > indices   = {0, 1, 2};
    std::vector< Renderer::Vertex > vertices;
    for (const auto& p : points) {
      Renderer::Vertex v{p, glm::vec3{0.0f}, glm::vec2{0.0f}};
      vertices.push_back(v);
    }

    /* ------------------- */
    using GL::Shader;
    using GL::ShaderType;
    using GL::SubShader;
    using Renderer::Mesh;
    ptr_t< Mesh > mesh = std::make_shared< Mesh >(vertices, indices);
#define FOLDER_PATH "./shaders"
    auto vert_src           = Utility::ReadTextFile(FOLDER_PATH "/pass.vert");
    auto frag_src           = Utility::ReadTextFile(FOLDER_PATH "/color.frag");
    ptr_t< SubShader > vert = std::make_shared< SubShader >(ShaderType::VertexShader, vert_src);
    ptr_t< SubShader > frag = std::make_shared< SubShader >(ShaderType::FragmentShader, frag_src);
    ptr_t< Shader > shader  = std::make_shared< Shader >();
    shader->AttachShader(vert);
    shader->AttachShader(frag);
    shader->Link();
    /* ------------------- */

    /* ================ */

    // TODO: Wrap WindowShouldClose in Window class
    while (!glfwWindowShouldClose(window->GetNativeWindow())) {
      // TODO: Separate ImGUI calls
      // ImGui_ImplOpenGL3_NewFrame();
      // ImGui_ImplGlfw_NewFrame();
      // ImGui::NewFrame();

      GL::Context::ClearBuffers();

      /* -------------------------- */
      shader->Use();
      mesh->Use();
      glDrawElements(mesh->GetPrimitive(), mesh->ElementCount(), GL_UNSIGNED_INT, NULL);
      /* -------------------------- */

      // TODO: Change name
      window->OnUpdate();

      // ImGui::ShowDemoWindow();
      // ImGui::Render();
      // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // glfwTerminate();
    return 0;
  }
}  // namespace Engine