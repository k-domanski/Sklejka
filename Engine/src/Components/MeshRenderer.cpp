#include "pch.h"
#include "MeshRenderer.h"
namespace Engine::Components {
  auto MeshRenderer::GetModel() -> std::shared_ptr< Renderer::Model > {
    return _model;
  }

  auto MeshRenderer::SetModel(std::shared_ptr< Renderer::Model > model) -> void {
    _model  = model;
    _dirty = true;
  }

  auto MeshRenderer::GetMaterial() -> std::shared_ptr< Renderer::Material > {
    return _material;
  }

  auto MeshRenderer::SetMaterial(std::shared_ptr< Renderer::Material > material) -> void {
    _material = material;
    _dirty    = true;
  }

  auto MeshRenderer::IsDirty() -> bool {
    return _dirty;
  }

  void MeshRenderer::SetDirty(bool dirty) {
    _dirty = dirty;
  }
}  // namespace Engine::Components
