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

    std::vector< glm::vec3 > bg_points = {{-1.0f, -0.7f, 0.0f},
                                          {1.0f, -0.7f, 0.0f},
                                          {1.f, 1.f, 0.0f},
                                          {-1.f, 1.f, 0.0f}};
    std::vector< Renderer::Vertex > bg_verts(bg_points.size());
    for (int i = 0; i < vertices.size(); ++i) {
      auto p      = bg_points[i];
      auto u      = uv[i];
      bg_verts[i] = {p, glm::vec3{0.0f}, u};
    }

    std::vector< glm::vec3 > jojo_points = {{-0.7f, -1.0f, 0.0f},
                                            {-0.1f, -1.0f, 0.0f},
                                            {-0.1f, -0.1f, 0.0f},
                                            {-0.7f, -0.1f, 0.0f}};
    std::vector< Renderer::Vertex > jojo_verts(jojo_points.size());
    for (int i = 0; i < vertices.size(); ++i) {
      auto p        = jojo_points[i];
      auto u        = uv[i];
      jojo_verts[i] = {p, glm::vec3{0.0f}, u};
    }

    /* ------------------- */
    using GL::Shader;
    using GL::ShaderType;
    using GL::SubShader;
    using GL::TextureBase;
    using Renderer::Mesh;
    /* ----- Texture ----- */
    // PEPO
    stbi_set_flip_vertically_on_load(true);
    int x, y, n;
    auto pixel_data = stbi_load("./textures/pepo_sad.png", &x, &y, &n, 4);
    TextureBase texture(GL_TEXTURE_2D);
    texture.Create(x, y, pixel_data);
    stbi_image_free(pixel_data);

    // JOJO
    pixel_data = stbi_load("./textures/jojo.png", &x, &y, &n, 4);
    TextureBase jojo_tex(GL_TEXTURE_2D);
    jojo_tex.Create(x, y, pixel_data);
    stbi_image_free(pixel_data);
    /* ----------- */

    ptr_t< Mesh > pepe_mesh = std::make_shared< Mesh >(vertices, indices);
    ptr_t< Mesh > bg_mesh   = std::make_shared< Mesh >(bg_verts, indices);
    ptr_t< Mesh > jojo_mesh = std::make_shared< Mesh >(jojo_verts, indices);
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
      // Draw bg
      bg_mesh->Use();
      glBindTexture(GL_TEXTURE_2D, 0);
      shader->SetValue("u_time", glm::radians(90.0f));
      shader->SetVector("u_color", glm::vec3{0.4f, 0.4f, 1.0f});
      glDrawElements(bg_mesh->GetPrimitive(), bg_mesh->ElementCount(), GL_UNSIGNED_INT, NULL);

      // Draw Jojo
      jojo_mesh->Use();
      jojo_tex.Bind(0);
      shader->SetVector("u_color", glm::vec3{0.0f});
      shader->SetValue("u_mainTexture", 0);
      glDrawElements(jojo_mesh->GetPrimitive(), jojo_mesh->ElementCount(), GL_UNSIGNED_INT, NULL);

      // Draw pepe
      pepe_mesh->Use();
      texture.Bind(0);
      shader->SetVector("u_color", glm::vec3{0.0f});
      shader->SetValue("u_mainTexture", 0);
      shader->SetValue("u_time", time);
      glDrawElements(pepe_mesh->GetPrimitive(), pepe_mesh->ElementCount(), GL_UNSIGNED_INT, NULL);

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