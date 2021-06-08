#pragma once
#include "Image.h"
#include "Text.h"
#include "UIElement.h"

namespace Engine::Renderer {
  class Button : public UIElement {
  public:
    glm::vec2 Offset() const;
    void Offset(const glm::vec2& offset);
    // Image Background() const;
    // void Background(const Image& background);
    auto Size() -> glm::vec2;
    auto Size(glm::vec2 size) -> void;
    std::string text() const;
    void text(std::string text);
    void TextOffset(glm::vec2 offset);
    glm::vec2 TextOffset();
    void TextColor(glm::vec4 color);
    glm::vec4 TextColor();
    glm::vec2 HandleSize() const;
    void HandleSize(const glm::vec2& handle_size);
    glm::vec4 PressedColor() const;
    void PressedColor(const glm::vec4& pressed_color);
    glm::vec4 Color() const;
    void Color(const glm::vec4& color);
    std::function< void() > OnPress() const;
    void OnPress(const std::function< void() >& on_press);
    void Background(std::shared_ptr< GL::Texture2D > texture);
    // void(* on_press() const)();
    // void on_press(void(* on_press)());
    void SelectedColor(const glm::vec4& selected_color);
    glm::vec4 SelectedColor() const;
    bool IsSelected() const;
    void IsSelected(bool is_selected);
    void TriggerOnPress();

    bool IsMouseOverButton(glm::vec2 mousePos);
    Button();
    auto Draw(glm::mat4 model, glm::mat4 proj) -> void override;
    auto OnPressHandle(glm::vec2 position) -> void;
    auto OnReleaseHandle(glm::vec2 position) -> void;

  private:
    Image _background;
    Text _text;
    glm::vec2 _handleSize;
    glm::vec4 _pressedColor;
    glm::vec4 _selectedColor;
    glm::vec4 _color;
    bool _isSelected;
    glm::vec2 _offset;

    std::function< void() > _onPress;
    // void (*OnPress)();
  };

}  // namespace Engine::Renderer
