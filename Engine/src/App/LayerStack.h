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
    void AddOverlay(Layer* layer);

    std::vector< Layer* >::iterator begin() { return m_Layers.begin(); }
    std::vector< Layer* >::iterator end() { return m_Layers.end(); }
    std::vector< Layer* >::reverse_iterator rbegin() { return m_Layers.rbegin(); }
    std::vector< Layer* >::reverse_iterator rend() { return m_Layers.rend(); }

  private:
    std::vector< Layer* > m_Layers;
    unsigned int m_Index = 0;
  };
}  // namespace Engine