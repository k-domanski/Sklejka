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
private:
  std::shared_ptr< Engine::Scene > _scene;
  std::shared_ptr< Engine::Renderer::Image > _image;
  //std::shared_ptr< Engine::Transform > _imageTransform;
  std::shared_ptr< Engine::Renderer::Button > _exitButton;
  std::shared_ptr< Engine::Renderer::Button > _startButton;
  std::shared_ptr< Engine::Renderer::Text > _title;

};
