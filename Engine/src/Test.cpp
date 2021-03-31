#include "pch.h"
#include "Test.h"
#include <stdio.h>
#include <glm/vec3.hpp>                   // glm::vec3
#include <glm/vec4.hpp>                   // glm::vec4
#include <glm/mat4x4.hpp>                 // glm::mat4
#include <glm/ext/matrix_transform.hpp>   // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp>  // glm::perspective
#include <glm/ext/scalar_constants.hpp>   // glm::pi
#include "imgui_impl_glfw.h"
#include "imguiOpenGL.h"
#include "stb_image.h"
#include "assimp/Importer.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <irrKlang.h>
#include <rttr/type>
// tmp
#include <GL/GLCore.h>
#include "App/Window.h"
#include <App/Log.h>
#include <App/Timer.h>
#include <Renderer/Mesh.h>
#include <Utility/Utility.h>

#include <ft2build.h>

#include "freetype/freetype.h"

#include FT_FREETYPE_H 

struct Character {
  unsigned int TextureID;  // ID handle of the glyph texture
  glm::ivec2 Size;         // Size of glyph
  glm::ivec2 Bearing;      // Offset from baseline to left/top of glyph
  unsigned int Advance;    // Offset to advance to next glyph
};

std::map< char, Character > Characters;

inline void RenderText(std::shared_ptr< Engine::GL::Shader > s, std::string text, float x, float y,
                       float scale, glm::vec3 color, unsigned int VAO, unsigned int VBO,
                       const std::unique_ptr< Engine::Window >& window) {
  // activate corresponding render state
  s->Use();
  s->SetMatrix("projection",
               glm::ortho(0.0f, (float)window->GetWidth(), 0.0f, (float)window->GetHeight()));
  glUniform3f(glGetUniformLocation(s->GetHandle(), "textColor"), color.x, color.y, color.z);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);

  // iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    Character ch = Characters[*c];

    float xpos = x + ch.Bearing.x * scale;
    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;
    // update VBO for each character
    float vertices[6][4] = {{xpos, ypos + h, 0.0f, 0.0f},    {xpos, ypos, 0.0f, 1.0f},
                            {xpos + w, ypos, 1.0f, 1.0f},

                            {xpos, ypos + h, 0.0f, 0.0f},    {xpos + w, ypos, 1.0f, 1.0f},
                            {xpos + w, ypos + h, 1.0f, 0.0f}};
    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    x += (ch.Advance >> 6) * scale;  // bitshift by 6 to get value in pixels (2^6 = 64)
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

