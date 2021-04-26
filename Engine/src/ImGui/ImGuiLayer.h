#pragma once
#include "App/Layer.h"
#include "imgui.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Engine {
  class ImGuiLayer : public Layer {
  public:
    ImGuiLayer();
    ~ImGuiLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(Event& event) override;
    void BlockEvents(bool block) {
      m_BlockEvents = block;
    }
    /*Temporary*/
    virtual void OnImGuiRender() override;

    void Begin();
    void End();

  private:
    bool m_BlockEvents = true;
  };
}  // namespace Engine