#include "EditorLayer.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Engine;
EditorLayer::EditorLayer(const std::string& name): Layer(name) {
}
void EditorLayer::OnAttach() {
  /*Assets*/
  auto texture   = AssetManager::GetTexture2D("./textures/pepo_sad.png");
  auto coneModel = AssetManager::GetModel("./models/smolCone.fbx");
  m_Shader       = AssetManager::GetShader("./shaders/default.glsl");
  assert(("Failed to acquire shader", m_Shader != nullptr));
  m_ConeMesh = coneModel->getRootMesh();
  m_PepeModel = AssetManager::GetModel("./models/Pepe.obj");
  auto tex_shader = AssetManager::GetShader("./shaders/texture_shader.glsl");
  auto pepe_texture = AssetManager::GetTexture2D("./textures/Pepe_Texture.png");
  m_PepeMaterial    = AssetManager::GetMaterial(tex_shader, "pepe_sh", "pepe_tex", pepe_texture);

  auto aspect           = Engine::Window::Get().GetAspectRatio();
  auto camera_entity = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  m_EditorCamera.camera = camera_entity->AddComponent< Engine::Camera >(45.0f, aspect, 0.001f, 1000.0f);
  m_EditorCamera.transform = camera_entity->AddComponent< Engine::Transform >();
  m_Scene.SceneGraph()->AddEntity(camera_entity->GetID());

  /*Camera*/
  m_EditorCamera.camera->flags.Set(Engine::CameraFlag::MainCamera);
  editorCameraArgs.screenSize = Window::Get().GetScreenSize();
  m_EditorCamera.transform->Position({0.0f, 0.0f, 2.0f});
  m_EditorCamera.transform->Rotate(glm::radians(180.0f), {0.0f, 1.0f, 0.0f});
  

  /*ECS Scene*/
  m_Entity1  = ECS::EntityManager::GetInstance().CreateEntity();
  m_Entity2  = ECS::EntityManager::GetInstance().CreateEntity();
  m_Pepe  = ECS::EntityManager::GetInstance().CreateEntity();
  m_Material = AssetManager::GetMaterial(m_Shader, "path/to/shader.glsl,",
                                         "./textures/pepo_sad.png", texture);
  // m_Material->SetShader(m_Shader, "path/to/shader.glsl");

  m_Entity1->AddComponent< Transform >();
  m_Entity1->AddComponent< Components::MeshRenderer >(m_ConeMesh, m_Material);
  m_Entity2->AddComponent< Transform >();
  m_Entity2->AddComponent< Components::MeshRenderer >(m_ConeMesh, m_Material);
  m_PepeTransform = m_Pepe->AddComponent< Transform >();
  m_Pepe->AddComponent< Components::MeshRenderer >(m_PepeModel->getRootMesh(), m_PepeMaterial);
  auto sg = m_Scene.SceneGraph();
  auto id = m_Entity1->GetID();
  sg->AddEntity(id);
  m_Scene.SceneGraph()->AddEntity(m_Entity2->GetID(), m_Entity1->GetID());

  auto tr1 = m_Entity1->GetComponent< Transform >();
  auto tr2 = m_Entity2->GetComponent< Transform >();

  tr1->Position({1.0f, 0.0f, 0.0f});
  tr1->Scale({0.2f, 0.2f, 0.2f});
  tr2->Position({-10.0f, 0.0f, 0.0f});
  m_PepeTransform->Position({0.0f, 1.0f, 0.0f});
  m_Scene.SceneGraph()->AddEntity(m_PepeTransform->GetEntityID(), tr2->GetEntityID());
}

void EditorLayer::OnUpdate(double deltaTime) {
  /* -------------------------- */
  /* TODO: CameraController move to separate class?*/
  UpdateEditorCamera();
  /* -------------------------- */

  auto tr1 = m_Entity1->GetComponent< Transform >();
  tr1->Rotate(deltaTime * 0.3, {0.0f, 1.0f, 0.0f});
  m_PepeTransform->Rotate(deltaTime * 0.1, {0.0f, 1.0f, 0.0f});

  m_Scene.Update(deltaTime);
  /*Update system�w w aplikacji?*/
  m_Scene.Draw();
}

void EditorLayer::OnDetach() {
}

void EditorLayer::OnEvent(Event& event) {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch< MouseScrolledEvent >(BIND_EVENT_FN(EditorLayer::OnMouseScroll));
  dispatcher.Dispatch< MouseButtonPressedEvent >(BIND_EVENT_FN(EditorLayer::OnMouseButtonPress));
  dispatcher.Dispatch< MouseButtonReleasedEvent >(BIND_EVENT_FN(EditorLayer::OnMouseButtonRelease));
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
                                + m_EditorCamera.transform->Forward() * editorCameraArgs.scrollDelta * 0.1f);
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
    auto angle =
        glm::degrees(glm::angle(glm::rotation(m_EditorCamera.transform->Forward(), {0.0f, 1.0f, 0.0f})));
    if (auto fa = angle + dy; fa < 1.0f || fa > 179.0f) {
      dy = 0.0f;
    }
    m_EditorCamera.transform->Rotate(glm::radians(dy), m_EditorCamera.transform->Right());

    editorCameraArgs.m2LastPos = cursorPos;
  }
}
