#pragma once

#include "Entity.h"

namespace ECS
{
  class EntityManager
  {
  public:
    EntityManager()  = default;
    ~EntityManager() = default;

    std::shared_ptr<Entity> CreateEntity();

  private:
    std::set< std::shared_ptr<Entity>, std::greater<> > _entities;
    
  };
}

