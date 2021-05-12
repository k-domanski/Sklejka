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
    /*
      On OpenGL side vec3 and vec4 have 16 byte alignment.
      This means that data for these types have to begin at addresses (offsets)
        that are a multiple of 16: 0x0, 0x16, 0x32 ...
      On C++ side, vec3 is 3 * float (4 bytes) = 12 bytes.
      So consecutive vec3s have addresses (offsets): 0x0, 0x12, 0x24, 0x32...
      We have to add padding in the struct
    */
    glm::vec3 direction{0};  // Data 12 bytes
    float __pad1[1]{0};      // Padding 4 bytes
                             // Total 16 bytes

    glm::vec3 ambient{0};  // Data 12 bytes
    float __pad2[1]{0};    // Padding 4 byte
                           // Total 16 bytes

    glm::vec3 diffuse{0};  // Data 12 bytes
    float __pad3[1]{0};    // Padding 4 bytes
                           // Total 16 bytes

    glm::vec3 specular{0};  // Data 12 bytes
    /*
      Now for the intensity: 1 float has alighment of 4 bytes and has to be on addresses
        that are multipleof 4: 0x0, 0x4, 0x8, ...
      Since previous data type was vec3 witch has 12 bytes,
        the intensity is already aligned - no padding.
    */
    float intensity{0};   // Data 4 bytes
    float shadowBias{0};  // Data 4 bytes
  };
  struct ShadowUniformData {
    glm::mat4 lightSpaceMatrix;
  };
}  // namespace Engine::GL