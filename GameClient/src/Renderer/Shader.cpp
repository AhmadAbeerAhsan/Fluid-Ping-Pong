#include "Shader.hpp"

Shader::Shader()
{
    ID = std::shared_ptr<GLuint>(new GLuint(glCreateProgram()), [](GLuint* id){
            std::cout << "glDeleteProgram(*id): " << *id << std::endl;
            glDeleteProgram(*id);
            delete id;
        }
    );
}

void Shader::Load(std::string vertexPath, std::string fragmentPath)
{
    auto cwd = std::filesystem::current_path();
    std::cout << "Working directory: " << cwd << "\n";

    // 1.retrievethevertex/fragmentsourcecodefromfilePath
    std::string vertexCode;
    std::string fragmentCode;

    std::cout << "Opening shader..." << "\n";

    // openfiles
    std::ifstream vShaderFile(vertexPath.c_str());
    std::cout << "Opening vertex shader" << "\n";
    if (!vShaderFile)
    {
        std::cout << "Error: could not open input file vertex shader" << "\n";
        //std::exit(1);
    }

    std::cout << "Opening fragment shader" << "\n";
    std::ifstream fShaderFile(fragmentPath.c_str());
    if (!fShaderFile)
    {
        std::cout << "Error: could not open input file fragment shader" << "\n";
        //std::exit(1);
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
        m_vShaderCode = vShaderStream.str();   // copies into the member directly — no dangling
        m_fShaderCode = fShaderStream.str();
    }
    catch (std::ifstream::failure)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
}

void Shader::GLCompleteShader()
{
    // 2.compileshaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertexShader
    const char* vSrc = m_vShaderCode.c_str();
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vSrc, NULL);
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
    const char* fSrc = m_fShaderCode.c_str();
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fSrc, NULL);
    glCompileShader(fragment);

    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    };

    glAttachShader(*ID, vertex);
    glAttachShader(*ID, fragment);
    glLinkProgram(*ID);

    // print linking errors if any
    glGetProgramiv(*ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(*ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    // delete shaders; they’re linked into our program and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Activate()
{
    glUseProgram(*ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(*ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(*ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(*ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    if (static_cast<GLuint>(current) != *ID)
    {
        std::cout << "WARNING: setMat4(\"" << name << "\") called on program " << *ID
                   << " but program " << current << " is currently bound — uniform update will be lost/wrong" << std::endl;
    }

    GLint loc = glGetUniformLocation(*ID, name.c_str());
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
    if (static_cast<GLuint>(current) != *ID)
    {
        std::cout << "WARNING: setVec3(\"" << name << "\") called on program " << *ID
                   << " but program " << current << " is currently bound — uniform update will be lost/wrong" << std::endl;
    }

    GLint loc = glGetUniformLocation(*ID, name.c_str());
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

    if (static_cast<GLuint>(current) != *ID)
    {
        std::cout << "WARNING: setVec2(\"" << name << "\") called on program " << *ID
                  << " but program " << current
                  << " is currently bound — uniform update will be lost/wrong"
                  << std::endl;
    }

    GLint loc = glGetUniformLocation(*ID, name.c_str());

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

GLint Shader::GetUniformLocation(const std::string& uniform)
{
    return glGetUniformLocation(*ID, uniform.data());
}
