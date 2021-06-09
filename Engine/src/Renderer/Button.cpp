#include "pch.h"
#include "Button.h"

#include "App/AssetManager.h"

// void (*Engine::Renderer::Button::on_press() const)() {
//  return OnPress;
//}
//
// void Engine::Renderer::Button::on_press(void (*on_press)()) {
//  OnPress = on_press;
//}

// Engine::Renderer::Image Engine::Renderer::Button::Background() const {
//  return _background;
//}
//
// void Engine::Renderer::Button::Background(const Image& background) {
//  _background = background;
//}

glm::vec2 Engine::Renderer::Button::Offset() const {
  return _offset;
}

void Engine::Renderer::Button::Offset(const glm::vec2& offset) {
  _offset = offset;
}

auto Engine::Renderer::Button::Size() -> glm::vec2 {
  return _background.Size();
}

auto Engine::Renderer::Button::Size(glm::vec2 size) -> void {
  _background.Size(size);
}

std::string Engine::Renderer::Button::text() const {
  return _text.GetText();
}

void Engine::Renderer::Button::text(std::string text) {
  _text.SetText(text);
}

void Engine::Renderer::Button::TextOffset(glm::vec2 offset) {
  _text.Offset(offset);
}

glm::vec2 Engine::Renderer::Button::TextOffset() {
  return _text.Offset();
}

void Engine::Renderer::Button::TextColor(glm::vec4 color) {
  _text.Color(color);
}

glm::vec4 Engine::Renderer::Button::TextColor() {
  return _text.Color();
}

// void Engine::Renderer::Button::text(Text& text) {
//  _text = std::move(text);
//}

glm::vec2 Engine::Renderer::Button::HandleSize() const {
  return _handleSize;
}

void Engine::Renderer::Button::HandleSize(const glm::vec2& handle_size) {
  _handleSize = handle_size;
}

glm::vec4 Engine::Renderer::Button::PressedColor() const {
  return _pressedColor;
}

void Engine::Renderer::Button::PressedColor(const glm::vec4& pressed_color) {
  _pressedColor = pressed_color;
}

glm::vec4 Engine::Renderer::Button::Color() const {
  return _color;
}

void Engine::Renderer::Button::Color(const glm::vec4& color) {
  _color = color;
  _background.Color(_color);
}

std::function< void() > Engine::Renderer::Button::OnPress() const {
  return _onPress;
}

void Engine::Renderer::Button::OnPress(const std::function< void() >& on_press) {
  _onPress = on_press;
}

void Engine::Renderer::Button::Background(std::shared_ptr< GL::Texture2D > texture) {
  _background.Texture(texture);
}

bool Engine::Renderer::Button::IsMouseOverButton(glm::vec2 mousePos) {
  auto handle = _handleSize / 2.0f;
  return (handle.x > mousePos.x && -handle.x < mousePos.x && handle.y > mousePos.y
          && -handle.y < mousePos.y);
}

Engine::Renderer::Button::Button()
    : UIElement(), _handleSize(glm::vec2(100, 50)), _color(glm::vec4(0.5f)),
      _pressedColor(glm::vec4(0.0f)), _selectedColor(glm::vec4(1.0f)), _isSelected(false),
      _offset(glm::vec2(0.0f)) {
  _background = Image();
  _background.Texture(AssetManager::GetTexture2D("./textures/button_default.png"));
  _text = Text();
  _text.Size(1);
}

auto Engine::Renderer::Button::Draw(glm::mat4 model, glm::mat4 proj) -> void {
  model = glm::translate(model, glm::vec3(_offset, 0.0f));
  _background.Draw(model, proj);
  _text.Draw(model, proj);
}

auto Engine::Renderer::Button::OnReleaseHandle(glm::vec2 position) -> void {
  _background.Color(_color);
  auto handle = _handleSize / 2.0f;
  if (handle.x > position.x && -handle.x < position.x && handle.y > position.y
      && -handle.y < position.y) {
    if (_onPress != nullptr)
      _onPress();
    // here we can put on releasae if needed
  }
}

glm::vec4 Engine::Renderer::Button::SelectedColor() const {
  return _selectedColor;
}

void Engine::Renderer::Button::SelectedColor(const glm::vec4& selected_color) {
  _selectedColor = selected_color;
}

bool Engine::Renderer::Button::IsSelected() const {
  return _isSelected;
}

void Engine::Renderer::Button::IsSelected(bool is_selected) {
  _isSelected = is_selected;
  if (_isSelected)
    _background.Color(_selectedColor);
  else
    _background.Color(_color);
}

void Engine::Renderer::Button::TriggerOnPress() {
  if (_onPress != nullptr)
    _onPress();
}

auto Engine::Renderer::Button::OnPressHandle(glm::vec2 position) -> void {
  auto handle = _handleSize / 2.0f;
  if (handle.x > position.x && -handle.x < position.x && handle.y > position.y
      && -handle.y < position.y) {
    _background.Color(_pressedColor);
  }
}
