#include <pch.h>
#include "LightSystem.h"
#include <ECS/EntityManager.h>

namespace Engine::Systems {
  using ECS::EntityManager;
  LightSystem::LightSystem(): m_LightUniformSlot(2u), m_Light(nullptr) {
    AddSignature< DirectionalLight >();
    m_LightUniformBuffer.BindToSlot(GL::UniformBlock::LightData);
  }
  auto LightSystem::Update(float deltaTime) -> void {
    if (m_Light == nullptr) {
      for (auto entityID : _entities) {
        auto light  = EntityManager::GetComponent< DirectionalLight >(entityID);
        m_Light     = light;
        m_Transform = EntityManager::GetComponent< Transform >(entityID);
      }
    }

    if (m_Light != nullptr) {
      if (m_Transform->flags.Get(TransformFlag::NewData)) {
        m_Light->Direction(m_Transform->Forward());
        // LOG_TRACE("Direction changed: [{}, {}, {}]", m_Transform->Forward().x,
        //          m_Transform->Forward().y, m_Transform->Forward().z);
      }
      if (m_Light->flags.Get(LightFlag::NewData)) {
        m_LightUniformBuffer.SetData(m_Light->UniformData());
      }
    }
  }
  auto LightSystem::Light() const noexcept -> std::shared_ptr< DirectionalLight > {
    return m_Light;
  }
}  // namespace Engine::Systems