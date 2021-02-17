#pragma once

#include <entt/entt.hpp>

#include "Window.hpp"
#include "../systems/RenderSystem.hpp"
#include "../systems/InputSystem.hpp"

namespace Core {
    class Game final
    {
        public:
            Game();
            ~Game() noexcept;

            const int run();

        private:
            void update(const double time);

            entt::registry m_registry;
            entt::dispatcher m_dispatcher;

            Window m_window;
            RenderSystem m_renderSystem;
            InputSystem m_inputSystem;

            entt::resource_cache<Component::Mesh> meshCache;
    };
}