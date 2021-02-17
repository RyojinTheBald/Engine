#pragma once

#include <vector>
#include <entt/entt.hpp>

namespace scene {
    class Node {
        public:
            Node(Node* parentNode = nullptr);
            void addChild(Node* childNode);
            void removeChild(Node* childNode);

            operator entt::registry::entity_type() const { return mEntity; }

        protected:

            Node* parent;
            std::vector<Node*> children;
            entt::registry::entity_type mEntity;
    };


    
}