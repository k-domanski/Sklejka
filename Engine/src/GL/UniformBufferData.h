#pragma once
#include <pch.h>

namespace Engine::GL {
  struct TransformUniformData {
    glm::mat4 model;                // 16 byte alignment
    glm::mat4 modelView;            // 16 byte alignment
    glm::mat4 modelViewProjection;  // 16 byte alignment
  };
  struct CameraUniformData {
    glm::mat4 view;            // 16 byte alignment
    glm::mat4 projection;      // 16 byte alignment
    glm::mat4 viewProjection;  // 16 byte alignment
  };
  struct DirectionalLightUniformData {
    glm::vec3 direction;  // 16 byte alignment
    glm::vec3 ambient;    // 16 byte alignment
    glm::vec3 diffuse;    // 16 byte alignment
    glm::vec3 specular;   // 16 byte alignment
    float intensity;      // 16 byte alignment
  };
}  // namespace Engine::GL