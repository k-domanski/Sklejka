#pragma once
#include "Engine.h"
#include <string>

class EditorLayer : public Engine::Layer {
public:
  EditorLayer(const std::string& name);
  virtual ~EditorLayer() = default;

  virtual void OnAttach() override;
  virtual void OnUpdate() override;
  virtual void OnDetach() override;
};