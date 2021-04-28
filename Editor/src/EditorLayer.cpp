#include "EditorLayer.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

#include "Engine/SceneManager.h"
#include "Systems/SceneGraph.h"

using namespace Engine;
EditorLayer::EditorLayer(const std::string& name): Layer(name) {
  // Scene manager test
  SceneManager::AddScene(std::make_shared< Scene >(1));
  SceneManager::OpenScene(1);
}
void EditorLayer::OnAttach() {
  LOG_TRACE("Working directory: {}", AssetManager::GetWoringDir());
  /*Assets*/
  auto texture   = AssetManager::GetTexture2D("./textures/pepo_sad.png");
  auto coneModel = AssetManager::GetModel("./models/cube.fbx");
  m_Shader       = AssetManager::GetShader("./shaders/default.glsl");
  assert(("Failed to acquire shader", m_Shader != nullptr));
  m_ConeMesh = coneModel->GetRootMesh();
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
  SceneManager::GetDisplayScene()->SceneGraph()->AddChild(0, camera_entity->GetID());

  /*Camera*/
  m_EditorCamera.camera->flags.Set(Engine::CameraFlag::MainCamera);
  m_EditorCamera.camera->flags.Set(Engine::CameraFlag::EditorCamera);
  editorCameraArgs.screenSize = Window::Get().GetScreenSize();
  m_EditorCamera.transform->Position({0.0f, 0.0f, 2.0f});
  m_EditorCamera.transform->Rotate(glm::radians(180.0f), {0.0f, 1.0f, 0.0f});

  /*ECS Scene*/
  m_Entity1 = ECS::EntityManager::GetInstance().CreateEntity();
  m_Entity2 = ECS::EntityManager::GetInstance().CreateEntity();
  m_Pepe    = ECS::EntityManager::GetInstance().CreateEntity();
  // LOG_TRACE("Past");
  m_Material = nullptr;
  // m_Material = AssetManager::GetMaterial(m_Shader, "./shaders/default.glsl",
  //                                       "./textures/pepo_sad.png", texture);

  m_Entity1->AddComponent< Transform >();
  /*m_Entity1->AddComponent< Components::MeshRenderer >();*/
  m_Entity1->AddComponent< Components::MeshRenderer >(coneModel, m_Material);

  m_Entity2->AddComponent< Transform >();
  m_Entity2->AddComponent< Components::MeshRenderer >(coneModel, m_Material);
  m_PepeTransform = m_Pepe->AddComponent< Transform >();
  m_Pepe->AddComponent< Components::MeshRenderer >(
      m_PepeModel, AssetManager::GetMaterial("./scenes/pepeMat.json"));
  auto sg = SceneManager::GetDisplayScene()->SceneGraph();
  sg->AddChild(0, m_Pepe->GetID());
  sg->AddChild(0, m_Entity1->GetID());
  sg->AddChild(0, m_Entity2->GetID());

  /*Editor Panels*/
  m_SceneHierarchyPanel.SetScene(SceneManager::GetDisplayScene());
  m_SceneHierarchyPanel.SetEditorLayer(this);
  m_SceneHierarchyPanel.SetSelectionCallback(
      [this](auto& entity) { m_InspectorPanel.AttachEntity(entity); });
  m_FileSystemPanel.SetScene(SceneManager::GetDisplayScene());
  m_FileSystemPanel.SetEditorLayer(this);
  /*------------------------*/

  auto box1 = m_Entity1->AddComponent< Components::Collider >();
  auto rb1  = m_Entity1->AddComponent< Components::Rigidbody >();
  auto box2 = m_Entity2->AddComponent< Components::Collider >();
  auto rb2  = m_Entity2->AddComponent< Components::Rigidbody >();

  // m_Entity1->GetComponent< Components::MeshRenderer
  // >()->SaveToJson("./scenes/meshRenderer1.json");

  // box1->LoadFromJson("./scenes/box1.json");
  // box2->LoadFromJson("./scenes/box2.json");

  /*box1->Size      = glm::vec3(1.0f);
  box1->IsTrigger = false;
  box1->Center    = glm::vec3(0.0f);
  rb1->SetGravity(false);
  rb1->SetKinematic(true);
  box2->Size      = glm::vec3(1.0f);
  box2->IsTrigger = false;
  box2->Center    = glm::vec3(0.0f);
  rb2->SetGravity(false);
  rb2->SetKinematic(true);

  box1->SaveToJson("./scenes/box1.json");
  box2->SaveToJson("./scenes/box2.json");*/

  m_PepeTransform->Position({0.0f, 1.0f, 0.0f});
  m_PepeTransform->Scale({0.2f, 0.2f, 0.2f});

  // m_Entity1->SaveToJson("./scenes/m_Entity1.entity");
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
  if (e.GetMouseButton() == 2)
    editorCameraArgs.m3LastPos = Input::GetMousePosition();
  else if (e.GetMouseButton() == 1)
    editorCameraArgs.m2LastPos = Input::GetMousePosition();

  return true;
}

