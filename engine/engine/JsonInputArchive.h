#pragma once

namespace engine
{
	class JsonInputArchive
	{
		using entityType = std::underlying_type_t<entt::entity>;

	public:
		JsonInputArchive(const nlohmann::json& json_string)
		{
			root = json_string;
		}

		~JsonInputArchive() {}

		void operator()(entt::entity& entity)
		{
			currentEntity = entity;
		}

		void operator()(entityType& size)
		{
			
		}

		template<typename T>
		void operator()(T& component)
		{
			const entt::type_info info = entt::type_id<T>();
			const std::string typeName = std::string(info.name());

			// typeName에 해당하는 컴포넌트 배열을 찾습니다.
			if (root.contains(typeName) && root[typeName].is_array()) 
			{
				for (auto& item : root[typeName]) 
				{
					if (item["entity"].get<entityType>() == static_cast<entityType>(currentEntity)) {
						// 해당 엔티티 ID를 가진 컴포넌트 데이터를 찾았다면, component에 데이터를 할당합니다.
						component = item["data"].get<T>();
						break;
					}
				}
			}
		}

	private:
		nlohmann::json root;
		entt::entity currentEntity = entt::null;
	};
}