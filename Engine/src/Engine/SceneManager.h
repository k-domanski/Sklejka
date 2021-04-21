#pragma once
#include "Scene.h"

namespace Engine {
  class SceneManager {
  public:
    SceneManager()  = default;
    ~SceneManager() = default;

    static std::shared_ptr< Scene > GetCurrentScene();
    static std::shared_ptr< Scene > GetDisplayScene();
    static void LoadScene(const std::string& file);
    static void SaveScene(size_t id);
    static void OpenScene(size_t id);
    static void AddScene(std::shared_ptr< Scene > scene);

  private:
    static inline std::unordered_map< size_t, std::shared_ptr< Scene > > _loadedScenes;
    static inline std::shared_ptr< Scene > _currentScene;
    static inline std::shared_ptr< Scene > _loadingScene;
    static inline bool _openingScene{false};
  };
}  // namespace Engine
