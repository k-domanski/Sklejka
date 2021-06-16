#pragma once
#include <functional>
#include <memory>

namespace Engine {
  namespace ECS {
    class Entity;
  }

  namespace Components {
    class UIRenderer;
  }

  namespace Renderer {
    class Bar;
    class Image;
    class Text;
    class Button;
  }  // namespace Renderer

  class Scene;
}  // namespace Engine

struct OptionsData {
  std::shared_ptr< Engine::Renderer::Button > upButton;
  std::shared_ptr< Engine::Renderer::Button > downButton;
  std::shared_ptr< Engine::Renderer::Bar > bar;
  std::shared_ptr< Engine::Renderer::Text > text;
  float minValue;
  float maxValue;
  float currentValue;
};

class OptionsMenu {
public:
  size_t OpenedFromScene() const;
  void OpenedFromScene(size_t opened_from_scene, int name);
  OptionsMenu();
  auto Show(std::function<void()> returnFunc) -> void;
  auto Hide()  -> void;
  auto HideFromButton()  -> void;
  auto IsVisible() -> bool;
  ~OptionsMenu() = default;
  auto Scene() -> std::shared_ptr< Engine::Scene >;

private:
  std::shared_ptr< Engine::Scene > _scene;
  std::shared_ptr< Engine::Renderer::Image > _background;
  std::shared_ptr< Engine::Renderer::Image > _titleImage;
  std::shared_ptr< Engine::Renderer::Text > _title;
  std::shared_ptr< Engine::Renderer::Button > _returnButton;
  std::shared_ptr< Engine::ECS::Entity > _backgroundEntity;
  std::shared_ptr< Engine::ECS::Entity > _titleEntity;
  std::shared_ptr< Engine::ECS::Entity > _buttonsEntity;
  OptionsData _musicData;
  OptionsData _soundData;
  OptionsData _brightnessData;
  OptionsData _contrastData;
  OptionsData _gammaData;
  size_t _openedFromScene;
  int _openedFromSceneName;
  bool _isVisible;

  auto AddAllElements() -> void;
  auto RemoveAllElements() -> void;
};
