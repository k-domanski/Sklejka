#include "EditorLayer.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

#include "Engine/SceneManager.h"
#include "Systems/SceneGraph.h"

using namespace Engine;
EditorLayer::EditorLayer(const std::string& name): Layer(name) {
  // Scene manager test
  size_t sceneID = AssetManager::GenerateAssetID();
  SceneManager::AddScene(std::make_shared< Scene >(sceneID));
  std::cout << "sceneID: " << sceneID << "\n";
  SceneManager::OpenScene(sceneID);
}
void EditorLayer::OnAttach() {
  LOG_TRACE("Working directory: {}", AssetManager::GetWoringDir());

  /* ---------Camera--------- */
  auto aspect           = Engine::Window::Get().GetAspectRatio();
  auto camera_entity    = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  m_EditorCamera.entity = camera_entity;
  m_EditorCamera.camera =
      camera_entity->AddComponent< Engine::Camera >(45.0f, aspect, 0.001f, 1000.0f);
  m_EditorCamera.transform = camera_entity->AddComponent< Engine::Transform >();
  SceneManager::GetDisplayScene()->SceneGraph()->AddChild(0, camera_entity->GetID());

  m_EditorCamera.camera->flags.Set(Engine::CameraFlag::MainCamera);
  m_EditorCamera.camera->flags.Set(Engine::CameraFlag::EditorCamera);
  editorCameraArgs.screenSize = Window::Get().GetScreenSize();
  m_EditorCamera.transform->Position({0.0f, 0.0f, 2.0f});
  m_EditorCamera.transform->Rotate(glm::radians(180.0f), {0.0f, 1.0f, 0.0f});
  /* ----------------------- */
  //auto entity = ECS::EntityManager::GetInstance().CreateEntity();
  //entity->AddComponent<Transform>();
  //entity->GetComponent<Transform>()->Scale(glm::vec3(0.005f));
  //auto model = AssetManager::GetModel("models/Pilot_LP_Animated.fbx");
  ////auto model = AssetManager::GetModel("models/dancing_vampire.dae");
  //auto material = AssetManager::GetMaterial("materials/anim_trooper.mat");
  //entity->AddComponent<Components::MeshRenderer>(model, material);
  //SceneManager::GetCurrentScene()->SceneGraph()->AddChild(0, entity->GetID());

  /* ---------Editor Panels--------- */
  m_SceneHierarchyPanel.SetScene(SceneManager::GetDisplayScene());
  m_SceneHierarchyPanel.SetEditorLayer(this);
  m_SceneHierarchyPanel.SetSelectionCallback(
      [this](auto& entity) { m_InspectorPanel.AttachEntity(entity); });
  m_FileSystemPanel.SetScene(SceneManager::GetDisplayScene());
  m_FileSystemPanel.SetEditorLayer(this);
  /*---------------------------------*/

  /* -----------------------Start scene---------------------------- */
  // auto scene = AssetManager::LoadScene("./\\scenes\\dzielo_sztuki_with_no_cam.scene");
  //
  // SceneManager::AddScene(scene);
  // SceneManager::OpenScene(scene->GetID());
  // m_SceneHierarchyPanel.SetScene(SceneManager::GetCurrentScene());
  //
  ///* Force inject editor camera into the scene */
  // scene->SceneGraph()->AddChild(0, m_EditorCamera.entity->GetID());
  // ECS::EntityManager::InjectEntity(m_EditorCamera.entity);
  // ECS::EntityManager::InjectComponent< Transform >(m_EditorCamera.transform);
  // ECS::EntityManager::InjectComponent< Camera >(m_EditorCamera.camera);
  /* ---------------------------------------------------------------- */
}

void EditorLayer::OnUpdate(double deltaTime) {
  UpdateEditorCamera();

  SceneManager::GetCurrentScene()->Update(deltaTime);
  /*Update systemów w aplikacji?*/
  SceneManager::GetCurrentScene()->Draw();
}

void EditorLayer::OnDetach() {
}

