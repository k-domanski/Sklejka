#include "EditorLayer.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

EditorLayer::EditorLayer(const std::string& name): Layer(name), m_Camera(45.0f, 0.001f, 1000.0f) {
}
void EditorLayer::OnAttach() {
	auto coneModel = AssetManager::GetModel("./models/smolCone.fbx");
	m_Shader = AssetManager::GetShader("./shaders/default.glsl");
	assert(("Failed to acquire shader", m_Shader != nullptr));
	m_ConeMesh = coneModel->getRootMesh();

	mouseState.screenSize = { Window::Get().GetWidth(), Window::Get().GetHeight() };
	m_Camera.transform.Position({ 0.0f, 0.0f, 2.0f });
	m_Camera.transform.Rotate(glm::radians(180.0f), { 0.0f, 1.0f, 0.0f });
	camera_data.view = m_Camera.GetViewMatrix();
	camera_data.projection = m_Camera.GetProjectionMatrix();
	camera_buffer.SetData(camera_data);

	camera_buffer.BindToSlot(0);
	m_Shader->SetValue("u_mainTexture", 0);
	m_Shader->BindUniformBlock("u_Camera", 0);

	m_Entity = ECS::EntityManager::GetInstance().CreateEntity();
	m_Material = std::make_shared<Renderer::Material>(0);
	m_Material->SetShader(m_Shader, "wtf");

	/*Cone jako sharedptr, tu wywala? zakomentowac 2 linijki ponizej*/
	//auto coneMesh = std::shared_ptr<Renderer::Mesh>(coneModel->getRootMesh());
	//m_Entity->AddComponent<Components::MeshRenderer>(coneMesh, m_Material);
	m_Entity->AddComponent<Transform>();
	auto transform = m_Entity->GetComponent<Transform>();
	transform->Position({ 2.0f,0.0f,0.0f });
	transform->Scale(glm::vec3(0.2f));
}

void EditorLayer::OnUpdate(double deltaTime) {
	m_Time += deltaTime * 0.1f;
	auto model_matrix =
		          glm::eulerAngleXYZ(m_Time, m_Time, m_Time) * glm::scale(glm::mat4{1.0f}, glm::vec3{0.2f});
	
	/* -------------------------- */
	/* TODO: CameraController move to separate class?*/
	/*Mouse Scroll*/
	if (mouseState.scrollDelta != 0.0f) {
		m_Camera.transform.Position(m_Camera.transform.Position()
			+ m_Camera.transform.Forward() * mouseState.scrollDelta * 0.1f);
		mouseState.scrollDelta = 0.0f;
	}
	/*Scroll Pressed Movement*/
	if (Input::IsMouseButtonPressed(2))
	{
		auto cursorPos = Input::GetMousePosition();
		auto delta = cursorPos - mouseState.m3LastPos;
		delta /= mouseState.screenSize;

		auto position = m_Camera.transform.Position();
		position += m_Camera.transform.Right() * delta.x * mouseState.sensitivity;
		position += m_Camera.transform.Up() * delta.y * mouseState.sensitivity;
		m_Camera.transform.Position(position);
		mouseState.m3LastPos = cursorPos;
	}
	/*Right Mouse Button*/
	else if (Input::IsMouseButtonPressed(1))
	{
		auto cursorPos = Input::GetMousePosition();
		auto delta = cursorPos - mouseState.m2LastPos;
		delta /= mouseState.screenSize;

		// TODO: Add clamping in X axis;
		const auto sensitivity = 120.0f;
		m_Camera.transform.Rotate(glm::radians(-delta.x * sensitivity), { 0.0f, 1.0f, 0.0f });
		auto dy = delta.y * sensitivity;
		// Locking rotation in [-89.0f, 89.0f] range
		auto angle = glm::degrees(
			glm::angle(glm::rotation(m_Camera.transform.Forward(), { 0.0f, 1.0f, 0.0f })));
		if (auto fa = angle + dy; fa < 1.0f || fa > 179.0f) {
			dy = 0.0f;
		}
		m_Camera.transform.Rotate(glm::radians(dy), m_Camera.transform.Right());

		mouseState.m2LastPos = cursorPos;
	}
	/* -------------------------- */
	/*Update camera*/
	camera_data.view = m_Camera.GetViewMatrix();
	camera_data.projection = m_Camera.GetProjectionMatrix();
	camera_buffer.SetData(camera_data);
	m_Shader->Use();
	m_Shader->SetMatrix("u_model_matrix", model_matrix);
	m_ConeMesh->Use();
	glDrawElements(m_ConeMesh->GetPrimitive(), m_ConeMesh->ElementCount(), GL_UNSIGNED_INT, NULL);
}

void EditorLayer::OnDetach() {
}

void EditorLayer::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(EditorLayer::OnMouseScroll));
	dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseButtonPress));
	dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseButtonRelease));


}

bool EditorLayer::OnMouseScroll(MouseScrolledEvent& e)
{
	mouseState.scrollDelta = e.GetYOffset();
	return true;
}

bool EditorLayer::OnMouseButtonPress(MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() == 2)
		mouseState.m3LastPos = Input::GetMousePosition();
	else if (e.GetMouseButton() == 1)
		mouseState.m2LastPos = Input::GetMousePosition();
	
	return true;
}

bool EditorLayer::OnMouseButtonRelease(MouseButtonReleasedEvent& e)
{
	return false;
}
