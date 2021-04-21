#include "pch.h"
#include "SceneManager.h"
/// <summary>
/// For Entity Manager usage to add entities when loading scene;
/// </summary>
/// <returns></returns>
std::shared_ptr< Engine::Scene > Engine::SceneManager::GetCurrentScene() {
  return _currentScene;
}
/// <summary>
/// Get scene to display, returns loading scene when another scene is opening and current used scene
/// when not
/// </summary>
/// <returns></returns>
std::shared_ptr< Engine::Scene > Engine::SceneManager::GetDisplayScene() {
  if (_openingScene)
    return _loadingScene;
  return _currentScene;
}

void Engine::SceneManager::LoadScene(const std::string& file) {
}

void Engine::SceneManager::SaveScene(size_t id) {
}

void Engine::SceneManager::OpenScene(size_t id) {
  if (_currentScene->GetID() == id)
    return;
  _openingScene = true;

  // lets say loading scene may take some time here
  _currentScene = _loadedScenes[id];
  _currentScene->OpenScene();

  _openingScene = false;
}

void Engine::SceneManager::AddScene(std::shared_ptr< Scene > scene) {
  _loadedScenes[scene->GetID()] = scene;
}
