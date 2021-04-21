#include "EditorLayer.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

using namespace Engine;
EditorLayer::EditorLayer(const std::string& name): Layer(name) {
}
void EditorLayer::OnAttach() {
  LOG_TRACE("Working directory: {}", AssetManager::GetWoringDir());
  /*Assets*/
  auto texture   = AssetManager::GetTexture2D("./textures/pepo_sad.png");
  auto coneModel = AssetManager::GetModel("./models/cube.fbx");
  m_Shader       = AssetManager::GetShader("./shaders/default.glsl");
  assert(("Failed to acquire shader", m_Shader != nullptr));
  m_ConeMesh = coneModel->getRootMesh();
  // m_PepeModel       = AssetManager::GetModel("./models/squirrel.fbx");
  m_PepeModel = AssetManager::GetModel("./models/silly_dancing.fbx");
  // auto tex_shader   = AssetManager::GetShader("./shaders/texture_shader.glsl");
  auto pepe_texture = AssetManager::GetTexture2D("./textures/Stormtrooper_D.png");
  // auto pepe_texture = AssetManager::GetTexture2D("./textures/Untilted.png");
  m_PepeMaterial = AssetManager::CreateMaterial();

  m_PepeMaterial->SetShader(m_Shader);
  m_PepeMaterial->SetMainTexture(pepe_texture);

  auto aspect        = Engine::Window::Get().GetAspectRatio();
  auto camera_entity = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  m_EditorCamera.camera =
      camera_entity->AddComponent< Engine::Camera >(45.0f, aspect, 0.001f, 1000.0f);
  m_EditorCamera.transform = camera_entity->AddComponent< Engine::Transform >();
  m_Scene->SceneGraph()->AddChild(0, camera_entity->GetID());

  /*Camera*/
  m_EditorCamera.camera->flags.Set(Engine::CameraFlag::MainCamera);
  editorCameraArgs.screenSize = Window::Get().GetScreenSize();
  m_EditorCamera.transform->Position({0.0f, 0.0f, 2.0f});
  m_EditorCamera.transform->Rotate(glm::radians(180.0f), {0.0f, 1.0f, 0.0f});

  /*ECS Scene*/
  m_Entity1 = ECS::EntityManager::GetInstance().CreateEntity();
  m_Entity2 = ECS::EntityManager::GetInstance().CreateEntity();
  m_Pepe    = ECS::EntityManager::GetInstance().CreateEntity();
  // LOG_TRACE("Past");
  m_Material = AssetManager::GetMaterial("./material.json");
  // m_Material = AssetManager::GetMaterial(m_Shader, "./shaders/default.glsl",
  //                                       "./textures/pepo_sad.png", texture);

  m_Entity1->AddComponent< Transform >();
  m_Entity1->AddComponent< Components::MeshRenderer >(m_ConeMesh, m_Material);
  m_Entity2->AddComponent< Transform >();
  m_Entity2->AddComponent< Components::MeshRenderer >(m_ConeMesh, m_Material);
  m_PepeTransform = m_Pepe->AddComponent< Transform >();
  m_Pepe->AddComponent< Components::MeshRenderer >(m_PepeModel->getRootMesh(), m_PepeMaterial);
  auto sg = m_Scene->SceneGraph();
  sg->AddChild(0, m_Pepe->GetID());
  sg->AddChild(0, m_Entity1->GetID());
  sg->AddChild(0, m_Entity2->GetID());

  /*Editor Panels*/
  m_SceneHierarchyPanel.SetScene(m_Scene);
  m_SceneHierarchyPanel.SetSelectionCallback(
      [this](auto& entity) { m_InspectorPanel.AttachEntity(entity); });
  m_FileSystemPanel.SetScene(m_Scene);
  m_FileSystemPanel.SetEditorLayer(this);
  /*------------------------*/

  auto box1 = m_Entity1->AddComponent< Components::Collider >();
  auto rb1  = m_Entity1->AddComponent< Components::Rigidbody >();
  auto box2 = m_Entity2->AddComponent< Components::Collider >();
  auto rb2  = m_Entity2->AddComponent< Components::Rigidbody >();

  box1->Size      = glm::vec3(1.0f);
  box1->IsTrigger = false;
  box1->Center    = glm::vec3(0.0f);
  rb1->SetGravity(false);
  rb1->SetKinematic(true);
  box2->Size      = glm::vec3(1.0f);
  box2->IsTrigger = false;
  box2->Center    = glm::vec3(0.0f);
  rb2->SetGravity(false);
  rb2->SetKinematic(true);

  box1->SaveToJson("./box1.json");
  box2->SaveToJson("./box2.json");

  auto tr1 = m_Entity1->GetComponent< Transform >();
  auto tr2 = m_Entity2->GetComponent< Transform >();

  tr1->LoadFromJson("./tr1.json");
  tr2->LoadFromJson("./tr2.json");
  m_PepeTransform->Position({0.0f, 1.0f, 0.0f});
  m_PepeTransform->Scale({0.2f, 0.2f, 0.2f});
}

