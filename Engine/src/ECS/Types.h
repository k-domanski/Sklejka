#pragma once
namespace Engine::ECS {
  typedef std::size_t ComponentTypeID;
  typedef std::size_t SystemTypeID;
  typedef std::size_t EntityID;
  typedef std::set< ComponentTypeID > EntitySignature;
  typedef std::set< ComponentTypeID > SystemSignature;

  static const ComponentTypeID GetRuntimeComponentTypeID() {
    static ComponentTypeID typeID = 0u;
    return typeID++;
  }

  static const SystemTypeID GetRuntimeSystemTypeID() {
    static SystemTypeID typeID = 0u;
    return typeID++;
  }

  template< typename T >
  static const ComponentTypeID GetComponentTypeID() {
    static_assert((std::is_base_of< Component, T >::value && !std::is_same< Component, T >::value),
                  "INVALID TEMPLATE TYPE");
    static const ComponentTypeID typeID = GetRuntimeComponentTypeID();
    return typeID;
  }

  template< typename T >
  static const ComponentTypeID GetSystemTypeID() {
    static_assert((std::is_base_of< System, T >::value && !std::is_same< System, T >::value),
                  "INVALID TEMPLATE TYPE");
    static const SystemTypeID typeID = GetRuntimeSystemTypeID();
    return typeID;
  }
}  // namespace ECS
