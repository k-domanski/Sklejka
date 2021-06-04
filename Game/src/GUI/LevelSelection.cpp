#include "LevelSelection.h"

#include "GameManager.h"
#include "App/AssetManager.h"
#include "App/Window.h"
#include "Components/Transform.h"
#include "Components/UIRenderer.h"
#include "Engine/SceneManager.h"
#include "Engine/Scene.h"
#include "ECS/EntityManager.h"

LevelSelection::LevelSelection() {
  _scene             = std::make_shared< Engine::Scene >(969696);
  auto current_scene = Engine::SceneManager::GetCurrentScene();
  Engine::SceneManager::AddScene(_scene);
  Engine::SceneManager::OpenScene(_scene->GetID());

  auto window_size = Engine::Window::Get().GetScreenSize();

  auto backgroundEntity     = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto backgroundUIRenderer = backgroundEntity->AddComponent< Engine::Components::UIRenderer >();
  auto backgroundTransform  = backgroundEntity->AddComponent< Engine::Transform >();
  _background               = std::make_shared< Engine::Renderer::Image >();
  _background->Size(glm::vec2(window_size));
  _background->Texture(Engine::AssetManager::GetTexture2D("./textures/squirrelNinja.png"));
  backgroundTransform->Position(glm::vec3(window_size * 0.5f, 0.0f));
  backgroundUIRenderer->AddElement(_background);

  auto entity     = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto uiRenderer = entity->AddComponent< Engine::Components::UIRenderer >();
  auto transform  = entity->AddComponent< Engine::Transform >();
  _lvl1Button     = std::make_shared< Engine::Renderer::Button >();
  uiRenderer->AddButton(_lvl1Button);
  uiRenderer->AddElement(_lvl1Button);
  _lvl1Button->Color(glm::vec4(1.0f));
  _lvl1Button->PressedColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  _lvl1Button->OnPress([]() { GameManager::SwitchScene(SceneName::Cutscene); });
  _lvl1Button->text("Level 1");
  _lvl1Button->TextColor(glm::vec4(0, 0, 0, 1));
  _lvl1Button->TextOffset(glm::vec2(-50.0f, -20.0f));
  _lvl1Button->Size(glm::vec2(300.0f, 100.0f));
  _lvl1Button->HandleSize(glm::vec2(300.0f, 100.0f));
  _lvl1Button->Background(Engine::AssetManager::GetTexture2D("./textures/lvl1ImagePlaceholder.png"));
  transform->Position(glm::vec3(window_size.x * 0.5f, window_size.y * 0.5f, 0.0f));

  _title = std::make_shared< Engine::Renderer::Text >();
  _title->Size(1);
  _title->SetText("Choose level");
  _title->Color(glm::vec4(1.0f));
  _title->Offset(glm::vec2(0.0f, window_size.y * 0.25f));
  backgroundUIRenderer->AddElement(_title);



  if (current_scene != nullptr) {
    Engine::SceneManager::OpenScene(current_scene->GetID());
  }
}

auto LevelSelection::Scene() -> std::shared_ptr< Engine::Scene > {
  return _scene;
}
