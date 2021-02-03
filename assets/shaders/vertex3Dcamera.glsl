#version 330 core

layout (location = 0) in vec3 inPos;

out vec3 vertCol;

uniform mat4 u_projectionMat44;
uniform mat4 u_viewMat44;
uniform mat4 u_modelMat44;

void main()
{
    vertCol = inPos;
    vec4 modelPos = u_modelMat44 * vec4( inPos, 1.0 );
    vec4 viewPos  = u_viewMat44 * modelPos;
    gl_Position   = u_projectionMat44 * viewPos;
}