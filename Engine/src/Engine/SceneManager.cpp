#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
/// <summary>
/// For Entity Manager usage to add entities when loading scene;
/// </summary>
/// <returns></returns>
auto Engine::SceneManager::GetCurrentScene() -> std::shared_ptr< Scene > {
  return _currentScene;
}
/// <summary>
/// Get scene to display, returns loading scene when another scene is opening and current used scene
/// when not
/// </summary>
/// <returns></returns>
auto Engine::SceneManager::GetDisplayScene() -> std::shared_ptr< Scene > {
  //if (_openingScene)
  //  return _loadingScene;
  return _currentScene;
}

auto Engine::SceneManager::LoadScene(const std::string& file) -> void {

}

auto Engine::SceneManager::SaveScene(size_t id) -> void {

}

auto Engine::SceneManager::OpenScene(size_t id) -> void {
  if (_currentScene != nullptr)
    if (_currentScene->GetID() == id)
      return;
  _openingScene = true;

  // lets say loading scene may take some time here
  _currentScene = _loadedScenes[id];
  _currentScene->OpenScene();

  _openingScene = false;
}

auto Engine::SceneManager::AddScene(std::shared_ptr< Scene > scene) -> void {
  _loadedScenes[scene->GetID()] = scene;
}
