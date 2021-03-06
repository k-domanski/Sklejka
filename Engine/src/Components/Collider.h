#pragma once
#include <pch.h>
#include "ECS/Component.h"
#include "Utility/GJK.h"
#include <ECS/Types.h>


namespace Engine::Components {
  // enum ColliderType { Sphere, Box };
  BETTER_ENUM(ColliderType, int, Sphere, Box);

  // typedef __ColliderType ColliderType;
  class Collider : public ECS::Component {
  public:
    [[nodiscard]] Utility::GJK::Simplex get_simplex() const;
    void set_simplex(const Utility::GJK::Simplex& simplex);
    __declspec(property(get = get_simplex, put = set_simplex)) Utility::GJK::Simplex Simplex;
    [[nodiscard]] glm::vec3 get_center() const;
    void set_center(const glm::vec3& center);
    __declspec(property(get = get_center, put = set_center)) glm::vec3 Center;
    [[nodiscard]] glm::vec3 get_size() const;
    void set_size(const glm::vec3& size);
    __declspec(property(get = get_size, put = set_size)) glm::vec3 Size;
    [[nodiscard]] bool is_trigger() const;
    void set_trigger(bool trigger);
    __declspec(property(get = is_trigger, put = set_trigger)) bool IsTrigger;
    [[nodiscard]] bool is_static() const;
    void set_static(bool static_);
    __declspec(property(get = is_static, put = set_static)) bool Static;
    [[nodiscard]] ColliderType get_type() const;
    void set_type(ColliderType type);
    __declspec(property(get = get_type, put = set_type)) ColliderType Type;

    auto LoadFromJson(std::string filePath) -> void override;
    // auto LoadFromJsonString(std::string jsonString) -> void override;

    Collider(const glm::vec3 size, bool trigger, bool static_, ColliderType type)
        : Component("Collider", ECS::GetComponentTypeID< Collider >()), _size(size),
          _center(glm::vec3(0.0f)), _trigger(trigger), _static(static_), _type(type) {

    }
    Collider()
        : Component("Collider", ECS::GetComponentTypeID< Collider >()), _size(glm::vec3(1.0f)),
          _center(glm::vec3(0.0f)), _trigger(false), _static(true), _type(ColliderType::Box) {
    }
    ~Collider() override = default;
    std::string SaveToJson(std::string filePath) override;
    std::string SaveToJson() override;



  private:
    glm::vec3 _size;
    glm::vec3 _center;
    bool _trigger;
    bool _static;
    ColliderType _type;
    Utility::GJK::Simplex _simplex;
  };
}  // namespace Engine::Components
