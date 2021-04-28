#pragma once
#include <ECS/ECS.h>
#include <Components/DirectionalLight.h>
#include <Components/Transform.h>
#include <GL/Buffer.h>


namespace Engine::Systems {
	class LightSystem : public ECS::System {
	public:
		LightSystem();
		auto Update(float deltaTime) -> void override;
		auto Light() const noexcept -> std::shared_ptr<DirectionalLight>;
	private:
		uint32_t m_LightUniformSlot{ 2u };
		std::shared_ptr<DirectionalLight> m_Light;
		GL::DirectionalLightUniformBuffer m_LightUniformBuffer;
	};
}