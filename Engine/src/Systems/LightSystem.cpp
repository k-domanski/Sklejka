#include <pch.h>
#include "LightSystem.h"
#include <ECS/EntityManager.h>

namespace Engine::Systems {
  using ECS::EntityManager;
  LightSystem::LightSystem(): m_LightUniformSlot(2u), m_Light(nullptr) {
    AddSignature< DirectionalLight >();
  }
  auto LightSystem::Update(float deltaTime) -> void {
    if (m_Light == nullptr) {
      for (auto entityID : _entities) {
        auto light = EntityManager::GetComponent< DirectionalLight >(entityID);
        m_Light    = light;
      }
    }

    if (m_Light != nullptr) {
      if (m_Light->flags.Get(LightFlag::NewData)) {
        m_LightUniformBuffer.SetData(m_Light->UniformData());
      }

      m_LightUniformBuffer.BindToSlot(m_LightUniformSlot);
    }
  }
  auto LightSystem::Light() const noexcept -> std::shared_ptr< DirectionalLight > {
    return m_Light;
  }
}  // namespace Engine::Systems