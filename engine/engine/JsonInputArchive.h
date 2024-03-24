﻿#pragma once

namespace engine
{
    class JsonInputArchive {
    private:
        nlohmann::json root;
        nlohmann::json current;

        int root_idx = -1;
        int current_idx = 0;

    public:
        JsonInputArchive(const std::string& json_string)
        {
            root = nlohmann::json::parse(json_string);
        }

        ~JsonInputArchive() {
        }

        void next_root() {
            root_idx++;
            if (root_idx >= root.size()) {
                // ERROR
                return;
            }
            current = root[root_idx];
            current_idx = 0;
        }

        void operator()(std::underlying_type_t<entt::entity>& s) {
            next_root();
            int size = current[0].get<int>();
            current_idx++;
            s = (std::underlying_type_t<entt::entity>)size; // pass amount to entt
        }

        void operator()(entt::entity& entity) {
            uint32_t ent = current[current_idx].get<uint32_t>();
            entity = entt::entity(ent);
            current_idx++;
        }

        template<typename T>
        void operator()(entt::entity& ent, T& t) {
            nlohmann::json component_data = current[current_idx * 2];

            auto comp = component_data.get<T>();
            t = comp;

            uint32_t _ent = current[current_idx * 2 - 1];
            ent = entt::entity(_ent); // last element is the entity-id
            current_idx++;
        }

        template<typename T>
        void operator()(T& value) {
            // 현재 처리 중인 JSON 요소에서 값을 추출하여 value에 할당
            value = current[current_idx].get<T>();
            current_idx++; // 다음 요소로 이동
        }
    };
}