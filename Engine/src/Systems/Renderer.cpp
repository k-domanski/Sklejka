#include "pch.h"
#include "Renderer.h"
#include "Components/MeshRenderer.h"
#include "Components/Transform.h"
#include "ECS/EntityManager.h"
#include <App/Window.h>
#include <App/AssetManager.h>

#include "Components/Collider.h"

namespace Engine::Systems {
  using namespace GL;
  Renderer::Renderer() {
    AddSignature< Components::MeshRenderer >();
    AddSignature< Transform >();
    // OnWindowResize(Window::Get().GetScreenSize());
    auto size     = Window::Get().GetScreenSize();
    _renderTarget = std::make_shared< RenderTarget >(size.x, size.y);
    _screenTexture =
        std::make_shared< TextureAttachment >(size.x, size.y, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
    _renderTarget->AttachColor(0, _screenTexture);
    _renderTarget->AttachDepthStencil(std::make_shared< Renderbuffer >(size.x, size.y));

    _quad         = Engine::Renderer::Mesh::GetPrimitive(Engine::Renderer::MeshPrimitive::Plane);
    _quadShader   = AssetManager::GetShader("./shaders/screen_quad.glsl");

    _boxCollider = Engine::Renderer::Mesh::GetPrimitive(Engine::Renderer::MeshPrimitive::WireframeBox);
    _boxColliderShader = AssetManager::GetShader("./shaders/color.glsl");

    _sphereCollider = Engine::Renderer::Mesh::GetPrimitive(Engine::Renderer::MeshPrimitive::WireframeSphere);
    _sphereColliderShader = AssetManager::GetShader("./shaders/color.glsl");

    _cameraSystem = ECS::EntityManager::GetInstance().GetSystem< CameraSystem >();
    _transformUniformBuffer.BindToSlot(_transformUniformSlot);

    _debugMaterial = AssetManager::GetMaterial("./materials/_error_.mat");
    if (_debugMaterial == nullptr) {
      LOG_ERROR("Failed to load debug material: {}", "./materials/_error_.mat");
    }
  }
  void Renderer::Update(float deltaTime) {
    using Engine::Components::MeshRenderer;
    using Engine::ECS::Entity;
    using Engine::Renderer::Material;
    using Engine::Renderer::Mesh;

    const auto camera = _cameraSystem->MainCamera();
    if (camera == nullptr) {
      return;
    }
    SortByMaterial();
    // Geometry
    _renderTarget->Bind(FramebufferTarget::ReadWrite);
    GL::Context::ClearBuffers(GL::BufferBit::Color | GL::BufferBit::Depth);
    GL::Context::DepthTest(true);

    // for (auto [material, vec] : _sortedEntities) {
    for (auto& entityID : _entities) {
      auto mesh_renderer = ECS::EntityManager::GetComponent< MeshRenderer >(entityID);
      auto material      = mesh_renderer->GetMaterial();
      auto model         = mesh_renderer->GetModel();
      std::shared_ptr< Mesh > mesh;
      // If there is no mesh, there is nothing to show
      if (model == nullptr || (mesh = model->GetRootMesh()) == nullptr) {
        continue;
      }

      // If no material -> Use debug
      if (material == nullptr || material->GetShader() == nullptr) {
        material = _debugMaterial;
      }
      auto shader          = material->GetShader();
      const auto transform = ECS::EntityManager::GetComponent< Transform >(entityID);
      material->Use();
      mesh->Use();

      _transformUniformData.model     = transform->GetWorldMatrix();
      _transformUniformData.modelView = camera->ViewMatrix() * _transformUniformData.model;
      _transformUniformData.modelViewProjection =
          camera->ProjectionMatrix() * _transformUniformData.modelView;

      // HACK: Assume for now that under slot 1 is camera uniform buffer
      // TODO: Get the actual slot number from somewhere, somehow :)
      _transformUniformBuffer.SetData(_transformUniformData);
      shader->BindUniformBlock("u_Transform", 0);
      shader->BindUniformBlock("u_Camera", 1);
      glDrawElements(mesh->GetPrimitive(), mesh->ElementCount(), GL_UNSIGNED_INT, NULL);

      //collider
      std::shared_ptr< Components::Collider > collider;
      if ((collider = ECS::EntityManager::GetComponent< Components::Collider>(entityID)) != nullptr)
      {
        if (collider->Type == +Components::ColliderType::Box)
        {
          _boxColliderShader->Use();
          _boxCollider->Use();

          auto colModel = transform->GetWorldMatrix();
          colModel      = glm::translate(colModel, collider->Center);
          colModel      = glm::scale(colModel, collider->Size);

          _transformUniformData.model     = colModel;
          _transformUniformData.modelView = camera->ViewMatrix() * _transformUniformData.model;
          _transformUniformData.modelViewProjection =
              camera->ProjectionMatrix() * _transformUniformData.modelView;

          // HACK: Assume for now that under slot 1 is camera uniform buffer
          // TODO: Get the actual slot number from somewhere, somehow :)
          _transformUniformBuffer.SetData(_transformUniformData);
          _boxColliderShader->BindUniformBlock("u_Transform", 0);
          _boxColliderShader->BindUniformBlock("u_Camera", 1);
          _boxColliderShader->SetVector("u_MainColor", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
          glDrawElements(_boxCollider->GetPrimitive(), _boxCollider->ElementCount(), GL_UNSIGNED_INT, NULL);
        } else if (collider->Type == +Components::ColliderType::Sphere)
        {
          _sphereColliderShader->Use();
          _sphereCollider->Use();

          glEnable(GL_PRIMITIVE_RESTART);
          glPrimitiveRestartIndex(2137);

          auto colModel = transform->GetWorldMatrix();
          
          colModel = glm::translate(colModel, collider->Center);
          colModel      = glm::scale(colModel, glm::vec3(collider->Size.x));
          
          _transformUniformData.model     = colModel;
          _transformUniformData.modelView = camera->ViewMatrix() * _transformUniformData.model;
          _transformUniformData.modelViewProjection =
              camera->ProjectionMatrix() * _transformUniformData.modelView;

          // HACK: Assume for now that under slot 1 is camera uniform buffer
          // TODO: Get the actual slot number from somewhere, somehow :)
          _transformUniformBuffer.SetData(_transformUniformData);
          _sphereColliderShader->BindUniformBlock("u_Transform", 0);
          _sphereColliderShader->BindUniformBlock("u_Camera", 1);
          _sphereColliderShader->SetVector("u_MainColor", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
          glDrawElements(_sphereCollider->GetPrimitive(), _sphereCollider->ElementCount(),
                         GL_UNSIGNED_INT, NULL);

          glDisable(GL_PRIMITIVE_RESTART);
        }

      }


      // for (auto entityID : vec) {
      //  auto shader       = material->GetShader();
      //  auto transform    = ECS::EntityManager::GetComponent< Transform >(entityID);
      //  auto meshRenderer = ECS::EntityManager ::GetComponent< Components::MeshRenderer
      //  >(entityID); auto mesh         = meshRenderer->GetModel()->GetRootMesh(); mesh->Use();

      //  _transformUniformData.model     = transform->GetWorldMatrix();
      //  _transformUniformData.modelView = camera->ViewMatrix() * _transformUniformData.model;
      //  _transformUniformData.modelViewProjection =
      //      camera->ProjectionMatrix() * _transformUniformData.modelView;

      //  // HACK: Assume for now that under slot 1 is camera uniform buffer
      //  // TODO: Get the actual slot number from somewhere, somehow :)
      //  _transformUniformBuffer.SetData(_transformUniformData);
      //  shader->BindUniformBlock("u_Transform", 0);
      //  shader->BindUniformBlock("u_Camera", 1);
      //  glDrawElements(mesh->GetPrimitive(), mesh->ElementCount(), GL_UNSIGNED_INT, NULL);
      //}
    }
    // Post process
    GL::Context::BindFramebuffer(FramebufferTarget::ReadWrite, 0);
    GL::Context::ClearBuffers(GL::BufferBit::Color);
    GL::Context::DepthTest(false);
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
    using Engine::Components::MeshRenderer;
    using Engine::ECS::EntityID;
    using Engine::Renderer::Material;
    const auto OrderFunction = [](const EntityID lhs, const EntityID rhs) {
      auto lmr  = ECS::EntityManager::GetComponent< MeshRenderer >(lhs);
      auto rmr  = ECS::EntityManager::GetComponent< MeshRenderer >(rhs);
      auto lmat = lmr->GetMaterial();
      auto rmat = rmr->GetMaterial();
      // If there is no material -> push at the end
      if (lmat == nullptr) {
        return false;
      }
      if (rmat == nullptr) {
        return true;
      }
      // Sort by the queue and then by the asset ID
      if (rmat->Queue() == rmat->Queue()) {
        return rmat->GetAssetID() < rmat->GetAssetID();
      }
      return rmat->Queue() < rmat->Queue();
    };
    // Actual sort
    std::sort(_entities.begin(), _entities.end(), OrderFunction);
    // What the fuck :)
    return;  // Retun the fuck out
    /*for (auto entityID : _entities) {
      auto meshRenderer = ECS::EntityManager::GetComponent< Components::MeshRenderer >(entityID);
      if (meshRenderer->IsDirty()) {
        _entitiesToSort.insert(entityID);
        meshRenderer->SetDirty(false);
      }
    }

    for (auto entityID : _entitiesToSort) {
      auto meshRenderer = ECS::EntityManager::GetComponent< Components::MeshRenderer >(entityID);
      auto material     = meshRenderer->GetMaterial();
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

    _entitiesToSort.clear();*/
  }
}  // namespace Engine::Systems