#pragma once
#include "ECS/Component.h"
#include "Renderer/Image.h"
#include "Renderer/UIElement.h"

namespace Engine::Renderer {
  class Bar : public UIElement {
  public:
    Image Background() const;
    Image FillArea() const;
    float FillRatio() const;
    void FillRatio(float fill_ratio);
    glm::vec2 Padding() const;
    void Padding(const glm::vec2& padding);
    glm::vec2 Size() const;
    void Size(const glm::vec2& size);
    bool Horizontal() const;
    void Horizontal(bool horizontal);
    glm::vec4 FillColor();
    void FillColor(glm::vec4 color);
    glm::vec4 BackgroundColor();
    void BackgroundColor(glm::vec4 color);
    std::shared_ptr<GL::Texture2D> FillTexture();
    void FillTexture(std::shared_ptr< GL::Texture2D > texture);
    std::shared_ptr< GL::Texture2D > BackgroundTexture();
    void BackgroundTexture(std::shared_ptr< GL::Texture2D > texture);
    Bar();
    ~Bar() = default;
    auto Draw(glm::mat4 model, glm::mat4 proj) -> void override;

  private:
    Image _background;
    Image _fillArea;
    float _fillRatio;
    glm::vec2 _padding;
    glm::vec2 _size;
    bool _horizontal;
  };
}  // namespace Engine::Renderer
