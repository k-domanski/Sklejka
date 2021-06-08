#include "pch.h"
#include "Renderer.h"
#include "Components/MeshRenderer.h"
#include "Components/Transform.h"
#include "ECS/EntityManager.h"
#include <App/Window.h>
#include <App/AssetManager.h>

#include "Components/Collider.h"
#include "GL/Cubemap.h"
#include <Systems/NodeSystem.h>
#include <Systems/Physics.h>
#include "Components/Animator.h"

namespace Engine::Systems {
  using namespace GL;
  using Engine::Renderer::PingPongBuffer;
  Renderer::Renderer() {
    AddSignature< Components::MeshRenderer >();
    AddSignature< Transform >();
    const auto size = Window::Get().GetScreenSize();

    /* Data */
    _transformUniformBuffer.BindToSlot(GL::UniformBlock::TransformData);
    /* -=-=- */

    /* Systems */
    _cameraSystem = ECS::EntityManager::GetInstance().GetSystem< CameraSystem >();
    _lightSystem  = ECS::EntityManager::GetInstance().GetSystem< LightSystem >();
    /* -=-=-=- */

    /* Skybox */
    _cube    = Engine::Renderer::Mesh::GetPrimitive(Engine::Renderer::MeshPrimitive::Cube);
    _cubemap = AssetManager::GetCubemap("./Assets/skyboxes/forest");
    //_cubemap      = AssetManager::GetCubemap("./Assets/skyboxes/blue_sky");
    _skyboxShader = AssetManager::GetShader("./shaders/skybox.glsl");
    _skyboxShader->SetValue("u_Skybox", (int)_skyboxSlot);
    /* -=-=-=- */

    /* Shadow Mapping */
    _shadowMapSize = glm::vec2(1024.0f, 1024.0f) * 3.0f;
    _shadowTarget  = std::make_shared< GL::RenderTarget >(_shadowMapSize.x, _shadowMapSize.y);

    _shadowTexture = std::make_shared< GL::TextureAttachment >(
        _shadowMapSize.x, _shadowMapSize.y, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
    _shadowTexture->WrappingMode(GL::TextureWrapping::ClampToBorder);
    _shadowTexture->BorderColor(glm::vec3(1.0f));

    _shadowTarget->AttachDepth(_shadowTexture);
    _shadowTarget->Bind(FramebufferTarget::ReadWrite);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    _shadowProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, -30.0f, 30.0f);
    _shadowUniformBuffer.BindToSlot(GL::UniformBlock::ShadowData);
    _shadowMapShader     = AssetManager::GetShader("./shaders/shadow_map.glsl");
    _shadowMapAnimShader = AssetManager::GetShader("./shaders/shadow_map_anim.glsl");
    /* -=-=-=--=-=-=- */

    /* Occlusion Culling */
    _depthTarget = std::make_shared< GL::RenderTarget >(size.x, size.y);
    auto depth   = std::make_shared< GL::Renderbuffer >(size.x, size.y);
    _depthTarget->AttachDepthStencil(depth);
    /* -=-=-=-=-=-=-=-=- */

    /* Post Processing */
    _quad            = Engine::Renderer::Mesh::GetPrimitive(Engine::Renderer::MeshPrimitive::Plane);
    _pingPongBuffer  = std::make_shared< PingPongBuffer >(size);
    _blurShader      = AssetManager::GetShader("./shaders/blur.glsl");
    _fishEyeShader   = AssetManager::GetShader("./shaders/fish_eye.glsl");
    _finalPassShader = AssetManager::GetShader("./shaders/final_pass.glsl");
    /* -=-=-=-=-=-=-=- */

    /* DEBUG */
    _debugMaterial = AssetManager::GetMaterial("./materials/_error_.mat");
    if (_debugMaterial == nullptr) {
      LOG_ERROR("Failed to load debug material: {}", "./materials/_error_.mat");
    }
    _boxCollider =
        Engine::Renderer::Mesh::GetPrimitive(Engine::Renderer::MeshPrimitive::WireframeBox);
    _sphereCollider =
        Engine::Renderer::Mesh::GetPrimitive(Engine::Renderer::MeshPrimitive::WireframeSphere);
    _boxColliderShader    = AssetManager::GetShader("./shaders/color.glsl");
    _sphereColliderShader = AssetManager::GetShader("./shaders/color.glsl");
    /* -=-=- */
  }

