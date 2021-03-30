#pragma once
#include "Layer.h"
#include <vector>

namespace Engine {
  class LayerStack {
  public:
    LayerStack();
    ~LayerStack();
    void AddLayer(Layer* layer);
    void RemoveLayer(Layer* layer);

    std::vector< Layer* >::iterator begin() { return m_Layers.begin(); }
    std::vector< Layer* >::iterator end() { return m_Layers.end(); }

  private:
    std::vector< Layer* > m_Layers;
  };
}  // namespace Engine