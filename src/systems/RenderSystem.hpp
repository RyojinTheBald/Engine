#pragma once

#include <entt/entt.hpp>

#include "../core/Window.hpp"

#include "../components/all.hpp"

class RenderSystem final
{
    public:
        RenderSystem() = default;
        ~RenderSystem() = default;

        void render(Core::Window& window, entt::registry& registry);
};