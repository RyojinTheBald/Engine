#include "RenderSystem.hpp"

#include "Shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

void RenderSystem::render(Core::Window& window, entt::registry& registry)
{
    //clear buffers
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glm::mat4 cameraViewMatrix;
    glm::mat4 cameraProjectionMatrix;

    /* i'm sure there's a cleaner way to do this, however, this will do for now.
       at this stage there will only ever be one camera within the registry, so
       just using the first one returned and aborting the loop should suffice
    */
    for (auto& cameraEntity : registry.view<Component::Camera>())
    {
        //calculate camera view matrix
        auto position = registry.get<Component::Position>(cameraEntity);
        auto orientation = registry.get<Component::Orientation>(cameraEntity);
        cameraViewMatrix = glm::toMat4(orientation) * glm::translate(glm::mat4(1), position);

        //calculate camera projection matrix
        auto camera = registry.get<Component::Camera>(cameraEntity);
        cameraProjectionMatrix = camera.getProjection();

        break;
    }

    //TODO: what do we do if there is no camera?

    //get all entities with an assigned shader
    auto renderableGroup = registry.group<Component::Shader>(entt::get<Component::Mesh, Component::Position, Component::Orientation>);

    //to minimise context switches, keep track of currently used shader
    auto currentProgram = 0;

    for (auto entity : renderableGroup)
    {
        auto &shader = renderableGroup.get<Component::Shader>(entity);
        auto program = shader.getProgram();

        //we only need to set the program and update the camera matrices if the shader changes
        if (currentProgram == 0 || currentProgram != program)
        {
            currentProgram = program;
            glUseProgram(program);

            //update cameara uniforms
            glUniformMatrix4fv(shader.getUniformLocation("u_projectionMat44"), 1, GL_FALSE, glm::value_ptr(cameraProjectionMatrix));
            glUniformMatrix4fv(shader.getUniformLocation("u_viewMat44"), 1, GL_FALSE, glm::value_ptr(cameraViewMatrix));
        }
        
        //calculate model matrix
        auto position = registry.get<Component::Position>(entity);
        auto orientation = registry.get<Component::Orientation>(entity);

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1), position) * glm::toMat4(orientation);

        //set model matrix from entity's position and rotation
        glUniformMatrix4fv(shader.getUniformLocation("u_modelMat44"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

        //draw the mesh
        auto &mesh = renderableGroup.get<Component::Mesh>(entity);
        mesh.draw();
    }

    window.swapBuffers();
}