#pragma once
#include <pch.h>
#include <ECS/ECS.h>
#include <Components/Camera.h>
#include <Components/Transform.h>
#include <GL/Buffer.h>
#include <GL/RenderTarget.h>
#include <Renderer/Mesh.h>

namespace Engine::Systems {
  class CameraSystem : public ECS::System {
  private:
    uint32_t _cameraUniformSlot{1u};
    std::shared_ptr< Camera > _mainCamera;
    GL::CameraUniformBuffer _cameraUniformBuffer;
    bool _editorView;
    /*std::shared_ptr< GL::RenderTarget > _renderTexture;
    std::shared_ptr< Renderer::Mesh > _screenQuad;*/

  public:
    CameraSystem();
    auto Update(float deltaTime) -> void override;
    auto MainCamera() const noexcept -> std::shared_ptr< Camera >;
    auto SwitchView() -> void;
    auto EditorView(bool enable) -> void;
    auto FindMainCamera() -> void;
  };
}  // namespace Engine::Systems