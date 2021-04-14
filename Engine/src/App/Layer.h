#pragma once
#include <string>
#include "Events/Event.h"

namespace Engine {
  class Layer {
  public:
    Layer(const std::string& name);
    virtual ~Layer();

    virtual void OnAttach() = 0;
    virtual void OnDetach() = 0;
    virtual void OnUpdate(double deltaTime) {};
    virtual void OnEvent(Event& event) = 0;
    virtual void OnImGuiRender() {};

  private:
    std::string m_LayerName;
  };
}  // namespace Engine