#include "pch.h"
#include "MeshRenderer.h"
namespace Engine::Components {
  auto MeshRenderer::GetMesh() -> std::shared_ptr< Renderer::Mesh > {
    return _mesh;
  }

  auto MeshRenderer::SetMesh(std::shared_ptr< Renderer::Mesh > mesh) -> void {
    _mesh  = mesh;
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
