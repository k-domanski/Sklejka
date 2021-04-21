#include "pch.h"
#include "Collider.h"

#include <nlohmann/json.hpp>

#include "../../../Editor/src/ImGui/Panels/FileSystemPanel.h"

Engine::Utility::GJK::Simplex Engine::Components::Collider::get_simplex() const {
  return _simplex;
}

void Engine::Components::Collider::set_simplex(const Utility::GJK::Simplex& simplex) {
  _simplex = simplex;
}

glm::vec3 Engine::Components::Collider::get_center() const {
  return _center;
}

void Engine::Components::Collider::set_center(const glm::vec3& center) {
  _center = center;
}

glm::vec3 Engine::Components::Collider::get_size() const {
  return _size;
}

void Engine::Components::Collider::set_size(const glm::vec3& size) {
  _size = size;
}

bool Engine::Components::Collider::is_trigger() const {
  return _trigger;
}

void Engine::Components::Collider::set_trigger(bool trigger) {
  _trigger = trigger;
}

bool Engine::Components::Collider::is_static() const {
  return _static;
}

void Engine::Components::Collider::set_static(bool static_) {
  _static = static_;
}

Engine::Components::ColliderType Engine::Components::Collider::get_type() const {
  return _type;
}

void Engine::Components::Collider::set_type(ColliderType type) {
  _type = type;
}

auto Engine::Components::Collider::LoadFromJson(std::string filePath) -> void
{
  auto content        = Utility::ReadTextFile(filePath);
  nlohmann::json json = nlohmann::json::parse(content.begin(), content.end());

  _type = json["colliderType"] == "Sphere" ? Type.Sphere : Type.Box;
  _trigger = json["trigger"];
  _static  = json["static"];
  _center  = glm::vec3(json["center"]["x"], json["center"]["y"], json["center"]["z"]);
  _size    = glm::vec3(json["size"]["x"], json["size"]["y"], json["size"]["z"]);
}

std::string Engine::Components::Collider::SaveToJson(std::string filePath)
{

  std::ofstream ofstream;
  ofstream.open(filePath);
  ofstream << SaveToJson();
  ofstream.close();

  return SaveToJson();
}

std::string Engine::Components::Collider::SaveToJson()
{
  nlohmann::json json = nlohmann::json{
      {"componentType", "collider"},
      {"colliderType", _type._to_string()},
      {"trigger", _trigger},
      {"static", _static},
      {"center", {{"x", _center.x}, {"y", _center.y}, {"z", _center.z}}},
      {"size", {{"x", _size.x}, {"y", _size.y}, {"z", _size.z}}}
  };

  return json.dump(4);
}
