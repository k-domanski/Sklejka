#include "pch.h"
#include "Entity.h"

#include <nlohmann/json.hpp>

#include "../../../Editor/src/ImGui/Panels/FileSystemPanel.h"
#include "Components/Collider.h"
#include "Components/DirectionalLight.h"
#include "Components/Rigidbody.h"
#include "Components/Animator.h"
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

  std::string Entity::SaveToJson(size_t parentID) {
    std::string separator = "42091169692137SUPERJSONCOMPONENTSEPARATOR42091169692137";

    std::vector< std::shared_ptr< Component > > components = GetAllComponents();

    using namespace nlohmann;
    json json               = nlohmann::json{{"entityID", std::to_string(GetID())},
                               {"entityName", _name},
                               {"parentID", parentID}};
    std::string fileContent = json.dump(4);

    for (std::shared_ptr< Component > component : components) {
      fileContent.append("\n" + separator);
      fileContent.append("\n" + component->SaveToJson());
    }

    return fileContent;
  }

  std::string Entity::SaveToJson(std::string filepath, size_t parentID) {
    std::ofstream ofstream;
    ofstream.open(filepath);
    ofstream << SaveToJson(parentID);
    ofstream.close();

    return SaveToJson(parentID);
  }

  void Entity::LoadFromJson(std::string filepath) {
    std::string content = "";
    if (filepath[0] == '{' || filepath[0] == '\n'
        || filepath[0] == ' ')  // HACK: Check if string is json
      content = filepath;
    else
      content = Utility::ReadTextFile(filepath);

    std::vector< std::string > separated_jsons;

    std::string delimiter =
        "42091169692137SUPERJSONCOMPONENTSEPARATOR42091169692137";  // TODO: Move to one place
                                                                    // instead of declaring each time

    size_t pos = 0;
    std::string token;
    while ((pos = content.find(delimiter)) != std::string::npos) {
      token = content.substr(0, pos);
      separated_jsons.push_back(token);
      content.erase(0, pos + delimiter.length());
    }

    separated_jsons.push_back(content);

    nlohmann::json entity_json =
        nlohmann::json::parse(separated_jsons[0].begin(), separated_jsons[0].end());

    std::string entityID_string = entity_json["entityID"];
    std::stringstream ss(entityID_string);
    size_t entityID;
    ss >> entityID;

    _entityID = entityID;
    _name     = entity_json["entityName"];

    std::cout << "Loading entity with ID " << entityID_string << "\n";

    for (int i = 1; i < separated_jsons.size(); i++) {
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
      else if (component_type == "camera")
          AddComponent< Camera >()->LoadFromJson(separated_jsons[i]);
      else if (component_type == "directionalLight")
          AddComponent< DirectionalLight >()->LoadFromJson(separated_jsons[i]);
      else if (component_type == "node")
          AddComponent< Node >()->LoadFromJson(separated_jsons[i]);
      else if (component_type == "animator")
          AddComponent<Animator>()->LoadFromJson(separated_jsons[i]);
    }
  }

  EntityID Entity::GetParentFromJson(std::string json_string) {
    std::vector< std::string > separated_jsons;
    std::string delimiter =
        "42091169692137SUPERJSONCOMPONENTSEPARATOR42091169692137";  // TODO: Move to one place
                                                                    // instead of declaring each time

    size_t pos = 0;
    std::string token;
    while ((pos = json_string.find(delimiter)) != std::string::npos) {
      token = json_string.substr(0, pos);
      separated_jsons.push_back(token);
      json_string.erase(0, pos + delimiter.length());
    }

    separated_jsons.push_back(json_string);

    nlohmann::json entity_json =
        nlohmann::json::parse(separated_jsons[0].begin(), separated_jsons[0].end());

    return entity_json["parentID"] != nullptr ? entity_json["parentID"] : 0;
  }
}  // namespace Engine::ECS