glm::mat4 camera(float Translate, glm::vec2 const& Rotate) {
  glm::mat4 Projection = glm::perspective(glm::pi< float >() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
  glm::mat4 View       = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
  View                 = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
  View                 = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 Model      = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
  return Projection * View * Model;
}

template< typename T >
using ptr_t = std::shared_ptr< T >;

BETTER_ENUM(Word, int, Hello, World)
namespace Engine {
  // Remove
  void Print() {
    printf("Welcome to Sklejka Engine!\n");
  }

  int TestWindow() {
    Log::Init();

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    

    // TODO: Window as singleton
    std::unique_ptr< Engine::Window > window = Engine::Window::Create(Engine::WindowData());

    // TODO: Spearate ImGUI calls
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window->GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 430");
    ImGui::StyleColorsDark();

    /* OpenGL calls for initialization */
    GL::Context::Initialize();
    GL::Context::SetClearBufferMask(GL::BufferBit::Color);
    GL::Context::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});

    stbi_set_flip_vertically_on_load(true);

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
      std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
      return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft, "fonts/arialn/Arialn.ttf", 0, &face)) {
      std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
      return -1;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);
    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      return -1;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++) {
      // load character glyph
      if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
      }
      // generate texture
      unsigned int texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0,
                   GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
      // set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // now store character for later use
      Character character = {
          texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
          glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x};
      Characters.insert(std::pair< char, Character >(c, character));
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    unsigned int textVAO, textVBO;
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* Example square */
    std::vector< glm::vec3 > points = {{-0.5f, -0.5f, 0.0f},
                                       {0.5f, -0.5f, 0.0f},
                                       {0.5f, 0.5f, 0.0f},
                                       {-0.5f, 0.5f, 0.0f}};
    std::vector< glm::vec2 > uv     = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
    std::vector< GLuint > indices   = {0, 1, 2, 2, 3, 0};
    std::vector< Renderer::Vertex > vertices(points.size());
    for (int i = 0; i < vertices.size(); ++i) {
      auto p      = points[i];
      auto u      = uv[i];
      vertices[i] = {p, glm::vec3{0.0f}, u};
    }

    std::vector< glm::vec3 > bg_points = {{-1.0f, -0.7f, 0.0f},
                                          {1.0f, -0.7f, 0.0f},
                                          {1.f, 1.f, 0.0f},
                                          {-1.f, 1.f, 0.0f}};
    std::vector< Renderer::Vertex > bg_verts(bg_points.size());
    for (int i = 0; i < vertices.size(); ++i) {
      auto p      = bg_points[i];
      auto u      = uv[i];
      bg_verts[i] = {p, glm::vec3{0.0f}, u};
    }

    std::vector< glm::vec3 > jojo_points = {{-0.7f, -1.0f, 0.0f},
                                            {-0.1f, -1.0f, 0.0f},
                                            {-0.1f, -0.1f, 0.0f},
                                            {-0.7f, -0.1f, 0.0f}};
    std::vector< Renderer::Vertex > jojo_verts(jojo_points.size());
    for (int i = 0; i < vertices.size(); ++i) {
      auto p        = jojo_points[i];
      auto u        = uv[i];
      jojo_verts[i] = {p, glm::vec3{0.0f}, u};
    }

    /* ------------------- */
    using GL::Shader;
    using GL::ShaderType;
    using GL::SubShader;
    using GL::TextureBase;
    using Renderer::Mesh;
    /* ----- Texture ----- */
    // PEPO
    stbi_set_flip_vertically_on_load(true);
    int x, y, n;
    auto pixel_data = stbi_load("./textures/pepo_sad.png", &x, &y, &n, 4);
    TextureBase texture(GL_TEXTURE_2D);
    texture.Create(x, y, pixel_data);
    stbi_image_free(pixel_data);

    // JOJO
    pixel_data = stbi_load("./textures/jojo.png", &x, &y, &n, 4);
    TextureBase jojo_tex(GL_TEXTURE_2D);
    jojo_tex.Create(x, y, pixel_data);
    stbi_image_free(pixel_data);
    /* ----------- */

    ptr_t< Mesh > pepe_mesh = std::make_shared< Mesh >(vertices, indices);
    ptr_t< Mesh > bg_mesh   = std::make_shared< Mesh >(bg_verts, indices);
    ptr_t< Mesh > jojo_mesh = std::make_shared< Mesh >(jojo_verts, indices);
