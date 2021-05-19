#include "pch.h"
#include "Image.h"

#include "Mesh.h"
#include "App/AssetManager.h"
#include "App/Window.h"

float Engine::Renderer::Image::Ratio() const {
  return _ratio;
}

void Engine::Renderer::Image::Ratio(float ratio) {
  _ratio = ratio;
}

glm::vec4 Engine::Renderer::Image::Color() const {
  return _color;
}

void Engine::Renderer::Image::Color(const glm::vec4& color) {
  _color = color;
}

glm::vec2 Engine::Renderer::Image::Size() const {
  return _size;
}

void Engine::Renderer::Image::Size(const glm::vec2& size) {
  _size = size;
}

bool Engine::Renderer::Image::Horizontal() const {
  return _horizontal;
}

void Engine::Renderer::Image::Horizontal(bool horizontal) {
  _horizontal = horizontal;
}

std::shared_ptr< Engine::GL::Texture2D > Engine::Renderer::Image::Texture() const {
  return _texture;
}

void Engine::Renderer::Image::Texture(const std::shared_ptr< GL::Texture2D >& texture) {
  _texture = texture;
}

std::shared_ptr< Engine::GL::Shader > Engine::Renderer::Image::Shader() const {
  return _shader;
}

void Engine::Renderer::Image::Shader(const std::shared_ptr< GL::Shader >& shader) {
  _shader = shader;
}

Engine::Renderer::Image::Image() : UIElement() {
  _shader     = AssetManager::GetShader("./shaders/imageUI.glsl");
  _texture    = AssetManager::GetTexture2D("./textures/white.png");
  _mesh       = Mesh::GetPrimitive(MeshPrimitive::Plane);
  _ratio      = 1.0;
  _color      = glm::vec4(1.0f);
  _horizontal = true;
  _size       = glm::vec2(100.0f, 100.0f);
}

auto Engine::Renderer::Image::Draw(glm::mat4 model, glm::mat4 proj) -> void {
  model = glm::scale(model, glm::vec3(_size.x, _size.y, 1.0f));
  _shader->Use();
  _mesh->Use();
  _texture->Bind(0);
  _shader->SetMatrix("projection", proj);
  _shader->SetMatrix("model", model);
  _shader->SetValue("u_MainTexture", 0);
  _shader->SetVector("u_Color", _color);
  _shader->SetValue("ratio", _ratio);
  _shader->SetValue("horizontal", _horizontal);
  glDrawElements(_mesh->GetPrimitive(), _mesh->ElementCount(), GL_UNSIGNED_INT, NULL);
}

// auto Engine::Renderer::Image::Draw() -> void {
//  //_shader->Use();
//  auto mesh = Mesh::GetPrimitive(MeshPrimitive::Plane);
//
//  //glDisable(GL_DEPTH_TEST);
//
//  mesh->Use();
//  glActiveTexture(GL_TEXTURE0);
//  _texture->Bind(0);
//  auto size       = Window::Get().GetScreenSize();
//  auto projection = glm::ortho(0.0f, size.x, 0.0f, size.y, -1.0f, 1.0f);
//  _shader->SetMatrix("projection", projection);
//  _shader->SetValue("u_MainTexture", 0);
//
//  glDrawElements(mesh->GetPrimitive(), mesh->ElementCount(), GL_UNSIGNED_INT, NULL);
//  //glEnable(GL_DEPTH_TEST);
//}
