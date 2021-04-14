#include "pch.h"
#include "Physics.h"

#include "Components/Transform.h"
#include "ECS/EntityManager.h"

void Engine::Systems::Physics::Update(float deltaTime) {
  for (auto& entity : _entities) {
    auto collider =
        ECS::EntityManager::GetInstance().GetComponent< Components::BoxCollider >(entity);
    auto transform = ECS::EntityManager::GetInstance().GetComponent< Transform >(entity);
    auto rigidbody =
        ECS::EntityManager::GetInstance().GetComponent< Components::Rigidbody >(entity);

    if (rigidbody->UseGravity()) {
      auto velocity = rigidbody->GetVelocity();
      rigidbody->SetVelocity(velocity + (_gravity * deltaTime));
    }

    if (rigidbody->IsKinematic()) {
      auto velocity = rigidbody->GetVelocity();
      auto position = transform->Position();
      transform->Position(position + (velocity * deltaTime));
    }

    if (collider->IsTrigger())
      continue;

    auto firstPos  = transform->Position();
    auto firstSize = collider->GetSize();

    glm::vec3 firstMin(firstPos.x - firstSize.x, firstPos.y - firstSize.y,
                       firstPos.z - firstSize.z);
    glm::vec3 firstMax(firstPos.x + firstSize.x, firstPos.y + firstSize.y,
                       firstPos.z + firstSize.z);

    // detection
    for (auto entitySec : _entities) {
      if (entity != entitySec) {
        auto transformSec = ECS::EntityManager::GetInstance().GetComponent< Transform >(entitySec);
        auto colliderSec =
            ECS::EntityManager::GetInstance().GetComponent< Components::BoxCollider >(entitySec);

        auto secPos  = transformSec->Position();
        auto secSize = colliderSec->GetSize();

        glm::vec3 secMin(secPos.x - secSize.x, secPos.y - secSize.y, secPos.z - secSize.z);

        glm::vec3 secMax(secPos.x + secSize.x, secPos.y + secSize.y, secPos.z + secSize.z);

        if ((firstMin.x <= secMax.x && firstMax.x >= secMin.x)
            && (firstMin.y <= secMax.y && firstMax.y >= secMin.y)
            && (firstMin.z <= secMax.z && firstMax.z >= secMin.z)) {
          _collisions[entity].push_back(entitySec);
          std::cout << "Colliding" << std::endl;
        }
      }
    }
  }
  // separation
  for (auto& [frst, vec] : _collisions) {
    for (auto& sec : vec) {
      auto collider =
          ECS::EntityManager::GetInstance().GetComponent< Components::BoxCollider >(frst);
      auto& transform = ECS::EntityManager::GetInstance().GetComponent< Transform >(frst);

      auto& firstPos  = transform->Position();
      auto firstSize = collider->GetSize();

      glm::vec3 firstMin(firstPos.x - firstSize.x, firstPos.y - firstSize.y,
                         firstPos.z - firstSize.z);
      glm::vec3 firstMax(firstPos.x + firstSize.x, firstPos.y + firstSize.y,
                         firstPos.z + firstSize.z);

      auto transformSec = ECS::EntityManager::GetInstance().GetComponent< Transform >(sec);
      auto colliderSec =
          ECS::EntityManager::GetInstance().GetComponent< Components::BoxCollider >(sec);

      auto secPos  = transformSec->Position();
      auto secSize = colliderSec->GetSize();

      glm::vec3 secMin(secPos.x - secSize.x, secPos.y - secSize.y, secPos.z - secSize.z);

      glm::vec3 secMax(secPos.x + secSize.x, secPos.y + secSize.y, secPos.z + secSize.z);

      float left   = firstMax.x - secMin.x;
      float right  = secMax.x - firstMin.x;
      float bottom = firstMax.y - secMin.y;
      float top    = secMax.y - firstMin.y;
      float back   = firstMax.z - secMin.z;
      float front  = secMax.z - firstMin.z;

      glm::vec3 separation(0.0f);

      separation.x = left < right ? -left : right;
      separation.y = bottom < top ? -bottom : top;
      separation.z = back < front ? -back : front;

      if (separation.x < separation.y) {
        separation.y = 0.0f;
        if (separation.x < separation.z)

          separation.z = 0.0f;
        else
          separation.x = 0.0f;
      } else {
        separation.x = 0.0f;
        if (separation.y < separation.z)
          separation.z = 0.0f;
        else
          separation.y = 0.0f;
      }

      separation = separation / 2.0f;

      if (separation.length() < 0.0001f)
          continue;

      std::cout << "before: " << transform->Position().x << std::endl;
      transform->Position(firstPos + separation);
      std::cout << "after: " << transform->Position().x << std::endl;
    }
  }

  _collisions.clear();
}
