#include "InspectorPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace EditorGUI
{
	InspectorPanel::InspectorPanel(std::shared_ptr<ECS::Entity> entity)
		:m_CurrentEntity(entity)
	{
	}
	void InspectorPanel::OnImGuiRender(std::shared_ptr<ECS::Entity> entity)
	{
		ImGui::Begin("Inspector");
		if (entity)
			DrawComponents(entity);
		ImGui::End();
	}
	void InspectorPanel::SetCurrentEntity(std::shared_ptr<ECS::Entity> entity)
	{
		m_CurrentEntity = entity;
	}
	
	template<class T, typename UIFunction>
	static void DrawComponent(const std::string& name, std::shared_ptr<ECS::Entity> entity, UIFunction function)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		
		/*if(entity.hasComponent<T>()??*/
		if (entity->GetComponent<T>() != nullptr)
		{
			auto& component = entity->GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}


			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))

					ImGui::EndPopup();
			}

			if (open)
			{
				function(component);
				ImGui::TreePop();
			}
		}

	}
	/*Transform only to start with*/
	void InspectorPanel::DrawComponents(std::shared_ptr<ECS::Entity> entity)
	{

		DrawComponent<Transform>("Transform", entity, [](auto& component)
			{
				//DrawVec3("Translation", component.Position());
				//glm::vec3 rotation = glm::degrees(component.Euler());
				//DrawVec3("Rotation", rotation);
				//DrawVec3("Scale", component.Scale(), 1.0f);
				//auto& comp = entity->GetComponent<Transform>();
				DrawVec3("Translation", component->Position());
				glm::vec3 rotation = glm::degrees(component->Euler());
				DrawVec3("Rotation", rotation);
				DrawVec3("Scale", component->Scale(), 1.0f);

			});
	}
	static void DrawVec3(const std::string& name, glm::vec3& value, float resetValue = 0.0f)
	{
		float columnWidth = 100.0f;
		ImGuiIO& io = ImGui::GetIO();

		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(name.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(name.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			value.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			value.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			value.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &value.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
}