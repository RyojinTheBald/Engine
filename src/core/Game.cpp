#include "Game.hpp"

namespace Core {
    Game::Game() : m_dispatcher(), m_window(&m_dispatcher)
    {

    }

    Game::~Game()
    {

    }

    const int Game::run()
    {
        while (!m_window.shouldClose())
        {
            // m_inputSystem.events(m_window, m_dispatcher);
            m_renderSystem.render(m_window, m_registry);
        }
        return 0;
    }

    void Game::events()
    {

    }

    void Game::update(double time)
    {

    }

    void Game::render()
    {

    }
}