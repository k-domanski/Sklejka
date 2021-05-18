#pragma once
#include "UIElement.h"
#include "GL/Shader.h"
#include "GL/Texture2D.h"
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
    Text();
    ~Text() = default;
    auto Draw(glm::mat4 model, glm::mat4 proj) -> void override;

  private:
    std::shared_ptr< GL::Shader > _shader;
    std::string _text;
    glm::vec4 _color;
    float _size;
    std::shared_ptr< std::map< char, Utility::Character > > _characters;
    unsigned int VAO;
    unsigned int VBO;

    auto RenderText(float x, float y,
                    float scale) -> void;
  };

}  // namespace Engine::Renderer