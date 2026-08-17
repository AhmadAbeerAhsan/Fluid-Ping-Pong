#include "Texture.hpp"

Texture::Texture(const char* path)
{
    m_texture_id = std::make_unique<GLuint>();
    stbi_set_flip_vertically_on_load(true);
    std::string paths{path};

     // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data == nullptr)
    {
        std::cout << "Failed to load texture: " << path << std::endl;
        return;
    }

    GLenum colorCode;
    switch (nrChannels)
    {
        case 1: colorCode = GL_RED;  break;
        case 2: colorCode = GL_RG;   break;
        case 3: colorCode = GL_RGB;  break;
        case 4: colorCode = GL_RGBA; break;
        default:
            std::cout << "Texture Error: unsupported channel count (" << nrChannels
                       << ") in " << path << std::endl;
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
    glTexImage2D(GL_TEXTURE_2D, 0, colorCode, width, height, 0, colorCode, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(data);
}

void Texture::Use(int gl_texpos)
{
    glActiveTexture(gl_texpos);
    glBindTexture(GL_TEXTURE_2D, *m_texture_id);
}
