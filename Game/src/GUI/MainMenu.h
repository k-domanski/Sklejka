#pragma once
#include <memory>


#include "Components/Transform.h"
#include "Engine/Scene.h"
#include "Renderer/Button.h"
#include "Renderer/Image.h"

class MainMenu
{
public:
  MainMenu();
  ~MainMenu() = default;
  auto Scene() -> std::shared_ptr< Engine::Scene >;
  auto Show() -> void;
  auto Hide() -> void;
private:
  std::shared_ptr< Engine::Scene > _scene;
  std::shared_ptr< Engine::Renderer::Image > _image;
  //std::shared_ptr< Engine::Transform > _imageTransform;
  std::shared_ptr< Engine::Renderer::Button > _startButton;
  std::shared_ptr< Engine::Renderer::Button > _optionsButton;
  std::shared_ptr< Engine::Renderer::Button > _creditsButton;
  std::shared_ptr< Engine::Renderer::Button > _tutorialButton;
  std::shared_ptr< Engine::Renderer::Button > _exitButton;
  std::shared_ptr< Engine::Renderer::Text > _title;

};
