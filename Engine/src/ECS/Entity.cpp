#include "pch.h"
#include "Entity.h"

#include <nlohmann/json.hpp>


#include "../../../Editor/src/ImGui/Panels/FileSystemPanel.h"
#include "Components/Collider.h"
#include "Components/Rigidbody.h"
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
    std::string separator = "42091169692137SUPERJSONCOMPONENTSEPARATOR42091169692137";

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

  void Entity::LoadFromJson(std::string filepath)
  {
    auto content        = Utility::ReadTextFile(filepath);
    std::vector< std::string > separated_jsons;
   
    std::string delimiter = "42091169692137SUPERJSONCOMPONENTSEPARATOR42091169692137"; //TODO: Move to one place instead of declaring each time

    size_t pos = 0;
    std::string token;
    while ((pos = content.find(delimiter)) != std::string::npos) {
      token = content.substr(0, pos);
      separated_jsons.push_back(token);
      content.erase(0, pos + delimiter.length());
    }

    nlohmann::json entity_json =
        nlohmann::json::parse(separated_jsons[0].begin(), separated_jsons[0].end());

    std::string entityID_string = entity_json["entityID"];
    std::stringstream ss(entityID_string);
    size_t entityID;
    ss >> entityID;

    _entityID = entityID;
    _name     = entity_json["entityName"];

    for (int i = 1; i < separated_jsons.size(); i++)
    {
      nlohmann::json component_json =
          nlohmann::json::parse(separated_jsons[i].begin(), separated_jsons[i].end());

      std::string component_type = component_json["componentType"];
      if (component_type == "transform")
        AddComponent< Transform >()->LoadFromJson(separated_jsons[i]);
      else if (component_type == "collider")
        AddComponent< Components::Collider >()->LoadFromJson(separated_jsons[i]);
      else if (component_type == "meshRenderer")
        AddComponent< Components::MeshRenderer >()->LoadFromJson(separated_jsons[i]);
      else if (component_type == "rigidbody")
        AddComponent< Components::Rigidbody >()->LoadFromJson(separated_jsons[i]);

    }
  }
}  // namespace Engine::ECS