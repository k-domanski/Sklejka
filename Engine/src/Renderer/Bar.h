#pragma once
#include "ECS/Component.h"
#include "Renderer/Image.h"
#include "Renderer/UIElement.h"

namespace Engine::Renderer {
  class Bar : public UIElement {
  public:
    glm::vec2 Offset() const;
    void Offset(const glm::vec2& offset);
    glm::vec2 FillAreaOffset() const;
    void FillAreaOffset(const glm::vec2& offset);
    Image Background() const;
    Image FillArea() const;
    float FillRatio() const;
    void FillRatio(float fill_ratio);
    glm::vec2 Padding() const;
    void Padding(const glm::vec2& padding);
    glm::vec2 Size() const;
    void Size(const glm::vec2& size);
    glm::vec2 FillAreaSize() const;
    void FillAreaSize(const glm::vec2& size);
    bool Horizontal() const;
    void Horizontal(bool horizontal);
    bool Middle() const;
    void Middle(bool middle);
    glm::vec4 FillColor();
    void FillColor(glm::vec4 color);
    glm::vec4 BackgroundColor();
    void BackgroundColor(glm::vec4 color);
    std::shared_ptr< GL::Texture2D > FillTexture();
    void FillTexture(std::shared_ptr< GL::Texture2D > texture);
    std::shared_ptr< GL::Texture2D > BackgroundTexture();
    void BackgroundTexture(std::shared_ptr< GL::Texture2D > texture);
    Bar();
    ~Bar() = default;
    auto Draw(glm::mat4 model, glm::mat4 proj) -> void override;
    auto OnWindowResize(glm::vec2 ratio) -> void override;

  private:
    Image _background;
    Image _fillArea;
    float _fillRatio;
    glm::vec2 _padding;
    glm::vec2 _size;
    bool _horizontal;
    bool _middle;
    glm::vec2 _offset;
    glm::vec2 _fillAreaOffset;
  };
}  // namespace Engine::Renderer