bool EditorLayer::OnMouseButtonRelease(MouseButtonReleasedEvent& e) {
  return false;
}

bool EditorLayer::OnKeyPress(Engine::KeyPressedEvent& e) {
  if (e.GetKeyCode() == GLFW_KEY_C) {
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

auto EditorLayer::DrawMenuBar() -> void {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open...")) {
        LoadScene();
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
    std::string separator = "42091169692137SUPERJSONENTITYSEPARATOR42091169692137";

    auto sg           = SceneManager::GetCurrentScene()->SceneGraph();
    auto entities_ids = sg->GetChildren(0);
    std::shared_ptr< ECS::Entity > entity;

    // TODO: Serialize sceneID

    std::string fileContent = "";

    bool first = true;
    for (auto id : entities_ids) {
      auto entity = ECS::EntityManager::GetInstance().GetEntity(id);
      if (entity == nullptr) {
        continue;
      }

      if (!first)
        fileContent.append("\n" + separator + "\n");
      else
        first = false;

      auto entity_json = entity->SaveToJson();

      std::cout << "\nAdding to file content:\n\n" << entity_json;

      fileContent.append(entity_json);
    }

    std::ofstream ofstream;
    ofstream.open(filepath.value());
    ofstream << fileContent;
    ofstream.close();
  }
}

auto EditorLayer::LoadScene() -> void {
  std::optional< std::string > filepath = FileDialog::OpenFile("Scene (*.scene)\0*.scene\0");
  if (filepath) {
    SceneManager::AddScene(std::make_shared< Scene >(2137));
    SceneManager::OpenScene(2137);
    m_SceneHierarchyPanel.SetScene(SceneManager::GetCurrentScene());

    auto current_scene = SceneManager::GetCurrentScene();
    auto sg            = current_scene->SceneGraph();
    auto entities_ids  = sg->GetChildren(0);

    // for (auto id : entities_ids) {
    //  if (!ECS::EntityManager::GetInstance().GetEntity(id)->HasComponent< Camera >())
    //    ECS::EntityManager::GetInstance().RemoveEntity(id);
    //}

    auto content = Utility::ReadTextFile(filepath.value());
    std::vector< std::string > separated_jsons;

    std::string delimiter =
        "42091169692137SUPERJSONENTITYSEPARATOR42091169692137";  // TODO: Move to one place
                                                                 // instead of declaring each time
    size_t pos = 0;
    std::string token;
    while ((pos = content.find(delimiter)) != std::string::npos) {
      token = content.substr(0, pos);
      separated_jsons.push_back(token);
      content.erase(0, pos + delimiter.length());
    }

    separated_jsons.push_back(content);

    for (std::string separated_json : separated_jsons) {
      auto entity = ECS::EntityManager::GetInstance().CreateEntity();
      entity->LoadFromJson(separated_json);
      if (entity->HasComponent< Camera >())  // HACK: only camera is editor camera
      {
        m_EditorCamera.camera    = entity->GetComponent< Camera >();
        m_EditorCamera.transform = entity->GetComponent< Transform >();
        // HACK: set camera to editor camera only, camera switching
        m_EditorCamera.camera->flags.Set(CameraFlag::EditorCamera);
        m_EditorCamera.camera->flags.Clear(CameraFlag::MainCamera);
      }
      sg->AddChild(0, entity->GetID());
    }
  }
}

auto EditorLayer::AddObjectOnScene(const std::string& path, Engine::ECS::EntityID parent) -> void {
  auto model = AssetManager::GetModel(path);
  if (model->GetRootMesh() == nullptr)
    return;
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
}
