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
    std::unordered_map< ECS::EntityID, std::vector< std::shared_ptr< ECS::Entity > > >
        _parentChildMap;
    std::unordered_map< ECS::EntityID, std::shared_ptr< ECS::Entity > > _childParentMap;

  public:
    SceneGraph();
    auto Update(float deltaTime) -> void override;
    auto SetParent(const std::shared_ptr< ECS::Entity >& child,
                   const std::shared_ptr< ECS::Entity >& parent) -> void;
    auto AddChild(const std::shared_ptr< ECS::Entity >& parent,
                  const std::shared_ptr< ECS::Entity >& child) -> void;
    auto DetachFromGraph(const std::shared_ptr< ECS::Entity >& entity) -> void;

    /*Editor Gui*/
    auto GetRootID() -> ECS::EntityID;
    auto GetChildren(const std::shared_ptr< ECS::Entity >& entity)
        -> std::vector< std::shared_ptr< ECS::Entity > >&;
    auto GetParent(const std::shared_ptr< ECS::Entity >& entity) -> std::shared_ptr< ECS::Entity >;

  private:
    auto RecursiveUpdateParentAndChildren(std::shared_ptr< Transform >& transform,
                                          std::shared_ptr< Transform >& parent) -> void;
  };

}  // namespace Engine::Systems