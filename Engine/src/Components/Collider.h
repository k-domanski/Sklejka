#pragma once
#include "ECS/Component.h"

namespace Engine::Components {
  enum ColliderType { Sphere, Box };

  class Collider : public ECS::Component {
  public:
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

    Collider(const glm::vec3 size, bool trigger, bool static_, ColliderType type)
        : Component(), _size(size), _trigger(trigger), _static(static_), _type(type) {
    }
    Collider(): Component() {
    }
    ~Collider() override = default;

  private:
    glm::vec3 _size;
    glm::vec3 _center;
    bool _trigger;
    bool _static;
    ColliderType _type;
  };
}  // namespace Engine::Components
