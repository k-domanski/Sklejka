#include "pch.h"
#include "Text.h"

#include "App/AssetManager.h"
#include "freetype/freetype.h"
#include "glm/gtx/matrix_decompose.hpp"

std::string Engine::Renderer::Text::GetText() const {
  return _text;
}

void Engine::Renderer::Text::SetText(const std::string& text) {
  _text = text;
}

glm::vec4 Engine::Renderer::Text::Color() const {
  return _color;
}

void Engine::Renderer::Text::Color(const glm::vec4& color) {
  _color = color;
}

glm::vec2 Engine::Renderer::Text::Size() const {
  return _size;
}

void Engine::Renderer::Text::Size(glm::vec2 size) {
  _size = size;
}

void Engine::Renderer::Text::Size(float size) {
  _size = glm::vec2(size);
}

glm::vec2 Engine::Renderer::Text::Offset() {
  return _offset;
}

void Engine::Renderer::Text::Offset(glm::vec2 offset) {
  _offset = offset;
}

Engine::Renderer::Text::Text() {
  _characters = AssetManager::GetCharacters("./fonts/finger-paint/FingerPaint-Regular.ttf", 48);
  _shader     = AssetManager::GetShader("./shaders/text.glsl");

  _size   = {1.0f, 1.0f};
  _color  = glm::vec4(1.0f);
  _text   = "New Text";
  _offset = glm::vec2(0.0f);

  // glGenVertexArrays(1, &VAO);
  // glGenBuffers(1, &VBO);
  // glBindVertexArray(VAO);
  _vao.Bind();
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  _vbo.Bind();
  // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  _vbo.SetData(sizeof(float) * 6 * 4, NULL);

  /* Enable Vertex Attrib and Attrib Pointer - OK, can leave it here */
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);
}
Engine::Renderer::Text::Text(Text&& other) {
  _vao        = std::move(other._vao);
  _vbo        = std::move(other._vbo);
  _shader     = other._shader;
  _characters = other._characters;
  _size       = other._size;
  _color      = other._color;
  _text       = other._text;
  _offset     = other._offset;
}

auto Engine::Renderer::Text::operator=(Text&& other) noexcept -> Text& {
  _vao        = std::move(other._vao);
  _vbo        = std::move(other._vbo);
  _shader     = other._shader;
  _characters = other._characters;
  _size       = other._size;
  _color      = other._color;
  _text       = other._text;
  _offset     = other._offset;
  return *this;
}

auto Engine::Renderer::Text::Draw(glm::mat4 model, glm::mat4 proj) -> void {
  // auto pos = glm::vec3(model[3]);
  // Hack for now like that later maybe just use fonst size as scale or smth
  /*glm::vec3 scale;
  glm::quat rotation;
  glm::vec3 translation;
  glm::vec3 skew;
  glm::vec4 perspective;
  glm::decompose(model, scale, rotation, translation, skew, perspective);*/

  _shader->Use();
  _shader->SetMatrix("projection", proj);
  _shader->SetMatrix("model", model);
  _shader->SetValue("u_MainTexture", 0);

  RenderText(_offset.x, _offset.y, _size);
}

auto Engine::Renderer::Text::OnWindowResize(glm::vec2 ratio) -> void {
  _size *= ratio;
  _offset *= ratio;
}

auto Engine::Renderer::Text::RenderText(float x, float y, glm::vec2 scale) -> void {
  //_shader->Use();
  // glUniform3f(glGetUniformLocation(_shader->GetHandle(), "textColor"), color.x, color.y,
  // color.z);
  _shader->SetVector("u_Color", _color);
  GL::Context::SetActiveTexture(0);
  // glActiveTexture(GL_TEXTURE0);
  _vao.Bind();
  // glBindVertexArray(VAO);
  auto startXPos = x;

  // iterate through all characters
  std::string::const_iterator c;
  auto& characters = *_characters;
  for (c = _text.begin(); c != _text.end(); c++) {
    if (*c == (char)10) {
      x = startXPos;
      y -= _newLineOffset * scale.y;
      continue;
    }
    Utility::Character ch = characters[*c];

    float xpos = x + ch.Bearing.x * scale.x;
    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale.y;

    float w = ch.Size.x * scale.x;
    float h = ch.Size.y * scale.y;
    // update VBO for each character
    float vertices[6][4] = {{xpos, ypos + h, 0.0f, 0.0f},    {xpos, ypos, 0.0f, 1.0f},
                            {xpos + w, ypos, 1.0f, 1.0f},

                            {xpos, ypos + h, 0.0f, 0.0f},    {xpos + w, ypos, 1.0f, 1.0f},
                            {xpos + w, ypos + h, 1.0f, 0.0f}};
    // render glyph texture over quad
    // glBindTexture(GL_TEXTURE_2D, ch.Texture->GetHandle());
    ch.Texture->Bind(0);
    // update content of VBO memory
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    _vbo.Bind();
    _vbo.SetData(sizeof(vertices), vertices);
    // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    x += (ch.Advance >> 6) * scale.x;  // bitshift by 6 to get value in pixels (2^6 = 64)
  }
  // glBindVertexArray(0);
  // glBindTexture(GL_TEXTURE_2D, 0);
}