  void Renderer::Update(float deltaTime) {
    using Engine::Components::MeshRenderer;
    using Engine::ECS::Entity;
    using Engine::Renderer::Material;
    using Engine::Renderer::Mesh;
    // Hack: for now here to repair bug, consider OnStart in systems

    // int i = 0;
    // int j = 0;

    const auto camera = _cameraSystem->MainCamera();
    if (camera == nullptr) {
      return;
    }
    const auto camera_tr = ECS::EntityManager::GetComponent< Transform >(camera->GetEntity());
    /* CULLING */
    if (false) {
      SortByDistance(camera);
      _depthTarget->Bind(FramebufferTarget::ReadWrite);
      GL::Context::ClearBuffers(GL::BufferBit::Depth);
      GL::Context::DepthTest(true);
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glStencilMask(GL_FALSE);
      // glDepthMask(GL_FALSE);  //----------------------
      // calculate frustum culling and occlusion
      _debugMaterial->Use();
      auto debugShader = _debugMaterial->GetShader();
      for (auto& entity : _entities) {
        auto mesh_renderer = ECS::EntityManager::GetComponent< MeshRenderer >(entity);
        auto model         = mesh_renderer->GetModel();
        std::shared_ptr< Mesh > mesh;
        // If there is no mesh, there is nothing to show
        if (model == nullptr || (mesh = model->GetRootMesh()) == nullptr) {
          continue;
        }

        const auto transform = ECS::EntityManager::GetComponent< Transform >(entity);

        _transformUniformData.model     = transform->GetWorldMatrix();
        _transformUniformData.modelView = camera->ViewMatrix() * _transformUniformData.model;
        _transformUniformData.modelViewProjection =
            camera->ProjectionMatrix() * _transformUniformData.modelView;

        // frustum culling
        CalculateFrustrum(_transformUniformData.modelViewProjection);
        auto sphere = mesh->GetBoundingSphere();
        if (!SphereInFrustum(sphere.first, sphere.second)) {
          // i++;
          continue;
        }

        auto& query = mesh->GetQuery();
        // auto lastFrameRes = query.AnySamplesPassed();

        query.Start();

        auto boundingMesh = mesh->GetBoundingBox();
        boundingMesh->Use();
        _transformUniformBuffer.SetData(_transformUniformData);

        glDrawElements(boundingMesh->GetPrimitive(), boundingMesh->ElementCount(), GL_UNSIGNED_INT,
                       NULL);

        /*if (lastFrameRes) {
          glDepthMask(GL_TRUE);
          _visibleEntities.push_back(entityID);
          mesh->Use();
          _transformUniformBuffer.SetData(_transformUniformData);



          glDrawElements(mesh->GetPrimitive(), mesh->ElementCount(), GL_UNSIGNED_INT, NULL);
        } else {
          glDepthMask(GL_FALSE);
          auto boundingMesh = model->GetBoundingBox();
          boundingMesh->Use();
          _transformUniformBuffer.SetData(_transformUniformData);



          glDrawElements(boundingMesh->GetPrimitive(), boundingMesh->ElementCount(),
        GL_UNSIGNED_INT, NULL); j++;
        }*/

        query.End();

        auto res = query.AnySamplesPassed();
        if (res)
          _visibleEntities.push_back(entity);
        // else
        // j++;
      }
      glDepthMask(GL_TRUE);
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      glStencilMask(GL_TRUE);
    }
    SortByMaterial();

    /* Shadows */
    GL::Context::DepthTest(true);
    if (_lightSystem != nullptr) {
      auto light = _lightSystem->Light();
      if (light != nullptr && light->flags.GetAny(LightFlag::Dirty | LightFlag::NewData)) {
        auto light_tr = ECS::EntityManager::GetComponent< Transform >(light->GetEntity());
        if (light_tr->flags.Get(TransformFlag::NewData)) {
          const auto& fr  = light_tr->Forward();
          const auto& pos = camera_tr->Position() + camera_tr->Forward() * 5.0f;
          _shadowUniformData.lightSpaceMatrix =
              _shadowProjection * glm::lookAt(pos, pos + fr, {0.0f, 1.0f, 0.0f});
          _shadowUniformBuffer.SetData(_shadowUniformData);
        }
        _shadowTarget->Bind(FramebufferTarget::ReadWrite);
        GL::Context::Viewport(0, 0, _shadowMapSize.x, _shadowMapSize.y);
        GL::Context::ClearBuffers(BufferBit::Depth);
        GL::Context::FaceCulling(true);
        GL::Context::CullFace(Face::Front);

        _shadowMapShader->Use();

        for (auto& entity : _entities) {
          auto mesh_renderer = ECS::EntityManager::GetComponent< MeshRenderer >(entity);
          auto has_animator  = entity->HasComponent< Animator >();
          if (has_animator) {
            _shadowMapAnimShader->Use();
          }
          auto material = mesh_renderer->GetMaterial();
          auto model    = mesh_renderer->GetModel();
          if (material == nullptr || model == nullptr) {
            continue;
          }
          auto mesh = model->GetMesh(mesh_renderer->MeshIndex());
          mesh->Use();
          const auto transform        = ECS::EntityManager::GetComponent< Transform >(entity);
          _transformUniformData.model = transform->GetWorldMatrix();
          _transformUniformBuffer.SetData(_transformUniformData);
          glDrawElements(mesh->GetPrimitive(), mesh->ElementCount(), GL_UNSIGNED_INT, NULL);
          if (has_animator) {
            _shadowMapShader->Use();
          }
        }
        /*GL::Context::CullFace(Face::Back);
        auto entityID      = _playerShadowTarget->GetTargetID();
        auto trans         = ECS::EntityManager::GetComponent< Transform >(entityID);
        auto mesh_renderer = ECS::EntityManager::GetComponent< MeshRenderer >(entityID);
        auto material      = mesh_renderer->GetMaterial();
        auto model         = mesh_renderer->GetModel();
        if (material != nullptr && model != nullptr) {
          auto& query = model->GetQuery();
          query.Start(GL_SAMPLES_PASSED);
          auto mesh = model->GetMesh(mesh_renderer->MeshIndex());
          mesh->Use();
          const auto transform        = ECS::EntityManager::GetComponent< Transform >(entityID);
          _transformUniformData.model = transform->GetWorldMatrix();
          _transformUniformBuffer.SetData(_transformUniformData);
          glDrawElements(mesh->GetPrimitive(), mesh->ElementCount(), GL_UNSIGNED_INT, NULL);

          query.End();
          auto res = query.SamplesPassed();

          _playerShadowTarget->SamplesPassed(res);
        }*/

        const auto window_size = Window::Get().GetScreenSize();
        GL::Context::Viewport(0, 0, window_size.x, window_size.y);
        GL::Context::CullFace(Face::Back);
      }
    }

#if defined(_DEBUG)
    // Debug - remove later
    if (false) {
      GL::Context::BindFramebuffer(FramebufferTarget::ReadWrite, 0);
      _quad->Use();
      GL::Context::DepthTest(false);
      auto draw_depth = AssetManager::GetShader("./shaders/depth_draw.glsl");
      draw_depth->Use();
      draw_depth->SetValue("u_ShadowDepthTexture", (int)_shadowMapSlot);
      _shadowTexture->Bind(_shadowMapSlot);
      glDrawElements(_quad->GetPrimitive(), _quad->ElementCount(), GL_UNSIGNED_INT, NULL);
      GL::Context::DepthTest(true);
      return;
    }
#endif

    // Geometry
    _pingPongBuffer->Bind(FramebufferTarget::ReadWrite);
    GL::Context::ClearBuffers(GL::BufferBit::Color | GL::BufferBit::Depth);
    GL::Context::DepthTest(true);
    _shadowTexture->Bind(_shadowMapSlot);
    // for (auto& entityID : _visibleEntities) {
    for (auto& entityID : _entities) {
      auto mesh_renderer = ECS::EntityManager::GetComponent< MeshRenderer >(entityID);
      auto material      = mesh_renderer->GetMaterial();
      auto model         = mesh_renderer->GetModel();
      std::shared_ptr< Mesh > mesh;
      // If there is no mesh, there is nothing to show
      if (model == nullptr || (mesh = model->GetMesh(mesh_renderer->MeshIndex())) == nullptr) {
        continue;
      }

      // If no material -> Use debug
      if (material == nullptr || material->GetShader() == nullptr) {
        material = _debugMaterial;
      }
      auto shader          = material->GetShader();
      const auto transform = ECS::EntityManager::GetComponent< Transform >(entityID);

      _transformUniformData.model     = transform->GetWorldMatrix();
      _transformUniformData.modelView = camera->ViewMatrix() * _transformUniformData.model;
      _transformUniformData.modelViewProjection =
          camera->ProjectionMatrix() * _transformUniformData.modelView;

      // frustum culling
      CalculateFrustrum(_transformUniformData.modelViewProjection);
      auto sphere = mesh->GetBoundingSphere();
      if (!SphereInFrustum(sphere.first, sphere.second)) {
        // i++;
        continue;
      }

      material->Use();
      mesh->Use();

      // HACK: Assume for now that under slot 1 is camera uniform buffer
      // TODO: Get the actual slot number from somewhere, somehow :)
      _transformUniformBuffer.SetData(_transformUniformData);

      shader->SetValue("u_ShadowDepthTexture", (int)_shadowMapSlot);
      glDrawElements(mesh->GetPrimitive(), mesh->ElementCount(), GL_UNSIGNED_INT, NULL);
    }

    GL::Context::CullFace(GL::Face::Front);
    DrawSkybox();
    GL::Context::CullFace(GL::Face::Back);

#if defined(_DEBUG)
    DrawNodes();
    DrawColliders();
#endif

    PostProcessing();
    _visibleEntities.clear();
  }

