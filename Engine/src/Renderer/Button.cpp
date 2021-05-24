#include "pch.h"
#include "Button.h"

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

Engine::Renderer::Button::Button()
    : UIElement(), _handleSize(glm::vec2(100, 50)), _color(glm::vec4(1.0f)),
      _pressedColor(glm::vec4(0.0f)) {
  _background = Image();
  _text       = Text();
  _text.Size(1);
}

auto Engine::Renderer::Button::Draw(glm::mat4 model, glm::mat4 proj) -> void {
  _background.Draw(model, proj);
  _text.Draw(model, proj);
}

auto Engine::Renderer::Button::OnReleaseHandle(glm::vec2 position) -> void {
  // if (_handleSize.x > position.x && -_handleSize.x < position.x && _handleSize.y > position.y
  //  && -_handleSize.y < position.y) {
  // here we can put on releasae if needed
  _background.Color(_color);
  //}
}

auto Engine::Renderer::Button::OnPressHandle(glm::vec2 position) -> void {
  if (_handleSize.x > position.x && -_handleSize.x < position.x && _handleSize.y > position.y
      && -_handleSize.y < position.y) {
    if (_onPress != nullptr)
      _onPress();
    _background.Color(_pressedColor);
  }
}
