#include "pch.h"
#include "Physics.h"

//#include "../../GJK.h"
//#include "Components/Collider.h"
//#include "Components/Transform.h"
#include "ECS/EntityManager.h"

void Engine::Systems::Physics::Update(float deltaTime) {
  for (auto entityId : _entities) {
    auto collider =
        ECS::EntityManager::GetInstance().GetComponent< Components::Collider >(entityId);
    auto rigidbody =
        ECS::EntityManager::GetInstance().GetComponent< Components::Rigidbody >(entityId);
    auto transform = ECS::EntityManager::GetInstance().GetComponent< Transform >(entityId);

    if (!rigidbody->IsKinematic())
      continue;
    for (auto entity : _entities) {
      if (entity == entityId)
        continue;

      auto collider2 =
          ECS::EntityManager::GetInstance().GetComponent< Components::Collider >(entity);
      auto rigidbody2 =
          ECS::EntityManager::GetInstance().GetComponent< Components::Rigidbody >(entity);
      auto transform2 = ECS::EntityManager::GetInstance().GetComponent< Transform >(entity);

      if (CheckCollision(collider, transform, collider2, transform2)) {
        std::cout << "Colliding" << std::endl;
        _collisions[entityId].push_back(entity);
      }
    }
  }

  _collisions.clear();
}

auto Engine::Systems::Physics::CheckCollision(std::shared_ptr< Components::Collider > c1,
                                              std::shared_ptr< Transform > t1,
                                              std::shared_ptr< Components::Collider > c2,
                                              std::shared_ptr< Transform > t2) -> bool {
  Utility::GJK::Shape s1, s2;
  if (c1->Type == Components::Sphere)
    s1 = CreateSphereShape(c1, t1);
  else
    s1 = CreateBoxShape(c1, t1);

  if (c2->Type == Components::Sphere)
    s2 = CreateSphereShape(c2, t2);
  else
    s2 = CreateBoxShape(c2, t2);

  return Intersects(s1, s2);
}

auto Engine::Systems::Physics::CreateSphereShape(std::shared_ptr< Components::Collider > c,
                                                 std::shared_ptr< Transform > t)
    -> Utility::GJK::Shape {
  auto pos = t->WorldPosition() + c->Center;
  return Utility::GJK::Shape(true, c->Size.x, pos);
}

auto Engine::Systems::Physics::CreateBoxShape(std::shared_ptr< Components::Collider > c,
                                              std::shared_ptr< Transform > t)
    -> Utility::GJK::Shape {
  // Test this if works
  auto center4 = glm::vec4(c->Center, 1.0f) * t->GetWorldMatrix();

  auto center = make_vec3(center4);

  std::vector< glm::vec3 > vertices;

  float xSize = c->Size.x / 2.0f;
  float ySize = c->Size.y / 2.0f;
  float zSize = c->Size.z / 2.0f;

  vertices.push_back(glm::make_vec3(glm::vec4(-xSize, -ySize, -zSize, 1.0f) * t->GetWorldMatrix()));
  vertices.push_back(glm::make_vec3(glm::vec4(-xSize, ySize, -zSize, 1.0f) * t->GetWorldMatrix()));
  vertices.push_back(glm::make_vec3(glm::vec4(-xSize, ySize, zSize, 1.0f) * t->GetWorldMatrix()));
  vertices.push_back(glm::make_vec3(glm::vec4(-xSize, -ySize, zSize, 1.0f) * t->GetWorldMatrix()));
  vertices.push_back(glm::make_vec3(glm::vec4(xSize, -ySize, -zSize, 1.0f) * t->GetWorldMatrix()));
  vertices.push_back(glm::make_vec3(glm::vec4(xSize, ySize, -zSize, 1.0f) * t->GetWorldMatrix()));
  vertices.push_back(glm::make_vec3(glm::vec4(xSize, ySize, zSize, 1.0f) * t->GetWorldMatrix()));
  vertices.push_back(glm::make_vec3(glm::vec4(xSize, -ySize, zSize, 1.0f) * t->GetWorldMatrix()));

  return Utility::GJK::Shape(vertices, center);
}
