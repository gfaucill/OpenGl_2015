#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include "../tools/texture.hpp"

/*
 * Only one texture can be bind in the shader, if a texture is already bind
 * this texture are saved and re-bind after
*/

class TextureFactory
{
public :
    class Texture {
    public :
        /**
         * @brief Texture  Create a wrapped Texture2D
         * @param fileName  location of the image file
         */
        Texture(const char * fileName);

        /**
         * @brief load  load and bind texture in the shader
         */
        void load();

        /**
         * @brief unload  unload and unbind the texture
         */
        void unload();

        /**
         * @brief initTexture creates a OpenGL texture binding
         */
        void initTexture();

    private :
        STP3D::Texture2D m_texture;
    };


public:
    TextureFactory();

    /**
     * @brief createTexture  Create and init a wrapped Texture2D
     * @param fileName location of the image file
     * @return a TextureNode to put in the tree
     */
    Texture* createTexture(const char * fileName);

    ~TextureFactory();

private :
    std::vector<Texture*>  textures;
};

#endif // TEXTURE_H