  auto Renderer::OnWindowResize(glm::vec2 windowSize) -> void {
    using namespace GL;
    auto size       = windowSize;
    _pingPongBuffer = std::make_shared< PingPongBuffer >(size);
    _depthTarget    = std::make_shared< GL::RenderTarget >(size.x, size.y);
    auto depth      = std::make_shared< GL::Renderbuffer >(size.x, size.y);
    _depthTarget->AttachDepthStencil(depth);
    /*_renderTarget = std::make_shared< RenderTarget >(size.x, size.y);
    _screenTexture =
        std::make_shared< TextureAttachment >(size.x, size.y, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
    _renderTarget->AttachColor(0, _screenTexture);
    _renderTarget->AttachDepthStencil(std::make_shared< Renderbuffer >(size.x, size.y));*/
  }

  auto Renderer::ObjectInShadow(const std::shared_ptr< ECS::Entity >& entity) -> GLint {
    const auto camera = _cameraSystem->MainCamera();
    if (camera == nullptr) {
      return 0;
    }
    const auto camera_tr = ECS::EntityManager::GetComponent< Transform >(camera->GetEntity());
    GL::Context::DepthTest(true);
    if (_lightSystem != nullptr) {
      auto light = _lightSystem->Light();
      if (light != nullptr && light->flags.GetAny(LightFlag::Dirty | LightFlag::NewData)) {
        auto light_tr = ECS::EntityManager::GetComponent< Transform >(light->GetEntity());
        if (light_tr->flags.Get(TransformFlag::NewData)) {
          const auto& fr  = light_tr->Forward();
          const auto& pos = camera_tr->Position() + camera_tr->Forward() * 5.0f;
          _shadowUniformData.lightSpaceMatrix =
              _shadowProjection * glm::lookAt(pos, pos + fr, {0.0f, 1.0f, 0.0f});
          _shadowUniformBuffer.SetData(_shadowUniformData);
        }
        _shadowTarget->Bind(FramebufferTarget::ReadWrite);
        GL::Context::Viewport(0, 0, _shadowMapSize.x, _shadowMapSize.y);
        // GL::Context::ClearBuffers(BufferBit::Depth);
        GL::Context::FaceCulling(true);
        GL::Context::CullFace(Face::Front);
        _shadowMapShader->Use();

        GL::Context::CullFace(Face::Back);
        // auto entityID      = _playerShadowTarget->GetTargetID();
        auto trans         = ECS::EntityManager::GetComponent< Transform >(entity);
        auto mesh_renderer = ECS::EntityManager::GetComponent< Components::MeshRenderer >(entity);
        auto material      = mesh_renderer->GetMaterial();
        auto model         = mesh_renderer->GetModel();
        if (material != nullptr && model != nullptr) {
          auto mesh   = model->GetMesh(mesh_renderer->MeshIndex());
          auto& query = mesh->GetQuery();
          query.Start(GL_SAMPLES_PASSED);
          mesh->Use();
          const auto transform        = ECS::EntityManager::GetComponent< Transform >(entity);
          _transformUniformData.model = transform->GetWorldMatrix();
          _transformUniformBuffer.SetData(_transformUniformData);
          glDrawElements(mesh->GetPrimitive(), mesh->ElementCount(), GL_UNSIGNED_INT, NULL);

          query.End();
          auto res = query.SamplesPassed();

          //_playerShadowTarget->SamplesPassed(res);
          return res;
        }
        return 0;
      }
    }
  }

