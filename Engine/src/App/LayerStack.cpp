#include "pch.h"
#include "LayerStack.h"

namespace Engine {
  LayerStack::LayerStack() {
  }
  LayerStack::~LayerStack() {
    for (Layer* layer : m_Layers) {
      layer->OnDetach();
      delete layer;
    }
  }
  void LayerStack::AddLayer(Layer* layer) {
    m_Layers.push_back(layer);
  }
  void LayerStack::RemoveLayer(Layer* layer) {
    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end()) {
      layer->OnDetach();
      m_Layers.erase(it);
    }
  }
}  // namespace Engine