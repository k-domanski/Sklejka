#include "pch.h"
#include "Renderer.h"

#include "Components/MeshRenderer.h"
#include "Components/Transform.h"
#include "ECS/EntityManager.h"
#include <App/Window.h>
#include <App/AssetManager.h>
namespace Engine::Systems {
  Renderer::Renderer() {
    using namespace GL;
    AddSignature< Components::MeshRenderer >();
    AddSignature< Transform >();
    // OnWindowResize(Window::Get().GetScreenSize());
    auto size     = Window::Get().GetScreenSize();
    _renderTarget = std::make_shared< RenderTarget >(size.x, size.y);
    _screenTexture =
        std::make_shared< TextureAttachment >(size.x, size.y, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
    _renderTarget->AttachColor(0, _screenTexture);
    _renderTarget->AttachDepthStencil(std::make_shared< Renderbuffer >(size.x, size.y));

    _quad       = Engine::Renderer::Mesh::GetPrimitive(Engine::Renderer::MeshPrimitive::Plane);
    _quadShader = AssetManager::GetShader("./shaders/screen_quad.glsl");
    assert(("Screen shader not loaded", _quadShader != nullptr));
  }
  void Renderer::Update(float deltaTime) {
    SortByMaterial();
    // Geometry
    _renderTarget->Bind(GL_FRAMEBUFFER);
    GL::Context::ClearBuffers(GL::BufferBit::Color | GL::BufferBit::Depth);
    glEnable(GL_DEPTH_TEST);

    for (auto [material, vec] : _sortedEntities) {
      if (material == nullptr)
        continue;
      material->Use();
      for (auto entityID : vec) {
        auto meshRenderer =
            ECS::EntityManager ::GetInstance().GetComponent< Components::MeshRenderer >(entityID);
        auto mesh      = meshRenderer->GetMesh();
        auto transform = ECS::EntityManager ::GetInstance().GetComponent< Transform >(entityID);
        mesh->Use();
        // TODO: change for world matrix
        material->GetShader()->SetMatrix("u_model_matrix", transform->GetWorldMatrix());
        // HACK: Assume for now that under slot 1 is camera uniform buffer
        // TODO: Get the actual slot number from somewhere, somehow :)
        material->GetShader()->BindUniformBlock("u_Camera", 1);
        glDrawElements(mesh->GetPrimitive(), mesh->ElementCount(), GL_UNSIGNED_INT, NULL);
      }
    }
    // Post process
    GL::Context::BindFramebuffer(GL_FRAMEBUFFER, 0);
    GL::Context::ClearBuffers(GL::BufferBit::Color);
    glDisable(GL_DEPTH_TEST);
    _quad->Use();
    _quadShader->Use();
    _quadShader->SetValue("u_mainTexture", 0);
    _screenTexture->Bind(0);
    glDrawElements(_quad->GetPrimitive(), _quad->ElementCount(), GL_UNSIGNED_INT, NULL);
  }

  auto Renderer::AddEntity(ECS::EntityID id) -> void {
    System::AddEntity(id);
  }

  auto Renderer::OnWindowResize(glm::vec2 windowSize) -> void {
    using namespace GL;
    auto size     = windowSize;
    _renderTarget = std::make_shared< RenderTarget >(size.x, size.y);
    _screenTexture =
        std::make_shared< TextureAttachment >(size.x, size.y, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
    _renderTarget->AttachColor(0, _screenTexture);
    _renderTarget->AttachDepthStencil(std::make_shared< Renderbuffer >(size.x, size.y));
  }

  auto Renderer::SortByMaterial() -> void {
    for (auto entityID : _entities) {
      auto meshRenderer =
          ECS::EntityManager::GetInstance().GetComponent< Components::MeshRenderer >(entityID);
      if (meshRenderer->IsDirty()) {
        _entitiesToSort.insert(entityID);
        meshRenderer->SetDirty(false);
      }
    }

    for (auto entityID : _entitiesToSort) {
      auto meshRenderer =
          ECS::EntityManager::GetInstance().GetComponent< Components::MeshRenderer >(entityID);
      auto material = meshRenderer->GetMaterial();
      if (material == nullptr)
        continue;
      for (auto& [materialFRST, vec] : _sortedEntities) {
        auto it =
            std::find_if(vec.begin(), vec.end(), [entityID](auto id) { return id == entityID; });
        if (it != vec.end()) {
          vec.erase(it);
          break;
        }
      }
      if (_sortedEntities.count(material) == 0) {
        std::vector< ECS::EntityID > vec;
        vec.push_back(entityID);
        _sortedEntities[material] = std::move(vec);
      } else
        _sortedEntities[material].push_back(entityID);
    }

    _entitiesToSort.clear();
  }
}  // namespace Engine::Systems