void EditorLayer::OnEvent(Event& event) {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch< WindowResizeEvent >(BIND_EVENT_FN(EditorLayer::OnWindowResize));
  dispatcher.Dispatch< MouseScrolledEvent >(BIND_EVENT_FN(EditorLayer::OnMouseScroll));
  dispatcher.Dispatch< MouseButtonPressedEvent >(BIND_EVENT_FN(EditorLayer::OnMouseButtonPress));
  dispatcher.Dispatch< MouseButtonReleasedEvent >(BIND_EVENT_FN(EditorLayer::OnMouseButtonRelease));
  dispatcher.Dispatch< KeyPressedEvent >(BIND_EVENT_FN(EditorLayer::OnKeyPress));
}

void EditorLayer::OnImGuiRender() {
  DrawMenuBar();
  m_FileSystemPanel.OnImGuiRender();
  m_SceneHierarchyPanel.OnImGuiRender();
  m_InspectorPanel.OnImGuiRender();
  m_MaterialPanel.OnImGuiRender();
}

bool EditorLayer::OnWindowResize(Engine::WindowResizeEvent& e) {
  glm::vec2 size = {(float)e.GetWidth(), (float)e.GetHeight()};
  SceneManager::GetDisplayScene()->OnWindowResize(size);
  editorCameraArgs.screenSize = size;
  m_EditorCamera.camera->Aspect(Window::Get().GetAspectRatio());
  return true;
}

bool EditorLayer::OnMouseScroll(MouseScrolledEvent& e) {
  editorCameraArgs.scrollDelta = e.GetYOffset();
  return true;
}

bool EditorLayer::OnMouseButtonPress(MouseButtonPressedEvent& e) {
  if (e.GetMouseButton() == MouseCode::ButtonMiddle)
    editorCameraArgs.m3LastPos = Input::GetMousePosition();
  else if (e.GetMouseButton() == MouseCode::ButtonRight)
    editorCameraArgs.m2LastPos = Input::GetMousePosition();

  return true;
}

bool EditorLayer::OnMouseButtonRelease(MouseButtonReleasedEvent& e) {
  return false;
}

bool EditorLayer::OnKeyPress(Engine::KeyPressedEvent& e) {
  if (e.GetKeyCode() == Key::C) {
    // TODO: switch camera somehow
    // SceneManager::GetCurrentScene().
    ECS::EntityManager::GetInstance().GetSystem< Systems::CameraSystem >()->SwitchView();
  }
  return true;
}

