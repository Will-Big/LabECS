#pragma once

namespace engine
{
	using json = nlohmann::json;

	class JsonOutputArchive
	{
		using entityType = std::underlying_type_t<entt::entity>;

	public:
		JsonOutputArchive()
		{
			root = json::object();
		}

		void operator()(entityType size)
		{
			// 1. 저장할 컨테이너 사이즈
		}

		void operator()(entt::entity entity)
		{
			// 2. 저장할 객체의 엔티티 번호
			currentEntity = entity;
		}

		template<typename T>
		void operator()(const T& component)
		{
			// 3. 실제 저장될 객체
			const entt::type_info info = entt::type_id<T>();
			const std::string typeName = std::string(info.name());

			// typeName에 해당하는 객체가 없거나, "components" 키가 배열이 아니면 초기화
			if (root.find(typeName) == root.end()) {
				root[typeName]= nlohmann::json::array();
			}

			// 현재 엔티티와 컴포넌트 데이터를 저장
			nlohmann::json entityComponentData;
			entityComponentData["entity"] = static_cast<uint32_t>(currentEntity);
			entityComponentData["data"] = component;

			// 컴포넌트 데이터를 해당 타입의 "components" 배열에 추가
			root[typeName].push_back(entityComponentData);
		}

		// create a json as string
		std::string ToString() const
		{
			return root.dump(2);
		}

		// create bson-data
		std::vector<uint8_t> ToBson() const
		{
			return nlohmann::json::to_bson(root);
		}

	private:
		nlohmann::json root;
		entt::entity currentEntity = entt::null;
	};

}

