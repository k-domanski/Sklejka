#pragma once
#include "UIElement.h"
#include "GL/Buffer.h"
#include "GL/Shader.h"
#include "GL/Texture2D.h"
#include "GL/VertexArray.h"
#include "Utility/Utility.h"

namespace Engine::Renderer {
  class Text : public UIElement {
  public:
    std::string GetText() const;
    void SetText(const std::string& text);
    glm::vec4 Color() const;
    void Color(const glm::vec4& color);
    float Size() const;
    void Size(float size);
    glm::vec2 Offset();
    void Offset(glm::vec2 offset);
    Text();
    Text(Text&& other);
    auto operator=(Text&& other) noexcept -> Text&;
    auto operator=(Text& other) = delete;
    //~Text() = default;
    auto Draw(glm::mat4 model, glm::mat4 proj) -> void override;

  private:
    std::shared_ptr< GL::Shader > _shader;
    std::string _text;
    glm::vec4 _color;
    float _size;
    glm::vec2 _offset;
    float _newLineOffset{50};
    std::shared_ptr< std::map< char, Utility::Character > > _characters;
    GL::VertexBuffer _vbo;
    GL::VertexArray _vao;
    /*unsigned int VAO;
    unsigned int VBO;*/

    auto RenderText(float x, float y,
                    float scale) -> void;
  };

}  // namespace Engine::Renderer
