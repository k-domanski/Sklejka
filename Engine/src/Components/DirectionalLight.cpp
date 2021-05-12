#include "pch.h"
#include "DirectionalLight.h"
#include <nlohmann/json.hpp>

namespace Engine {
  DirectionalLight::DirectionalLight(): Component("DirectionalLight") {
    m_Data.direction = glm::vec3(0);
    m_Data.ambient   = glm::vec3(0);
    m_Data.diffuse   = glm::vec3(0);
    m_Data.specular  = glm::vec3(0);
    m_Data.intensity = 1;
  }
  auto DirectionalLight::Direction() const noexcept -> glm::vec3 {
    return m_Data.direction;
  }
  auto DirectionalLight::Direction(glm::vec3 direction) noexcept -> glm::vec3 {
    flags.Set(LightFlag::Dirty | LightFlag::NewData);
    return m_Data.direction = direction;
  }
  auto DirectionalLight::Ambient() const noexcept -> glm::vec3 {
    return m_Data.ambient;
  }
  auto DirectionalLight::Ambient(glm::vec3 ambient) noexcept -> glm::vec3 {
    flags.Set(LightFlag::Dirty | LightFlag::NewData);
    return m_Data.ambient = ambient;
  }
  auto DirectionalLight::Diffuse() const noexcept -> glm::vec3 {
    return m_Data.diffuse;
  }
  auto DirectionalLight::Diffuse(glm::vec3 diffuse) noexcept -> glm::vec3 {
    flags.Set(LightFlag::Dirty | LightFlag::NewData);
    return m_Data.diffuse = diffuse;
  }
  auto DirectionalLight::Specular() const noexcept -> glm::vec3 {
    return m_Data.specular;
  }
  auto DirectionalLight::Specular(glm::vec3 specular) noexcept -> glm::vec3 {
    flags.Set(LightFlag::Dirty | LightFlag::NewData);
    return m_Data.specular = specular;
  }
  auto DirectionalLight::Intensity() const noexcept -> float {
    return m_Data.intensity;
  }
  auto DirectionalLight::Intensity(float intensity) noexcept -> float {
    flags.Set(LightFlag::Dirty | LightFlag::NewData);
    return m_Data.intensity = intensity;
  }
  auto DirectionalLight::ShadowBias() const noexcept -> float {
    return m_Data.shadowBias;
  }
  auto DirectionalLight::ShadowBias(float value) noexcept -> float {
    flags.Set(LightFlag::Dirty | LightFlag::NewData);
    return m_Data.shadowBias = value;
  }
  auto DirectionalLight::UniformData() const noexcept -> GL::DirectionalLightUniformData {
    return m_Data;
  }
  auto DirectionalLight::SaveToJson() -> std::string {
    nlohmann::json json = nlohmann::json{
        {"componentType", "directionalLight"},
        /*{"direction",
         {{"x", m_Data.direction.x}, {"y", m_Data.direction.y}, {"z", m_Data.direction.z}}},*/
        {"ambient", {{"x", m_Data.ambient.x}, {"y", m_Data.ambient.y}, {"z", m_Data.ambient.z}}},
        {"diffuse", {{"x", m_Data.diffuse.x}, {"y", m_Data.diffuse.y}, {"z", m_Data.diffuse.z}}},
        {"specular",
         {{"x", m_Data.specular.x}, {"y", m_Data.specular.y}, {"z", m_Data.specular.z}}},
        {"intensity", m_Data.intensity},
        {"shadowBias", m_Data.shadowBias},
        {"flags", flags.GetState()}};
    return json.dump(4);
  }
  auto DirectionalLight::SaveToJson(std::string filePath) -> std::string {
    std::ofstream ofstream;
    ofstream.open(filePath);
    ofstream << SaveToJson();
    ofstream.close();

    return SaveToJson();
  }
  auto DirectionalLight::LoadFromJson(std::string filePath) -> void {
    nlohmann::json json;
    if (filePath[0] == '{' || filePath[0] == '\n')  // HACK: Check if string is json
      json = nlohmann::json::parse(filePath.begin(), filePath.end());
    else {
      auto content = Utility::ReadTextFile(filePath);
      json         = nlohmann::json::parse(content.begin(), content.end());
    }

    // Direction(glm::vec3(json["direction"]["x"], json["direction"]["y"], json["direction"]["z"]));
    Ambient(glm::vec3(json["ambient"]["x"], json["ambient"]["y"], json["ambient"]["z"]));
    Diffuse(glm::vec3(json["diffuse"]["x"], json["diffuse"]["y"], json["diffuse"]["z"]));
    Specular(glm::vec3(json["specular"]["x"], json["specular"]["y"], json["specular"]["z"]));
    Intensity(json["intensity"]);
    ShadowBias(READ_VALUE(json, "shadowBias", 0.001f));
    flags.Set(json["flags"]);
  }
}  // namespace Engine