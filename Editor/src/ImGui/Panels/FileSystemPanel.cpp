#include "FileSystemPanel.h"
#include <ImGui/imgui.h>
#include <queue>
#include <EditorLayer.h>

namespace fs = std::filesystem;

struct EntryPair {
  bool is_directory;
  fs::path path;
  EntryPair(bool b, const fs::path& p): is_directory(b), path(p) {
  }
};

auto operator<(const EntryPair& lhs, const EntryPair& rhs) -> bool {
  if (lhs.is_directory == rhs.is_directory) {
    return lhs.path > rhs.path;
  }
  return lhs.is_directory < rhs.is_directory;
}

using namespace Engine;
namespace Editor {
  FileSystemPanel::FileSystemPanel() {
    _rootPath          = AssetManager::GetAssetsFolders().assets;
    _currentPath       = _rootPath;
    _selectedDirectory = _rootPath;
    _selectedFile      = _rootPath;
    m_fileIcon         = AssetManager::GetTexture2D("./resources/icons/file_icon.png");
    m_folderIcon       = AssetManager::GetTexture2D("./resources/icons/folder_icon.png");
    m_backArrowIcon    = AssetManager::GetTexture2D("./resources/icons/back_arrow.png");
  }
  auto FileSystemPanel::SetScene(const std::shared_ptr< Engine::Scene >& scene) -> void {
    _scene = scene;
  }
  auto FileSystemPanel::SetEditorLayer(EditorLayer* layer) -> void {
    _editorLayer = layer;
  }
  auto FileSystemPanel::OnImGuiRender() -> void {
    ImGui::Begin("File System");

    const auto window_content_width = ImGui::GetWindowContentRegionWidth();
    auto tree_view_width            = std::clamp(window_content_width * 0.3f, 150.0f, 300.0f);
    auto folder_view_width          = window_content_width - tree_view_width;

    /* Left - Tree View */
    if (ImGui::BeginChild("TreeView", ImVec2(tree_view_width, 0), true)) {
      FolderTree(_rootPath, _selectedDirectory);
    }
    ImGui::EndChild();

    /* Right - Folder View*/
    ImGui::SameLine();
    ImGui::BeginGroup();

    ImGui::Text("Directory: %s", _selectedDirectory.string().c_str());
    ImGui::Separator();
    if (ImGui::BeginChild("FolderView", ImVec2(folder_view_width, 0))) {
      FolderView(_selectedDirectory, folder_view_width);
    }

    ImGui::EndChild();
    ImGui::EndGroup();

    ImGui::End();
  }
  auto FileSystemPanel::FolderTree(const std::filesystem::path& currentPath,
                                   const std::filesystem::path& seekPath) -> void {
    const auto is_directory    = fs::is_directory(currentPath);
    const auto is_empty        = fs::is_empty(currentPath);
    const auto seek_str        = seekPath.string();
    const auto is_seek_reached = currentPath == seekPath;

    ImGuiTreeNodeFlags flags = ((is_empty || !is_directory) ? ImGuiTreeNodeFlags_Leaf : 0)
                               | (_selectedFile == currentPath || _selectedDirectory == currentPath
                                      ? ImGuiTreeNodeFlags_Selected
                                      : 0)
                               | ImGuiTreeNodeFlags_OpenOnArrow
                               | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    const auto id =
        reinterpret_cast< const void* >(std::hash< std::string >{}(currentPath.string()));

    std::priority_queue< EntryPair > entry_queue;
    if (!is_seek_reached && seek_str.find(currentPath.string()) != std::string::npos) {
      ImGui::SetNextItemOpen(true);
    } else if (is_seek_reached && _openFolder) {
      ImGui::SetNextItemOpen(true);
      _openFolder = false;
    }
    auto is_open = ImGui::TreeNodeEx(id, flags, "%s", currentPath.filename().string().c_str());
    if (ImGui::IsItemClicked()) {
      if (is_directory) {
        _selectedDirectory = currentPath;
      } else {
        _selectedFile = currentPath;
      }
    }
    if (is_open) {
      if (is_directory) {
        // Sort entries
        for (auto& entry : fs::directory_iterator(currentPath)) {
          const auto& path = entry.path();

          entry_queue.emplace(fs::is_directory(path), path);
        }
        // Go deeper
        while (!entry_queue.empty()) {
          auto& [_, path] = entry_queue.top();
          FolderTree(path, seekPath);
          entry_queue.pop();
        }
      }
      ImGui::TreePop();
    }
  }
  auto FileSystemPanel::FolderView(const std::filesystem::path& currentPath,
                                   float folder_view_width) -> void {
    const auto icon_size      = ImVec2{64, 64};
    const auto folder_icon_id = reinterpret_cast< ImTextureID >(m_folderIcon->GetHandle());
    const auto file_icon_id   = reinterpret_cast< ImTextureID >(m_fileIcon->GetHandle());
    const auto arrow_icon_id  = reinterpret_cast< ImTextureID >(m_backArrowIcon->GetHandle());
    const auto btn_width      = 100;
    const auto spacing        = ImGui::GetStyle().ItemSpacing.x;
    const auto text_length    = ImGui::GetWindowContentRegionWidth() - btn_width - spacing;

    const auto horizontal_item_count =
        static_cast< int >(folder_view_width / (icon_size.x + ImGui::GetStyle().ItemSpacing.x));
    const auto wrap_pos = ImGui::GetCursorPos().x + std::max(300.0f, text_length);
    ;
    ImGui::PushTextWrapPos(wrap_pos);
    ImGui::TextWrapped("File: %s", _selectedFile.string().c_str());
    ImGui::PopTextWrapPos();
    auto ext = _selectedFile.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    if (ext == ".fbx" || ext == ".obj") {
      ImGui::SameLine(text_length);
      if (ImGui::Button("Add Model", {btn_width, 0})) {
        _editorLayer->AddObjectOnScene(_selectedFile.string());
      }
    }

    // Sort entries
    std::priority_queue< EntryPair > entry_queue;
    for (auto& entry : fs::directory_iterator(currentPath)) {
      const auto& path = entry.path();
      entry_queue.emplace(fs::is_directory(path), path);
    }

    int i = 0;
    if (currentPath != _rootPath) {
      if (ImGui::ImageButton(arrow_icon_id, icon_size)) {
        _selectedDirectory = currentPath.parent_path();
        _openFolder        = true;
      }
      ++i;
      ImGui::SameLine();
    }
    while (!entry_queue.empty()) {
      auto& [_, path]      = entry_queue.top();
      const auto file_name = path.filename().string();
      if (i > 0) {
        ImGui::SameLine();
      }

      ImGui::BeginGroup();
      auto is_directory = fs::is_directory(path);
      auto icon_id      = folder_icon_id;
      if (!is_directory) {
        icon_id = file_icon_id;
      }

      auto btn_id = std::hash< std::string >{}(path.string());
      ImGui::PushID(btn_id);
      if (ImGui::ImageButton(icon_id, icon_size)) {
        if (is_directory) {
          _selectedDirectory = path;
          _openFolder        = true;

        } else {
          // Select file
          _selectedFile = path;
        }
      }
      if (!is_directory) {
        _dragAndDropPath               = path;
        const ImGuiDragDropFlags flags = ImGuiDragDropFlags_None;
        if (ImGui::BeginDragDropSource(flags)) {
          ImGui::SetDragDropPayload("FILE", _dragAndDropPath.string().c_str(),
                                    _dragAndDropPath.string().size() + 1);
          ImGui::Text("%s", _dragAndDropPath.filename().string().c_str());
          ImGui::EndDragDropSource();
        }
      }
      ImGui::PopID();

      ImGui::PushTextWrapPos(icon_size.x + ImGui::GetCursorPos().x);
      ImGui::TextWrapped("%s", file_name.c_str());
      ImGui::PopTextWrapPos();

      ImGui::EndGroup();

      if (++i >= horizontal_item_count - 1) {
        i = 0;
      }
      entry_queue.pop();
    }
  }
}  // namespace Editor