auto EditorLayer::UpdateEditorCamera() -> void {
  if (editorCameraArgs.scrollDelta != 0.0f) {
    m_EditorCamera.transform->Position(m_EditorCamera.transform->Position()
                                       + m_EditorCamera.transform->Forward()
                                             * editorCameraArgs.scrollDelta * 0.1f);
    editorCameraArgs.scrollDelta = 0.0f;
  }
  /*Scroll Pressed Movement*/
  if (Input::IsMouseButtonPressed(MouseCode::ButtonMiddle)) {
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
  else if (Input::IsMouseButtonPressed(MouseCode::ButtonRight)) {
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

auto EditorLayer::OpenModel() {
  std::optional< std::string > filepath = FileDialog::OpenFile("Model (*.fbx, *.fbx)\0*.fbx\0");
  if (filepath)
  {
    std::shared_ptr< Renderer::Model > model_ptr = std::make_shared< Renderer::Model >(filepath.value());
    std::vector< ECS::EntityID > loadedMeshes_ids(100);

    ECS::EntityID rootID = AddObjectOnScene(model_ptr, 0, 0, &loadedMeshes_ids);

    for (int i = 1; i < model_ptr->GetMeshCount(); i++)
    {
      AddObjectOnScene(model_ptr, i, rootID, &loadedMeshes_ids);
    }
  }
}

auto EditorLayer::DrawMenuBar() -> void {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open...")) {
        LoadScene();
      }
      if (ImGui::MenuItem("Load model...")) {
        OpenModel();
      }
      if (ImGui::MenuItem("Save As...")) {
        SaveScene();
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}

auto EditorLayer::SaveScene() -> void {
  std::optional< std::string > filepath = FileDialog::SaveFile("Scene (*.scene)\0*.scene\0");
  if (filepath) {
    auto scene = SceneManager::GetCurrentScene();
    auto graph = scene->SceneGraph();

    // Detach Editor Camera before saving
    graph->DetachFromGraph(m_EditorCamera.entity->GetID());
    AssetManager::SaveScene(SceneManager::GetCurrentScene(), filepath.value());
    // Reattach Editor Camera before saving
    graph->AddChild(0, m_EditorCamera.entity->GetID());
  }
}

auto EditorLayer::LoadScene() -> void {
  std::optional< std::string > filepath = FileDialog::OpenFile("Scene (*.scene)\0*.scene\0");
  if (filepath) {
    /* Remove editor camera from current scene */
    ECS::EntityManager::GetInstance().RemoveEntity(m_EditorCamera.entity->GetID());

    auto scene = AssetManager::LoadScene(filepath.value());

    SceneManager::AddScene(scene);
    SceneManager::OpenScene(scene->GetID());
    m_SceneHierarchyPanel.SetScene(SceneManager::GetCurrentScene());

    scene->CameraSystem()->EditorView(true);

    /* Force inject editor camera into the new scene */
    scene->SceneGraph()->AddChild(0, m_EditorCamera.entity->GetID());
    ECS::EntityManager::InjectEntity(m_EditorCamera.entity);
    ECS::EntityManager::InjectComponent< Transform >(m_EditorCamera.transform);
    ECS::EntityManager::InjectComponent< Camera >(m_EditorCamera.camera);
  }
}

auto EditorLayer::AddObjectOnScene(const std::string& path, Engine::ECS::EntityID parent) -> ECS::EntityID {
  auto model = AssetManager::GetModel(path);
  if (model->GetRootMesh() == nullptr)
    return 0;
  using namespace Engine::ECS;
  using namespace Engine::Components;
  auto entity = EntityManager::GetInstance().CreateEntity();
  entity->Name(std::filesystem::path(path).filename().stem().string());
  entity->AddComponent< Transform >();
  /*auto shader = AssetManager::GetShader("./shaders/default.glsl");
  auto mat    = AssetManager::GetMaterial(shader, nullptr);*/
  auto mat = AssetManager::GetMaterial("./materials/default_color.mat");
  entity->AddComponent< MeshRenderer >(model, mat);

  SceneManager::GetDisplayScene()->SceneGraph()->AddChild(parent, entity->GetID());
  return entity->GetID();
}

auto EditorLayer::AddObjectOnScene(std::shared_ptr< Renderer::Model > model, int meshIndex,
                                   Engine::ECS::EntityID parent, std::vector<ECS::EntityID>* loadedMeshes) -> ECS::EntityID {
  if (model->GetRootMesh() == nullptr)
    return 0;
  using namespace Engine::ECS;
  using namespace Engine::Components;
  auto entity = EntityManager::GetInstance().CreateEntity();
  entity->Name(model->GetMesh(meshIndex)->GetName());
  entity->AddComponent< Transform >();
  entity->GetComponent< Transform >()->SetLocalMatrix(model->GetMesh(meshIndex)->GetModelMatrix());
  /*auto shader = AssetManager::GetShader("./shaders/default.glsl");
  auto mat    = AssetManager::GetMaterial(shader, nullptr);*/
  auto mat = AssetManager::GetMaterial("./materials/default_color.mat");
  entity->AddComponent< MeshRenderer >(model, meshIndex, mat);

  int parentMeshIndex = model->GetMesh(meshIndex)->GetParentMesh();
  SceneManager::GetDisplayScene()->SceneGraph()->AddChild(
      parentMeshIndex != -1 ? (*loadedMeshes)[parentMeshIndex] : 0, entity->GetID());
  loadedMeshes->insert(loadedMeshes->begin() + meshIndex, entity->GetID());
  return entity->GetID();
}
