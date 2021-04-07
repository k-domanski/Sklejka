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
#include <ECS/ECS.h>
#include <ECS/EntityManager.h>

#include <GL/UniformBufferData.h>
#include <Components/Transform.h>

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

using namespace Engine;

class MeshRenderer : public ECS::Component {};

class RendererSystem : public ECS::System {
public:
  RendererSystem() {
    AddSignature< Transform >();
    AddSignature< MeshRenderer >();
  }
  void Update() override {
    std::cout << "Updating renderer" << std::endl;
  }
  void Draw() override {
    std::cout << "Drawing renderer" << std::endl;
  }
};

class RandomSystem : public ECS::System {
public:
  RandomSystem() {
    AddSignature< Transform >();
  }

  void Update() override {
    std::cout << "Updating random" << std::endl;
  }
  void Draw() override {
    std::cout << "Drawing random" << std::endl;
  }
};

std::shared_ptr< ECS::Entity > ent;

template< typename T >
using ptr_t = std::shared_ptr< T >;

namespace Engine {
  // Event test
  void OnEvent(Event& event) {
    //CORE_TRACE("{0}", event);
  }

  int TestWindow() {
    Log::Init();

    ECS::EntityManager::GetInstance().RegisterSystem< RendererSystem >();
    ECS::EntityManager::GetInstance().RegisterSystem< RandomSystem >();

    ent = ECS::EntityManager::GetInstance().CreateEntity();
    ent->AddComponent< Transform >();

    auto entity = ECS::EntityManager::GetInstance().CreateEntity();
    entity->AddComponent< Transform >();

    auto notentity = ECS::EntityManager::GetInstance().CreateEntity();
    notentity->AddComponent< MeshRenderer >();

    auto entity2 = ECS::EntityManager::GetInstance().CreateEntity();
    entity2->AddComponent< Transform >();
    entity2->AddComponent< MeshRenderer >();

    ECS::EntityManager::GetInstance().Update();
    ECS::EntityManager::GetInstance().Draw();

    ent->AddComponent< MeshRenderer >();
    entity->AddComponent< MeshRenderer >();

    // TODO: Window as singleton
    std::unique_ptr< Engine::Window > window = Engine::Window::Create(Engine::WindowProperties());
    window->SetEventCallback(std::bind(OnEvent, std::placeholders::_1));
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
    Transform tr;
    tr.Rotate(glm::radians(90.0f), {0.0f, 1.0f, 0.0f});
    tr.Rotate(glm::radians(90.0f), {0.0f, 0.0f, 1.0f});
    CORE_TRACE("Right   [{0}, {1}, {2}]", tr.Right().x, tr.Right().y, tr.Right().z);
    CORE_TRACE("Up      [{0}, {1}, {2}]", tr.Up().x, tr.Up().y, tr.Up().z);
    CORE_TRACE("Forward [{0}, {1}, {2}]", tr.Forward().x, tr.Forward().y, tr.Forward().z);

    /* Uniform Buffer */
    GL::CameraUniformData camera_data;
    GL::CameraUniformBuffer camera_buffer;
    glm::vec3 camera_pos{0.0f, 0.0f, 2.0f};
    camera_data.view =
        glm::lookAt(camera_pos, camera_pos + glm::vec3{0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f});
    auto aspect            = window->GetWidth() / (float)window->GetHeight();
    camera_data.projection = glm::perspective(45.0f, aspect, 0.001f, 1000.0f);

    camera_buffer.SetData(camera_data);
    camera_buffer.BindToSlot(0);
    texture->Bind(0);
    shader->SetValue("u_mainTexture", 0);
    shader->BindUniformBlock("u_Camera", 0);
    /* ------------------- */

    /* ================ */

    Timer timer;
    float time = 0.0f;
    // TODO: Wrap WindowShouldClose in Window class
    while (!glfwWindowShouldClose(window->GetNativeWindow())) {
      timer.Update();
      time += timer.DeltaTime() * 0.1f;
      // TODO: Separate ImGUI calls
      // ImGui_ImplOpenGL3_NewFrame();
      // ImGui_ImplGlfw_NewFrame();
      // ImGui::NewFrame();

      GL::Context::ClearBuffers();
      auto model_matrix =
          glm::eulerAngleXYZ(time, time, time) * glm::scale(glm::mat4{1.0f}, glm::vec3{0.2f});
      /* -------------------------- */
      shader->Use();
      shader->SetMatrix("mvp", camera(7.f, glm::vec2(time, time)));
      shader->SetMatrix("u_model_matrix", model_matrix);
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