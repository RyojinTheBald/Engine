#pragma once

#include <map>
#include <string>
#include <fstream>
#include <streambuf>
#include <iterator>
#include <assert.h>

#include <GL/glew.h>

enum ShaderType
{
    SHADER_VERTEX = GL_VERTEX_SHADER,
    SHADER_FRAGMENT = GL_FRAGMENT_SHADER
};

class Shader
{
public:
    Shader() : valid(false){};

    void addFile(std::string path, ShaderType Type)
    {
        std::string code(std::istreambuf_iterator<char>{std::ifstream(path)}, {});
        const char *codeCstr = code.c_str();

        unsigned int shader = glCreateShader(Type);
        glShaderSource(shader, 1, &codeCstr, NULL);
        glCompileShader(shader);

        int success;
        char infoLog[512];

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "Error: Shader compilation failed\n\t" << infoLog << std::endl;
            // exit(EXIT_FAILURE);
        }

        shaders[Type] = shader;
    }

    void link()
    {
        int success = 0;
        char infoLog[512];

        programID = glCreateProgram();

        for (auto const &[type, shaderID] : shaders)
        {
            glAttachShader(programID, shaderID);
        }

        glLinkProgram(programID);

        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(programID, 512, NULL, infoLog);
            std::cerr << "Error: Shader program linking failed\n\t" << infoLog << std::endl;
            // exit(EXIT_FAILURE);
        }

        for (auto const &[type, shaderID] : shaders)
        {
            glDeleteShader(shaderID);
        }

        valid = true;
    }

    [[nodiscard]] inline const bool isValid() { return valid; };

    [[nodiscard]] inline const unsigned int getProgram()
    {
        assert(valid);
        return programID;
    };

private:
    bool valid;
    std::map<ShaderType, unsigned int> shaders;
    unsigned int programID;
};