#pragma once

#include <glad/glad.h> // include glad to get the required OpenGL headers
#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

class Shader
{
public:
    GLuint ID{};

    Shader(const char* vertexPath, const char* fragmentPath);
    Shader() = default;
    ~Shader();
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;

    void addAttribute(std::string attribute);
    void addUniform(std::string uniform);

private:
    std::map<std::string, int> attributes{};
    std::map<std::string, int> uniforms{};

    int attributeId{0};
    int uniformId{0};
};
