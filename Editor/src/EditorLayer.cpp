#include "EditorLayer.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

EditorLayer::EditorLayer(const std::string& name): Layer(name), m_Camera(45.0f, 0.001f, 1000.0f) {
}
void EditorLayer::OnAttach() {
  /*Assets*/
  auto texture   = AssetManager::GetTexture2D("./textures/pepo_sad.png");
  auto coneModel = AssetManager::GetModel("./models/smolCone.fbx");
  m_Shader       = AssetManager::GetShader("./shaders/default.glsl");
  assert(("Failed to acquire shader", m_Shader != nullptr));
  m_ConeMesh = coneModel->getRootMesh();

  /*Camera*/
  mouseState.screenSize = {Window::Get().GetWidth(), Window::Get().GetHeight()};
  m_Camera.transform.Position({0.0f, 0.0f, 2.0f});
  m_Camera.transform.Rotate(glm::radians(180.0f), {0.0f, 1.0f, 0.0f});
  camera_data.view       = m_Camera.GetViewMatrix();
  camera_data.projection = m_Camera.GetProjectionMatrix();
  camera_buffer.SetData(camera_data);

  camera_buffer.BindToSlot(0);
  m_Shader->SetValue("u_mainTexture", 0);
  m_Shader->BindUniformBlock("u_Camera", 0);

  /*ECS Scene*/
  m_Entity1  = ECS::EntityManager::GetInstance().CreateEntity();
  m_Entity2  = ECS::EntityManager::GetInstance().CreateEntity();
  m_Material = AssetManager::GetMaterial(m_Shader, "path/to/shader.glsl,",
                                         "./textures/pepo_sad.png", texture);
  // m_Material->SetShader(m_Shader, "path/to/shader.glsl");

  m_Entity1->AddComponent< Transform >();
  m_Entity1->AddComponent< Components::MeshRenderer >(m_ConeMesh, m_Material);
  m_Entity2->AddComponent< Transform >();
  m_Entity2->AddComponent< Components::MeshRenderer >(m_ConeMesh, m_Material);
  auto sg = m_Scene.SceneGraph();
  auto id = m_Entity1->GetID();
  sg->AddEntity(id);
  m_Scene.SceneGraph()->AddEntity(m_Entity2->GetID(), m_Entity1->GetID());

  auto tr1 = m_Entity1->GetComponent< Transform >();
  auto tr2 = m_Entity2->GetComponent< Transform >();

  tr1->Position({1.0f, 0.0f, 0.0f});
  tr1->Scale({0.2f, 0.2f, 0.2f});
  tr2->Position({-10.0f, 0.0f, 0.0f});
}

void EditorLayer::OnUpdate(double deltaTime) {
  /* -------------------------- */
  /* TODO: CameraController move to separate class?*/
  /*Mouse Scroll*/
  if (mouseState.scrollDelta != 0.0f) {
    m_Camera.transform.Position(m_Camera.transform.Position()
                                + m_Camera.transform.Forward() * mouseState.scrollDelta * 0.1f);
    mouseState.scrollDelta = 0.0f;
  }
  /*Scroll Pressed Movement*/
  if (Input::IsMouseButtonPressed(2)) {
    auto cursorPos = Input::GetMousePosition();
    auto delta     = cursorPos - mouseState.m3LastPos;
    delta /= mouseState.screenSize;

    auto position = m_Camera.transform.Position();
    position += m_Camera.transform.Right() * delta.x * mouseState.sensitivity;
    position += m_Camera.transform.Up() * delta.y * mouseState.sensitivity;
    m_Camera.transform.Position(position);
    mouseState.m3LastPos = cursorPos;
  }
  /*Right Mouse Button*/
  else if (Input::IsMouseButtonPressed(1)) {
    auto cursorPos = Input::GetMousePosition();
    auto delta     = cursorPos - mouseState.m2LastPos;
    delta /= mouseState.screenSize;

    // TODO: Add clamping in X axis;
    const auto sensitivity = 120.0f;
    m_Camera.transform.Rotate(glm::radians(-delta.x * sensitivity), {0.0f, 1.0f, 0.0f});
    auto dy = delta.y * sensitivity;
    // Locking rotation in [-89.0f, 89.0f] range
    auto angle =
        glm::degrees(glm::angle(glm::rotation(m_Camera.transform.Forward(), {0.0f, 1.0f, 0.0f})));
    if (auto fa = angle + dy; fa < 1.0f || fa > 179.0f) {
      dy = 0.0f;
    }
    m_Camera.transform.Rotate(glm::radians(dy), m_Camera.transform.Right());

    mouseState.m2LastPos = cursorPos;
  }
  /* -------------------------- */
  /*Update camera*/
  camera_data.view       = m_Camera.GetViewMatrix();
  camera_data.projection = m_Camera.GetProjectionMatrix();
  camera_buffer.SetData(camera_data);

  auto tr1 = m_Entity1->GetComponent< Transform >();
  tr1->Rotate(deltaTime * 0.3, {0.0f, 1.0f, 0.0f});

  m_Scene.Update(deltaTime);
  /*Update systemów w aplikacji?*/
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
  mouseState.scrollDelta = e.GetYOffset();
  return true;
}

bool EditorLayer::OnMouseButtonPress(MouseButtonPressedEvent& e) {
  if (e.GetMouseButton() == 2)
    mouseState.m3LastPos = Input::GetMousePosition();
  else if (e.GetMouseButton() == 1)
    mouseState.m2LastPos = Input::GetMousePosition();

  return true;
}

bool EditorLayer::OnMouseButtonRelease(MouseButtonReleasedEvent& e) {
  return false;
}
