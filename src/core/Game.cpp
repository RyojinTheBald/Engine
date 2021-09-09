#include "Game.hpp"

#include "../events/all.hpp"

#include "../loader/Mesh.hpp"

#include "../components/all.hpp"


#include <glm/gtx/string_cast.hpp>

double deltaTime()
{
  static double lastTime = glfwGetTime();
  double thisTime = glfwGetTime();
  double ret = thisTime - lastTime;
  lastTime = thisTime;
  return ret;
}

namespace Core {
    Game::Game() : m_dispatcher(), m_window(&m_dispatcher), m_inputSystem(&m_registry)
    {
        Component::Shader shader;
        shader.addFile("../../assets/shaders/vertex3Dcamera.glsl", Component::ShaderType::SHADER_VERTEX);
        shader.addFile("../../assets/shaders/fragment.glsl", Component::ShaderType::SHADER_FRAGMENT);
        shader.link();

        if (!shader.isValid())
        {
            std::cerr << "Error linking shader" << std::endl;
            exit(EXIT_FAILURE);
        }

        //i get the feeling that this should actually be taking the place of the glfw callback setup stuff
        //possibly move into inputSystem constructor?
        m_dispatcher.sink<Events::Key>().connect<&InputSystem::onKey>(&m_inputSystem);
        m_dispatcher.sink<Events::MouseMove>().connect<&InputSystem::onMouseMove>(&m_inputSystem);
        m_dispatcher.sink<Events::MouseButton>().connect<&InputSystem::onMouseButton>(&m_inputSystem);
        m_dispatcher.sink<Events::MouseScroll>().connect<&InputSystem::onMouseScroll>(&m_inputSystem);


        const auto camera = m_registry.create();
        m_registry.assign<Component::Camera>(camera);
        m_registry.assign<Component::Position>(camera);
        m_registry.assign<Component::Orientation>(camera, 1.f, 0.f, 0.f, 0.f);
        m_registry.assign<Component::PlayerControl>(camera);
        //TODO: attach control to this entity, could do with a "controlled" component, then search for and apply transforms from within input system?

        const auto cabin = m_registry.create();
        m_registry.assign<Component::Position>(cabin, 0, 0, -500);
        m_registry.assign<Component::Orientation>(cabin, glm::quat(glm::vec3(0, 3.14f / 4.f, 0)));
        m_registry.assign<Component::Shader>(cabin, shader);

        if(entt::resource_handle handle = meshCache.load<Loader::Mesh>(entt::hashed_string("mesh/cabin"), "../../assets/cabin.dae"); handle){
            m_registry.assign<Component::Mesh>(cabin, handle);
        }

    }

    Game::~Game()
    {

    }

    const int Game::run()
    {
        while (!m_window.shouldClose())
        {
            m_window.events();
            update(deltaTime());
            m_renderSystem.render(m_window, m_registry);
        }
        return 0;
    }

    void Game::update(double time)
    {

        const float moveSpeed = 100.f;
        const float sensitivity = 1;
        for(auto &entity : m_registry.view<Component::PlayerControl>())
        {
            auto control = m_registry.get<Component::PlayerControl>(entity);

            auto position = m_registry.get<Component::Position>(entity);
            auto orientation = m_registry.get<Component::Orientation>(entity);

            glm::vec3 positionDelta = orientation * (control.direction * (float)time * moveSpeed) ;

            m_registry.replace<Component::Position>(entity, position + positionDelta);
            m_registry.replace<Component::Orientation>(entity, orientation * control.orientation);
        }
    }
}