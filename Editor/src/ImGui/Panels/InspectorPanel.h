#pragma once
#include "Engine.h"


using namespace Engine;

namespace EditorGUI
{
	class InspectorPanel
	{
	public:
		InspectorPanel() = default;
		InspectorPanel(std::shared_ptr<ECS::Entity> entity);

		void OnImGuiRender(std::shared_ptr<ECS::Entity> entity);
		void SetCurrentEntity(std::shared_ptr<ECS::Entity> entity);
	private:
		void DrawComponents(std::shared_ptr<ECS::Entity> entity);
	private:
		std::shared_ptr<ECS::Entity> m_CurrentEntity;
	};
}