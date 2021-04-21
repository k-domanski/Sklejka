#include "pch.h"
#include "Entity.h"

#include <nlohmann/json.hpp>

#include "ECS/EntityManager.h"
namespace Engine::ECS {
  auto Entity::GetID() const -> EntityID {
    return _entityID;
  }

  auto Entity::GetSignature() const -> std::shared_ptr< EntitySignature > {
    return _signature;
  }

  auto Entity::GetAllComponents() -> std::vector< std::shared_ptr< Component > > {
    return EntityManager::GetInstance().GetAllComponents(_entityID);
  }

  std::string Entity::SaveToJson()
  {
    std::string separator = "42091169692137SUPERJSONSEPARATOR42091169692137";

    std::vector< std::shared_ptr< Component > > components = GetAllComponents();

    using namespace nlohmann;
    json json = nlohmann::json{
        {"entityID", std::to_string(GetID())},
        {"entityName", _name},
    };
    std::string fileContent = json.dump(4);

    for (std::shared_ptr< Component > component : components)
    {
      fileContent.append("\n" + separator);
      fileContent.append("\n" + component->SaveToJson());
    }

    return fileContent;
  }

  std::string Entity::SaveToJson(std::string filepath)
  {
    std::ofstream ofstream;
    ofstream.open(filepath);
    ofstream << SaveToJson();
    ofstream.close();

    return SaveToJson();
  }
}  // namespace Engine::ECS