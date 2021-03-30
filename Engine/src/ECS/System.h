#pragma once
#include "Component.h"
#include "Types.h"
//#include "pch.h"

namespace ECS {
  class System {
  public:
    System()          = default;
    virtual ~System() = default;

    virtual void Awake() = 0;
    virtual void Update() = 0;
    virtual void Destroy() = 0;
    virtual SystemTypeID GetType();

    virtual std::shared_ptr< ComponentsSignatures > GetSignatures();
    template<typename T>
    void AddSignature() const;
    template<typename T>
    void RemoveSignature() const;
    

    template<typename T>
    std::shared_ptr< Component > AddComponent();
    void AddComponent(std::shared_ptr< Component > component);
    void RemoveComponent(std::shared_ptr<Component> component);

  private:
    SystemTypeID _typeID;
    std::shared_ptr< ComponentsSignatures > _componentsSignatures;
    std::vector< std::shared_ptr< Component > > _components;
  };

}  // namespace ECS