  /*auto Renderer::SetShadowChecker(std::shared_ptr< ShadowTarget > target) -> void {
    _playerShadowTarget = target;
  }*/

  auto Renderer::SortByMaterial() -> void {
    using Engine::Components::MeshRenderer;
    using Engine::ECS::EntityID;
    using Engine::Renderer::Material;
    const auto OrderFunction = [](const std::shared_ptr< ECS::Entity >& lhs,
                                  const std::shared_ptr< ECS::Entity >& rhs) {
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
    std::sort(_visibleEntities.begin(), _visibleEntities.end(), OrderFunction);
    return;
  }

  auto Renderer::SortByDistance(std::shared_ptr< Camera > cam) -> void {
    using Engine::Components::MeshRenderer;
    using Engine::ECS::EntityID;
    using Engine::Renderer::Material;

    auto camTrans = ECS::EntityManager::GetComponent< Transform >(cam->GetEntity());
    auto camPos   = camTrans->WorldPosition();

    const auto OrderFunction = [camPos](const std::shared_ptr< ECS::Entity >& lhs,
                                        const std::shared_ptr< ECS::Entity >& rhs) {
      auto ltr  = ECS::EntityManager::GetComponent< Transform >(lhs);
      auto rtr  = ECS::EntityManager::GetComponent< Transform >(rhs);
      auto lpos = ltr->WorldPosition();
      auto rpos = rtr->WorldPosition();
      auto ldis = glm::distance2(camPos, lpos);
      auto rdis = glm::distance2(camPos, rpos);
      return ldis < rdis;
    };
    // Actual sort
    std::sort(_entities.begin(), _entities.end(), OrderFunction);
  }

  auto Renderer::CalculateFrustrum(glm::mat4 clip) -> void {
    // auto clip = proj * modelView;
    float t;
    // right
    _frustum[0][0] = clip[0][3] - clip[0][0];
    _frustum[0][1] = clip[1][3] - clip[1][0];
    _frustum[0][2] = clip[2][3] - clip[2][0];
    _frustum[0][3] = clip[3][3] - clip[3][0];
    t              = sqrt(_frustum[0][0] * _frustum[0][0] + _frustum[0][1] * _frustum[0][1]
             + _frustum[0][2] * _frustum[0][2]);
    _frustum[0][0] /= t;
    _frustum[0][1] /= t;
    _frustum[0][2] /= t;
    _frustum[0][3] /= t;
    // left
    _frustum[1][0] = clip[0][3] + clip[0][0];
    _frustum[1][1] = clip[1][3] + clip[1][0];
    _frustum[1][2] = clip[2][3] + clip[2][0];
    _frustum[1][3] = clip[3][3] + clip[3][0];
    t              = sqrt(_frustum[1][0] * _frustum[1][0] + _frustum[1][1] * _frustum[1][1]
             + _frustum[1][2] * _frustum[1][2]);
    _frustum[1][0] /= t;
    _frustum[1][1] /= t;
    _frustum[1][2] /= t;
    _frustum[1][3] /= t;
    // bottom
    _frustum[2][0] = clip[0][3] + clip[0][1];
    _frustum[2][1] = clip[1][3] + clip[1][1];
    _frustum[2][2] = clip[2][3] + clip[2][1];
    _frustum[2][3] = clip[3][3] + clip[3][1];
    t              = sqrt(_frustum[2][0] * _frustum[2][0] + _frustum[2][1] * _frustum[2][1]
             + _frustum[2][2] * _frustum[2][2]);
    _frustum[2][0] /= t;
    _frustum[2][1] /= t;
    _frustum[2][2] /= t;
    _frustum[2][3] /= t;
    // top
    _frustum[3][0] = clip[0][3] - clip[0][1];
    _frustum[3][1] = clip[1][3] - clip[1][1];
    _frustum[3][2] = clip[2][3] - clip[2][1];
    _frustum[3][3] = clip[3][3] - clip[3][1];
    t              = sqrt(_frustum[3][0] * _frustum[3][0] + _frustum[3][1] * _frustum[3][1]
             + _frustum[3][2] * _frustum[3][2]);
    _frustum[3][0] /= t;
    _frustum[3][1] /= t;
    _frustum[3][2] /= t;
    _frustum[3][3] /= t;
    // far
    _frustum[4][0] = clip[0][3] - clip[0][2];
    _frustum[4][1] = clip[1][3] - clip[1][2];
    _frustum[4][2] = clip[2][3] - clip[2][2];
    _frustum[4][3] = clip[3][3] - clip[3][2];
    t              = sqrt(_frustum[4][0] * _frustum[4][0] + _frustum[4][1] * _frustum[4][1]
             + _frustum[4][2] * _frustum[4][2]);
    _frustum[4][0] /= t;
    _frustum[4][1] /= t;
    _frustum[4][2] /= t;
    _frustum[4][3] /= t;
    // near
    _frustum[5][0] = clip[0][3] + clip[0][2];
    _frustum[5][1] = clip[1][3] + clip[1][2];
    _frustum[5][2] = clip[2][3] + clip[2][2];
    _frustum[5][3] = clip[3][3] + clip[3][2];
    t              = sqrt(_frustum[5][0] * _frustum[5][0] + _frustum[5][1] * _frustum[5][1]
             + _frustum[5][2] * _frustum[5][2]);
    _frustum[5][0] /= t;
    _frustum[5][1] /= t;
    _frustum[5][2] /= t;
    _frustum[5][3] /= t;
  }

  auto Renderer::SphereInFrustum(glm::vec3 center, float radius) -> bool {
    for (int i = 0; i < 6; ++i) {
      if (_frustum[i][0] * center.x + _frustum[i][1] * center.y + _frustum[i][2] * center.z
              + _frustum[i][3]
          <= -radius)
        return false;
    }
    return true;
  }

  auto Renderer::PostProcessing() -> void {
    /* Context Setup */
    GL::Context::DepthTest(false);
    _quad->Use();

    /* Fish Eye */
    if (true) {
      _fishEyeShader->Use();
      _fishEyeShader->SetValue("u_MainTexture", 0);
      _pingPongBuffer->Swap();
      _pingPongBuffer->BackTexture()->Bind(0);
      glDrawElements(_quad->GetPrimitive(), _quad->ElementCount(), GL_UNSIGNED_INT, NULL);
    }

    /* Blur */
    if (false) {
      _blurShader->Use();
      _blurShader->SetValue("u_MainTexture", 0);
      // Horizontal
      _pingPongBuffer->Swap();
      _pingPongBuffer->BackTexture()->Bind(0);
      _blurShader->SetValue("u_Horizontal", 1);
      glDrawElements(_quad->GetPrimitive(), _quad->ElementCount(), GL_UNSIGNED_INT, NULL);

      // Vertical
      _pingPongBuffer->Swap();
      _pingPongBuffer->BackTexture()->Bind(0);
      _blurShader->SetValue("u_Horizontal", 0);
      glDrawElements(_quad->GetPrimitive(), _quad->ElementCount(), GL_UNSIGNED_INT, NULL);
    }

    /* Final Pass */
    _pingPongBuffer->FrontTexture()->Bind(0);
    GL::Context::BindFramebuffer(FramebufferTarget::ReadWrite, 0);
    GL::Context::ClearBuffers(GL::BufferBit::Color);
    _finalPassShader->Use();
    _finalPassShader->SetValue("u_MainTexture", 0);
    glDrawElements(_quad->GetPrimitive(), _quad->ElementCount(), GL_UNSIGNED_INT, NULL);
  }

  auto Renderer::DrawSkybox() -> void {
    glDepthFunc(GL_LEQUAL);

    _cubemap->Bind(_skyboxSlot);
    _cube->Use();
    _skyboxShader->Use();
    glDrawElements(_cube->GetPrimitive(), _cube->ElementCount(), GL_UNSIGNED_INT, NULL);

    glDepthFunc(GL_LESS);
  }

/* Debug draw calls */
#if defined(_DEBUG)
  auto Renderer::DrawColliders() -> void {
    const auto& camera = _cameraSystem->MainCamera();
    const auto& all_entities =
        ECS::EntityManager::GetInstance().GetSystem< Systems::Physics >()->Entities();
    for (auto& entityID : all_entities) {
      auto transform = entityID->GetComponent< Transform >();
      if (transform == nullptr) {
        continue;
      }
      std::shared_ptr< Components::Collider > collider;
      if ((collider = ECS::EntityManager::GetComponent< Components::Collider >(entityID))
          != nullptr) {
        if (collider->Type == +Components::ColliderType::Box) {
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

          _boxColliderShader->SetVector("u_Color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
          glDrawElements(_boxCollider->GetPrimitive(), _boxCollider->ElementCount(),
                         GL_UNSIGNED_INT, NULL);
        } else if (collider->Type == +Components::ColliderType::Sphere) {
          _sphereColliderShader->Use();
          _sphereCollider->Use();

          glEnable(GL_PRIMITIVE_RESTART);
          glPrimitiveRestartIndex(2137);

          auto colModel = transform->GetWorldMatrix();

          colModel = glm::translate(colModel, collider->Center);
          colModel = glm::scale(colModel, glm::vec3(collider->Size.x));

          _transformUniformData.model     = colModel;
          _transformUniformData.modelView = camera->ViewMatrix() * _transformUniformData.model;
          _transformUniformData.modelViewProjection =
              camera->ProjectionMatrix() * _transformUniformData.modelView;

          // HACK: Assume for now that under slot 1 is camera uniform buffer
          // TODO: Get the actual slot number from somewhere, somehow :)
          _transformUniformBuffer.SetData(_transformUniformData);

          _sphereColliderShader->SetVector("u_Color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
          glDrawElements(_sphereCollider->GetPrimitive(), _sphereCollider->ElementCount(),
                         GL_UNSIGNED_INT, NULL);

          glDisable(GL_PRIMITIVE_RESTART);
        }
      }
    }
  }
  auto Renderer::DrawNodes() -> void {
    const auto node_system = ECS::EntityManager::GetInstance().GetSystem< Engine::NodeSystem >();
    const auto cube        = AssetManager::GetModel(Engine::Renderer::ModelPrimitive::Cube);
    const auto camera      = _cameraSystem->MainCamera();

    if (!cube) {
      return;
    }
    const auto mesh = cube->GetRootMesh();
    if (!(node_system && camera && mesh)) {
      return;
    }

    mesh->Use();
    _debugMaterial->Use();
    for (auto entity : node_system->Entities()) {
      const auto& transform = ECS::EntityManager::GetComponent< Transform >(entity);
      transform->Scale(glm::vec3(0.1f));
      _transformUniformData.model     = transform->GetWorldMatrix();
      _transformUniformData.modelView = camera->ViewMatrix() * _transformUniformData.model;
      _transformUniformData.modelViewProjection =
          camera->ProjectionMatrix() * _transformUniformData.modelView;
      _transformUniformBuffer.SetData(_transformUniformData);
      glDrawElements(mesh->GetPrimitive(), mesh->ElementCount(), GL_UNSIGNED_INT, NULL);
    }
  }
#endif
}  // namespace Engine::Systems