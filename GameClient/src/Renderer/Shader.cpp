#include "Shader.hpp"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    auto cwd = std::filesystem::current_path();
    std::cout << "Working directory: " << cwd << "\n";

    // 1.retrievethevertex/fragmentsourcecodefromfilePath
    std::string vertexCode;
    std::string fragmentCode;

    std::cout << "Opening shader..." << "\n";

    // openfiles
    std::ifstream vShaderFile(vertexPath);
    std::cout << "Opening vertex shader" << "\n";
    if (!vShaderFile)
    {
        std::cout << "Error: could not open input file vertex shader" << "\n";
        std::exit(1);
    }

    std::cout << "Opening fragment shader" << "\n";
    std::ifstream fShaderFile(fragmentPath);
    if (!fShaderFile)
    {
        std::cout << "Error: could not open input file fragment shader" << "\n";
        std::exit(1);
    }

    std::cout << "Reading shaders..." << "\n";
    try
    {

        std::stringstream vShaderStream, fShaderStream;

        // readfile’sbuffercontentsintostreams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // closefilehandlers
        vShaderFile.close();
        fShaderFile.close();

        // convertstreamintostring
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // 2.compileshaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertexShader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    };

    // fragmentShader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    };

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    // delete shaders; they’re linked into our program and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    if (static_cast<GLuint>(current) != ID)
    {
        std::cout << "WARNING: setMat4(\"" << name << "\") called on program " << ID
                   << " but program " << current << " is currently bound — uniform update will be lost/wrong" << std::endl;
    }

    GLint loc = glGetUniformLocation(ID, name.c_str());
    if (loc == -1)
    {
        //std::cout << name << ": uniform not found on program " << ID << std::endl;
        return;
    }
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, glm::vec3 value) const
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    if (static_cast<GLuint>(current) != ID)
    {
        std::cout << "WARNING: setVec3(\"" << name << "\") called on program " << ID
                   << " but program " << current << " is currently bound — uniform update will be lost/wrong" << std::endl;
    }

    GLint loc = glGetUniformLocation(ID, name.c_str());
    if (loc == -1)
    {
        //std::cout << name << ": uniform not found on program " << ID << std::endl;
        return;
    }
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Shader::setVec2(const std::string &name, glm::vec2 value) const
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);

    if (static_cast<GLuint>(current) != ID)
    {
        std::cout << "WARNING: setVec2(\"" << name << "\") called on program " << ID
                  << " but program " << current
                  << " is currently bound — uniform update will be lost/wrong"
                  << std::endl;
    }

    GLint loc = glGetUniformLocation(ID, name.c_str());

    if (loc == -1)
    {
        // std::cout << name << ": uniform not found on program " << ID << std::endl;
        return;
    }

    glUniform2fv(loc, 1, glm::value_ptr(value));
}

void Shader::addAttribute(std::string attribute)
{
    attributes[attribute] = attributeId;
    attributeId++;
}

void Shader::addUniform(std::string uniform)
{
    uniforms[uniform] = uniformId;
    uniformId++;
}
