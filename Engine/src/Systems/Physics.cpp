#include "pch.h"
#include "Physics.h"

//#include "../../GJK.h"
//#include "Components/Collider.h"
//#include "Components/Transform.h"
#include "ECS/EntityManager.h"
#include "Components/NativeScript.h"

void Engine::Systems::Physics::Update(float deltaTime) {
  const auto ent_count = _entities.size();
  for (int i = 0; i < ent_count; ++i) {
    const auto& ent1       = _entities[i];
    const auto& collider1  = ECS::EntityManager::GetComponent< Components::Collider >(ent1);
    const auto& rigidbody1 = ECS::EntityManager::GetComponent< Components::Rigidbody >(ent1);
    const auto& transform1 = ECS::EntityManager::GetComponent< Transform >(ent1);

    if (!rigidbody1->IsKinematic()) {
      continue;
    }

    auto pos = transform1->Position();
    pos += rigidbody1->GetVelocity() * deltaTime;
    transform1->Position(pos);

    for (int j = i + 1; j < ent_count; ++j) {
      const auto& ent2       = _entities[j];
      const auto layer_check = ent1->collisionLayer.Get(ent2->layer.GetState());

      if (layer_check == 0) {
        continue;
      }
      const auto& transform2 = ECS::EntityManager::GetComponent< Transform >(ent2);

      if (glm::distance2(transform1->WorldPosition(), transform2->WorldPosition()) > 20.0f) {
        continue;
      }

      const auto& collider2  = ECS::EntityManager::GetComponent< Components::Collider >(ent2);
      const auto& rigidbody2 = ECS::EntityManager::GetComponent< Components::Rigidbody >(ent2);

      if (CheckCollision(collider1, transform1, collider2, transform2)) {
        // std::cout << "Colliding" << std::endl;
        //_collisions[ent1].push_back(ent2);
        if (!collider1->IsTrigger || !collider2->IsTrigger) {
          ResolveCollisions(collider1, transform1, !rigidbody1->IsKinematic(), collider2,
                            transform2, !rigidbody2->IsKinematic());
        }
        LOG_DEBUG("COLLISION DETECTED");
        using Engine::ECS::EntityManager;
        const auto& ns1 = EntityManager::GetComponent< NativeScript >(collider1->GetEntity());
        const auto& ns2 = EntityManager::GetComponent< NativeScript >(collider2->GetEntity());
        if (ns1) {
          ns1->OnCollisionEnter(collider1);
        }
        if (ns2) {
          ns2->OnCollisionEnter(collider1);
        }
      }
    }
  }

  //_collisions.clear();
}

auto Engine::Systems::Physics::CheckCollision(std::shared_ptr< Components::Collider > c1,
                                              std::shared_ptr< Transform > t1,
                                              std::shared_ptr< Components::Collider > c2,
                                              std::shared_ptr< Transform > t2) -> bool {
  Utility::GJK::Shape s1, s2;
  if (c1->Type == +Components::ColliderType::Sphere)
    s1 = CreateSphereShape(c1, t1);
  else
    s1 = CreateBoxShape(c1, t1);

  if (c2->Type == +Components::ColliderType::Sphere)
    s2 = CreateSphereShape(c2, t2);
  else
    s2 = CreateBoxShape(c2, t2);

  auto result = Intersects(s1, s2);

  c1->Simplex = result.second;
  // c2->Simplex = result.second;

  return result.first;
}

auto Engine::Systems::Physics::CreateSphereShape(std::shared_ptr< Components::Collider > c,
                                                 std::shared_ptr< Transform > t)
    -> Utility::GJK::Shape {
  auto center4 = t->GetWorldMatrix() * glm::vec4(c->Center, 1.0f);

  auto center = make_vec3(center4);

  auto radius = c->Size * t->WorldScale();

  return Utility::GJK::Shape(true, radius.x, center);
}

