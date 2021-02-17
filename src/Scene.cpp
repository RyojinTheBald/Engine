#include "Scene.hpp"

#include <algorithm>
#include "Registry.hpp"

namespace scene {
    Node::Node(Node* parentNode) {
        parent = parentNode;
        mEntity = registry.create();
    }

    void Node::addChild(Node* childNode) {
        if(childNode == nullptr)
            return;

        childNode->parent->removeChild(childNode);
        childNode->parent = this;
        children.push_back(childNode);
    }

    void Node::removeChild(Node* childNode) {
        if(childNode == nullptr)
            return;

        children.erase(std::remove(children.begin(), children.end(), childNode));
        childNode->parent = nullptr;
    }
}

