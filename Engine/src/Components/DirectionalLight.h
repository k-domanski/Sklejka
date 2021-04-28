#pragma once
#include <pch.h>
#include <Utility/Utility.h>
#include "ECS/ECS.h"
#include "GL/UniformBufferData.h"

namespace Engine {
  using Helpers::Bit32;
  BETTER_ENUM(__LightFlag, uint32_t, Dirty = Bit32(0), NewData = Bit32(1));
  typedef __LightFlag LightFlag;
  typedef Utility::BitFlags< LightFlag > LightBitFlags;

  class DirectionalLight : public ECS::Component {
  public:
    LightBitFlags flags;
    DirectionalLight();
    auto Direction() const noexcept -> glm::vec3;
    auto Direction(glm::vec3 direction) noexcept -> glm::vec3;
    auto Ambient() const noexcept -> glm::vec3;
    auto Ambient(glm::vec3 ambient) noexcept -> glm::vec3;
    auto Diffuse() const noexcept -> glm::vec3;
    auto Diffuse(glm::vec3 diffuse) noexcept -> glm::vec3;
    auto Specular() const noexcept -> glm::vec3;
    auto Specular(glm::vec3 specular) noexcept -> glm::vec3;
    auto Intensity() const noexcept -> float;
    auto Intensity(float intensity) noexcept -> float;
    auto UniformData() const noexcept -> GL::DirectionalLightUniformData;
    auto SaveToJson() -> std::string override;
    auto SaveToJson(std::string filePath) -> std::string override;
    auto LoadFromJson(std::string filePath) -> void override;

  private:
    GL::DirectionalLightUniformData m_Data;
  };
}  // namespace Engine