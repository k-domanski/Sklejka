#include <pch.h>
#include "Types.h"

namespace Engine::ECS {
  ComponentTypeID GetRuntimeComponentTypeID() {
    static ComponentTypeID typeID = 0u;
    return typeID++;
  }
  SystemTypeID GetRuntimeSystemTypeID() {
    static SystemTypeID typeID = 0u;
    return typeID++;
  }
}  // namespace Engine::ECS