auto Engine::Systems::Physics::CreateBoxShape(std::shared_ptr< Components::Collider > c,
                                              std::shared_ptr< Transform > t)
    -> Utility::GJK::Shape {
  // Test this if works
  auto center4 = t->GetWorldMatrix() * glm::vec4(c->Center, 1.0f);

  auto center = make_vec3(center4);

  std::vector< glm::vec3 > vertices;

  float xSize = 0.5f;
  float ySize = 0.5f;
  float zSize = 0.5f;
  /*float xSize = c->Size.x / 2.0f;
  float ySize = c->Size.y / 2.0f;
  float zSize = c->Size.z / 2.0f;*/

  auto model = t->GetWorldMatrix();
  model      = glm::translate(model, c->Center);
  model      = glm::scale(model, c->Size);

  vertices.push_back(glm::make_vec3(model * glm::vec4(-xSize, -ySize, -zSize, 1.0f)));
  vertices.push_back(glm::make_vec3(model * glm::vec4(-xSize, ySize, -zSize, 1.0f)));
  vertices.push_back(glm::make_vec3(model * glm::vec4(-xSize, ySize, zSize, 1.0f)));
  vertices.push_back(glm::make_vec3(model * glm::vec4(-xSize, -ySize, zSize, 1.0f)));
  vertices.push_back(glm::make_vec3(model * glm::vec4(xSize, -ySize, -zSize, 1.0f)));
  vertices.push_back(glm::make_vec3(model * glm::vec4(xSize, ySize, -zSize, 1.0f)));
  vertices.push_back(glm::make_vec3(model * glm::vec4(xSize, ySize, zSize, 1.0f)));
  vertices.push_back(glm::make_vec3(model * glm::vec4(xSize, -ySize, zSize, 1.0f)));

  /*vertices.push_back(glm::make_vec3(t->GetWorldMatrix() * glm::vec4(-xSize, -ySize,
  -zSize, 1.0f))); vertices.push_back(glm::make_vec3(t->GetWorldMatrix() * glm::vec4(-xSize, ySize,
  -zSize, 1.0f))); vertices.push_back(glm::make_vec3(t->GetWorldMatrix() * glm::vec4(-xSize, ySize,
  zSize, 1.0f))); vertices.push_back(glm::make_vec3(t->GetWorldMatrix() * glm::vec4(-xSize, -ySize,
  zSize, 1.0f))); vertices.push_back(glm::make_vec3(t->GetWorldMatrix() * glm::vec4(xSize, -ySize,
  -zSize, 1.0f))); vertices.push_back(glm::make_vec3(t->GetWorldMatrix() * glm::vec4(xSize, ySize,
  -zSize, 1.0f))); vertices.push_back(glm::make_vec3(t->GetWorldMatrix() * glm::vec4(xSize, ySize,
  zSize, 1.0f))); vertices.push_back(glm::make_vec3(t->GetWorldMatrix() * glm::vec4(xSize, -ySize,
  zSize, 1.0f)));*/

  return Utility::GJK::Shape(vertices, center);
}

auto Engine::Systems::Physics::ResolveCollisions(std::shared_ptr< Components::Collider > c1,
                                                 std::shared_ptr< Transform > t1, bool static1,
                                                 std::shared_ptr< Components::Collider > c2,
                                                 std::shared_ptr< Transform > t2, bool static2)
    -> void {
  Utility::GJK::Shape s1, s2;
  if (c1->Type == +Components::ColliderType::Sphere)
    s1 = CreateSphereShape(c1, t1);
  else
    s1 = CreateBoxShape(c1, t1);

  if (c2->Type == +Components::ColliderType::Sphere)
    s2 = CreateSphereShape(c2, t2);
  else
    s2 = CreateBoxShape(c2, t2);

  auto collisionPoint = EPA(c1->Simplex, s1, s2);

  auto separate = collisionPoint.Normal * collisionPoint.PenetrationDepth / 2.0f;
  if (!static1) {
    auto pos = t1->Position() - separate;
    t1->Position(pos);
  }

  if (!static2) {
    auto pos = t2->Position() + separate;
    t2->Position(pos);
  }
}
