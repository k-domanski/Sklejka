#pragma once
#include <pch.h>
#include <GL/GLCore.h>
#include <nlohmann/json.hpp>

namespace Engine::Renderer {
  class Material {
  private:
    static inline std::size_t s_currentMaterial{0};

    std::size_t _assetID;
    std::string _filePath;
    uint32_t _queue{1000};
    std::shared_ptr< GL::Shader > _shader;
    std::shared_ptr< GL::Texture2D > _mainTexture;
    std::shared_ptr< GL::Texture2D > _roughnessMap;
    std::shared_ptr< GL::Texture2D > _metalnessMap;
    glm::vec4 _mainColor{1.0f, 1.0f, 1.0f, 1.0f};
    float _roughness = 0.0f;
    float _metalness = 0.0f;

    /* Stencil */
    bool _useStencil{false};
    uint8_t _stencilRef{0};
    GL::StencilFunc _stencilFunc{GL::StencilFunc::Always};
    GL::StencilOp _sfail{GL::StencilOp::Keep}, _zfail{GL::StencilOp::Keep},
        _zpass{GL::StencilOp::Keep};
    /* -=-=-=- */

  public:
    Material(std::size_t assetID);
    auto SetShader(const std::shared_ptr< GL::Shader >& shader) noexcept -> void;
    auto SetMainTexture(const std::shared_ptr< GL::Texture2D >& mainTexture) noexcept -> void;

    auto GetShader() -> std::shared_ptr< GL::Shader >;
    auto GetMainTexture() -> std::shared_ptr< GL::Texture2D >;
    auto RoughnessMap() const noexcept -> std::shared_ptr< GL::Texture2D >;
    auto RoughnessMap(const std::shared_ptr< GL::Texture2D >& map) noexcept
        -> std::shared_ptr< GL::Texture2D >;
    auto MetalnessMap() const noexcept -> std::shared_ptr< GL::Texture2D >;
    auto MetalnessMap(const std::shared_ptr< GL::Texture2D >& map) noexcept
        -> std::shared_ptr< GL::Texture2D >;
    auto MainColor() const noexcept -> glm::vec4;
    auto MainColor(const glm::vec4& color) noexcept -> glm::vec4;
    auto Roughness() const noexcept -> float;
    auto Roughness(float value) noexcept -> float;
    auto Metalness() const noexcept -> float;
    auto Metalness(float value) noexcept -> float;
    auto MainColorPtr() -> float*;
    auto Queue() const noexcept -> uint32_t;
    auto Queue(uint32_t queue) noexcept -> uint32_t;
    /* Stencil */
    auto UseStencil() const noexcept -> bool;
    auto UseStencil(bool value) noexcept -> bool;
    auto StencilRef() const noexcept -> uint8_t;
    auto StencilRef(uint8_t ref) noexcept -> uint8_t;
    auto StencilFunc() const noexcept -> GL::StencilFunc;
    auto StencilFunc(GL::StencilFunc func) noexcept -> GL::StencilFunc;
    auto StencilSFail() const noexcept -> GL::StencilOp;
    auto StencilSFail(GL::StencilOp op) noexcept -> GL::StencilOp;
    auto StencilZFail() const noexcept -> GL::StencilOp;
    auto StencilZFail(GL::StencilOp op) noexcept -> GL::StencilOp;
    auto StencilZPass() const noexcept -> GL::StencilOp;
    auto StencilZPass(GL::StencilOp op) noexcept -> GL::StencilOp;
    /* -=-=-=- */

    std::size_t GetAssetID();
    std::string ToJson();
    auto FilePath() const noexcept -> std::string;
    auto FilePath(const std::string& filePath) noexcept -> std::string;
    auto Use() noexcept -> void;
    auto SetShaderData() noexcept -> void;
  };
}  // namespace Engine::Renderer