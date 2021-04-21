#pragma once
//#include <pch.h>
//#include "Scene.h"

namespace Engine {
  class SceneManager {
  public:
    static auto GetCurrentScene() -> std::shared_ptr< class Scene >;
    static auto GetDisplayScene() -> std::shared_ptr< Scene >;
    static auto LoadScene(const std::string& file) -> void;
    static auto SaveScene(size_t id) -> void;
    static auto OpenScene(size_t id) -> void;
    static auto AddScene(std::shared_ptr< Scene > scene) -> void;

  private:
    static inline std::unordered_map< size_t, std::shared_ptr< Scene > > _loadedScenes;
    static inline std::shared_ptr< Scene > _currentScene;
    static inline std::shared_ptr< Scene > _loadingScene;
    static inline bool _openingScene;
  };
}  // namespace Engine
