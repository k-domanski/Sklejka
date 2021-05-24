#pragma once
#include <Engine.h>

class LoadingScreen {
private:
  std::shared_ptr< Engine::Scene > _scene;
  std::shared_ptr< Engine::ECS::Entity > _entity;
  std::shared_ptr< Engine::Renderer::Image > _image;

public:
  LoadingScreen();
  auto Scene() -> std::shared_ptr< Engine::Scene >;
};