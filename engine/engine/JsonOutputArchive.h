#pragma once

namespace engine
{
	using json = nlohmann::json;

	class JsonOutputArchive {
	public:
		JsonOutputArchive() {
			root = nlohmann::json::array(); 
		}

		void operator()(std::underlying_type_t<entt::entity> size)
		{

		}

		void operator()(entt::entity entity) {
			// 현재 엔티티를 저장
			currentEntity = entity;
		}

		template<typename T>
		void operator()(const T& component) {
			entt::type_info info = entt::type_id<T>();
			std::string typeName = std::string(info.name());

			// 컴포넌트 데이터를 JSON으로 변환
			nlohmann::json componentJson = component;

			// 현재 엔티티와 컴포넌트 데이터를 저장
			nlohmann::json entityComponentData;
			entityComponentData["entity"] = static_cast<uint32_t>(currentEntity);
			entityComponentData["data"] = componentJson;

			// 타입별로 데이터를 그룹화
			if (typeGroups.find(typeName) == typeGroups.end()) {
				// 해당 타입의 첫 번째 데이터인 경우, 새로운 구조를 생성
				typeGroups[typeName] = { {"type", typeName}, {"size", 0}, {"components", nlohmann::json::array()} };
			}

			// 컴포넌트 데이터를 해당 타입의 "components" 배열에 추가
			typeGroups[typeName]["components"].push_back(entityComponentData);
			// 사이즈 업데이트
			typeGroups[typeName]["size"] = typeGroups[typeName]["components"].size();
		}

		void Close() {
			// 모든 타입 그룹을 root 배열에 추가
			for (auto& data : typeGroups | std::views::values) {
				root.push_back(data);
			}
		}

		// create a json as string
		std::string AsString()
		{
			std::string output = root.dump(2);
			return output;
		}

		// create bson-data
		std::vector<uint8_t> AsBson()
		{
			std::vector<std::uint8_t> as_bson = nlohmann::json::to_bson(root);
			return as_bson;
		}

	private:
		nlohmann::json root = nlohmann::json::array();
		std::unordered_map<std::string, nlohmann::json> typeGroups;
		entt::entity currentEntity = entt::null;
	};

}