void EditorLayer::OnUpdate(double deltaTime) {
  /* -------------------------- */
  /* TODO: CameraController move to separate class?*/
  UpdateEditorCamera();
  /* -------------------------- */
  m_Time += deltaTime / 2.0f;
  // auto tr1 = m_Entity1->GetComponent< Transform >();
  // tr1->Rotate(deltaTime * 0.3, {0.0f, 1.0f, 0.0f});
  // m_PepeTransform->Rotate(deltaTime * 0.1, {0.0f, 1.0f, 0.0f});
  // auto pos = tr1->Position();
  // tr1->Position(glm::vec3(sin(m_Time) * m_it, 0.0f, 0.0f));
  // m_it += deltaTime / 20.0f;
  // std::cout << "sin time: " << sin(m_Time)*m_it << std::endl;

  m_Scene->Update(deltaTime);
  /*Update systemów w aplikacji?*/
  m_Scene->Draw();
}

void EditorLayer::OnDetach() {
}

void EditorLayer::OnEvent(Event& event) {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch< WindowResizeEvent >(BIND_EVENT_FN(EditorLayer::OnWindowResize));
  dispatcher.Dispatch< MouseScrolledEvent >(BIND_EVENT_FN(EditorLayer::OnMouseScroll));
  dispatcher.Dispatch< MouseButtonPressedEvent >(BIND_EVENT_FN(EditorLayer::OnMouseButtonPress));
  dispatcher.Dispatch< MouseButtonReleasedEvent >(BIND_EVENT_FN(EditorLayer::OnMouseButtonRelease));
}

void EditorLayer::OnImGuiRender() {
  m_SceneHierarchyPanel.OnImGuiRender();
  m_InspectorPanel.OnImGuiRender();
  m_FileSystemPanel.OnImGuiRender();
  m_MaterialPanel.OnImGuiRender();
}

bool EditorLayer::OnWindowResize(Engine::WindowResizeEvent& e) {
  glm::vec2 size = {(float)e.GetWidth(), (float)e.GetHeight()};
  m_Scene->OnWindowResize(size);
  editorCameraArgs.screenSize = size;
  m_EditorCamera.camera->Aspect(Window::Get().GetAspectRatio());
  return true;
}

bool EditorLayer::OnMouseScroll(MouseScrolledEvent& e) {
  editorCameraArgs.scrollDelta = e.GetYOffset();
  return true;
}

bool EditorLayer::OnMouseButtonPress(MouseButtonPressedEvent& e) {
  if (e.GetMouseButton() == 2)
    editorCameraArgs.m3LastPos = Input::GetMousePosition();
  else if (e.GetMouseButton() == 1)
    editorCameraArgs.m2LastPos = Input::GetMousePosition();

  return true;
}

bool EditorLayer::OnMouseButtonRelease(MouseButtonReleasedEvent& e) {
  return false;
}

auto EditorLayer::UpdateEditorCamera() -> void {
  if (editorCameraArgs.scrollDelta != 0.0f) {
    m_EditorCamera.transform->Position(m_EditorCamera.transform->Position()
                                       + m_EditorCamera.transform->Forward()
                                             * editorCameraArgs.scrollDelta * 0.1f);
    editorCameraArgs.scrollDelta = 0.0f;
  }
  /*Scroll Pressed Movement*/
  if (Input::IsMouseButtonPressed(2)) {
    auto cursorPos = Input::GetMousePosition();
    auto delta     = cursorPos - editorCameraArgs.m3LastPos;
    delta /= editorCameraArgs.screenSize;

    auto position = m_EditorCamera.transform->Position();
    position += m_EditorCamera.transform->Right() * delta.x * editorCameraArgs.sensitivity;
    position += m_EditorCamera.transform->Up() * delta.y * editorCameraArgs.sensitivity;
    m_EditorCamera.transform->Position(position);
    editorCameraArgs.m3LastPos = cursorPos;
  }
  /*Right Mouse Button*/
  else if (Input::IsMouseButtonPressed(1)) {
    auto cursorPos = Input::GetMousePosition();
    auto delta     = cursorPos - editorCameraArgs.m2LastPos;
    delta /= editorCameraArgs.screenSize;

    // TODO: Add clamping in X axis;
    const auto sensitivity = 120.0f;
    m_EditorCamera.transform->Rotate(glm::radians(-delta.x * sensitivity), {0.0f, 1.0f, 0.0f});
    auto dy = delta.y * sensitivity;
    // Locking rotation in [-89.0f, 89.0f] range
    auto angle = glm::degrees(
        glm::angle(glm::rotation(m_EditorCamera.transform->Forward(), {0.0f, 1.0f, 0.0f})));
    if (auto fa = angle + dy; fa < 1.0f || fa > 179.0f) {
      dy = 0.0f;
    }
    m_EditorCamera.transform->Rotate(glm::radians(dy), m_EditorCamera.transform->Right());

    editorCameraArgs.m2LastPos = cursorPos;
  }
}

auto EditorLayer::AddObjectOnScene(const std::string& path, Engine::ECS::EntityID parent) -> void {
  auto model = AssetManager::GetModel(path);
  if (model->getRootMesh() == nullptr)
    return;
  using namespace Engine::ECS;
  using namespace Engine::Components;
  auto entity = EntityManager::GetInstance().CreateEntity();
  entity->Name(std::filesystem::path(path).filename().stem().string());
  entity->AddComponent< Transform >();
  /*auto shader = AssetManager::GetShader("./shaders/default.glsl");
  auto mat    = AssetManager::GetMaterial(shader, nullptr);*/
  entity->AddComponent< MeshRenderer >(model->getRootMesh(), nullptr);

  m_Scene->SceneGraph()->AddChild(parent, entity->GetID());
}
