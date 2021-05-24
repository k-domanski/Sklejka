#pragma once
#include "Image.h"
#include "Text.h"
#include "UIElement.h"

namespace Engine::Renderer
{
  class Button : public UIElement
  {
  public:
    //Image Background() const;
    //void Background(const Image& background);
    auto Size() -> glm::vec2;
    auto Size(glm::vec2 size) -> void;
    std::string text() const;
    void text(std::string text);
    void TextOffset(glm::vec2 offset);
    glm::vec2 TextOffset();
    glm::vec2 HandleSize() const;
    void HandleSize(const glm::vec2& handle_size);
    glm::vec4 PressedColor() const;
    void PressedColor(const glm::vec4& pressed_color);
    glm::vec4 Color() const;
    void Color(const glm::vec4& color);
    std::function<void()> OnPress() const;
    void OnPress(const std::function<void()>& on_press);
    //void(* on_press() const)();
    //void on_press(void(* on_press)());
    Button();
    auto Draw(glm::mat4 model, glm::mat4 proj) -> void override;
    auto OnPressHandle(glm::vec2 position) -> void;
    auto OnReleaseHandle(glm::vec2 position) -> void;

  private:
    Image _background;
    Text _text;
    glm::vec2 _handleSize;
    glm::vec4 _pressedColor;
    glm::vec4 _color;

    std::function< void() > _onPress;
    //void (*OnPress)();
  };
  
}
