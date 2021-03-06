#include "pch.h"
#include "Bar.h"
#include "Engine.h"

glm::vec2 Engine::Renderer::Bar::Offset() const {
  return _offset;
}

void Engine::Renderer::Bar::Offset(const glm::vec2& offset) {
  _offset = offset;
}

glm::vec2 Engine::Renderer::Bar::FillAreaOffset() const {
  return _fillAreaOffset;
}

void Engine::Renderer::Bar::FillAreaOffset(const glm::vec2& offset) {
  _fillAreaOffset = offset;
}

Engine::Renderer::Image Engine::Renderer::Bar::Background() const {
  return _background;
}

Engine::Renderer::Image Engine::Renderer::Bar::FillArea() const {
  return _fillArea;
}

float Engine::Renderer::Bar::FillRatio() const {
  return _fillRatio;
}

void Engine::Renderer::Bar::FillRatio(float fill_ratio) {
  _fillRatio = fill_ratio;
  _fillArea.Ratio(_fillRatio);
}

glm::vec2 Engine::Renderer::Bar::Padding() const {
  return _padding;
}

void Engine::Renderer::Bar::Padding(const glm::vec2& padding) {
  _padding = padding;
  _fillArea.Size(_size - _padding);
}

glm::vec2 Engine::Renderer::Bar::Size() const {
  return _size;
}

void Engine::Renderer::Bar::Size(const glm::vec2& size) {
  _size = size;
  _background.Size(_size);
  _fillArea.Size(_size - _padding);
}

glm::vec2 Engine::Renderer::Bar::FillAreaSize() const {
  return _fillArea.Size();
}

void Engine::Renderer::Bar::FillAreaSize(const glm::vec2& size) {
  _fillArea.Size(size);
}

bool Engine::Renderer::Bar::Horizontal() const {
  return _horizontal;
}

void Engine::Renderer::Bar::Horizontal(bool horizontal) {
  _horizontal = horizontal;
  _fillArea.Horizontal(_horizontal);
}

bool Engine::Renderer::Bar::Middle() const {
  return _middle;
}

void Engine::Renderer::Bar::Middle(bool middle) {
  _middle = middle;
  _fillArea.Middle(_middle);
}

glm::vec4 Engine::Renderer::Bar::FillColor() {
  return _fillArea.Color();
}

void Engine::Renderer::Bar::FillColor(glm::vec4 color) {
  _fillArea.Color(color);
}

glm::vec4 Engine::Renderer::Bar::BackgroundColor() {
  return _background.Color();
}

void Engine::Renderer::Bar::BackgroundColor(glm::vec4 color) {
  _background.Color(color);
}

std::shared_ptr< Engine::GL::Texture2D > Engine::Renderer::Bar::FillTexture() {
  return _fillArea.Texture();
}

void Engine::Renderer::Bar::FillTexture(std::shared_ptr< GL::Texture2D > texture) {
  _fillArea.Texture(texture);
}

std::shared_ptr< Engine::GL::Texture2D > Engine::Renderer::Bar::BackgroundTexture() {
  return _background.Texture();
}

void Engine::Renderer::Bar::BackgroundTexture(std::shared_ptr< GL::Texture2D > texture) {
  _background.Texture(texture);
}

Engine::Renderer::Bar::Bar(): UIElement() {
  _background = Image();
  _fillArea   = Image();
  _fillArea.Shader(Engine::AssetManager::GetShader("./shaders/bar.glsl"));
  _fillRatio  = 1.0f;
  _horizontal = true;
  _middle     = false;
  _padding    = glm::vec2(0.0f);
  _size       = glm::vec2(100.0f);
  _background.Size(_size);
  _fillArea.Size(_size - _padding);
  _fillArea.Ratio(_fillRatio);
  _offset         = glm::vec2(0.0f);
  _fillAreaOffset = glm::vec2(0.0f);
}

auto Engine::Renderer::Bar::Draw(glm::mat4 model, glm::mat4 proj) -> void {
  model = glm::translate(model, glm::vec3(_offset, 0.0f));
  _background.Draw(model, proj);
  model = glm::translate(model, glm::vec3(_fillAreaOffset, 0.0f));
  _fillArea.Draw(model, proj);
}

auto Engine::Renderer::Bar::OnWindowResize(glm::vec2 ratio) -> void {
  _background.OnWindowResize(ratio);
  _fillArea.OnWindowResize(ratio);
  _padding *= ratio;
  _size *= ratio;
  _offset *= ratio;
  _fillAreaOffset *= ratio;
}
