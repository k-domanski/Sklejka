#pragma once
#include <memory>

namespace Engine {
  namespace Renderer {
    class Bar;
    class Image;
    class Text;
    class Button;
  }

  class Scene;
}

struct OptionsData
{
  std::shared_ptr< Engine::Renderer::Button > upButton;
  std::shared_ptr< Engine::Renderer::Button > downButton;
  std::shared_ptr< Engine::Renderer::Bar > bar;
  std::shared_ptr< Engine::Renderer::Text > text;
  float minValue;
  float maxValue;
  float currentValue;
};

class OptionsMenu
{
public:
  OptionsMenu();
  ~OptionsMenu() = default;
  auto Scene() -> std::shared_ptr< Engine::Scene >;

private:
  std::shared_ptr< Engine::Scene > _scene;
  std::shared_ptr< Engine::Renderer::Image > _background;
  std::shared_ptr< Engine::Renderer::Text > _title;
  std::shared_ptr< Engine::Renderer::Button > _returnButton;
  OptionsData _musicData;
  OptionsData _soundData;
  OptionsData _brightnessData;
  OptionsData _contrastData;
  OptionsData _gammaData;

};
