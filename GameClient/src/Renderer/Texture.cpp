#include "Texture.hpp"

Texture::Texture(TextureType type) :
    m_texture_type(type)
{
    switch (m_texture_type)
    {
    case TextureType::TwoD:
        m_texture_id = std::shared_ptr<GLuint>(
            new GLuint(0),
            [](GLuint* id){
                std::cout << "glDeleteTextures(1, id): " << *id << std::endl;
                glDeleteTextures(1, id);
                delete id;
            }
        );

        break;
    case TextureType::ThreeD:
        m_texture_id = std::shared_ptr<GLuint>(
            new GLuint(0),
            [](GLuint* id){
                std::cout << "glDeleteTextures(1, id): " << *id << std::endl;
                glDeleteTextures(1, id);
                delete id;
            }
        );

        break;
    default:
        break;
    }
}

void Texture::Load2DTexture(const char *path)
{
    stbi_set_flip_vertically_on_load(true);
    std::string paths{path};

     // load and generate the texture
    int nrChannels;
    m_2d_raw_data = stbi_load(path, &m_width, &m_height, &nrChannels, 0);
    if (m_2d_raw_data == nullptr)
    {
        std::cout << "Failed to load texture: " << path << std::endl;
        return;
    }

    switch (nrChannels)
    {
        case 1: m_color_code = GL_RED;  break;
        case 2: m_color_code = GL_RG;   break;
        case 3: m_color_code = GL_RGB;  break;
        case 4: m_color_code = GL_RGBA; break;
        default:
            std::cout << "Texture Error: unsupported channel count (" << nrChannels
                       << ") in " << path << std::endl;
            return;
    }
}

void Texture::Load3DTexture(std::vector<std::string> paths)
{
    stbi_set_flip_vertically_on_load(false);

    // load and generate the texture
    int nrChannels;
    for (unsigned int i = 0; i < paths.size(); i++)    
    {
        m_3d_raw_datas[i] = stbi_load(paths[i].c_str(), &m_width, &m_height, &nrChannels, 0);
        if (m_3d_raw_datas[i] == nullptr)
        {
            std::cout << "Failed to load texture: " << paths[i] << std::endl;
            stbi_image_free(m_3d_raw_datas[i]);
            return;
        }

        switch (nrChannels)
        {
            case 1: m_color_code = GL_RED;  break;
            case 2: m_color_code = GL_RG;   break;
            case 3: m_color_code = GL_RGB;  break;
            case 4: m_color_code = GL_RGBA; break;
            default:
                std::cout << "Texture Error: unsupported channel count (" << nrChannels
                        << ") in " << paths[i] << std::endl;
                return;
        }
    }
}

void Texture::GLCompleteTexture()
{
    switch (m_texture_type)
    {
    case TextureType::TwoD:
        
        if (m_2d_raw_data == nullptr)
        {
            return;
        }
        
        
        glGenTextures(1, m_texture_id.get());
        glBindTexture(GL_TEXTURE_2D, *m_texture_id);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, m_color_code, m_width, m_height, 0, m_color_code, GL_UNSIGNED_BYTE, m_2d_raw_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        stbi_image_free(m_2d_raw_data);
        m_2d_raw_data = nullptr;

        break;
    case TextureType::ThreeD:
        glGenTextures(1, m_texture_id.get());
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, *m_texture_id);

        for (unsigned int i = 0; i < m_3d_raw_datas.size(); i++)    
        {
            if (m_3d_raw_datas[i] == nullptr)
            {
                return;
            }
            
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                0, m_color_code, m_width, m_height, 0, m_color_code, GL_UNSIGNED_BYTE, m_3d_raw_datas[i]
            );
            stbi_image_free(m_3d_raw_datas[i]);
            m_3d_raw_datas[i] = nullptr;
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        break;
    default:
        break;
    }
}

void Texture::Use(int gl_texPos) const
{
    switch (m_texture_type)
    {
    case TextureType::TwoD:
        glActiveTexture(gl_texPos);
        glBindTexture(GL_TEXTURE_2D, *m_texture_id);
        break;
    case TextureType::ThreeD:
        glActiveTexture(gl_texPos);
        glBindTexture(GL_TEXTURE_CUBE_MAP, *m_texture_id);
        break;
    default:
        break;
    }
}
