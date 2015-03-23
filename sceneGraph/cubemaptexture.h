#ifndef CUBEMAPTEXTURE_H
#define CUBEMAPTEXTURE_H

#include "../tools/texture.hpp"

class CubemapTexture
{
public:
	/*
	 * Construct a cubeMap texture with 6 square ppm image
	 */
    CubemapTexture(const char * PosXFilename,
               const char * NegXFilename,
               const char * PosYFilename,
               const char * NegYFilename,
               const char * PosZFilename,
               const char * NegZFilename);

    uint gl_id_tex; ///< OpenGL id binding
    uint last_tex_unit; ///< Last texture unit binding used

    ~CubemapTexture();

    /*
     * init the texture in open pipeline
     */
    void initTexture();

    /*
     * bind the texture in the shader
     */
    void loadTexture(GLuint target_tex = 0);

    /*
     * unbind the texture in the shader
     */
    void unloadTexture(GLuint target_tex = 0);

private:
    /*
     * 6 2D texture use to load() image in memory an get data image
     */
    STP3D::Texture2D *tex[6];
};

#endif // CUBEMAPTEXTURE_H
