#pragma once
#include "Mesh.h"
#include "UIElement.h"
#include "GL/Shader.h"
#include "GL/Texture2D.h"

namespace Engine::Renderer {

  class Image : public UIElement {
  public:
    float Ratio() const;
    void Ratio(float ratio);
    glm::vec4 Color() const;
    void Color(const glm::vec4& color);
    glm::vec2 Size() const;
    void Size(const glm::vec2& size);
    glm::vec2 Offset() const;
    void Offset(const glm::vec2& offset);
    bool Horizontal() const;
    void Horizontal(bool horizontal);
    bool Middle() const;
    void Middle(bool middle);
    std::shared_ptr< GL::Texture2D > Texture() const;
    void Texture(const std::shared_ptr< GL::Texture2D >& texture);
    std::shared_ptr< GL::Shader > Shader() const;
    void Shader(const std::shared_ptr< GL::Shader >& shader);
    Image();
    Image(std::shared_ptr< GL::Texture2D > texture, std::shared_ptr< GL::Shader > shader)
        : UIElement(), _texture(texture), _shader(shader), _ratio(1.0f), _color(glm::vec4(1.0f)),
          _size(glm::vec2(100.0f, 100.0f)), _horizontal(true) {
    }

    ~Image() = default;
    // auto Draw() -> void override;

    auto Draw(glm::mat4 model, glm::mat4 proj) -> void override;
    auto OnWindowResize(glm::vec2 ratio) -> void override;

  private:
    std::shared_ptr< Mesh > _mesh;
    std::shared_ptr< GL::Texture2D > _texture;
    std::shared_ptr< GL::Shader > _shader;
    float _ratio;
    glm::vec4 _color;
    glm::vec2 _size;
    glm::vec2 _offset;
    bool _horizontal;  // TODO: Consider better enum  -- Maybe some day :)
    bool _middle;
  };
}  // namespace Engine::Renderer
