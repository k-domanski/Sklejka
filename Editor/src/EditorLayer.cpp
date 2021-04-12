#include "EditorLayer.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 camera(float Translate, glm::vec2 const& Rotate) {
	glm::mat4 Projection = glm::perspective(glm::pi< float >() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

EditorLayer::EditorLayer(const std::string& name): Layer(name) {
}
void EditorLayer::OnAttach() {
	auto coneModel = AssetManager::GetModel("./models/smolCone.fbx");
	m_Shader = AssetManager::GetShader("./shaders/default.glsl");
	assert(("Failed to acquire shader", m_Shader != nullptr));
	m_ConeMesh = coneModel->getRootMesh();

	glm::vec3 camera_pos{ 0.0f, 0.0f, 2.0f };
	camera_data.view =
		glm::lookAt(camera_pos, camera_pos + glm::vec3{ 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f });
	auto aspect            = (float)Window::Get().GetWidth() / (float)Window::Get().GetHeight();
	camera_data.projection = glm::perspective(45.0f, aspect, 0.001f, 1000.0f);

	camera_buffer.SetData(camera_data);
	camera_buffer.BindToSlot(0);
	m_Shader->SetValue("u_mainTexture", 0);
	m_Shader->BindUniformBlock("u_Camera", 0);
}

void EditorLayer::OnUpdate(double deltaTime) {
	m_Time += deltaTime * 0.1f;
	auto model_matrix =
		          glm::eulerAngleXYZ(m_Time, m_Time, m_Time) * glm::scale(glm::mat4{1.0f}, glm::vec3{0.2f});
	/* -------------------------- */
	m_Shader->Use();
	m_Shader->SetMatrix("mvp", camera(7.f, glm::vec2(m_Time, m_Time)));
	m_Shader->SetMatrix("u_model_matrix", model_matrix);
	m_ConeMesh->Use();
	glDrawElements(m_ConeMesh->GetPrimitive(), m_ConeMesh->ElementCount(), GL_UNSIGNED_INT, NULL);
}

void EditorLayer::OnDetach() {
}
