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
    virtual void OnUpdate(double deltaTime) = 0;
    virtual void OnEvent(Event& event) = 0;

  private:
    std::string m_LayerName;
  };
}  // namespace Engine