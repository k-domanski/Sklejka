#include <pch.h>
#include <Components/Camera.h>
#include <nlohmann/json.hpp>
#include <ECS/Types.h>

namespace Engine {
  Camera::Camera()
      : Component("Camera", ECS::GetComponentTypeID< Camera >()), _fov(45), _aspect(1.77),
        _nearPlane(0.001f), _farPlane(1000.0f) {
    flags.Set(CameraFlag::Dirty | CameraFlag::NewData);
  }
  Camera::Camera(float fov, float aspect, float nearPlane, float farPlane)
      : Component("Camera", ECS::GetComponentTypeID< Camera >()), _fov(fov), _aspect(aspect),
        _nearPlane(nearPlane), _farPlane(farPlane) {
    flags.Set(CameraFlag::Dirty | CameraFlag::NewData);
  }

  auto Camera::Fov() const noexcept -> float {
    return _fov;
  }

  auto Camera::Fov(float fov) noexcept -> float {
    flags.Set(CameraFlag::Dirty | CameraFlag::NewData);
    return _fov = fov;
  }

  auto Camera::Aspect() const noexcept -> float {
    return _aspect;
  }

  auto Camera::Aspect(float aspect) noexcept -> float {
    flags.Set(CameraFlag::Dirty | CameraFlag::NewData);
    return _aspect = aspect;
  }

  auto Camera::NearPlane() const noexcept -> float {
    return _nearPlane;
  }

  auto Camera::NearPlane(float nearPlane) noexcept -> float {
    flags.Set(CameraFlag::Dirty | CameraFlag::NewData);
    return _nearPlane = nearPlane;
  }

  auto Camera::FarPlane() const noexcept -> float {
    return _farPlane;
  }

  auto Camera::FarPlane(float farPlane) noexcept -> float {
    flags.Set(CameraFlag::Dirty | CameraFlag::NewData);
    return _farPlane = farPlane;
  }

  auto Camera::ViewMatrix() const noexcept -> glm::mat4 {
    return _matrices.view;
  }

  auto Camera::ViewMatrix(const glm::mat4& viewMatrix) noexcept -> glm::mat4 {
    flags.Set(CameraFlag::NewData);
    _matrices.viewProjection = _matrices.projection * viewMatrix;
    return _matrices.view    = viewMatrix;
  }

  auto Camera::ProjectionMatrix() const noexcept -> glm::mat4 {
    return _matrices.projection;
  }

  auto Camera::ProjectionMatrix(const glm::mat4& projectionMatrix) noexcept -> glm::mat4 {
    flags.Set(CameraFlag::NewData);
    _matrices.viewProjection    = projectionMatrix * _matrices.view;
    return _matrices.projection = projectionMatrix;
  }

  auto Camera::UniformData() const noexcept -> GL::CameraUniformData {
    return _matrices;
  }

  std::string Camera::SaveToJson() {
    nlohmann::json json = nlohmann::json{{"componentType", "camera"}, {"fov", _fov},
                                         {"aspect", _aspect},         {"nearPlane", _nearPlane},
                                         {"farPlane", _farPlane},     {"flags", flags.GetState()}};

    return json.dump(4);
  }

  std::string Camera::SaveToJson(std::string filePath) {
    std::ofstream ofstream;
    ofstream.open(filePath);
    ofstream << SaveToJson();
    ofstream.close();

    return SaveToJson();
  }

  auto Camera::LoadFromJson(std::string filePath) -> void {
    nlohmann::json json;
    if (filePath[0] == '{' || filePath[0] == '\n')  // HACK: Check if string is json
      json = nlohmann::json::parse(filePath.begin(), filePath.end());
    else {
      auto content = Utility::ReadTextFile(filePath);
      json         = nlohmann::json::parse(content.begin(), content.end());
    }

    Fov(json["fov"]);
    Aspect(json["aspect"]);
    NearPlane(json["nearPlane"]);
    FarPlane(json["farPlane"]);
    flags.Set(json["flags"]);
  }

  // auto Camera::GetViewMatrix() noexcept -> glm::mat4 {
  //  // TODO: Check if dirty
  //  _viewMatrix = glm::lookAt(transform.Position(), transform.Position() + transform.Forward(),
  //                            {0.0f, 1.0f, 0.0f});
  //  _viewMatrix = glm::lookAt(transform.Position(), transform.Position() + transform.Forward(),
  //                            {0.0f, 1.0f, 0.0f});
  //  return _viewMatrix;
  //}
  // auto Camera::GetProjectionMatrix() noexcept -> glm::mat4 {
  //  _projectionMatrix = glm::perspective(_fov, _aspect, _nearPlane, _farPlane);
  //  return _projectionMatrix;
  //}
}  // namespace Engine