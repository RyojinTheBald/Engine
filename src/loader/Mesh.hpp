#pragma once

#include <entt/entt.hpp>

#include "../components/Mesh.hpp"

namespace Loader
{
    struct Mesh final: entt::resource_loader<Mesh, Component::Mesh>
    {
        std::shared_ptr<Component::Mesh> load(const char* path) const;
    };
}