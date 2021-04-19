#pragma once
#include "Engine.h"
#include <filesystem>

namespace EditorGUI {
  class FileSystemPanel {
  private:
    std::shared_ptr< Engine::Scene > _scene = nullptr;
    std::filesystem::path _rootPath;
    std::filesystem::path _currentPath;
    std::filesystem::path _selectedFile;
    std::filesystem::path _selectedDirectory;
    /* Resources */
    std::shared_ptr< Engine::GL::Texture2D > m_fileIcon;
    std::shared_ptr< Engine::GL::Texture2D > m_folderIcon;
    std::shared_ptr< Engine::GL::Texture2D > m_backArrowIcon;
    bool _openFolder = false;

  public:
    FileSystemPanel();
    auto SetScene(const std::shared_ptr< Engine::Scene >& scene) -> void;
    auto OnImGuiRender() -> void;

  private:
    auto FolderTree(const std::filesystem::path& currentPath, const std::filesystem::path& seekPath)
        -> void;
    auto FolderView(const std::filesystem::path& currentPath, float folder_view_width) -> void;
  };
}  // namespace EditorGUI