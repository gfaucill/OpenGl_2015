#include "texture.h"


///////////////////////////////////////////////////////////////////////////////
///                          TextureFactory
///////////////////////////////////////////////////////////////////////////////

TextureFactory::TextureFactory() {}

TextureFactory::Texture *TextureFactory::createTexture(const char *fileName)
{
    Texture * t = new Texture(fileName);
    t->initTexture();
    textures.push_back(t);
    return t;
}

TextureFactory::~TextureFactory()
{
    for (std::vector<Texture*>::iterator it = textures.begin(); it != textures.end(); it++) {
        delete *it;
    }
}


///////////////////////////////////////////////////////////////////////////////
///                             Texture
///////////////////////////////////////////////////////////////////////////////
///
TextureFactory::Texture::Texture(const char *fileName) : m_texture(fileName)
{
}

void TextureFactory::Texture::load()
{
    m_texture.loadTexture();
    int id; glGetIntegerv(GL_CURRENT_PROGRAM, &id);
    glUniform1i(glGetUniformLocation(id,"texture0"), 0);
    glUniform1i(glGetUniformLocation(id,"textureActive"), 1);
}

void TextureFactory::Texture::unload()
{
    m_texture.unloadTexture();
    int id; glGetIntegerv(GL_CURRENT_PROGRAM, &id);
    glUniform1i(glGetUniformLocation(id,"textureActive"), 0);
}

void TextureFactory::Texture::initTexture()
{
    m_texture.initTexture();
}



