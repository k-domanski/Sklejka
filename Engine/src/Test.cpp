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
#include <App/Timer.h>
#include <Renderer/Mesh.h>
#include <Renderer/Model.h>
#include <Utility/Utility.h>
#include <App/AssetManager.h>

/// <summary>
///
/// </summary>
/// <param name="Translate">Only in Z axis because yes</param>
/// <param name="Rotate">Only in X and Y axis... You have Z axis in translate XD</param>
/// <returns></returns>
glm::mat4 camera(float Translate, glm::vec2 const& Rotate) {
  glm::mat4 Projection = glm::perspective(glm::pi< float >() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
  glm::mat4 View       = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
  View                 = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
  View                 = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 Model      = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
  return Projection * View * Model;
}

namespace Engine {
  // Remove
  void Print() {
    printf("Welcome to Sklejka Engine!\n");
  }

  int TestWindow() {
    Log::Init();

    // TODO: Window as singleton
    std::unique_ptr< Engine::Window > window = Engine::Window::Create(Engine::WindowData());
    stbi_set_flip_vertically_on_load(true);

    // TODO: Spearate ImGUI calls
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window->GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 430");
    ImGui::StyleColorsDark();

    /* OpenGL calls for initialization */
    GL::Context::Initialize();
    GL::Context::SetClearBufferMask(GL::BufferBit::Color);
    GL::Context::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});

    /* Example square */
    std::vector< glm::vec3 > points = {{-0.5f, -0.5f, 0.0f},
                                       {0.5f, -0.5f, 0.0f},
                                       {0.5f, 0.5f, 0.0f},
                                       {-0.5f, 0.5f, 0.0f}};
    std::vector< glm::vec2 > uv     = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
    std::vector< GLuint > indices   = {0, 1, 2, 2, 3, 0};
    std::vector< Renderer::Vertex > vertices(points.size());
    for (int i = 0; i < vertices.size(); ++i) {
      auto p      = points[i];
      auto u      = uv[i];
      vertices[i] = {p, glm::vec3{0.0f}, u};
    }

    /* ------------------- */
    using GL::Shader;
    using GL::ShaderType;
    using GL::SubShader;
    using GL::Texture2D;
    using Renderer::Mesh;
    using Renderer::Model;

    auto mesh      = std::make_shared< Mesh >(vertices, indices);
    auto texture   = AssetManager::GetTexture2D("./textures/pepo_sad.png");
    auto coneModel = AssetManager::GetModel("./models/smolCone.fbx");
    auto shader    = AssetManager::GetShader("./shaders/default.glsl");
    assert(("Failed to acquire shader", shader != nullptr));

    Mesh* coneMesh = coneModel->getRootMesh();

    texture->Bind(0);
    shader->SetValue("u_mainTexture", 0);
    /* ------------------- */

    /* ================ */

    Timer timer;
    float time = 0.0f;
    // TODO: Wrap WindowShouldClose in Window class
    while (!glfwWindowShouldClose(window->GetNativeWindow())) {
      timer.Update();
      time += timer.DeltaTime();
      // TODO: Separate ImGUI calls
      // ImGui_ImplOpenGL3_NewFrame();
      // ImGui_ImplGlfw_NewFrame();
      // ImGui::NewFrame();

      GL::Context::ClearBuffers();

      /* -------------------------- */
      shader->Use();
      shader->SetMatrix("mvp", camera(7.f, glm::vec2(time, time)));
      coneMesh->Use();
      glDrawElements(coneMesh->GetPrimitive(), coneMesh->ElementCount(), GL_UNSIGNED_INT, NULL);
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