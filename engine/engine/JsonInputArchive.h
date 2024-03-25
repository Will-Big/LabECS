#pragma once

namespace engine
{
	class JsonInputArchive
	{
	public:
		JsonInputArchive(const std::string& json_string) {
			root = nlohmann::json::parse(json_string);
		}

		~JsonInputArchive() {}

		void next_type_group() {
			if (root_idx >= root.size()) {
				// 모든 타입 그룹을 처리했음을 나타내는 처리
				throw std::out_of_range("No more component types to process.");
			}
			currentComponents = root[root_idx]["components"];
			component_idx = 0;
			root_idx++;
		}

		void operator()(std::underlying_type_t<entt::entity>& s) {
			// 이 함수는 현재 구현에서 사용되지 않습니다.
			// `JsonOutputArchive`에서 엔티티의 사이즈를 별도로 저장하지 않기 때문입니다.
		}

		void operator()(entt::entity& entity) {
			if (component_idx >= currentComponents.size()) {
				next_type_group(); // 다음 타입 그룹으로 이동
			}
			entity = static_cast<entt::entity>(currentComponents[component_idx]["entity"].get<uint32_t>());
			component_idx++;
		}

		template<typename T>
		void operator()(T& value) {
			if (component_idx >= currentComponents.size()) {
				next_type_group(); // 다음 타입 그룹으로 이동
			}
			auto& componentData = currentComponents[component_idx]["data"];
			value = componentData.get<T>();
			component_idx++; // 다음 컴포넌트로 이동
		}


	private:
		nlohmann::json root;
		nlohmann::json currentComponents;
		size_t root_idx = 0;
		size_t component_idx = 0;
	};
}