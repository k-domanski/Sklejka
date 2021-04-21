#include "pch.h"
#include "FileDialog.h"

#include <commdlg.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <App/Window.h>

namespace Engine {
  namespace FileDialog {
    std::optional< std::string > OpenFile(const char* filter) {
      OPENFILENAMEA ofn;
      CHAR file[260]       = {0};
      CHAR currentDir[256] = {0};
      ZeroMemory(&ofn, sizeof(OPENFILENAME));
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner   = glfwGetWin32Window((GLFWwindow*)Window::Get().GetNativeWindow());
      ofn.lpstrFile   = file;
      ofn.nMaxFile    = sizeof(file);
      if (GetCurrentDirectoryA(256, currentDir))
        ofn.lpstrInitialDir = currentDir;
      ofn.lpstrFilter  = filter;
      ofn.nFilterIndex = 1;
      ofn.Flags        = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

      if (GetOpenFileNameA(&ofn) == TRUE)
        return ofn.lpstrFile;
      return std::nullopt;
    }

    std::optional< std::string > SaveFile(const char* filter) {
      OPENFILENAMEA ofn;
      CHAR file[260]       = {0};
      CHAR currentDir[256] = {0};
      ZeroMemory(&ofn, sizeof(OPENFILENAME));
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner   = glfwGetWin32Window((GLFWwindow*)Window::Get().GetNativeWindow());
      ofn.lpstrFile   = file;
      ofn.nMaxFile    = sizeof(file);
      if (GetCurrentDirectoryA(256, currentDir))
        ofn.lpstrInitialDir = currentDir;
      ofn.lpstrFilter  = filter;
      ofn.nFilterIndex = 1;
      ofn.Flags        = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

      ofn.lpstrDefExt = strchr(filter, '\0') + 1;

      if (GetSaveFileNameA(&ofn) == TRUE)
        return ofn.lpstrFile;
      return std::nullopt;
    }
  }  // namespace FileDialog
}  // namespace Engine