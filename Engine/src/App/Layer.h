#pragma once
#include <string>

namespace Engine {
  class Layer {
  public:
    Layer(const std::string& name);
    virtual ~Layer();

    virtual void OnAttach() = 0;
    virtual void OnDetach() = 0;
    virtual void OnUpdate() = 0;

  private:
    std::string m_LayerName;
  };
}  // namespace Engine