#define FOLDER_PATH "./shaders"
    auto vert_src           = Utility::ReadTextFile(FOLDER_PATH "/pass.vert");
    auto frag_src           = Utility::ReadTextFile(FOLDER_PATH "/color.frag");
    ptr_t< SubShader > vert = std::make_shared< SubShader >(ShaderType::VertexShader, vert_src);
    ptr_t< SubShader > frag = std::make_shared< SubShader >(ShaderType::FragmentShader, frag_src);
    ptr_t< Shader > shader  = std::make_shared< Shader >();
    shader->AttachShader(vert);
    shader->AttachShader(frag);
    shader->Link();

    auto textvert_src = Utility::ReadTextFile(FOLDER_PATH "/textShader.vert");
    auto textfrag_src = Utility::ReadTextFile(FOLDER_PATH "/textShader.frag");
    ptr_t< SubShader > textvert =
        std::make_shared< SubShader >(ShaderType::VertexShader, textvert_src);
    ptr_t< SubShader > textfrag =
        std::make_shared< SubShader >(ShaderType::FragmentShader, textfrag_src);
    ptr_t< Shader > textshader = std::make_shared< Shader >();
    textshader->AttachShader(textvert);
    textshader->AttachShader(textfrag);
    textshader->Link();
    /* ------------------- */

    /* ================ */

    Timer timer;
    float time = 0.0f;
    // TODO: Wrap WindowShouldClose in Window class
    while (!glfwWindowShouldClose(window->GetNativeWindow())) {
      timer.Update();
      time += timer.DeltaTime();
      // TODO: Separate ImGUI calls
      // ImGui_ImplOpenGL3_NewFrame();
      // ImGui_ImplGlfw_NewFrame();
      // ImGui::NewFrame();

      GL::Context::ClearBuffers();

      /* -------------------------- */

      shader->Use();
      // Draw bg
      bg_mesh->Use();
      glBindTexture(GL_TEXTURE_2D, 0);
      shader->SetValue("u_time", glm::radians(90.0f));
      shader->SetVector("u_color", glm::vec3{0.4f, 0.4f, 1.0f});
      glDrawElements(bg_mesh->GetPrimitive(), bg_mesh->ElementCount(), GL_UNSIGNED_INT, NULL);

      // Draw Jojo
      jojo_mesh->Use();
      jojo_tex.Bind(0);
      shader->SetVector("u_color", glm::vec3{0.0f});
      shader->SetValue("u_mainTexture", 0);
      glDrawElements(jojo_mesh->GetPrimitive(), jojo_mesh->ElementCount(), GL_UNSIGNED_INT, NULL);

      // Draw pepe
      pepe_mesh->Use();
      texture.Bind(0);
      shader->SetVector("u_color", glm::vec3{0.0f});
      shader->SetValue("u_mainTexture", 0);
      shader->SetValue("u_time", time);
      glDrawElements(pepe_mesh->GetPrimitive(), pepe_mesh->ElementCount(), GL_UNSIGNED_INT, NULL);

      std::string textToShow = "Time: " + std::to_string(time);

      RenderText(textshader, textToShow, 25.0f, 25.0f, 1.0f,
                 glm::vec3(0.5, 0.8f, 0.2f), textVAO, textVBO, window);

      /* -------------------------- */

      // TODO: Change name
      window->OnUpdate();

      // ImGui::ShowDemoWindow();
      // ImGui::Render();
      // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // glfwTerminate();
    return 0;
  }

  //void RenderText(std::shared_ptr< Engine::GL::Shader > s, std::string text, float x, float y,
  //                float scale, glm::vec3 color, unsigned int VAO, unsigned int VBO,
  //                const std::unique_ptr< Engine::Window > &window) {
  //  // activate corresponding render state
  //  s->Use();
  //  s->SetMatrix("projection",
  //              glm::ortho(0.0f, (float)window->GetWidth(), 0.0f, (float)window->GetHeight()));
  //  glUniform3f(glGetUniformLocation(s->GetHandle(), "textColor"), color.x, color.y, color.z);
  //  glActiveTexture(GL_TEXTURE0);
  //  glBindVertexArray(VAO);

  //  // iterate through all characters
  //  std::string::const_iterator c;
  //  for (c = text.begin(); c != text.end(); c++) {
  //    Character ch = Characters[*c];

  //    float xpos = x + ch.Bearing.x * scale;
  //    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

  //    float w = ch.Size.x * scale;
  //    float h = ch.Size.y * scale;
  //    // update VBO for each character
  //    float vertices[6][4] = {{xpos, ypos + h, 0.0f, 0.0f},    {xpos, ypos, 0.0f, 1.0f},
  //                            {xpos + w, ypos, 1.0f, 1.0f},

  //                            {xpos, ypos + h, 0.0f, 0.0f},    {xpos + w, ypos, 1.0f, 1.0f},
  //                            {xpos + w, ypos + h, 1.0f, 0.0f}};
  //    // render glyph texture over quad
  //    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
  //    // update content of VBO memory
  //    glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
  //    glBindBuffer(GL_ARRAY_BUFFER, 0);
  //    // render quad
  //    glDrawArrays(GL_TRIANGLES, 0, 6);
  //    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
  //    x += (ch.Advance >> 6) * scale;  // bitshift by 6 to get value in pixels (2^6 = 64)
  //  }
  //  glBindVertexArray(0);
  //  glBindTexture(GL_TEXTURE_2D, 0);
  //}

}  // namespace Engine