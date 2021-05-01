#pragma once
#include <pch.h>
//#include <ECS/System.h>
//#include <ECS/ECS.h>
#include "ECS/EntityManager.h"
#include <Components/Transform.h>

namespace Engine::Systems {
  class SceneGraph : public ECS::System {
  private:
    ECS::EntityID _rootID{0};
    std::shared_ptr< ECS::Entity > _rootEntity;
    std::unordered_map< ECS::EntityID, std::vector< ECS::EntityID > > _parentChildMap;
    std::unordered_map< ECS::EntityID, ECS::EntityID > _childParentMap;

  public:
    SceneGraph();
    auto Update(float deltaTime) -> void override;
    auto SetParent(ECS::EntityID child, ECS::EntityID parent) -> void;
    auto AddChild(ECS::EntityID parent, ECS::EntityID child) -> void;
    auto DetachFromGraph(ECS::EntityID entityID) -> void;

    /*Editor Gui*/
    auto GetRootID() -> ECS::EntityID&;
    auto GetChildren(ECS::EntityID id) -> std::vector< ECS::EntityID >&;

  private:
    auto RecursiveUpdateParentAndChildren(std::shared_ptr< Transform >& transform,
                                          std::shared_ptr< Transform > parent) -> void;
  };

}  // namespace Engine::Systems